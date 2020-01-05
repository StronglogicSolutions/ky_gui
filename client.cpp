#include <client.hpp>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <functional>
#include <algorithm>
#include <cstring>
#include <QDebug>
#include <QByteArray>
#include <iostream>
#include <vector>
#include <future>
#include <headers/kmessage_codec.hpp>
#include <headers/json.hpp>
#include <headers/util.hpp>

using namespace KData;

using json = nlohmann::json;

static const int MAX_BUFFER_SIZE = 2048;
static const int MAX_PACKET_SIZE = 4096;
static const int HEADER_SIZE = 4;

flatbuffers::FlatBufferBuilder builder(1024);

/**
 * @brief Client::createMessageHandler
 * @param cb
 * @return
 */
Client::MessageHandler Client::createMessageHandler(
    std::function<void()> cb) {
  return MessageHandler(cb);
}

/**
 * @brief Client::Client
 * @constructor
 * @param parent
 * @param count
 * @param arguments
 */
Client::Client(QWidget *parent, int count, char** arguments) : QDialog(parent), argc(count), argv(arguments), m_client_socket_fd(-1), m_commands({}), executing(false) {
    qRegisterMetaType<QVector<QString>>("QVector<QString>");
}

/**
 * @brief Client::~Client
 * @destructor
 */
Client::~Client() {
    closeConnection();
}

 /**
 * @brief Client::handleMessages
 */
void Client::handleMessages() {
    uint8_t receive_buffer[2048];
    for (;;) {
        memset(receive_buffer, 0, 2048);
        ssize_t bytes_received = 0;
        bytes_received = recv(m_client_socket_fd, receive_buffer, 2048 - 2, 0);
        receive_buffer[2047] = 0;
        if (bytes_received == 0) {
            break;
        }
        size_t end_idx = findNullIndex(receive_buffer);
        std::string data_string{receive_buffer, receive_buffer + end_idx};
        StringVec s_v{};
        if (isNewSession(data_string.c_str())) {
            m_commands = getArgMap(data_string.c_str());
            for (const auto& [k, v] : m_commands) {
                s_v.push_back(v.data());
            }
            emit Client::messageReceived(COMMANDS_UPDATE_TYPE, "", s_v);
        } else if (serverWaitingForFile(data_string.c_str())) {
            sendFileEncoded(outgoing_file);
//        } else if (isEvent(data_string.c_str())) {
            // get event
            // get args
            // Main window should have an events container
        }
        std::string formatted_json = getJsonString(data_string);
        emit Client::messageReceived(MESSAGE_UPDATE_TYPE, QString::fromUtf8(formatted_json.data(), formatted_json.size()), {});
    }
    memset(receive_buffer, 0, 2048);
    ::close(m_client_socket_fd);
}

/**
 * @brief Client::start
 * @return A meaningless integer
 */
void Client::start() {
    if (m_client_socket_fd == -1) {
        m_client_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (m_client_socket_fd != -1) {
            sockaddr_in server_socket;
            char* end;
            server_socket.sin_family = AF_INET;
            auto port_value = strtol(argv[2], &end, 10);
            if (port_value < 0 || end == argv[2]) {
                return;
            }

            int socket_option = 1;
            // Free up the port to begin listening again
            setsockopt(m_client_socket_fd, SOL_SOCKET, SO_REUSEADDR, &socket_option,
                       sizeof(socket_option));

            server_socket.sin_port = htons(port_value);
            inet_pton(AF_INET, argv[1], &server_socket.sin_addr.s_addr);

            if (::connect(m_client_socket_fd, reinterpret_cast<sockaddr*>(&server_socket),
                          sizeof(server_socket)) != -1) {
                std::string start_operation_string = createOperation("start", {});
                // Send operation as an encoded message
                sendEncoded(start_operation_string);
                // Delegate message handling to its own thread
                std::function<void()> message_send_fn = [this]() {
                    this->handleMessages();
                };
                MessageHandler message_handler = createMessageHandler(message_send_fn);
                // Handle received messages on separate thread
                std::thread (message_handler).detach();

            } else {
                qDebug() << errno;
                ::close(m_client_socket_fd);
            }
        } else {
            qDebug() << "Failed to create new connection";
        }
    } else {
        qDebug() << "Connection already in progress";
    }
}

/**
 * @brief Client::sendMessage
 * @param s[in] <const QString&> The message to send
 */
void Client::sendMessage(const QString& s) {
    if (m_client_socket_fd != -1) {
        std::string json_string {"{\"type\":\"custom\", \"message\": \""};
        json_string += s.toUtf8().data();
        json_string += "\", \"args\":\"placeholder\"}";
        // Send custom message as an encoded message
        sendEncoded(json_string);
    } else {
        qDebug() << "You must first open a connection";
    }
}

