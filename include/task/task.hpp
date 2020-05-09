#ifndef __TASK_HPP__
#define __TASK_HPP__

#include <QQueue>
#include <QString>
#include <memory>
#include <variant>
#include <vector>

namespace Scheduler {

enum FileType { VIDEO = 1, IMAGE = 2 };

struct KFileData {
  QString name;
  FileType type;
  QString path;
  QByteArray bytes;
};

namespace Type {
static constexpr const char* TEXT = "Text";
static constexpr const char* FILE = "File";
static constexpr const char* STRINGVECTOR = "StringVector";
static constexpr const char* FILEVECTOR = "FileVector";
static constexpr const char* DATETIME = "DateTime";
static constexpr const char* BOOLEAN = "Boolean";
}  // namespace Type

class TaskArgumentBase;
class Task;

using ArgumentType = const char*;
using TypeVariant = std::variant<QString, bool, std::vector<std::string>, std::vector<KFileData>>;
using TaskIterator = std::vector<std::unique_ptr<TaskArgumentBase>>::iterator;
using TaskArguments = std::vector<std::unique_ptr<TaskArgumentBase>>;
using TaskQueue = QQueue<Task>;

class TaskArgumentBase {
 public:
  virtual QString text() const = 0;
  virtual void setValue(TypeVariant v) = 0;
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
  virtual void setValue(TypeVariant new_value) override { value = new_value; }
  QString name;
  ArgumentType type;
  T value;
};

class Task {
 public:
  virtual void defineTaskArguments() = 0;
  virtual bool isReady() = 0;
  virtual const TaskArguments getTaskArguments() = 0;
  virtual void clear() = 0;
  virtual ~Task(){};
};

}  // namespace Scheduler

#endif  // __TASK_HPP__
