#ifndef __UTIL_HPP__
#define __UTIL_HPP__

#include <string>
#include <charconv>
#include <utility>
#include <vector>
#include <QDebug>
#include "rapidjson/writer.h"
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

//std::string createMessage(const char* data, std::string args = "") {
//  json data_json{};
//  data_json["type"] = "custom";
//  data_json["args"] = args;
//  data_json["message"] = data;

//  return data_json.dump();
//}

std::string rapidCreateMessage(const char* data, std::string args = "") {
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

bool rapidIsOperation(const char* data) {
    Document d;
    d.Parse(data);
    return strcmp(d["type"].GetString(), "operation");
}

CommandMap rapidGetArgMap(const char* data) {
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

bool rapidIsStartOperation(const char* data) {
    Document d;
    d.Parse(data);
    return strcmp(d["command"].GetString(), "start");
}

bool rapidIsStopOperation(const char* data) {
    Document d;
    d.Parse(data);
    return strcmp(d["command"].GetString(), "stop");
}

bool rapidIsNewSession(const char* data) {
    Document d;
    d.Parse(data);
    if (d.HasMember("message")) {
        return strcmp(d["message"].GetString(), "New Session") == 0;
    }
    return false;
}

//std::string createMessageWithDataMap(const char* data,
//                          std::map<int, std::string> map = {}) {
//    json data_json{};
//    data_json["type"] = "custom";
//    data_json["message"] = data;
//    data_json["args"] = nullptr;
//    if (!map.empty()) {
//        for (const auto& [k, v] : map) {
//            data_json["args"][k] = v;
//        }
//    }
//    return data_json;
//}

//std::string createMessage(const char* data, const char* extra = "") {
//    json data_json{};
//    data_json["type"] = "custom";
//    data_json["message"] = data;
//    data_json["args"] = extra;
//    return data_json.dump();
//}

//std::string createMessage(const char* data, TupVec v = {}) {
//  json data_json{};
//  data_json["type"] = "custom";
//  data_json["message"] = data;
//  data_json["args"] = nullptr;
//  if (!v.empty()) {
//    for (const auto& r : v) {
//      data_json["args"][r.first] = r.second;
//    }
//  }
//  return data_json.dump();
//}

std::string createOperation(const char* op, std::vector<std::string> args) {
  json operation_json{};
  operation_json["type"] = "operation";
  operation_json["command"] = op;
  if (!args.empty()) {
    operation_json["args"] = args;
  }
  return operation_json.dump();
}

//std::string stringTupleVecToJson(
//    std::vector<std::pair<std::string, std::string>> v) {
//  json j{};
//  for (const auto& row : v) {
//    j[row.first] = row.second;
//  }
//  return j;
//}

//bool isOperation(json data) { return *(data.find("type")) == "operation"; }

//bool isNewSession(json data) {
//  return *(data.find("message")) == "New Session";
//}

//bool isStartOperation(std::string operation) { return operation == "start"; }

//bool isStopOperation(std::string operation) { return operation == "stop"; }

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