void Client::sendEncoded(std::string message) {
    std::vector<uint8_t> fb_byte_vector{message.begin(), message.end()};
    auto byte_vector = builder.CreateVector(fb_byte_vector);
    auto k_message = CreateMessage(builder, 69, byte_vector);

    builder.Finish(k_message);

    uint8_t* encoded_message_buffer = builder.GetBufferPointer();
    uint32_t size = builder.GetSize();

    qDebug() << "Size is " << size;

    uint8_t send_buffer[MAX_BUFFER_SIZE];
    memset(send_buffer, 0, MAX_BUFFER_SIZE);
    send_buffer[0] = (size & 0xFF) >> 24;
    send_buffer[1] = (size & 0xFF) >> 16;
    send_buffer[2] = (size & 0xFF) >> 8;
    send_buffer[3] = (size & 0xFF);
    std::memcpy(send_buffer + 4, encoded_message_buffer, size);
    qDebug() << "Ready to send:";
    std::string message_to_send{};
    for (unsigned int i = 0; i < (size + 4); i++) {
        message_to_send += (char)*(send_buffer + i);
    }
    qDebug() << message_to_send.c_str();
    // Send start operation
    ::send(m_client_socket_fd, send_buffer, size + 4, 0);
    builder.Clear();
}

void Client::sendPackets(uint8_t* data, int size) {
    uint32_t total_size = static_cast<uint32_t>(size + HEADER_SIZE);
    uint32_t total_packets = static_cast<uint32_t>(ceil(
        static_cast<double>(
            static_cast<double>(total_size) / static_cast<double>(MAX_PACKET_SIZE)) // total size / packet
        )
    );

    uint32_t idx = 0;

    for (; idx < total_packets; idx++) {
        bool is_first_packet = (idx == 0);
        bool is_last_packet = (idx == (total_packets - 1));
        if (is_first_packet) {
          uint32_t first_packet_size =
              std::min(size + HEADER_SIZE, MAX_PACKET_SIZE);
          uint8_t packet[first_packet_size];

          packet[0] = (total_size >> 24) & 0xFF;
          packet[1] = (total_size >> 16) & 0xFF;
          packet[2] = (total_size >> 8) & 0xFF;
          packet[3] = (total_size) & 0xFF;

          std::memcpy(packet + HEADER_SIZE, data, first_packet_size - HEADER_SIZE);
          /**
           * SEND PACKET !!!
           */
          ::send(m_client_socket_fd, packet, first_packet_size, 0);
          if (is_last_packet) {
            break;
          }
          continue;
        }
        int offset = (idx * MAX_PACKET_SIZE) - HEADER_SIZE;
        uint32_t packet_size = std::min(size - offset, MAX_PACKET_SIZE);
        uint8_t packet[packet_size];

        std::memcpy(packet, data + offset, packet_size);
        /**
         * SEND PACKET !!!
         */
        ::send(m_client_socket_fd, packet, packet_size, 0);
        if (is_last_packet) {
            // cleanup
            outgoing_file.clear();
        }
    }
}

void Client::sendFileEncoded(QByteArray bytes) {
    sendPackets(reinterpret_cast<uint8_t*>(bytes.data()), bytes.size());
}

void Client::closeConnection() {
    if (m_client_socket_fd != -1) {
        std::string stop_operation_string = createOperation("stop", {});
        // Send operation as an encoded message
        sendEncoded(stop_operation_string);
        // Clean up socket file descriptor
        ::shutdown(m_client_socket_fd, SHUT_RDWR);
        ::close(m_client_socket_fd);
        m_client_socket_fd = -1;
        return;
    }
    qDebug() << "There is no active connection to close";
}

void Client::setSelectedApp(std::vector<QString> app_names) {
    selected_commands.clear();
    for (const auto& name : app_names) {
        qDebug() << "Matching mask to " << name;
        for (const auto& command : m_commands) {
            if (command.second.c_str() == name.toUtf8()) {
                selected_commands.push_back(command.first);
            }
        }
    }
}

void Client::execute() {
    if (!selected_commands.empty()) {
        executing = true;
        for (const auto& command : selected_commands) {
            std::string execute_operation = createOperation("Execute", {std::to_string(command)});
            sendEncoded(execute_operation);
        }
    }
}

void Client::sendFile(QByteArray bytes) {
    if (outgoing_file.isNull()) {
        std::string send_file_operation = createOperation("FileUpload", {});
        int size = bytes.size();
        qDebug() << size << " bytes to send";
        sendEncoded(send_file_operation);
        outgoing_file = bytes;
    } else {
        qDebug() << "Outgoing file buffer is not ready";
    }
}
