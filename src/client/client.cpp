﻿#define FLATBUFFERS_DEBUG_VERIFICATION_FAILURE

#include <include/client/client.hpp>
#include <kproto/types.hpp>


using namespace KData;
using namespace IGData;
using namespace GenericData;

static const uint32_t MAX_PACKET_SIZE = 4096;
static const uint32_t HEADER_SIZE     = 4;

flatbuffers::FlatBufferBuilder builder(1024);

/**
 * @brief getTaskFileInfo
 * @param [in] {std::vector<SentFile>} files The files to produce an information string from
 * @return std::string A string with the following format denoting each file:
 * `1580057341filename|image::`
 */
std::string getTaskFileInfo(std::vector<SentFile> files)
{
  std::string info;
  for (const auto& f : files)
  {
    info += std::to_string(f.timestamp);
    info += f.name.toUtf8().constData();
    info += "|";
    if (f.type == Scheduler::FileType::VIDEO)
      info += "video";
    else
      info += "image";
    info += ":";
  }
  KLOG("File Info: ", info.c_str());
  return info;
}

flatbuffers::Offset<GenericTask> createGenericTask(
    Scheduler::Task* task, uint32_t app_mask, std::vector<SentFile> sent_files) {
  return CreateGenericTask(
    builder,
    Scheduler::GENERIC_TASK_ID, // ID
    builder.CreateString(getTaskFileInfo(sent_files)),
    builder.CreateString(std::string{std::get<QString>(
      task->getTaskArgumentValue("datetime")).toUtf8().constData()
    }),
    builder.CreateString(std::string{std::get<QString>(
      task->getTaskArgumentValue("description")).toUtf8().constData()
    }),
    std::get<bool>(
      task->getTaskArgumentValue("is_video")
    ),
      app_mask,
    builder.CreateString(std::string{std::get<QString>(
      task->getTaskArgumentValue("header")).toUtf8().constData()
    }),
    builder.CreateString(std::string{std::get<QString>(
      task->getTaskArgumentValue("user")).toUtf8().constData()
    }),
    std::get<int>(
      task->getTaskArgumentValue("recurring")
    ),
    std::get<0>(
      task->getTaskArgumentValue("notify")
    ),
    builder.CreateString(std::string{std::get<QString>(
      task->getTaskArgumentValue("runtime_string")).toUtf8().constData()
    })
  );
}

