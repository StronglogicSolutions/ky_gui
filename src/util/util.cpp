#include "util.hpp"

bool FileWrap::HasID() const { return !(id.isEmpty()); }

QString escapeText(QString s)
{
  s.replace("\t", "\\t");
  s.replace('"', "\\\"");
  s.replace("'", "'\"\'\"'");
  return s;
}

QString escapeMessage(QString s)
{
  s.replace("\t", "\\t");
  s.replace('"', "\\\"");
  s.replace("'", "\'");
  return s;
}

QString escapeTextToRaw(QString s)
{
    return escapeText(s).toUtf8().constData();
}


QString configValue(const QString& key, const QJsonObject& config, bool use_default)
{
  if (!config.contains(key) && use_default)
  {
    if (config.contains("default"))
      return config.value("default").toString();
  }
  else
    return config.value(key).toString();

  return "";
}

QList<QString> configValueToQList(const QString& key, const QJsonObject& config)
{
  QList<QString> list;
  if (config.contains(key))
  {
    if (auto value = config.value(key); value.isArray())
      for (const auto& item : value.toArray())
        list.append(item.toString());
  }
  return list;
}

QList<QString> configUsers(const QString& section, const QJsonObject& config)
{
  QList<QString> list;
  if (config.contains(section))
  {
    if (QJsonObject sub_object = config.value(section).toObject(); sub_object.contains("users"))
      for (auto&& item : sub_object.value("users").toArray())
        list.append(item.toString());
  }
  return list;
}

QString defaultConfigUser(const QJsonObject& config)
{
  auto users = configUsers("default", config);
  return users.isEmpty() ? "" : users.front();
}

QJsonObject configObject(QString key, QJsonObject config, bool use_default)
{
  auto key_value = key.toLower();
  if (!config.contains(key_value) && use_default)
  {
    if (config.contains("default"))
      return config["default"].toObject();
  }
  else
    return config[key_value].toObject();
  qDebug() << "Returning empty QJsonObject :(";
  return QJsonObject{};
}

QJsonObject loadJsonConfig(QString json_string)
{
  return QJsonDocument::fromJson(json_string.toUtf8()).object();
}

bool configBoolValue(QString key, QJsonObject config)
{
  if (config.contains(key))
    return bool{config.value(key).toString().compare("true") == 0};
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

std::string createMessage(const char* data, std::string args, const char* user, const char* token)
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
    if (!d.Parse(data).HasParseError()) {
      return strcmp(d["type"].GetString(), "operation") == 0;
    }
    return false;
}

bool isUploadCompleteEvent(const char* event) {
  return strcmp(event, "File Transfer Complete") == 0;
}

bool isUploadCompleteEvent(const QString& s) {
  return s == "File Transfer Complete";
}

bool isValidJson(const QString& s) {
  return !(Document{}.Parse(s.toUtf8().constData()).HasParseError());
}

bool isValidJson(const std::string& s) {
  return !(Document{}.Parse(s.c_str()).HasParseError());
}

bool isEvent(const char* data) {
  if (Document d; !d.Parse(data).HasParseError() && d.HasMember("type"))
    return strcmp(d["type"].GetString(), "event") == 0;
  return false;
}

bool isSchedule(const char* data) {
  if (Document d; !d.Parse(data).HasParseError() && d.HasMember("type"))
    return strcmp(d["type"].GetString(), "schedule") == 0;
  return false;
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
bool isMessage(const char* data) {
  Document d;
  return !(d.Parse(data).HasParseError()) && d.HasMember("message");
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

std::string createOperation(const char* op, std::vector<std::string> args, const char* name, const char* token)
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

std::string getOperation(const char* data)
{
  Document d;
  d.Parse(data);
  if (d.HasMember("command"))
    return d["command"].GetString();
  return "";
}

QString getOperation(const QString& data)
{
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

QString getEvent(const QString& data)
{
    Document d;
    d.Parse(data.toUtf8());
    if (d.HasMember("event")) {
        return d["event"].GetString();
    }
    return "";
}

QString getMessage(const char* data)
{
    Document d;
    d.Parse(data);
    if (d.HasMember("message")) {
        return d["message"].GetString();
    }
    return "";
}

QString getMessage(const QString& data)
{
  Document d;
  d.Parse(data.toUtf8());
  if (d.HasMember("message")) {
      return d["message"].GetString();
  }
  return "";
}

QVector<QString> getArgs(const char* data)
{
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

QList<QString> getValueArgs(const char* data, QString key)
{
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

CommandMap getArgMap(const char* data)
{
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

bool isStartOperation(const char* data)
{
    Document d;
    d.Parse(data);
    return strcmp(d["command"].GetString(), "start") == 0;
}

bool isStopOperation(const char* data)
{
    Document d;
    d.Parse(data);
    return strcmp(d["command"].GetString(), "stop") == 0;
}

bool isNewSession(const char* data)
{
    Document d;
    d.Parse(data);
    if (d.IsObject() && d.HasMember("message"))
    {
        return strcmp(d["message"].GetString(), "New Session") == 0;
    }
    return false;
}

bool serverWaitingForFile(const char* data)
{
    Document d;
    d.Parse(data);
    if (d.IsObject() && d.HasMember("message")) {
        return strcmp(d["message"].GetString(), "File Ready") == 0;
    }
    return false;
}

namespace FileUtils {
[[ maybe_unused ]]
QString generatePreview(QString video_path, QString video_name)
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
QString padVideo(const QString& input, const QString& name, const QString& path, const QString& w , const QString& h)
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
void infoMessageBox(QString text, QString title)
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
QString getTime()  { return QDateTime::currentDateTime().toString("hh:mm:ss"); }
[[maybe_unused]]
uint    unixtime() { return QDateTime::currentDateTime().toTime_t(); }
} // namespace TimeUtils
