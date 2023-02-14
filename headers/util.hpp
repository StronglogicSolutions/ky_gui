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

  bool HasID() { return !(id.isEmpty()); }
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

namespace {
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

static QString escapeText(QString s) {
  s.replace("\t", "\\t");
  s.replace('"', "\\\"");
  s.replace("'", "'\"\'\"'");
  return s;
}

static QString escapeMessage(QString s) {
  s.replace("\t", "\\t");
  s.replace('"', "\\\"");
  s.replace("'", "\'");
  return s;
}

static QString escapeTextToRaw(QString s) {
    return escapeText(s).toUtf8().constData();
}

QString configValue(QString key, QJsonObject config, bool use_default = false) {
  if (!config.contains(key) && use_default) {
    if (config.contains("default")) {
      return config.value("default").toString();
    }
  } else {
    return config.value(key).toString();
  }
  return "";
}

QList<QString> configValueToQList(QString key, QJsonObject config) {
  QList<QString> list;
  if (config.contains(key)) {    
    if (const auto value = config.value(key); value.isArray())
      for (auto && item : value.toArray())
        list.append(item.toString());          
  }
  return list;
}

QList<QString> configUsers(QString section, QJsonObject config)
{
  QList<QString> list;
  if (config.contains(section))
  {    
    if (const auto sub_object = config.value(section).toObject(); sub_object.contains("users"))
      for (auto&& item : sub_object.value("users").toArray())
        list.append(item.toString());    
  }
  return list;
}

QString defaultConfigUser(QJsonObject config)
{
  auto users = configUsers("default", config);
  return users.isEmpty() ? "" : users.front();
}

QJsonObject configObject(QString key, QJsonObject config, bool use_default = false)
{
  auto key_value = key.toLower();
  if (!config.contains(key_value) && use_default) {
    if (config.contains("default")) {
      return config["default"].toObject();
    }
  } else {
    return config[key_value].toObject();
  }
  qDebug() << "Returning empty QJsonObject :(";
  return QJsonObject{};
}

QJsonObject loadJsonConfig(QString json_string)
{
  return QJsonDocument::fromJson(json_string.toUtf8()).object();
}

bool configBoolValue(QString key, QJsonObject config)
{
  if (config.contains(key)) {
    return bool{config.value(key).toString().compare("true") == 0};
  }
  return false;
}

std::string getJsonString(std::string s)
{
    Document d;
    d.Parse(s.c_str());
    StringBuffer buffer;
    PrettyWriter<StringBuffer> writer(buffer);
    d.Accept(writer);
    return buffer.GetString();
}

std::string createMessage(const char* data, std::string args = "", const char* user = nullptr, const char* token = nullptr)
{
    StringBuffer s;
    Writer<StringBuffer> w(s);
    w.StartObject();
    w.Key("type");
    w.String("custom");
    w.Key("message");
    w.String(data);
    w.Key("user");
    w.String(user);
    w.Key("token");
    w.String(token);
    w.Key("args");
    w.String(args.c_str());
    w.EndObject();
    return s.GetString();
}

bool isOperation(const char* data)
{
    Document d;
    d.Parse(data);
    if (!d.Parse(data).HasParseError())
      return strcmp(d["type"].GetString(), "operation") == 0;
    return false;
}

bool isUploadCompleteEvent(const char* event) 
{
  return strcmp(event, "File Transfer Complete") == 0;
}

bool isUploadCompleteEvent(const QString& s) 
{
  return s == "File Transfer Complete";
}

bool isValidJson(const QString& s) 
{
  return !(Document{}.Parse(s.toUtf8().constData()).HasParseError());
}

bool isValidJson(const std::string& s) 
{
  return !(Document{}.Parse(s.c_str()).HasParseError());
}

bool isEvent(const char* data) 
{  
  if (Document d; !d.Parse(data).HasParseError() && d.HasMember("type"))
    return strcmp(d["type"].GetString(), "event") == 0;
  return false;
}

bool isSchedule(const char* data) 
{
  if (Document d; !d.Parse(data).HasParseError() && d.HasMember("type"))
    return strcmp(d["type"].GetString(), "schedule") == 0;
  return false;
}

template <typename T>
bool isKEvent(T event, const char* kEvent)
{
  if constexpr (std::is_same_v<T, std::string>)
    return strcmp(event.c_str(), kEvent) == 0;
  else if constexpr (std::is_same_v<T, QString>)
    return strcmp(event.toUtf8(), kEvent) == 0;
  else
    return strcmp(event, kEvent) == 0;  
}

bool isPong(const char* data)
{
  return strcmp(data, "PONG") == 0;
}

bool isPong(const uint8_t* bytes, size_t size)
{
  return (size     >  3    &&
          bytes[0] == 0x50 &&
          bytes[1] == 0x4f &&
          bytes[2] == 0x4e &&
          bytes[3] == 0x47    );
}
// TODO: This should be "message", no?
bool isMessage(const char* data)
{
  if (Document d; !(d.Parse(data).HasParseError()))
    return (d.HasMember("message"));
  return false;
}

template <typename T, typename P>
std::vector<std::string> ArgsToV(QVector<T> args, P arg)
{
  std::vector<std::string> v{};
  if constexpr (std::is_same_v<P, uint8_t>)
    v.emplace_back(std::to_string(arg));
  if constexpr (std::is_same_v<T, QString>)
    for (const auto& s : args)
      v.emplace_back(s.toStdString());
  return v;
}

template std::vector<std::string> ArgsToV(QVector<QString>, uint8_t);

std::string createOperation(const char* op, std::vector<std::string> args, const char* name = nullptr, const char* token = nullptr)
{
  StringBuffer s;
  Writer<StringBuffer, Document::EncodingType, ASCII<>> w(s);
  w.StartObject();
  w.Key("type");
  w.String("operation");
  w.Key("command");
  w.String(op);
  w.Key("user");
  w.String(name);
  w.Key("token");
  w.String(token);
  w.Key("args");
  w.StartArray();  
  for (const auto& arg : args)
    w.String(arg.c_str());
  w.EndArray();
  w.EndObject();
  return s.GetString();
}

std::string getOperation(const char* data) {
  Document d;
  d.Parse(data);
  if (d.HasMember("command"))
    return d["command"].GetString();
  return "";
}

QString getOperation(const QString& data) {
  Document d;
  d.Parse(data.toUtf8());
  if (d.HasMember("command"))
      return d["command"].GetString();
  return "";
}

QString getEvent(const char* data)
{
  Document d;
  d.Parse(data);
  if (d.HasMember("event"))
      return d["event"].GetString();
  return "";
}

QString getEvent(const QString& data) {
    Document d;
    d.Parse(data.toUtf8());
    if (d.HasMember("event")) {
        return d["event"].GetString();
    }
    return "";
}

QString getMessage(const char* data) {
    Document d;
    d.Parse(data);
    if (d.HasMember("message")) {
        return d["message"].GetString();
    }
    return "";
}

QString getMessage(const QString& data) {
  Document d;
  d.Parse(data.toUtf8());
  if (d.HasMember("message")) {
      return d["message"].GetString();
  }
  return "";
}

QVector<QString> getArgs(const char* data) {
  Document d;
  d.Parse(data);
  QVector<QString> args{};
  if (d.HasMember("args")) {
    for (const auto& m : d["args"].GetArray()) {
      args.push_back(m.GetString());
    }
  }
  return args;
}

QList<QString> getValueArgs(const char* data, QString key) {
  auto key_value = key.toUtf8();
  Document d;
  d.Parse(data);
  QList<QString> args{};
  if (d.IsObject()) {
    for (const auto& m : d.GetObject()) {
      auto name = m.name.GetString();
      if (name == key.toUtf8()) {
        if (m.value.IsArray()) {
          for (const auto& a : m.value.GetArray()) {
            args.push_back(a.GetString());
          }
        }
      }
    }
  }
  return args;
}

CommandMap getArgMap(const char* data) {
  Document d;
  d.Parse(data);
  CommandMap cm{};
  if (d.HasMember("args")) {
    for (const auto& m : d["args"].GetObject()) {
      cm.emplace(std::stoi(m.name.GetString()), m.value.GetString());
    }
  }
  return cm;
}

bool isStartOperation(const char* data) {
    Document d;
    d.Parse(data);
    return strcmp(d["command"].GetString(), "start") == 0;
}

bool isStopOperation(const char* data) {
    Document d;
    d.Parse(data);
    return strcmp(d["command"].GetString(), "stop") == 0;
}

bool isNewSession(const char* data) {
    Document d;
    d.Parse(data);
    if (d.IsObject() && d.HasMember("message")) {
        return strcmp(d["message"].GetString(), "New Session") == 0;
    }
    return false;
}

bool serverWaitingForFile(const char* data) {
    Document d;
    d.Parse(data);
    if (d.IsObject() && d.HasMember("message")) {
        return strcmp(d["message"].GetString(), "File Ready") == 0;
    }
    return false;
}

inline size_t findNullIndex(uint8_t* data) {
    size_t index = 0;
    while (data) {
        if (strcmp(const_cast<const char*>((char*)data), "\0") == 0) {
            break;
        }
        index++;
        data++;
    }
    return index;
}

namespace FileUtils {
[[ maybe_unused ]]
static QString generatePreview(QString video_path, QString video_name)
{
  QString preview_name = video_name.left(video_name.size() - 4) + "-preview.jpg";
  QString command{"ffmpeg -y -ss 0 -i '" + video_path +
                  "' -vf \"scale=w=640:h=640:force_original_aspect_ratio=decrease\" "
                  "-vframes 1 './assets/previews/" + preview_name + "'"};
  std::system(command.toUtf8());
  return preview_name;
}

/**
 * padVideo
 * (Uses Instagram defaults)
 *
 * @param [in] {QString} input
 * @param [in] {QString} name
 * @param [in] {QString} w
 * @param [in] {QString} h
 * @return
 */
[[ maybe_unused ]]
static QString padVideo(const QString& input, const QString& name, const QString& path, const QString& w = "1350", const QString& h = "1080")
{
  static const char* base{"ffmpeg -y -i "};
  static const char* aspect{":force_original_aspect_ratio=1,pad="};
  static const char* ratio{":(ow-iw)/2:(oh-ih)/2\" "};
  static const char* scale{" -vf \"scale=w="};
  const QString      file = '"' + input + '"';
  QString cmd{base + file + scale + w + ":h=" + h + aspect + w + ":" + h + ratio + path + name};
  std::system(cmd.toUtf8());
  return name;
}
}; // namespace FileUtils

namespace UI {
static void infoMessageBox(QString text, QString title = "KYGUI")
{
  QMessageBox box;
  box.setWindowTitle(title);
  box.setText(text);
  box.setButtonText(0, "Close");
  box.exec();
}

} // namespace UI
namespace TimeUtils {
[[maybe_unused]]
static QString getTime()  { return QDateTime::currentDateTime().toString("hh:mm:ss"); }
[[maybe_unused]]
static uint    unixtime() { return QDateTime::currentDateTime().toTime_t(); }
} // namespace TimeUtils

}  // namespace