flatbuffers::Offset<IGTask> createIGTask(
    Scheduler::Task* task, uint32_t app_mask, std::vector<SentFile> sent_files) {
  return CreateIGTask(
    builder,
    Scheduler::INSTAGRAM_TASK_ID, // ID
    builder.CreateString(getTaskFileInfo(sent_files)),
    builder.CreateString(std::string{std::get<QString>(
        task->getTaskArgumentValue("datetime")).toUtf8().constData()}),
    builder.CreateString(std::string{std::get<QString>(
        task->getTaskArgumentValue("description")).toUtf8().constData()}),
    builder.CreateString(std::string{std::get<QString>(
        task->getTaskArgumentValue("hashtags_string")).toUtf8().constData()}),
    builder.CreateString(std::string{std::get<QString>(
        task->getTaskArgumentValue("requested_by_string")).toUtf8().constData()}),
    builder.CreateString(std::string{std::get<QString>(
        task->getTaskArgumentValue("requested_by_phrase")).toUtf8().constData()}),
    builder.CreateString(std::string{std::get<QString>(
        task->getTaskArgumentValue("promote_share")).toUtf8().constData()}),
    builder.CreateString(std::string{std::get<QString>(
        task->getTaskArgumentValue("link_in_bio")).toUtf8().constData()}),
    std::get<bool>(
        task->getTaskArgumentValue("is_video")),
        app_mask,
    builder.CreateString(std::string{std::get<QString>(
        task->getTaskArgumentValue("header")).toUtf8().constData()}),
    builder.CreateString(std::string{std::get<QString>(
        task->getTaskArgumentValue("user")).toUtf8().constData()}));
}


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
Client::Client(QWidget* parent, int count, char** arguments)
: QObject(parent),
  argc(count),
  argv(arguments),
  m_client_socket_fd(-1),
  m_outbound_task(nullptr),
  executing(false),
  m_download_console(kiqoder::FileHandler{
    [this](int32_t id, uint8_t* buffer, size_t size) -> void
    {
      KLOG("Download Console received a file. Writing to buffer");
      m_download_console.Write(QString::number(id), buffer, size);
      m_download_console.rx_count++;

      if (--m_download_console.wt_count)
      {
        KLOG("Download Console Requesting next file. Setting Console to WAIT and sending FILE_ACK up the wire");
        m_download_console.Wait();
        sendEncoded(CreateOperation("FILE_ACK", {std::to_string(kiq::Request::RequestType::FETCH_FILE_ACK)}));
      }
      else
      {
        KLOG("Download Console received ALL files. Passing files to callback");
        onDownload(std::move(m_download_console.GetData()));
        m_download_console.Reset();
      }
    }
  }),
  m_server_ip(arguments[1]),
  m_server_port((arguments[2])),
  m_message_decoder(kiqoder::FileHandler{
    [this](int32_t, uint8_t* buffer, size_t size) -> void
    {
      if (!buffer) return;

      std::string message{buffer, buffer + size};
      try
      {
        if (isPong(buffer, size))
        {
          emit Client::messageReceived(PONG_REPLY_TYPE, "Pong");
          VLOG("Pong");
          return;
        }

        if (!isValidJson(message))
        {
          ELOG("Invalid JSON: {}", message);
          return;
        }


        if (isNewSession(message.c_str()))
          emit Client::messageReceived(COMMANDS_UPDATE_TYPE, "New Session", getArgs(message.c_str()));
        else
        if (serverWaitingForFile(message.c_str()))
          processFileQueue();
        else
        if (isEvent(message.c_str()))
        {
          const QString          event = getEvent(message.c_str());
          const QVector<QString> args  = getArgs(message.c_str());
          emit Client::messageReceived(EVENT_UPDATE_TYPE, event, args);
          if (isUploadCompleteEvent(event.toUtf8().constData()))
          {
            if (!args.isEmpty())
            {
              sent_files.at(sent_files.size() - 1).timestamp = std::stoi(args.at(0).toUtf8().constData());
              if (outgoing_files.isEmpty())
              {
                sendTaskEncoded(m_outbound_task);
                file_was_sent = false;
              }
              else
                sendEncoded(CreateOperation("FileUpload", {"Subsequent file"}));
            }
          }
        }        
        emit Client::messageReceived(MESSAGE_UPDATE_TYPE, QString::fromStdString(getJsonString(message)));
      }
      catch (const std::exception& e)
      {
        ELOG("Exception caught decoding message: ", e.what());
      }
    }
  })
{
  qRegisterMetaType<QVector<QString>> ("QVector<QString>");
  qRegisterMetaType<QVector<FileWrap>>("QVector<FileWrap>");
  QObject::connect(&m_network_manager, &QNetworkAccessManager::finished, this, [this](QNetworkReply* reply)
  {
    bool error{false};
    if (reply->error())
    {
      error = true;
      ELOG("Auth server returned error: {}", reply->errorString().toStdString());
    }
    else
    {     
      const auto json = loadJsonConfig(reply->readAll());
      if (!json.empty() && json.contains("token"))
      {        
        m_token = configValue("token", json).toUtf8().constData();

        if (m_reconnect)
          start(m_server_ip, m_server_port);
        else
        if (json.contains("refresh"))
          m_refresh = configValue("refresh", json);
      }
      else
        error = true;
    }
    onTokenReceived(error);
  });
}

void Client::SetCredentials(const QString& username, const QString& password, const QString& auth_address)
{
  m_user            = username;
  m_password        = password;
  m_auth_address    = auth_address + "/login";
  m_refresh_address = auth_address + "/refresh";  
}

QString Client::GetUsername() const
{
  return m_user;
}

void Client::FetchToken(bool reconnect)
{
  KLOG("Fetching token. Reconnect {}", reconnect);
  if (reconnect)
    m_network_manager.get(QNetworkRequest(QUrl(m_refresh_address + "?name=" + m_user + "&token=" + m_refresh)));
  else
    m_network_manager.get(QNetworkRequest(QUrl(m_auth_address    + "?name=" + m_user + "&password=" + m_password)));
  m_reconnect = reconnect;
}

/**
 * @brief Client::~Client
 * @destructor
 */
Client::~Client()
{
  closeConnection();
}

 /**
 * @brief Client::handleMessages
 */
