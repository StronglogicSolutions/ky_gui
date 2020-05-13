#ifndef __TASK_HPP__
#define __TASK_HPP__

#include <QQueue>
#include <QString>
#include <QVector>
#include <memory>
#include <variant>
#include <vector>

namespace Scheduler {

enum TaskType { INSTAGRAM = 1, OTHER = 2 };

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
using TaskQueue = QQueue<Task*>;
using ArgumentType = const char*;
using ArgumentValues = QVector<QString>;
using TypeVariant = std::variant<bool, int, QString, QVector<QString>, QVector<KFileData>>;
using TaskIterator = std::vector<std::unique_ptr<TaskArgumentBase>>::iterator;

/**
 * The interface expected on our Task Arguments
 */
class TaskArgumentBase {
 public:
  virtual const QString text() = 0;
  virtual const QString getStringValue() = 0;
  virtual uint8_t getTypeIndex();
  virtual TypeVariant getValue() = 0;
  virtual void insert(QString value) = 0;
  virtual void insert(KFileData file) = 0;
  virtual void setValue(TypeVariant v) = 0;
  virtual bool isContainer() = 0;
  virtual void clear() = 0;

};

/**
 * TaskArgument
 *
 * A templated class providing a generic way for handling arguments whose types can be one from the set defined
 * by our TypeVariant alias
 */
class TaskArgument : TaskArgumentBase {
 public:
  TaskArgument(QString n, ArgumentType t, TypeVariant _value) {
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
  TaskArgument(TaskArgument&& a) :
    name(std::move(a.name)), type(std::move(a.type)), value(std::move(a.value)) {}
  /**
   * text
   * @returns {QString} The name of the argument
   */
  virtual const QString text() override { return name; }

  /**
   * setValue
   * @param [in] {TypeVariant} new_value The new value for this argument
   */
  virtual void setValue(TypeVariant new_value) override { value = new_value; }

  /**
   * @brief getStringValue
   * @return [out] {QString}
   */
  virtual const QString getStringValue() override {
    if (isIndex(value.index(), VariantIndex::QSTRING)) {
      return std::get<VariantIndex::QSTRING>(value);
    } else if (isIndex(value.index(), VariantIndex::BOOLEAN)) {
      return QString::number(std::get<VariantIndex::BOOLEAN>(value));
    } else if (isIndex(value.index(), VariantIndex::INTEGER)) {
      return QString::number(std::get<VariantIndex::INTEGER>(value));
    }
    return "";
  }

  /**
   * @brief getValue
   * @return [out] {TypeVariant}
   */
  virtual TypeVariant getValue() override {
    if (isIndex(value.index(), VariantIndex::QSTRING)) {
      return std::get<VariantIndex::QSTRING>(value);
    } else if (isIndex(value.index(), VariantIndex::BOOLEAN)) {
      return std::get<VariantIndex::BOOLEAN>(value);
    } else if (isIndex(value.index(), VariantIndex::INTEGER)) {
      return std::get<VariantIndex::INTEGER>(value);
    }
  }

  /**
   * @brief clear
   */
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
      std::get<VariantIndex::BOOLEAN>(value) = false;
    }
  }

  /**
   * @brief isContainer
   * @return [out] {bool}
   */
  virtual bool isContainer() override {
    return (isIndex(value.index(), VariantIndex::STRVEC) || isIndex(value.index(), VariantIndex::FILEVEC));
  }

  /**
   * @brief insert
   * @param value
   */
  virtual void insert(QString string) override {
    if (isIndex(value.index(), VariantIndex::STRVEC)) {
      std::get<VariantIndex::STRVEC>(value).push_back(string);
    } else {
      // Unable to push. Throw?
    }
  }

  /**
   * @brief insert
   * @param file
   */
  virtual void insert(KFileData file) override {
    if (value.index() == VariantIndex::FILEVEC) {
      std::get<VariantIndex::FILEVEC>(value).push_back(file);
    } else {
      // Unable to push. Throw?
    }
  }

  /**
   * @brief getTypeIndex
   * @return
   */
  virtual uint8_t getTypeIndex() override {
    return value.index();
  }

 private:
  QString name;
  ArgumentType type;
  TypeVariant value;
};

using TaskArguments = std::vector<std::unique_ptr<TaskArgument>>;

/**
 * The interface expected to be implemented in all Task types
 */
class Task {
 public:
  Task(){};
  Task(KFileData);
  Task(QVector<KFileData>);
  virtual void setArgument(QString name, TypeVariant arg) = 0;
  virtual void addArgument(QString name, Scheduler::KFileData file) = 0;
  virtual void addArgument(QString name, QString string) = 0;
  virtual const TaskArguments getTaskArguments() = 0;
  virtual const TypeVariant getTaskArgument(QString name) = 0;
  virtual ArgumentValues getArgumentValues() = 0;
  virtual TaskType getType() = 0;
  virtual void defineTaskArguments() = 0;
  virtual void setDefaultValues() = 0;
  virtual const QVector<KFileData> getFiles() = 0;
  virtual void addFile(KFileData file) = 0;
  virtual bool hasFiles() = 0;
  virtual bool isReady() = 0;
  virtual void clear() = 0;
  virtual ~Task(){};
};
}  // namespace Scheduler

#endif  // __TASK_HPP__
