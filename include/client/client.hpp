#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <math.h>
#include <netdb.h>
#include <cstring>
#include <QByteArray>
#include <algorithm>
#include <functional>
#include <thread>
#include <utility>

#include <QComboBox>
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QMetaType>
#include <QPushButton>
#include <QThread>
#include <QUuid>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

#include <headers/kmessage_codec.hpp>
#include <headers/instatask_generated.h>
#include <headers/generictask_generated.h>
#include <headers/util.hpp>
#include <third_party/kiqoder/kiqoder.hpp>

#include <include/task/task.hpp>

static constexpr int MESSAGE_UPDATE_TYPE  = 1;
static constexpr int COMMANDS_UPDATE_TYPE = 2;
static constexpr int EVENT_UPDATE_TYPE    = 3;
static constexpr int PROCESS_REQUEST_TYPE = 4;
static constexpr int SCHEDULED_TASKS_TYPE = 5;
static constexpr int PONG_REPLY_TYPE      = 6;

using namespace Scheduler;

namespace TaskCode {
static constexpr int IGTASKBYTE  = 0xFF;
static constexpr int GENMSGBYTE  = 0xFE;
static constexpr int GENTASKBYTE = 0xFC;
static constexpr int PINGBYTE    = 0xFD;
}  // namespace TaskCode

using Commands   = QVector<KApplication>;
using StringVec  = QVector<QString>;

struct SentFile {
  int                 timestamp;
  QString             name;
  Scheduler::FileType type;
};

struct DownloadConsole
{
  using Files = QVector<FileWrap>;
  Kiqoder::FileHandler handler;
  int32_t              wt_count;
  int32_t              rx_count;
  Files                files;
  bool                 wt_for_metadata;

  DownloadConsole(Kiqoder::FileHandler f_handler)
  : handler(f_handler)
  {
    Reset();
  }

  FileWrap* GetFile (const QString& id)
  {
    for (auto it = files.begin(); it != files.end(); it++)
      if (it->id == id)
        return it;
    return nullptr;
  };

  bool WaitingForFile(const QString& id)
  {
    FileWrap* file = GetFile(id);
    return (file != nullptr && file->buffer == nullptr);
  }

  bool is_downloading()
  {
    return wt_count;
  }

  void Write(const QString& id, uint8_t* data, const size_t size)
  {
    FileWrap* file = GetFile(id);
    if (file != nullptr && file->HasID())
      file->buffer = QByteArray(reinterpret_cast<char*>(data), size);
    else
      KLOG("Failed to write incoming data");
  }

  void Receive(uint8_t* data, const size_t size)
  {
    handler.processPacket(data, size);
  }

  Files&& GetData()
  {
    return std::move(files);
  }

  void Wait(const bool wait = true)
  {
    wt_for_metadata = wait;
  }

  bool SetMetadata(const QVector<QString>& data)
  {
    static const int32_t FILE_ID_INDEX{1};
    const  auto&         id = data[FILE_ID_INDEX];
    if (WaitingForFile(id))
    {
      Wait(false);
      handler.setID(id.toUInt());
      KLOG("Set metadata");
      return true;
    }
    KLOG("Failed to set metadata");
    return false;
  }

  bool Waiting() { return wt_for_metadata; }

  void Reset()
  {
    wt_count        = 0;
    rx_count        = 0;
    wt_for_metadata = false;
  }
};

Q_DECLARE_METATYPE(StringVec)
Q_DECLARE_METATYPE(QVector<QByteArray>)

class Client : public QDialog {
  Q_OBJECT
  QThread workerThread;

 public:
  class MessageHandler {
   public:
    MessageHandler(std::function<void()> cb) : m_cb(cb) {}

    void operator()() { m_cb(); }

   private:
    std::function<void()> m_cb;
  };

  Client(QWidget* parent = nullptr);
  Client(QWidget* parent, int count, char** arguments);
  ~Client();

  void           start(QString ip = "", QString port = "");
  void           closeConnection();
  void           execute();
  QString        getAppName(int mask);
  bool           hasApp(KApplication application) {
    for (const auto& app : m_commands) {
      if (app.name == application.name) return true;
    }
    return false;
  }
  template <typename T>
  void           request(uint8_t request_code, T payload);
  void           request(uint8_t request_code);
  void           appRequest(KApplication application, uint8_t request_code);
  int            getSelectedApp();
  // Move this to private after moving responsibilities to Client
  void           scheduleTask(Scheduler::Task* task);
  MessageHandler createMessageHandler(std::function<void()> cb);

 public slots:
  void           sendMessage(const QString& s);
  void           setSelectedApp(std::vector<QString> app_names);
  void           setCommands(Commands commands);
  CommandMap     GetCommands() { return m_command_map; }
  void           addCommand(KApplication command)
  {
    m_commands.append(command);
  }
  void           sendFiles(Scheduler::Task* task);
  void           ping();  
  void           sendIPCMessage(const QString& type, const QString& message, const QString& user);
  void           setIncomingFile(const StringVec& files);
  void           setMetadata(const QVector<QString>& data);
  void           SetFetching(bool fetching = true) { m_fetching = fetching; }
  void           SetCredentials(const QString& username, const QString& password, const QString& auth_address);

 signals:
  void           messageReceived(int t, QString s, QVector<QString> args);
  void           eventReceived(int t, std::string event, StringVec args);
  void           clientDisconnected();
  void           onDownload(QVector<FileWrap> files);

 private:
  void           sendEncoded(std::string message);
  void           sendFileEncoded(QByteArray bytes);
  void           sendTaskEncoded(Scheduler::Task* task);
  void           processFileQueue();
  void           handleMessages();
  void           handleEvent(std::string data);
  void           handleDownload(uint8_t* data, ssize_t size);
  void           sendPackets(uint8_t* data, uint32_t size);  
  void           GetToken();
  std::string    CreateOperation(const char* op, std::vector<std::string> args);

  int                           argc;
  char**                        argv;
  int                           m_client_socket_fd;
  Task*                         m_outbound_task;
  bool                          executing;
  bool                          file_was_sent;
  Commands                      m_commands;
  CommandMap                    m_command_map;
  std::vector<int>              selected_commands;
  QQueue<Scheduler::KFileData>  outgoing_files;
  DownloadConsole               m_download_console;
  std::vector<SentFile>         sent_files;
  Scheduler::TaskQueue          m_task_queue;
  QString                       m_server_ip;
  QString                       m_server_port;
  Kiqoder::FileHandler          m_message_decoder;
  bool                          m_fetching;
  QString                       m_user;
  QString                       m_password;
  QString                       m_token;
  QString                       m_auth_address;
  QNetworkAccessManager         m_network_manager;
};
#endif // CLIENT_HPP