void Client::handleMessages()
{
  uint8_t receive_buffer[MAX_PACKET_SIZE];
  for (;;)
  {
    memset(receive_buffer, 0, MAX_PACKET_SIZE);
    ssize_t bytes_received = recv(m_client_socket_fd, receive_buffer, MAX_PACKET_SIZE, 0);

    if (bytes_received < 1)
      break;

#ifdef DEBUG_CLIENT
    if (isPong(receive_buffer, bytes_received) && m_message_decoder.isPending())
    {
      static auto bad_pongs = 0;
      WLOG("Received PONG but message decoder isn't finished. Ignoring");
      if (++bad_pongs > 10)
      {
        ELOG("Client is in a failed state. {} Bad pongs. Resetting decoder");
        m_message_decoder.reset();
        bad_pongs = 0;
      }
    }
#endif

    if (m_download_console.is_downloading())
      handleDownload(receive_buffer, bytes_received);
    else
      m_message_decoder.processPacket(receive_buffer, bytes_received);
  }
  memset(receive_buffer, 0, MAX_PACKET_SIZE);
  ::close(m_client_socket_fd);
}


/**
 * @brief Client::processFileQueue
 */
void Client::processFileQueue()
{
  Scheduler::KFileData outgoing_file = outgoing_files.dequeue();
  sendFileEncoded(outgoing_file.bytes);
  sent_files.push_back({.name = outgoing_file.name, .type = outgoing_file.type});
}

static const char* DNStoIP(const QString& dns)
{
  const auto start_index = (dns.contains("://")) ? (dns.size() - dns.lastIndexOf('/') - 1) : 0;
  const auto domain      = (start_index)         ? (dns.right(start_index))                : dns;
  struct hostent* he     = gethostbyname(domain.toUtf8());

  if (nullptr != he && nullptr != he->h_addr_list)
    return inet_ntoa(*(struct in_addr*)he->h_addr_list[0]);

  return nullptr;
}

static bool IsIP(const QString& address)
{
  if (address.isEmpty())         return false;
  if (address.front().isDigit()) return true;

  const bool has_https = (address.contains("https://"));
  const bool has_http  = (!has_https) && (address.contains("http://"));

  return (has_https) ? address.at(8).isDigit() :
                       (has_http) ? address.at(7).isDigit() :
                                    false;
}

/**
 * @brief Client::start
 */
void Client::start(QString ip, QString port)
{
  DLOG("start called with address {} and port {}", ip.toStdString(), port.toStdString());

  const QString port_number  = port.isEmpty() ? m_server_port : port;
  const QString host_address = (ip.isEmpty()) ? m_server_ip: ip;

  if (host_address.isEmpty()) return;

  const QString ip_address   = IsIP(host_address) ? host_address : DNStoIP(host_address);
  const char*   server_ip    = ip_address.toUtf8();
  const char*   server_port  = port_number.toUtf8();

  if (ip_address.isEmpty()) return;

  m_server_ip   = host_address;
  m_server_port = port_number;

  m_client_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (m_client_socket_fd != -1)
  {
    sockaddr_in server_socket;
    char*       end;
    auto        port_value = strtol(server_port, &end, 10);

    server_socket.sin_family = AF_INET;

    if (port_value < 0 || end == server_port) return;

    int socket_option = 1;
    // Free up the port to begin listening again
    setsockopt(m_client_socket_fd, SOL_SOCKET, SO_REUSEADDR, &socket_option, sizeof(socket_option));

    server_socket.sin_port = htons(port_value);
    inet_pton(AF_INET, server_ip, &server_socket.sin_addr.s_addr);

    if (::connect(m_client_socket_fd, reinterpret_cast<sockaddr*>(&server_socket),
                  sizeof(server_socket)) != -1)
    {
      sendEncoded(CreateOperation("start", {}));
      std::thread(createMessageHandler([this]() { this->handleMessages(); })).detach(); // TODO: use modern solution
    }
    else
    {
      ELOG("Client failed to connect. Errno {}", errno);
      ::close(m_client_socket_fd);
    }
  }
  else
    ELOG("Close connection before attempting new");
}

/**
 * @brief Client::sendMessage
 * @param [in] {const QString&} The message to send
 */
