#include <arpa/inet.h>
#include <math.h>
#include <netdb.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <QByteArray>
#include <QDebug>
#include <algorithm>
#include <cstring>
#include <functional>
#include <future>
#include <include/client.hpp>
#include <iostream>
#include <vector>
#define FLATBUFFERS_DEBUG_VERIFICATION_FAILURE
#include <headers/kmessage_codec.hpp>
#include <headers/instatask_generated.h>

using namespace KData;
using namespace IGData;

static const int MAX_PACKET_SIZE = 4096;
static const int HEADER_SIZE = 4;

flatbuffers::FlatBufferBuilder builder(1024);

/**
 * @param [in] {std::function<void()>} cb A non-returning function to be called without parameter
 * @returns {MessageHandler} A message loop handler
 */
Client::MessageHandler Client::createMessageHandler(std::function<void()> cb) {
  return MessageHandler(cb);
}

/**
 * @brief Client::Client
 * @constructor
 * @param [in] {QWidget*} parent
 * @param [in] {int} count
 * @param [in] {char**} arguments
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
    uint8_t receive_buffer[MAX_PACKET_SIZE];
    for (;;) {
        memset(receive_buffer, 0, MAX_PACKET_SIZE);
        ssize_t bytes_received = 0;
        bytes_received = recv(m_client_socket_fd, receive_buffer, MAX_PACKET_SIZE, 0);
        if (bytes_received == 0) { // Finish message loop
            break;
        }
        size_t end_idx = findNullIndex(receive_buffer);
        std::string data_string{receive_buffer, receive_buffer + end_idx};
        qDebug() << "Received data from KServer: \n" << data_string.c_str();
        if (isPong(data_string.c_str())) {
            qDebug() << "Server returned pong";
            continue;
        }
        StringVec s_v{};
        if (isNewSession(data_string.c_str())) { // Session Start
            m_commands = getArgMap(data_string.c_str());
            for (const auto& [k, v] : m_commands) { // Receive available commands
                s_v.push_back(v.data());
            }
            emit Client::messageReceived(COMMANDS_UPDATE_TYPE, "New Session", s_v); // Update UI
        } else if (serverWaitingForFile(data_string.c_str())) { // Server expects a file
            processFileQueue();
        } else if (isEvent(data_string.c_str())) { // Receiving event
            QString event = getEvent(data_string.c_str());
            QVector<QString> args = getArgs(data_string.c_str());
            emit Client::messageReceived(EVENT_UPDATE_TYPE, event, args); // Update UI (event)
            if (isUploadCompleteEvent(event.toUtf8().constData())) { // Upload complete
              if (!args.isEmpty()) {
                sent_files.at(sent_files.size() - 1).timestamp =
                    std::stoi(args.at(0).toUtf8().constData());
                if (outgoing_files.isEmpty()) {
                  sendTaskEncoded(TaskType::INSTAGRAM, m_task);
                  file_was_sent = false;
                } else {
                  sendEncoded(
                      createOperation("FileUpload", {"Subsequent file"}));
                }
                }
            }
        }
        std::string formatted_json = getJsonString(data_string);
        emit Client::messageReceived(MESSAGE_UPDATE_TYPE, QString::fromUtf8(formatted_json.data(), formatted_json.size()), {});
    }
    memset(receive_buffer, 0, 2048);
    ::close(m_client_socket_fd);
//    ::shutdown(m_client_socket_fd, SHUT_RDWR);
}

void Client::processFileQueue() {
    KFileData outgoing_file = outgoing_files.dequeue();
    sendFileEncoded(outgoing_file.bytes);
    sent_files.push_back(SentFile{.name = outgoing_file.name, .type = outgoing_file.type });
}
/**
 * @brief Client::start
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
 * @param [in] {const QString&} The message to send
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

/**
 * @brief Client::sendEncoded
 * @param [in] {std::string message} The message to send
 */
