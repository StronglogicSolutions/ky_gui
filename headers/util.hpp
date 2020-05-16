#ifndef UTIL_HPP
#define UTIL_HPP
#pragma GCC system_header
#include <QDebug>
#include <QQueue>
#include <QString>
#include <QVector>
#include <charconv>
#include <string>
#include <utility>
#include <vector>
#include "rapidjson/document.h"
#include "rapidjson/pointer.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

namespace Kontainer {
/** Reverse Iterator */
template <class T>
class ReverseIterator {
    T& _obj;
public:
    ReverseIterator(T &obj) : _obj(obj) {}
    auto begin() {return _obj.rbegin();}
    auto end() {return _obj.rend();}
};
}  // namespace Kontainer

namespace {
using namespace rapidjson;

typedef std::string KOperation;

typedef std::vector<std::pair<std::string, std::string>> TupVec;
typedef std::vector<std::map<int, std::string>> MapVec;
typedef std::vector<std::string> StdStringVec;
typedef std::map<int, std::string> CommandMap;
typedef std::map<QString, QString> ConfigJson;

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

/**
 * @brief configValue
 * @param key [in] {QString} The key whose corresponding value is to be sought
 * from the ConfigJson param
 * @param [in] {ConfigJson} A Key-Value JSON Config object
 * @return {QString} The value which corresonds to the key, or an empty string
 *
 * TODO: ConfigJson should probably be called something else, like
 * ConfigJsonObject
 */
QString configValue(QString key, ConfigJson config) {
  ConfigJson::iterator it{config.find(key)};  // Find iterator to element matching key
  if (it != std::end(config)) {               // If element was found
    return it->second;                        // Return the value of the Key-Pair element
  }
  return "";
}

bool configBoolValue(QString s, ConfigJson config) {
  if (auto it{config.find(s)}; it != std::end(config)) {
    return bool{it->second == "true"};
  }
}

std::string getJsonString(std::string s) {
    Document d;
    d.Parse(s.c_str());
    StringBuffer buffer;
    PrettyWriter<StringBuffer> writer(buffer);
    d.Accept(writer);
    return buffer.GetString();
}

std::string createMessage(const char* data, std::string args = "") {
    StringBuffer s;
    Writer<StringBuffer> w(s);
    w.StartObject();
    w.Key("type");
    w.String("custom");
    w.Key("message");
    w.String(data);
    w.Key("args");
    w.String(args.c_str());
    w.EndObject();
    return s.GetString();
}

bool isOperation(const char* data) {
    Document d;
    d.Parse(data);
    return strcmp(d["type"].GetString(), "operation") == 0;
}

bool isUploadCompleteEvent(const char* event) {
    return strcmp(event, "File Transfer Complete") == 0;
}

bool isEvent(const char* data) {
    Document d;
    d.Parse(data);
    if (d.HasMember("type")); {
      return strcmp(d["type"].GetString(), "event") == 0;
    }
    return false;
}

template <typename T>
bool isKEvent(T event, const char* kEvent) {
  if constexpr (std::is_same_v<T, std::string>) {
    return strcmp(event.c_str(), kEvent) == 0;
  } else if constexpr (std::is_same_v<T, QString>) {
    return strcmp(event.toUtf8(), kEvent) == 0;
  } else {
    return strcmp(event, kEvent) == 0;
  }
}

bool isPong(const char* data) {
    return strcmp(data, "PONG") == 0;
}

// TODO: This should be "message", no?
bool isMessage(const char* data) {
    Document d;
    d.Parse(data);
    if (d.HasMember("message")) {
        return true;
    } else {
        return false;
    }
}

std::string createOperation(const char* op, std::vector<std::string> args) {
    StringBuffer s;
    Writer<StringBuffer, Document::EncodingType, ASCII<>> w(s);
    w.StartObject();
    w.Key("type");
    w.String("operation");
    w.Key("command");
    w.String(op);
    w.Key("args");
    w.StartArray();
    if (!args.empty()) {
        for (const auto& arg : args) {
            w.String(arg.c_str());
        }
    }
    w.EndArray();
    w.EndObject();
    return s.GetString();
}

std::string getOperation(const char* data) {
    Document d;
    d.Parse(data);
    if (d.HasMember("command")) {
        return d["command"].GetString();
    }
    return "";
}

QString getEvent(const char* data) {
    Document d;
    d.Parse(data);
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

QVector<QString> getShortArgs(const char* data) {
    Document d;
    d.Parse(data);
    QVector<QString> args{};
    if (d.HasMember("args")) {
        if (d["args"].IsArray()) {
            for (const auto& m : d["args"].GetArray()) {
                if (m.GetStringLength() < 100) {
                    args.push_back(m.GetString());
                }
            }
        } else {
            for (const auto& m : d["args"].GetObject()) {
                QString arg = m.name.GetString();
                arg +=  ": ";
                arg += m.value.GetString();
                args.push_back(arg);
            }
        }
    }
    return args;
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

ConfigJson getConfigObject(QString json_string) {
    Document d;
    d.Parse(json_string.toUtf8());
    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    std::map<QString, QString> config_map{};
    if (d.IsObject()) {
        for (const auto& m : d.GetObject()) {
          auto type = m.value.GetType();
          if (m.value.GetType() == kStringType) {
            config_map.emplace(m.name.GetString(), m.value.GetString());
          }
          if (m.value.GetType() == kObjectType) {
            m.value.Accept(writer);
            QString config_value{buffer.GetString()};
            config_map.emplace(m.name.GetString(), config_value);
          }
        }
    }
    return config_map;
}

std::string createMessage(const char* data,
                          std::map<int, std::string> map = {}) {
    StringBuffer s;
    Writer<StringBuffer> w(s);
    w.StartObject();
    w.Key("type");
    w.String("custom");
    w.Key("message");
    w.String(data);
    w.Key("args");
    w.StartObject();
    if (!map.empty()) {
        for (const auto& [k, v] : map) {
            w.Key(std::to_string(k).c_str());
            w.String(v.c_str());
        }
    }
    w.EndObject();
    w.EndObject();
    return s.GetString();
}

std::string createMessage(const char* data, std::map<int, std::vector<std::string>> map = {}) {
    StringBuffer s;
    Writer<StringBuffer> w(s);
    w.StartObject();
    w.Key("type");
    w.String("custom");
    w.Key("message");
    w.String(data);
    w.Key("args");
    w.StartObject();
    if (!map.empty()) {
        for (const auto& [k, v] : map) {
            w.Key(std::to_string(k).c_str());
            if (!v.empty()) {
                w.StartArray();
                for (const auto& arg : v) {
                    w.String(arg.c_str());
                }
                w.EndArray();
            }
        }
    }
    w.EndObject();
    w.EndObject();
    return s.GetString();
}

std::string rapidCreateMessage(const char* data,
                               std::map<int, std::string> map = {}) {
    StringBuffer s;
    Writer<StringBuffer> w(s);
    w.StartObject();
    w.Key("type");
    w.String("custom");
    w.Key("message");
    w.String(data);
    w.Key("args");
    w.StartObject();
    if (!map.empty()) {
        for (const auto& [k, v] : map) {
            w.Key(std::to_string(k).c_str());
            w.String(v.c_str());
        }
    }
    w.EndObject();
    w.EndObject();
    return s.GetString();
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

// std::string stringTupleVecToJson(
//    std::vector<std::pair<std::string, std::string>> v) {
//    json j{};
//    for (const auto& row : v) {
//        j[row.first] = row.second;
//    }
//    return j;
//}

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
QString generatePreview(QString video_path, QString video_name) {
  QString preview_name =
      video_name.left(video_name.size() - 4) + "-preview.jpg";
  QString command{
      "ffmpeg -y -ss 0 -i '" + video_path +
      "' -vf \"scale=w=640:h=640:force_original_aspect_ratio=decrease\" "
      "-vframes 1 './assets/previews/" +
      preview_name + "'"};

  std::system(command.toUtf8());

  return preview_name;
}
}; // namespace FileUtils
}  // namespace
#endif  // UTIL_HPP