void Client::sendMessage(const QString& s)
{
  if (m_client_socket_fd != -1)
    sendEncoded(createMessage(s.toUtf8(), "", m_user.toUtf8().constData(), m_token.toUtf8().constData()));
  else
    ELOG("You must first open a connection");

}

/**
 * @brief Client::sendEncoded
 * @param [in] {std::string message} The message to send
 */
void Client::sendEncoded(std::string message)
{
  VLOG("Sending: {}", message);
  auto k_message = CreateMessage(builder, 69, builder.CreateVector(std::vector<uint8_t>{message.begin(), message.end()}));

  builder.Finish(k_message);

  uint8_t* encoded_message_buffer = builder.GetBufferPointer();
  uint32_t size                   = builder.GetSize();

  uint8_t send_buffer[MAX_PACKET_SIZE];
  memset(send_buffer, 0, MAX_PACKET_SIZE);
  send_buffer[0] = (size >> 24)          & 0xFF;
  send_buffer[1] = (size >> 16)          & 0xFF;
  send_buffer[2] = (size >> 8)           & 0xFF;
  send_buffer[3] = size                  & 0xFF;
  send_buffer[4] = (TaskCode::GENMSGBYTE & 0xFF);
  std::memcpy(send_buffer + 5, encoded_message_buffer, size);
  ::send(m_client_socket_fd, send_buffer, size + 5, 0);
  builder.Clear();
}

/**
 * @brief Client::sendTaskEncoded
 * @param [in] {Scheduler::Task*} task The task arguments
 */
void Client::sendTaskEncoded(Scheduler::Task* task)
{
  if (task->getType() == Scheduler::TaskType::INSTAGRAM)
  {
    flatbuffers::Offset<IGTask> ig_task =
    createIGTask(task, getSelectedApp(), sent_files);
    builder.Finish(ig_task);
  }
  else
  {
    flatbuffers::Offset<GenericTask> generic_task =
    createGenericTask(task, getSelectedApp(), sent_files);
    builder.Finish(generic_task);
  }

  uint8_t* encoded_message_buffer = builder.GetBufferPointer();
  uint32_t size = builder.GetSize();
  uint8_t send_buffer[MAX_PACKET_SIZE];

  memset(send_buffer, 0, MAX_PACKET_SIZE);

  send_buffer[0] = (size >> 24)         & 0xFF;
  send_buffer[1] = (size >> 16)         & 0xFF;
  send_buffer[2] = (size >> 8)          & 0xFF;
  send_buffer[3] = size                 & 0xFF;
  send_buffer[4] = (task->getTaskCode() & 0xFF);

  std::memcpy(send_buffer + 5, encoded_message_buffer, size);
  VLOG("Ready to send task");

  ::send(m_client_socket_fd, send_buffer, size + 5, 0);

  builder.Clear();
  sent_files.clear();
  m_outbound_task = nullptr;
  if (!m_task_queue.isEmpty())
  {
    m_outbound_task = m_task_queue.dequeue();

    if (m_outbound_task->hasFiles() && !outgoing_files.empty())
    {
      DLOG("Deleting unsent files from last task");
      outgoing_files.clear();
    }
    sendFiles(m_outbound_task);
  }
}

/**
 * @brief Client::sendPackets
 * @param [in] {uint8_t*} data A pointer to a buffer of bytes
 * @param [in] {int}      size The size of the buffer to be packetized and sent
 */
void Client::sendPackets(uint8_t* data, uint32_t size)
{
  uint32_t total_size    = static_cast<uint32_t>(size + HEADER_SIZE);
  uint32_t total_packets = static_cast<uint32_t>(ceil(static_cast<double>(
        static_cast<double>(total_size) / static_cast<double>(MAX_PACKET_SIZE))));

  for (uint32_t idx = 0; idx < total_packets; idx++)
  {
    bool is_first_packet = (idx == 0);
    bool is_last_packet  = (idx == (total_packets - 1));

    if (is_first_packet)
    {
      uint32_t first_packet_size = std::min(size + HEADER_SIZE, MAX_PACKET_SIZE);
      uint8_t  packet[first_packet_size];
      packet[0] = (total_size >> 24) & 0xFF;
      packet[1] = (total_size >> 16) & 0xFF;
      packet[2] = (total_size >> 8)  & 0xFF;
      packet[3] = (total_size)       & 0xFF;

      std::memcpy(packet + HEADER_SIZE, data, first_packet_size - HEADER_SIZE);

      ::send(m_client_socket_fd, packet, first_packet_size, 0);

      if (is_last_packet)
        break;
      continue;
    }

    uint32_t offset      = (idx * MAX_PACKET_SIZE) - HEADER_SIZE;
    uint32_t packet_size = std::min(size - offset, MAX_PACKET_SIZE);
    uint8_t  packet[packet_size];

    std::memcpy(packet, data + offset, packet_size);
    ::send(m_client_socket_fd, packet, packet_size, 0);

    if (is_last_packet)
      file_was_sent = true;
  }
}


