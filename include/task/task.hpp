#ifndef __TASK_HPP__
#define __TASK_HPP__

#include <QQueue>
#include <QString>
#include <QVector>
#include <memory>
#include <variant>
#include <vector>
#include <map>

namespace TaskCode {
static constexpr uint32_t GENTASKCODE = 0xFC;
static constexpr uint32_t IGTASKCODE  = 0xFF;
}

namespace Scheduler {
namespace Args {
const QString RUNTIME_ARG_TYPE = "runtime_arg";
} // namespace Args

enum TaskType { INSTAGRAM = 1, GENERIC = 2, OTHER = 3 };


static constexpr uint32_t GENERIC_TASK_ID   = 96;
static constexpr uint32_t INSTAGRAM_TASK_ID = 97;

namespace TaskIndex {
static constexpr uint32_t ERROR   = 0x03;
static constexpr uint32_t UUID    = 0x00;
static constexpr uint32_t ID      = 0x01;
static constexpr uint32_t MASK    = 0x02;
static constexpr uint32_t ENVFILE = 0x03;
static constexpr uint32_t FILENUM = 0x04;
} // namespace TaskInfo

inline static const std::map<std::string, uint32_t> TaskCodes{
    {"Generic",   TaskCode::GENTASKCODE},
    {"Instagram", TaskCode::IGTASKCODE}
};

inline static const std::map<std::string, int> TaskFrequency{
    {"No",      0x00},
    {"Hourly",  0x01},
    {"Daily",   0x02},
    {"Weekly",  0x03}
};

inline static uint32_t findTaskCode(QString key) {
  auto it = TaskCodes.find(key.toUtf8().constData());
  return it == TaskCodes.end() ?
    TaskCodes.at("Generic") :
    (*it).second;
}

inline static int findTaskFrequency(const QString& key) {
  auto it = TaskFrequency.find(key.toUtf8().constData());
  return it == TaskFrequency.end() ?
    TaskCodes.at("No") :
    (*it).second;
}

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
  QString     name;
  FileType    type;
  QString     path;
  QByteArray  bytes;

  bool valid() const
  {
    return (!(path.isEmpty()) && !(bytes.isEmpty()));
  }

//  friend std::ostream &operator<<(std::ostream &out, const KFileData& file) {
//    out << "Name: " << file.name.toUtf8()
//        << "\nType: " << file.type;
//    return out;
//  }
};

/**
 * Type namespace
 *
 * Describes the types of task arguments available for use
 */
namespace Type {
static constexpr const char* TEXT         = "Text";
static constexpr const char* FILE         = "File";
static constexpr const char* STRINGVECTOR = "StringVector";
static constexpr const char* FILEVECTOR   = "FileVector";
static constexpr const char* DATETIME     = "DateTime";
static constexpr const char* BOOLEAN      = "Boolean";
static constexpr const char* INTEGER      = "Integer";
}  // namespace Type

namespace VariantIndex {
static const uint8_t BOOLEAN  = 0;
static const uint8_t INTEGER  = 1;
static const uint8_t STRVEC   = 2;
static const uint8_t QSTRING  = 3;
static const uint8_t FILEVEC  = 4;
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
using TaskQueue       = QQueue<Task*>;
using ArgumentType    = const char*;
using ArgumentValues  = QVector<QString>;
using TypeVariant     = std::variant<
                          bool, int, QVector<QString>, QString, QVector<KFileData>
                        >;
using TaskIterator    = std::vector<TaskArgumentBase*>::iterator;

/**
 * The interface expected on our Task Arguments
 */
class TaskArgumentBase {
 public:
  virtual const QString text() = 0;
  virtual const QString getStringValue() = 0;
  virtual uint8_t getTypeIndex() = 0;
  virtual TypeVariant getValue() = 0;
  virtual void insert(QString value) = 0;
  virtual void insert(KFileData file) = 0;
  virtual void remove(TypeVariant value) = 0;
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
  TaskArgument(QString n, ArgumentType t, TypeVariant _value)
      : name(n),
        type(t),
        value(_value) {}
  /**
   * Move Constructor
   *
   * @constructor
   * @param [in] {TaskArgument&&} a The R-value reference to a TaskArgument
   */
  TaskArgument(TaskArgument&& a)
      : name(std::move(a.name)),
        type(std::move(a.type)),
        value(std::move(a.value)) {}