void Client::sendEncoded(std::string message) {
    std::vector<uint8_t> fb_byte_vector{message.begin(), message.end()};
    auto byte_vector = builder.CreateVector(fb_byte_vector);
    auto k_message = CreateMessage(builder, 69, byte_vector);

    builder.Finish(k_message);

    uint8_t* encoded_message_buffer = builder.GetBufferPointer();
    uint32_t size = builder.GetSize();

    uint8_t send_buffer[MAX_PACKET_SIZE];
    memset(send_buffer, 0, MAX_PACKET_SIZE);
    send_buffer[0] = (size & 0xFF) >> 24;
    send_buffer[1] = (size & 0xFF) >> 16;
    send_buffer[2] = (size & 0xFF) >> 8;
    send_buffer[3] = (size & 0xFF);
    send_buffer[4] = (TaskCode::GENMSGBYTE & 0xFF);
    std::memcpy(send_buffer + 5, encoded_message_buffer, size);
    qDebug() << "Sending encoded message";
    std::string message_to_send{};
    for (unsigned int i = 0; i < (size + 5); i++) {
        message_to_send += (char)*(send_buffer + i);
    }
    qDebug() << "Encoded message size: " << (size + 5);
    // Send start operation
    ::send(m_client_socket_fd, send_buffer, size + 5, 0);
    builder.Clear();
}
/**
 * @brief getTaskFileInfo
 * @param [in] {std::vector<SentFile>} files The files to produce an information string from
 * @return std::string A string with the following format denoting each file:
 * `1580057341filename|image::`
 */
std::string getTaskFileInfo(std::vector<SentFile> files) {
    std::string info{};
    for (const auto& f : files) {
        info += std::to_string(f.timestamp);
        info += f.name.toUtf8().constData();
        info += "|";
        if (f.type == FileType::VIDEO) {
            info += "video";
        } else {
            info += "image";
        }
        info += "::";
    }
    qDebug() << "File Info: " << info.c_str();
    return info;
}

/**
 * @brief Client::sendTaskEncoded
 * @param [in] {TaskType}                 type The type of task
 * @param [in] {std::vector<std::string>} args The task arguments
 */
void Client::sendTaskEncoded(TaskType type, std::vector<std::string> args) {
    if (type == TaskType::INSTAGRAM) {
      if (args.size() < 8) {
        qDebug() << "Not enough arguments to send an IGTask";
        return;
      }
        auto file_info = builder.CreateString(getTaskFileInfo(sent_files));
        auto time = builder.CreateString(args.at(0).c_str(), args.at(0).size());
        auto description = builder.CreateString(args.at(1).c_str(), args.at(1).size());
        auto hashtags = builder.CreateString(args.at(2).c_str(), args.at(2).size());
        auto requested_by = builder.CreateString(args.at(3).c_str(), args.at(3).size());
        auto requested_by_phrase = builder.CreateString(args.at(4).c_str(), args.at(4).size());
        auto promote_share = builder.CreateString(args.at(5).c_str(), args.at(5).size());
        auto link_bio = builder.CreateString(args.at(6).c_str(), args.at(6).size());
        auto is_video = args.at(7) == "1";
        auto header = builder.CreateString(args.at(8).c_str(), args.at(8).size());
        auto user = builder.CreateString(args.at(9).c_str(), args.at(9).size());

        flatbuffers::Offset<IGTask> ig_task =
            CreateIGTask(builder, 96, file_info, time, description, hashtags,
                         requested_by, requested_by_phrase, promote_share,
                         link_bio, is_video, 16, header, user);

        builder.Finish(ig_task);

        uint8_t* encoded_message_buffer = builder.GetBufferPointer();
        uint32_t size = builder.GetSize();

        uint8_t send_buffer[MAX_PACKET_SIZE];
        memset(send_buffer, 0, MAX_PACKET_SIZE);
        send_buffer[0] = (size >> 24) & 0xFF;
        send_buffer[1] = (size >> 16) & 0xFF;
        send_buffer[2] = (size >> 8) & 0xFF;
        send_buffer[3] = size & 0xFF;
        send_buffer[4] = (TaskCode::IGTASKBYTE & 0xFF);

        std::memcpy(send_buffer + 5, encoded_message_buffer, size);
        qDebug() << "Ready to send:";
        std::string message_to_send{};
        for (unsigned int i = 0; i < (size + 5); i++) {
            message_to_send += (char)*(send_buffer + i);
            qDebug() << (char)*(send_buffer + i);
        }
        qDebug() << "Final size: " << (size + 5);
        // Send start operation
        ::send(m_client_socket_fd, send_buffer, size + 5, 0);
        builder.Clear();
        sent_files.clear();
        m_task.clear();
        if (!m_task_queue.isEmpty()) {
          auto task = m_task_queue.dequeue();
          if (!task.files.empty()) {
            if (!outgoing_files.empty()) {
              qDebug() << "There are still outgoing files left over from last "
                          "task which were never sent. They are being deleted";
              outgoing_files.clear();
            }
          }
          // We simply need to send files. Once the last file is sent, Client
          // will check the value of m_task and send it to Server.
          m_task = task.args;
          sendFiles(task.files);
        }
    }
}