void Client::ping()
{
  if ((outgoing_files.isEmpty() || file_was_sent) && !(m_fetching))
  {
    uint8_t send_buffer[5];
    memset(send_buffer, 0, 5);
    send_buffer[4] = (TaskCode::PINGBYTE & 0xFF);
    ::send(m_client_socket_fd, send_buffer, 5, 0);
  }
}

/**
 * @brief Client::sendFileEncoded
 * @param [in] {QByteArray} bytes An array of bytes to send
 */
void Client::sendFileEncoded(QByteArray bytes)
{
  sendPackets(reinterpret_cast<uint8_t*>(bytes.data()), bytes.size());
}

/**
 * @brief Client::closeConnection
 */
void Client::closeConnection()
{
  if (m_client_socket_fd != -1)
  {
    sendEncoded(CreateOperation("stop", {}));
    ::shutdown(m_client_socket_fd, SHUT_RDWR);
    ::close(m_client_socket_fd);
    m_client_socket_fd = -1;
    return;
  }
  KLOG("There is no active connection to close");
}

/**
 * @brief Client::setSelectedApp
 * @param [in] TYPE SHOULD CHANGE app_names
 */
void Client::setSelectedApp(const std::vector<QString>& app_names)
{
  selected_commands.clear();
  for (const auto& name : app_names)
    for (const auto& command : m_commands)
      if (command.name == name)
        selected_commands.push_back(command.mask.toInt());
}


/**
 * @brief Client::getSelectedApp
 * @returns {int} The mask representing the selected application
 */
int Client::getSelectedApp() const
{
  if (selected_commands.size() == 1)
    return selected_commands.at(0);
  else
  {
    QMessageBox m_box{};
    m_box.setText("App Selection Error. Unable to retrieve app selection");
    m_box.exec();
  }
  return -1;
}

/**
 * @brief Client::getAppName
 * @param [in] {int} mask The mask representing the application
 * @returns {QString} The application name
 */
QString Client::getAppName(int mask)
{
  for (const auto& command : m_commands)
    if (command.mask.toInt() == mask)
      return command.name;
  return "";
}

/**
 * @brief Client::execute
 */
void Client::execute()
{
  using Payload = std::vector<std::string>;
  auto MakeUUID = []() { return QUuid::createUuid().toString(QUuid::StringFormat::WithoutBraces); };
  static const auto request = std::to_string(kiq::Request::RequestType::EXECUTE_PROCESS);

  if (!selected_commands.empty())
  {
    executing = true;
    for (const auto& command : selected_commands)
    {
      const auto app_name          = getAppName(command);
      const auto message           = app_name + " pending";
      const auto request_id        = MakeUUID();
      const auto payload           = Payload{request, std::to_string(command),  std::string(request_id.toUtf8().constData())};
      const auto execute_operation = CreateOperation("Execute", payload);

      emit Client::messageReceived(PROCESS_REQUEST_TYPE, message, {QString{command}, app_name, request_id});
      sendEncoded(execute_operation);
    }
  }
}

/**
 * @brief Client::scheduleTask
 * @param [in] {std::vector<std::string>} task_args The task arguments
 * @param [in] {bool}                     file_pending A boolean indicating whether there are
 *                                        files being sent for this task
 */
void Client::scheduleTask(Scheduler::Task* task)
{
  if (m_outbound_task == nullptr)
  {
    m_outbound_task = std::move(task);
    if (m_outbound_task->hasFiles())
      sendFiles(m_outbound_task);
    else
    {
      KLOG("Requesting a task to be scheduled");
      sendTaskEncoded(m_outbound_task);
    }
  }
  else
    m_task_queue.enqueue(task);
}

/**
 * @brief Client::sendFiles
 *
 * @param [in] {Task*}    A pointer to the task with files to be sent
 */
