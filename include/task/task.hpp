#ifndef __TASK_HPP__
#define __TASK_HPP__

#include <QString>
#include <memory>
#include <vector>

enum FileType { VIDEO = 1, IMAGE = 2 };

struct KFileData {
  QString name;
  FileType type;
  QString path;
  QByteArray bytes;
};

namespace Scheduler {

namespace Type {
static constexpr const char* TEXT = "Text";
static constexpr const char* FILE = "File";
static constexpr const char* STRINGVECTOR = "StringVector";
static constexpr const char* FILEVECTOR = "FileVector";
static constexpr const char* DATETIME = "DateTime";
static constexpr const char* BOOLEAN = "Boolean";
}  // namespace Type

using ArgumentType = const char*;

class TaskArgumentBase {
 public:
  virtual QString text() const = 0;
};

template <typename T>
class TaskArgument : TaskArgumentBase {
 public:
  TaskArgument(QString n, ArgumentType t, T _value) {
    name = n;
    type = t;
    value = _value;
  }
  TaskArgument(TaskArgument&& a) : name(std::move(a.name)), type(std::move(a.type)), value(std::move(a.value)) {}
  virtual QString text() const { return name; }
  QString name;
  ArgumentType type;
  T value;
};

using TaskArguments = std::vector<std::unique_ptr<TaskArgumentBase>>;

class Task {
 public:
  virtual void defineTaskArguments() = 0;
  virtual bool isReady() = 0;
  virtual const TaskArguments getTaskArguments() = 0;
  template <typename T>
  void setArgument(QString name, T new_value){};
  virtual void clear() = 0;
  virtual ~Task(){};
};
}  // namespace Scheduler

#endif  // __TASK_HPP__