/**
 * @brief Client::sendPackets
 * @param [in] {uint8_t*} data A pointer to a buffer of bytes
 * @param [in] {int}      size The size of the buffer to be packetized and sent
 */
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
            qDebug() << "Last packet of file sent";
            file_was_sent = true;
        }
    }
}

void Client::ping() {
  if (m_client_socket_fd != -1) {  // if we have active connection
    if (outgoing_files.size() == 0 || file_was_sent) {
      // 1st condition: we aren't sending file packets
      // 2nd condition: we're sending packets, but one file has sent and we want
      // to ping in case the server is unresponsive
      uint8_t send_buffer[5];
      memset(send_buffer, 0, 5);
      send_buffer[4] = (TaskCode::PINGBYTE & 0xFF);
      qDebug() << "Pinging server";
      ::send(m_client_socket_fd, send_buffer, 5, 0);
    }
  }
}

/**
 * @brief Client::sendFileEncoded
 * @param [in] {QByteArray} bytes An array of bytes to send
 */
void Client::sendFileEncoded(QByteArray bytes) {
    sendPackets(reinterpret_cast<uint8_t*>(bytes.data()), bytes.size());
}

/**
 * @brief Client::closeConnection
 */
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

/**
 * @brief Client::setSelectedApp
 * @param [in] TYPE SHOULD CHANGE app_names
 */
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

/**
 * @brief Client::getSelectedApp
 * @returns {int} The mask representing the selected application
 */
int Client::getSelectedApp() {
    if (selected_commands.size() == 1) {
        return selected_commands.at(0);
    } else {
        QMessageBox::warning(this, tr("App Selection Error"), tr("Unable to retrieve app selection"));
    }
    return -1;
}

/**
 * @brief Client::getAppName
 * @param [in] {int} mask The mask representing the application
 * @returns {QString} The application name
 */
QString Client::getAppName(int mask) {
    auto app = m_commands.find(mask);
    if (app != m_commands.end()) {
        return QString{app->second.c_str()};
    }
    return QString{""};
}

/**
 * @brief Client::execute
 */
void Client::execute() {
    if (!selected_commands.empty()) {
        executing = true;
        for (const auto& command : selected_commands) {
            auto app_name = getAppName(command);
            auto message = app_name + " pending";
            auto request_id = QUuid::createUuid().toString(QUuid::StringFormat::WithoutBraces);
            emit Client::messageReceived(PROCESS_REQUEST_TYPE, message, { QString{command}, app_name, request_id });
            std::string execute_operation = createOperation("Execute", {std::to_string(command), std::string(request_id.toUtf8().constData())});
            sendEncoded(execute_operation);
        }
    }
}

/**
 * @brief Client::scheduleTask
 * @param [in] {std::vector<std::string>} task_args The task arguments
 * @param [in] {bool}                     file_pending A boolean indicating whether there are files being sent for this task
 */
void Client::scheduleTask(std::vector<std::string> task_args, bool file_pending) {
    if (file_pending) {
      if (m_task.empty()) {
        m_task = task_args;
      } else {
        if (!m_task_queue.empty() && m_task_queue.front().args.empty()) {
          m_task_queue.front().args.assign(task_args.begin(), task_args.end());
        } else {
          qDebug() << "Could not identify the queued task for updating";
        }
      }
    } else {
        qDebug() << "Requesting a task to be scheduled";
        sendTaskEncoded(TaskType::INSTAGRAM, task_args);
    }
}

/**
 * @brief Client::sendFiles
 * @param [in] {QVector<const QByteArray} files The files to be sent
 */
void Client::sendFiles(QVector<KFileData> files) {
    if (outgoing_files.isEmpty()) {
      file_was_sent = false;
      for (const auto& file : files) {
        outgoing_files.enqueue(std::move(file));
        }
        std::string send_file_operation = createOperation("FileUpload", {});
        sendEncoded(send_file_operation);
    } else {
      // TODO: place in queue and check queue after we finish scheduling the
      // task associated with the outgoing files
      m_task_queue.enqueue(Task{.files = files});
      qDebug() << "Still attempting to send a different file";
    }
}
