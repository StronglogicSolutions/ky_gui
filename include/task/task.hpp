#ifndef __TASK_HPP__
#define __TASK_HPP__

#include <QQueue>
#include <QString>
#include <QVector>
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

namespace VariantIndex {
static const uint8_t BOOLEAN = 0;
static const uint8_t INTEGER = 1;
static const uint8_t QSTRING = 2;
static const uint8_t STRVEC = 3;
static const uint8_t FILEVEC = 4;
}  // namespace VariantIndex

inline bool isIndex(uint8_t v, uint8_t i) { return v == i; }

/**
 * Forward Declarations
 */
class TaskArgumentBase;
class Task;

/**
 * Aliases
 */
using ArgumentType = const char*;
using TypeVariant = std::variant<bool, int, QString, QVector<QString>, std::vector<KFileData>>;
using TaskIterator = std::vector<std::unique_ptr<TaskArgumentBase>>::iterator;
using TaskArguments = std::vector<std::unique_ptr<TaskArgumentBase>>;
using TaskQueue = QQueue<Task>;
using ArgumentValues = QVector<const QString>;

/**
 * The interface expected on our Task Arguments
 */
class TaskArgumentBase {
 public:
  virtual void setValue(TypeVariant v) = 0;
  virtual TypeVariant getValue() = 0;
  virtual const QString text() = 0;
  virtual void clear() = 0;
  virtual const QString getStringValue() = 0;
  virtual bool isContainer() = 0;
};

/**
 * TaskArgument
 *
 * A templated class providing a generic way for handling arguments whose types can be one from the set defined
 * by our TypeVariant alias
 */
template <typename T = TypeVariant>
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
  virtual const QString text() { return name; }

  /**
   * setValue
   * @param [in] {TypeVariant} new_value The new value for this argument
   */
  virtual void setValue(TypeVariant new_value) override { value = new_value; }

  virtual const QString getStringValue() override {
    if (isIndex(value.index(), VariantIndex::QSTRING)) {
      return value;
    } else if (isIndex(value.index(), VariantIndex::BOOLEAN)) {
      return QString::number(value);
    } else if (isIndex(value.index(), VariantIndex::INTEGER)) {
      return QString::number(value);
    }
  }

  virtual TypeVariant getValue() override { return std::get<value.index()>(value); }

  virtual void clear() override {
    if (isIndex(value.index(), VariantIndex::STRVEC)) {
      std::get<VariantIndex::STRVEC>(value).clear();
    } else if (isIndex(value.index(), VariantIndex::FILEVEC)) {
      std::get<VariantIndex::FILEVEC>(value).clear();
    } else if (isIndex(value.index(), VariantIndex::QSTRING)) {
      std::get<VariantIndex::QSTRING>(value).clear();
    } else if (isIndex(value.index(), VariantIndex::INTEGER)) {
      std::get<VariantIndex::INTEGER>(value) = 0;
    } else if (isIndex(value.index(), VariantIndex::BOOLEAN)) {
      std::get<VariantIndex::STRVEC>(value) = false;
    }
  }

  virtual bool isContainer() override {
    return (isIndex(value.index(), VariantIndex::STRVEC) || isIndex(value.index(), VariantIndex::FILEVEC));
  }

 private:
  QString name;
  ArgumentType type;
  TypeVariant value;
};

/**
 * The interface expected to be implemented in all Task types
 */
class Task {
 public:
  virtual void setArgument(QString name, TypeVariant arg) = 0;
  virtual const TaskArguments getTaskArguments() = 0;
  virtual TypeVariant getTaskArgument(QString name) = 0;
  virtual ArgumentValues getArgumentValues() = 0;
  virtual void defineTaskArguments() = 0;
  virtual void setDefaultValues() = 0;
  virtual bool isReady() = 0;
  virtual void clear() = 0;
  virtual ~Task(){};
};

}  // namespace Scheduler

#endif  // __TASK_HPP__
