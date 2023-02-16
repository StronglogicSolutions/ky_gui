#pragma once

#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QString>
#include <QMessageBox>
#include <QDateTime>
#include <QVector>
#include <string>
#include <vector>
#include "rapidjson/document.h"
#include "rapidjson/pointer.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include "logger.hpp"

template <>
struct fmt::formatter<QString>
{
  constexpr auto parse(format_parse_context& ctx) -> decltype(ctx.begin())
  {
    return ctx.begin();
  }

  template <typename FormatContext>
  auto format(const QString& s, FormatContext& ctx) const -> decltype(ctx.out())
  {
    return fmt::format_to(ctx.out(), "{}", s);
  }
};

struct KApplication {  
  QString name;
  QString path;
  QString data;
  QString mask;
};

struct ScheduledTask {
  QString          id;
  QString          app;
  QDateTime        time;
  QString          flags;
  QString          completed;
  QString          recurring;
  QString          notify;
  QString          runtime;
  QVector<QString> files;
  QString          envfile;

  bool operator==(const ScheduledTask& task){ return this->id.toUInt() == task.id.toUInt(); }

};

struct FileWrap
{
  QString    task_id;
  QString    id;
  QString    name;
  QString    type;
  QByteArray buffer;

  bool HasID() const;
};

namespace constants {
enum RequestType {
REGISTER              = 0x00,
UPDATE                = 0x01,
DELETE                = 0x02,
GET                   = 0x03,
FETCH_SCHEDULE        = 0x04,
UPDATE_SCHEDULE       = 0x05,
FETCH_SCHEDULE_TOKENS = 0x06,
TRIGGER_CREATE        = 0x07,
FETCH_TASK_FLAGS      = 0x08,
FETCH_FILE            = 0x09,
FETCH_FILE_ACK        = 0x0A,
FETCH_FILE_READY      = 0x0B,
FETCH_TASK_DATA       = 0x0C,
FETCH_TERM_HITS       = 0x13,
EXECUTE_PROCESS       = 0x14,
FETCH_POSTS           = 0x15,
UPDATE_POST           = 0x16
};

static const uint8_t SCHEDULED_TASK_ID_INDEX        = 0x00;
static const uint8_t SCHEDULED_TASK_NAME_INDEX      = 0x01;
static const uint8_t SCHEDULED_TASK_TIME_INDEX      = 0x02;
static const uint8_t SCHEDULED_TASK_FLAGS_INDEX     = 0x03;
static const uint8_t SCHEDULED_TASK_COMPLETED_INDEX = 0x04;
static const uint8_t SCHEDULED_TASK_RECURRING_INDEX = 0x05;
static const uint8_t SCHEDULED_TASK_NOTIFY_INDEX    = 0x06;
static const uint8_t SCHEDULED_TASK_RUNTIME_INDEX   = 0x07;
static const uint8_t SCHEDULED_TASK_FILES_INDEX     = 0x08;

static const uint8_t TASK_ID_INDEX       {0x00};
static const uint8_t TASK_TIME_INDEX     {0x01};
static const uint8_t TASK_FLAGS_INDEX    {0x02};
static const uint8_t TASK_COMPLETED_INDEX{0x03};
static const uint8_t TASK_RECURRING_INDEX{0x04};
static const uint8_t TASK_NOTIFY_INDEX   {0x05};
static const uint8_t TASK_RUNTIME_INDEX  {0x06};
static const uint8_t TASK_FILES_INDEX    {0x07};

}

using namespace rapidjson;

typedef std::string KOperation;

typedef std::vector<std::pair<std::string, std::string>> TupVec;
typedef std::vector<std::map<int, std::string>> MapVec;
typedef std::map<int, std::string> CommandMap;

struct KSession {
    int id;
    int fd;
    int status;
};

extern QString escapeText(QString s);
extern QString escapeMessage(QString s);
extern QString escapeTextToRaw(QString s);

QString configValue(const QString& key, const QJsonObject& config, bool use_default = false);
QList<QString> configValueToQList(const QString& key, const QJsonObject& config);
QList<QString> configUsers(const QString& section, const QJsonObject& config);
QString defaultConfigUser(const QJsonObject& config);
QJsonObject configObject(QString key, QJsonObject config, bool use_default = false);
QJsonObject loadJsonConfig(QString json_string);

bool configBoolValue(QString key, QJsonObject config);

std::string getJsonString(std::string s);
std::string createMessage(const char* data, std::string args = "", const char* user = nullptr, const char* token = nullptr);
bool isOperation(const char* data);

bool isUploadCompleteEvent(const char* event);

bool isUploadCompleteEvent(const QString& s);

bool isValidJson(const QString& s);

bool isValidJson(const std::string& s);

bool isEvent(const char* data);

bool isSchedule(const char* data);

template <typename T>
bool isKEvent(T event, const char* kEvent)
{
  if constexpr      (std::is_same_v<T, std::string>)
    return strcmp(event.c_str(), kEvent) == 0;
  else if constexpr (std::is_same_v<T, QString>)
    return strcmp(event.toUtf8(), kEvent) == 0;
  else
    return strcmp(event, kEvent) == 0;
}

bool isPong(const char* data);

bool isPong(const uint8_t* bytes, size_t size);
// TODO: This should be "message", no?
bool isMessage(const char* data);

template <typename T, typename P>
std::vector<std::string> ArgsToV(QVector<T> args, P arg);

//template std::vector<std::string> ArgsToV(QVector<QString>, uint8_t);

std::string createOperation(const char* op, std::vector<std::string> args, const char* name = nullptr, const char* token = nullptr);

std::string getOperation(const char* data);

QString getOperation(const QString& data);

QString getEvent(const char* data);

QString getEvent(const QString& data);

QString getMessage(const char* data);

QString getMessage(const QString& data);

QVector<QString> getArgs(const char* data);

QList<QString> getValueArgs(const char* data, QString key);

CommandMap getArgMap(const char* data);

bool isStartOperation(const char* data);

bool isStopOperation(const char* data);

bool isNewSession(const char* data);

bool serverWaitingForFile(const char* data);


namespace FileUtils {
[[ maybe_unused ]]
extern QString generatePreview(QString video_path, QString video_name);

[[ maybe_unused ]]
QString padVideo(const QString& input, const QString& name, const QString& path, const QString& w = "1350", const QString& h = "1080");
}; // namespace FileUtils

namespace UI {
extern void infoMessageBox(QString text, QString title = "KYGUI");

} // namespace UI
namespace TimeUtils {
[[maybe_unused]]
extern QString getTime();
[[maybe_unused]]
extern uint    unixtime();
} // namespace TimeUtils
