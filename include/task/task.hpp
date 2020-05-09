#ifndef __TASK_HPP__
#define __TASK_HPP__

#include <QQueue>
#include <QString>
#include <memory>
#include <variant>
#include <vector>

namespace Scheduler {

/**
 * Files
 */
enum FileType { VIDEO = 1, IMAGE = 2 };

/**
 * KFileData
 *
 * Structure for holding file bytes and metadata
 */
struct KFileData {
  QString name;
  FileType type;
  QString path;
  QByteArray bytes;
};

/**
 * Type namespace
 *
 * Describes the types of task arguments available for use
 */
namespace Type {
static constexpr const char* TEXT = "Text";
static constexpr const char* FILE = "File";
static constexpr const char* STRINGVECTOR = "StringVector";
static constexpr const char* FILEVECTOR = "FileVector";
static constexpr const char* DATETIME = "DateTime";
static constexpr const char* BOOLEAN = "Boolean";
}  // namespace Type

/**
 * Forward Declarations
 */
class TaskArgumentBase;
class Task;

/**
 * Aliases
 */
using ArgumentType = const char*;
using TypeVariant = std::variant<QString, bool, std::vector<std::string>, std::vector<KFileData>>;
using TaskIterator = std::vector<std::unique_ptr<TaskArgumentBase>>::iterator;
using TaskArguments = std::vector<std::unique_ptr<TaskArgumentBase>>;
using TaskQueue = QQueue<Task>;

/**
 * The interface expected on our Task Arguments
 */
class TaskArgumentBase {
 public:
  virtual QString text() const = 0;
  virtual void setValue(TypeVariant v) = 0;
};

/**
 * TaskArgument
 *
 * A templated class providing a generic way for handling arguments whose types can be one from the set defined
 * by our TypeVariant alias
 */
template <typename T>
class TaskArgument : TaskArgumentBase {
 public:
  TaskArgument(QString n, ArgumentType t, T _value) {
    name = n;
    type = t;
    value = _value;
  }
  /**
   * Move Constructor
   *
   * @constructor
   * @param [in] {TaskArgument&&} a The R-value reference to a TaskArgument
   */
  TaskArgument(TaskArgument&& a) : name(std::move(a.name)), type(std::move(a.type)), value(std::move(a.value)) {}
  /**
   * text
   * @returns {QString} The name of the argument
   */
  virtual QString text() const { return name; }

  /**
   * setValue
   * @param [in] {TypeVariant} new_value The new value for this argument
   */
  virtual void setValue(TypeVariant new_value) override { value = new_value; }

 private:
  QString name;
  ArgumentType type;
  T value;
};

/**
 * The interface expected to be implemented in all Task types
 */
class Task {
 public:
  virtual void defineTaskArguments() = 0;
  virtual const TaskArguments getTaskArguments() = 0;
  virtual bool isReady() = 0;
  virtual void clear() = 0;
  virtual ~Task(){};
};

}  // namespace Scheduler

#endif  // __TASK_HPP__
