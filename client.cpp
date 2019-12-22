#include <client.hpp>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <functional>
#include <cstring>
#include <QDebug>
#include <QByteArray>
#include <string>
#include <iostream>
#include <vector>
#include <future>
#include <headers/kmessage_codec.hpp>
#include <headers/json.hpp>

using namespace KData;

using json = nlohmann::json;

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
Client::Client(QWidget *parent, int count, char** arguments) : QDialog(parent), argc(count), argv(arguments), m_client_socket_fd(-1) {}

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
    char receive_buffer[2048];
    for (;;) {
        memset(receive_buffer, 0, 2048);
        ssize_t bytes_received = 0;
        bytes_received = recv(m_client_socket_fd, receive_buffer, 2048 - 2, 0);
        receive_buffer[2047] = 0;
        if (bytes_received == 0) {
            break;
        }
        emit Client::messageReceived(QString::fromUtf8(receive_buffer, 2048));
    }
    memset(receive_buffer, 0, 2048);
    ::close(m_client_socket_fd);
}

/**
 * @brief Client::start
 * @return A meaningless integer
 */
void Client::start() {
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
    }
}

/**
 * @brief Client::sendMessage
 * @param s[in] <const QString&> The message to send
 */
void Client::sendMessage(const QString& s) {
    if (m_client_socket_fd != -1) {
        // Convert QString to std byte vector
        // TODO: switch to using one or the other completely
        json data_json{};
        data_json["type"] = "custom";
        data_json["program"] = "placeholder";
        data_json["message"] = s.toUtf8().data();

        std::string json_string = data_json.dump();

        std::vector<uint8_t> fb_byte_vector{json_string.begin(), json_string.end()};
//        for (int i = 0; i < q_byte_array.size(); i++) {
//            fb_byte_vector.push_back(static_cast<uint8_t>(q_byte_array.at(i)));
//        }
        auto byte_vector = builder.CreateVector(fb_byte_vector);
        auto message = CreateMessage(builder, 69, byte_vector);
        builder.Finish(message);

        uint8_t* encoded_message_buffer = builder.GetBufferPointer();
        uint32_t size = builder.GetSize();

        qDebug() << "Size is " << size;

        uint8_t send_buffer[size + 4];
        send_buffer[0] = (size & 0xFF) >> 24;
        send_buffer[1] = (size & 0xFF) >> 16;
        send_buffer[2] = (size & 0xFF) >> 8;
        send_buffer[3] = (size & 0xFF);
        std::memcpy(send_buffer + 4, encoded_message_buffer, size);
        qDebug() << "Ready to send:";
        for (unsigned int i = 0; i < (size + 4); i++) {
            qDebug() << (char)*(send_buffer + i);
        }
        ::send(m_client_socket_fd, send_buffer, size + 4, 0);
    } else {
        qDebug() << "You must first open a connection";
    }
}

void Client::closeConnection() {
    if (m_client_socket_fd != -1) {
        ::close(m_client_socket_fd);
        m_client_socket_fd = -1;
    }
}
