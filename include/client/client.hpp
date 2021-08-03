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

typedef QVector<KApplication> Commands;
typedef std::map<int, std::vector<std::string>> CommandArgMap;
typedef QVector<QString> StringVec;

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

  DownloadConsole(Kiqoder::FileHandler f_handler)
  : handler(f_handler) {}

  bool is_downloading()
  {
    return (wt_count == 0);
  }

  void Write(const int32_t id, uint8_t* data, const size_t size)
  {
    files.push_back(FileWrap{.id = id, .buffer = QByteArray{reinterpret_cast<char*>(data), static_cast<int>(size)}});
  }
};

Q_DECLARE_METATYPE(StringVec)
Q_DECLARE_METATYPE(QVector<QByteArray>);

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
  void           setCommands(Commands commands) {
    if (!commands.empty() && selected_commands.empty()) {
      auto first_command = commands.front();
      selected_commands = {first_command.mask.toInt()};
    }
    m_commands = commands;
  }
  void           addCommand(KApplication command)
  {
    m_commands.append(command);
  }
  void           sendFiles(Scheduler::Task* task);
  void           ping();  
  void           sendIPCMessage(const QString& type, const QString& message, const QString& user);
  void           setIncomingFile(const StringVec& files);
  void           setIncomingID(const QString& id);

 signals:
  void           messageReceived(int t, QString s, QVector<QString> args);
  void           eventReceived(int t, std::string event, StringVec args);
  void           clientDisconnected();
  void           onDownload(const QVector<FileWrap>& files);

 private:
  void           sendEncoded(std::string message);
  void           sendFileEncoded(QByteArray bytes);
  void           sendTaskEncoded(Scheduler::Task* task);
  void           processFileQueue();
  void           handleMessages();
  void           handleEvent(std::string data);
  void           handleDownload(uint8_t* data, ssize_t size);
  void           sendPackets(uint8_t* data, uint32_t size);

  int                           argc;
  char**                        argv;
  int                           m_client_socket_fd;
  Task*                         m_outbound_task;
  bool                          executing;
  bool                          file_was_sent;
  Commands                      m_commands;
  CommandArgMap                 m_command_arg_map;
  std::vector<int>              selected_commands;
  QQueue<Scheduler::KFileData>  outgoing_files;
  DownloadConsole               m_download_console;
  std::vector<SentFile>         sent_files;
  Scheduler::TaskQueue          m_task_queue;
  QString                       m_server_ip;
  QString                       m_server_port;
};
#endif // CLIENT_HPP