void Client::sendFiles(Scheduler::Task* task)
{
  if (outgoing_files.isEmpty())
  {
    file_was_sent = false;
    for (const auto& file : task->getFiles())
      outgoing_files.enqueue(std::move(file));
    std::string send_file_operation = CreateOperation("FileUpload", {});
    sendEncoded(send_file_operation);
  }
  else
  {
    m_task_queue.enqueue(task);
    KLOG("Still attempting to send a different file");
  }
}

/**
 * @brief Client::appRequest
 *
 * @param [in] {KApplication}  application
 * @param [in] {uint8_t}       request_code
 */
void Client::appRequest(KApplication application, uint8_t request_code)
{
  std::vector<std::string> operation_args{
      std::to_string(request_code),
      application.name.toUtf8().constData(),
      application.path.toUtf8().constData(),
      application.data.toUtf8().constData(),
      application.mask.toUtf8().constData()};
  std::string operation_string = CreateOperation("AppRequest", operation_args);

  sendEncoded(operation_string);
}

/**
 * @brief Client::request
 *
 * @param [in] {uint8_t}       request_code
 * @param [in] {T}             payload
 */
template <typename T>
void Client::request(uint8_t request_code, T payload)
{
  using namespace constants;
  using namespace kiq::Request;
  try
  {
    std::string operation_string{};

    switch (request_code)
    {
      case (RequestType::REGISTER_APPLICATION):
      case (RequestType::REMOVE_APPLICATION):
        if constexpr (std::is_same_v<T, KApplication>)
          operation_string = CreateOperation("AppRequest", {
            std::to_string(request_code),
            payload.name.toUtf8().constData(),
            payload.path.toUtf8().constData(),
            payload.data.toUtf8().constData(),
            payload.mask.toUtf8().constData()});
        else
          throw std::invalid_argument{"Register payload must be KApplication object"};
      break;
      case (RequestType::FETCH_SCHEDULE):
        operation_string = CreateOperation("Schedule", {std::to_string(RequestType::FETCH_SCHEDULE)});
      break;
      case (RequestType::UPDATE_SCHEDULE):
      case (RequestType::FETCH_SCHEDULE_TOKENS):
        if constexpr (std::is_same_v<T, ScheduledTask>)
          operation_string = CreateOperation(
            "Schedule", std::vector<std::string>{
               std::to_string(request_code),
               payload.id.toUtf8().constData(),
               payload.app.toUtf8().constData(),
               std::to_string(payload.time.toTime_t()),
               payload.flags.toUtf8().constData(),
               payload.completed.toUtf8().constData(),
               payload.recurring.toUtf8().constData(),
               payload.notify.toUtf8().constData(),
               payload.runtime.toUtf8().constData(),
               (payload.files.isEmpty()) ? "" :
                                           payload.files.front().toUtf8().constData(),
               payload.envfile.toUtf8().constData()});
      break;
      case (RequestType::TASK_FLAGS):
        operation_string = CreateOperation("TaskOperation",
          std::vector<std::string>{std::to_string(request_code), std::to_string(getSelectedApp())});
      break;
    case (RequestType::FETCH_FILE):
      if constexpr (std::is_same_v<T, QVector<QString>>)
        operation_string = CreateOperation("FetchFileOperation", ArgsToV(payload, request_code));
    break;
    case (RequestType::FETCH_POSTS):
      if constexpr (std::is_same_v<T, std::vector<std::string>>)
        operation_string = CreateOperation("FetchPosts", {std::to_string(request_code)});
    break;
    case (RequestType::FETCH_TERM_HITS):
        operation_string = CreateOperation("FetchTermHits", {std::to_string(request_code)});
    break;
      case (RequestType::FETCH_TASK_DATA):
        if constexpr (std::is_same_v<T, QVector<QString>>)
        {
          std::vector<std::string> op_payload{};
          op_payload.reserve((payload.size() + 2));
          op_payload.emplace_back(std::to_string(request_code));
          op_payload.emplace_back(std::to_string(getSelectedApp()));
          for (const QString& chunk : payload) op_payload.emplace_back(chunk.toStdString());
          operation_string = CreateOperation("FetchTaskData", op_payload);
        }
      break;
    case (RequestType::UPDATE_POST):
      if constexpr (std::is_same_v<T, QVector<QString>>)
      {
        std::vector<std::string> op_payload{std::to_string(request_code)};
        for (const auto& arg : payload)
          op_payload.push_back(arg.toUtf8().constData());
        operation_string = CreateOperation("UpdatePost", op_payload);
      }
    break;
    case (RequestType::KIQ_STATUS):
      operation_string = CreateOperation("StatusReport", {std::to_string(request_code)});
    break;
    case (RequestType::CONVERT_TASK):
      if constexpr (std::is_same_v<T, std::pair<std::string, std::string>>)
      {
        std::vector<std::string> op_payload{std::to_string(request_code)};
        op_payload.push_back(payload.first);
        op_payload.push_back(payload.second);
        operation_string = CreateOperation("Convert Task", op_payload);
      }
    break;
    default:
      qDebug() << "Client is unable to process request";
      return;
    }
    sendEncoded(operation_string);
  }
  catch (const std::exception& e)
  {
    KLOG("Exception caught:\n", e.what());
  }
}