  /**
   * Copy Constructor
   *
   * @constructor
   * @param [in] {TaskArgument&&} a The const reference to a TaskArgument
   */
  TaskArgument(const TaskArgument& a)
      : name(std::move(a.name)),
        type(std::move(a.type)),
        value(std::move(a.value)) {}
  /**
   * text
   * @returns {QString} The name of the argument
   */
  virtual const QString text() override { return name; }

  /**
   * setValue
   * @param [in] {TypeVariant} new_value The new value for this argument
   */
  virtual void setValue(TypeVariant new_value) override {
    value = new_value;
  }

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
    return ""; // Throw?
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
    } else if (isIndex(value.index(), VariantIndex::STRVEC)) {
      return std::get<VariantIndex::STRVEC>(value);
    } else if (isIndex(value.index(), VariantIndex::FILEVEC)) {
      return std::get<VariantIndex::FILEVEC>(value);
    }
    return ""; // Throw?
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
   * @brief remove
   * @param value
   */
  virtual void remove(TypeVariant unwanted_value) override {
    if (value.index() == VariantIndex::STRVEC && unwanted_value.index() == VariantIndex::QSTRING) {
      auto&& container = std::get<VariantIndex::STRVEC>(value);
      auto value_to_remove = std::get<VariantIndex::QSTRING>(unwanted_value);
      auto it = std::find_if(container.begin(), container.end(), [&value_to_remove](QString s) {
        return (s == value_to_remove);
      });
      if (it != container.end()) {
        container.erase(it);
        return;
      } else {
        throw std::out_of_range("Could not find value requested for removal");
      }
    } else if (value.index() == VariantIndex::FILEVEC && unwanted_value.index() == VariantIndex::QSTRING) {
      auto&& container = std::get<VariantIndex::FILEVEC>(value);
      auto file_to_remove = std::get<VariantIndex::QSTRING>(unwanted_value);
      auto it = std::find_if(container.begin(), container.end(), [&file_to_remove](Scheduler::KFileData f) {
        return (f.name == file_to_remove);
      });
      if (it != container.end()) {
        container.erase(it);
        return;
      } else {
        throw std::out_of_range("Could not find value requested for removal");
      }
    }
    throw std::invalid_argument("The value provided does not match any existing container");
  }

  /**
   * @brief getTypeIndex
   * @return
   */
  virtual uint8_t getTypeIndex() override {
    return value.index();
  }

 private:
  QString       name;
  ArgumentType  type;
  TypeVariant   value;
};

using TaskArguments = std::vector<TaskArgument*>;

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
  virtual void removeArgument(QString name, TypeVariant arg) = 0;
  virtual const TaskArguments&& getTaskArguments() = 0;
  virtual TaskArgument&& getTaskArgument(QString name) = 0;
  virtual const TypeVariant getTaskArgumentValue(QString name) = 0;
  virtual ArgumentValues getArgumentValues() = 0;
  virtual QVector<QString> getArgumentNames() = 0;
  virtual TaskType getType() = 0;
  virtual uint32_t getTaskCode() = 0;
  virtual void defineTaskArguments() = 0;
  virtual void setDefaultValues() = 0;
  virtual const QVector<KFileData> getFiles() = 0;
  virtual bool hasFiles() = 0;
  virtual bool isReady() = 0;
  virtual bool isEmpty() {
    bool empty = true;
    for (const auto& arg : getArgumentValues()) {
      if (!arg.isEmpty()) {
        empty = false;
      }
    }
    return empty;
  }

  virtual void clear() = 0;
  virtual ~Task(){};
};
}  // namespace Scheduler

#endif  // __TASK_HPP__
