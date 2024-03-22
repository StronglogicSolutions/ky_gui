#pragma once

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

#include <QUuid>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

#include <headers/kmessage_codec.hpp>
#include <headers/instatask_generated.h>
#include <headers/generictask_generated.h>
#include "console.hpp"
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

Q_DECLARE_METATYPE(StringVec)
Q_DECLARE_METATYPE(QVector<QByteArray>)

class Client : public QObject
{
Q_OBJECT
public:
class MessageHandler
{
 public:
  MessageHandler(std::function<void()> cb)
  : m_cb(cb) {}

  void operator()() { m_cb(); }

 private:
  std::function<void()> m_cb;
};

explicit Client(QWidget* parent, int count, char** arguments);
~Client();

void           start(QString ip = "", QString port = "");
void           closeConnection();
void           execute();
QString        getAppName(int mask);
bool           hasApp(KApplication application);
template <typename T>
void           request(uint8_t request_code, T payload);
void           request(uint8_t request_code);
void           appRequest(KApplication application, uint8_t request_code);
int            getSelectedApp() const;
void           scheduleTask(Scheduler::Task* task);
MessageHandler createMessageHandler(std::function<void()> cb);

public slots:
void           sendMessage(const QString& s);
void           setSelectedApp(const std::vector<QString>& app_names);
void           setCommands(Commands commands);
CommandMap     GetCommands()                    { return m_command_map; }
void           addCommand(KApplication command) { m_commands.append(command); }
void           sendFiles(Scheduler::Task* task);
void           ping();
void           sendIPCMessage(const QString& type, const QString& platform, const QString& data, const QString& cmd, const QString& time, bool recurring);
void           setIncomingFile(const StringVec& files);
void           setMetadata(const QVector<QString>& data);
void           SetFetching(bool fetching = true) { m_fetching = fetching; }
void           SetCredentials(const QString& username, const QString& password, const QString& auth_address);
QString        GetUsername() const;
void           reconnect();
void           FetchToken(bool reconnect = false);

signals:
void           messageReceived(int t, QString s, QVector<QString> args = {});
void           eventReceived(int t, std::string event, StringVec args);
void           clientDisconnected();
void           onDownload(QVector<FileWrap> files);
void           onTokenReceived(bool error = false);

private:
void           sendEncoded(std::string message);
void           sendFileEncoded(QByteArray bytes);
void           sendTaskEncoded(Scheduler::Task* task);
void           processFileQueue();
void           handleMessages();
//void           handleEvent(std::string data);
void           handleDownload(uint8_t* data, ssize_t size);
void           sendPackets(uint8_t* data, uint32_t size);
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
kiqoder::FileHandler          m_message_decoder;
bool                          m_fetching;
QString                       m_user;
QString                       m_password;
QString                       m_token;
QString                       m_refresh;
QString                       m_auth_address;
QString                       m_refresh_address;
QNetworkAccessManager         m_network_manager;
bool                          m_reconnect{false};
};
