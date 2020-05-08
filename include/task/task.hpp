#ifndef __TASK_HPP__
#define __TASK_HPP__

#include <QString>
#include <QVector>

namespace Task {

namespace Type {
static constexpr const char* TEXT = "Text";
static constexpr const char* FILE = "File";
static constexpr const char* STRINGVECTOR = "StringVector";
static constexpr const char* FILEVECTOR = "FileVector";
static constexpr const char* DATETIME = "DateTime";
static constexpr const char* BOOLEAN = "Boolean";
}  // namespace Type

using ArgumentType = const char*;

class TaskArgument {
  QString name;
  ArgumentType type;

 public:
  TaskArgument(QString n, QString t) : name(n), type(t) {}
};

using TaskArguments = QVector<TaskArgument>;

class Task {
 public:
  virtual bool isReady() = 0;
  virtual const TaskArguments getTaskArguments() = 0;
  virtual ~Task(){};
};
}  // namespace Task

#endif  // __TASK_HPP__