template void Client::request(uint8_t request_code, ScheduledTask    payload);
template void Client::request(uint8_t request_code, KApplication     payload);
template void Client::request(uint8_t request_code, uint32_t         payload);
template void Client::request(uint8_t request_code, QVector<QString> payload);
template void Client::request(uint8_t request_code, QString          payload);
template void Client::request(uint8_t request_code, std::pair<std::string, std::string> payload);


/**
 * @brief Client::request
 *
 * Generic interface
 *
 * @param request_code
 */
void Client::request(uint8_t request_code)
{
  request(request_code, std::vector<std::string>{});
}

/**
 */
void Client::sendIPCMessage(const QString& type, const QString& platform, const QString& data, const QString& cmd, const QString& time, const QString& recur)
{
  sendEncoded(CreateOperation("ipc", {
    QString::number(kiq::Request::RequestType::IPC_REQUEST).toUtf8().constData(),
    type.toUtf8().constData(),
    platform.toUtf8().constData(),
    data.toUtf8().constData(),
    cmd.toUtf8().constData(),
    time.toUtf8().constData(),
    recur.toUtf8().constData()
  }));
}

/**
 * @brief Client::setIncomingFile
 * @param files
 */
void Client::setIncomingFile(const StringVec& files)
{
  if (m_download_console.is_downloading())
    return KLOG("Client already has incoming pending. Ignoring request.");

  m_download_console.files.clear();
  m_download_console.wt_count = files.front().toUInt();

  for (int32_t i = 0; i < m_download_console.wt_count; i++)
    m_download_console.files.push_back(FileWrap{.task_id = files[1 + (4 * i)],
                                                .id      = files[2 + (4 * i)],
                                                .name    = files[3 + (4 * i)],
                                                .type    = files[4 + (4 * i)],
                                                .buffer  = nullptr});
  m_download_console.Wait();
  sendEncoded(CreateOperation("FILE_ACK", {std::to_string(kiq::Request::RequestType::FETCH_FILE_ACK)}));
}

void Client::setMetadata(const StringVec& data)
{
  if (m_download_console.SetMetadata(data))
    sendEncoded(CreateOperation("FILE_READY", {std::to_string(kiq::Request::RequestType::FETCH_FILE_READY)}));
}

void Client::handleDownload(uint8_t* data, ssize_t size)
{
  if (m_download_console.Waiting())
    m_message_decoder.processPacket(data, size);
  else
    m_download_console.Receive(data, size);
}

void Client::setCommands(Commands commands)
{
  using Pair = std::pair<int32_t, std::string>;
  if (!commands.empty() && selected_commands.empty())
    selected_commands = {commands.front().mask.toInt()};
  m_commands = commands;
  for (const auto& command : commands)
    m_command_map.insert(Pair{command.mask.toInt(), command.name.toStdString()});
}

bool Client::hasApp(KApplication application)
{
  for (const auto& app : m_commands)
    if (app.name == application.name) return true;
  return false;
}

std::string Client::CreateOperation(const char* op, std::vector<std::string> args)
{
  return createOperation(op, args, m_user.toUtf8().constData(), m_token.toUtf8().constData());
}

void Client::reconnect()
{
  DLOG("Client::reconnect()");
  m_message_decoder.reset();
  m_download_console.Reset();
  FetchToken(true);
}
