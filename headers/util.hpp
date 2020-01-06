#ifndef UTIL_HPP
#define UTIL_HPP

#include <string>
#include <charconv>
#include <utility>
#include <vector>
#include <QDebug>
#include <QVector>
#include "rapidjson/writer.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/pointer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/document.h"
#include "json.hpp"

using namespace rapidjson;
using json = nlohmann::json;

typedef std::string KOperation;

typedef std::vector<std::pair<std::string, std::string>> TupVec;
typedef std::vector<std::map<int, std::string>> MapVec;
typedef std::vector<std::string> StdStringVec;
typedef std::map<int, std::string> CommandMap;

struct KSession {
    int id;
    int fd;
    int status;
};

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
    return strcmp(d["type"].GetString(), "event") == 0;
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
    if (d.HasMember("message")) {
        return strcmp(d["message"].GetString(), "New Session") == 0;
    }
    return false;
}

bool serverWaitingForFile(const char* data) {
    Document d;
    d.Parse(data);
    if (d.HasMember("message")) {
        return strcmp(d["message"].GetString(), "File Ready") == 0;
    }
    return false;
}

std::string stringTupleVecToJson(
    std::vector<std::pair<std::string, std::string>> v) {
    json j{};
    for (const auto& row : v) {
        j[row.first] = row.second;
    }
    return j;
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

#endif  // __UTIL_HPP__
