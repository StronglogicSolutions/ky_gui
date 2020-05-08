#ifndef __TASK_HPP__
#define __TASK_HPP__

#include <QString>
#include <QVector>

namespace Task {

class TaskArgument {
  QString name;
  QString type;

 public:
  TaskArgument(QString n, QString t) : name(n), type(t) {}
};

using TaskArguments = QVector<TaskArgument>;

class Task {
 public:
  virtual bool isReady() = 0;
  virtual const TaskArguments getTaskArguments() = 0;
  virtual ~Task() = 0;
};
}  // namespace Task

#endif  // __TASK_HPP__
