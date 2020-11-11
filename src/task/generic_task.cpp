#include <include/task/generic_task.hpp>
#include <QDebug>
using namespace Scheduler;

/**
 * These values contained inside the TaskIndex namespace represent the order in which the tasks are to be stored.
 */
namespace TaskIndex {
static const uint8_t HEADER         = 0;
static const uint8_t DESCRIPTION    = 1;
static const uint8_t DATETIME       = 2;
static const uint8_t FILES          = 3;
static const uint8_t USER           = 4;
static const uint8_t IS_VIDEO       = 5;
static const uint8_t RUNTIME        = 6;
static const uint8_t RUNTIME_STRING = 7;
}  // namespace TaskIndex

/**
 * @constructor
 */
GenericTask::GenericTask() {}

/**
 * @brief GenericTask::defineTaskArguments
 *
 * This method defines all of the arguments that are available to be set for the Task
 *
 */
void GenericTask::defineTaskArguments() {
  m_arguments.clear();
  m_arguments.emplace_back(std::move(new TaskArgument{"header", Type::TEXT, QString{}}));
  m_arguments.emplace_back(std::move(new TaskArgument{"description", Type::TEXT, TypeVariant{QString{}}}));
  m_arguments.emplace_back(std::move(new TaskArgument{"datetime", Type::TEXT, QString{}}));
  m_arguments.emplace_back(std::move(new TaskArgument{"files", Type::FILEVECTOR, QVector<KFileData>{}}));
  m_arguments.emplace_back(std::move(new TaskArgument{"user", Type::TEXT, QString{}}));
  m_arguments.emplace_back(std::move(new TaskArgument{"is_video", Type::BOOLEAN, bool{}}));
  m_arguments.emplace_back(std::move(new TaskArgument{"runtime", Type::STRINGVECTOR, QVector<QString>{}}));
  m_arguments.emplace_back(std::move(new TaskArgument{"runtime_string", Type::TEXT, QString{}}));
}

/**
 * @brief GenericTask::setArgument
 * @param [in] {QString}      name   The name of the argment
 * @param [in] {TypeVariant}  value  The value of the argument
 */
void GenericTask::setArgument(QString name, TypeVariant value) {
  for (auto&& argument : m_arguments) {
    if (argument->text() == name) {
      argument->setValue(value);
      return;
    }
  }
}

/**
 * @warning This method is used to add values to an argument, and can only be used on arguments whose type is a form of container.
 *
 * @brief GenericTask::addArgument
 *
 * @param [in] {QString}    name  The name of the argument
 * @param [in] {KFileData}  file  A data structure to be added to a container of files.
 *                                The KFileData structure contains metadata about a file and
 *                                its data as a byte array
 */
void GenericTask::addArgument(QString name, Scheduler::KFileData file) {
  for (const auto& argument : m_arguments) {
    if (argument->text() == name) {
      argument->insert(file);
      return;
    }
  }
}

/**
 * @warning This method is used to remove a value to an argument, and can only be used on arguments whose type is a form of container.
 *
 * @brief GenericTask::removeArgument
 *
 * @param [in] {QString}      name   The name of the argument, whose value is expected to be a container.
 * @param [in] {TypeVariant}  value  The value to be removed from the container argument.
 *
 */
void GenericTask::removeArgument(QString name, Scheduler::TypeVariant value) {
  for (auto&& argument : m_arguments) {
    if (argument->text() == name) {
      if (argument->isContainer()) {
        argument->remove(value);
      } else {
        argument->clear();
      }
      return;
    }
  }
}

/**
 * @warning This method is used to add values to an argument, and can only be used on arguments whose type is a form of container.
 *
 * @brief GenericTask::addArgument
 *
 * @param [in] {QString} name     The name of the argument
 * @param [in] {QString} string   A string value intended to be added to a container of strings
 */
void GenericTask::addArgument(QString name, QString string) {
  for (const auto& argument : m_arguments) {
    if (argument->text() == name) {
      argument->insert(string);
      return;
    }
  }
}


/**
 * @brief GenericTask::getTaskArgument
 *
 * @param [in] {QString} name   The name of the argument to retrieve
 * @return [out] {TaskArgument}  The argument
 */
TaskArgument&& GenericTask::getTaskArgument(QString name) {
  for (const auto& argument : m_arguments) {
    if (argument->text() == name) {
      return std::move(*argument);
    }
  }
  throw std::invalid_argument("Argument not found");
}

/**
 * @brief GenericTask::getTaskArgumentValue
 *
 * @param [in] {QString} name   The name of the argument to retrieve
 * @return [out] {TypeVariant}  The value of the argument
 */
const TypeVariant GenericTask::getTaskArgumentValue(QString name) {
  for (const auto& argument : m_arguments) {
    if (argument->text() == name) {
      return argument->getValue();
    }
  }
  return "";  // Perhaps we should throw
}

/**
 * @warning This method does not return any task value whose type is a form of container.
 *
 * @brief GenericTask::getArgumentValues
 * @typedef QVector<QString> is aliased to ArgumentValues
 *
 * @return [out] {ArgumentValues} A vector of strings for all of the arguments that can be represented as a string.
 */
ArgumentValues GenericTask::getArgumentValues() {
  ArgumentValues values{static_cast<int>(m_arguments.size())};
  for (auto& argument : m_arguments) {
    if (!argument->isContainer()) {
      values.push_back(argument->getStringValue());
    }
  }
  return values;
}

/**
 * @warning This method does not necessarily need to return all argument names.
 *          In this use case, we are retrieving names to populate options in our
 *          ArgType widget, thus we only return names for arguments whose values are
 *          to be set or modified using the ArgType widget. For this reason, we have
 *          hardcoded the return to be explicit about which arguments names are available.
 *
 * @brief GenericTask::getArgumentNames
 *
 * @return [out] {QVector<QString>} A vector of argument names as strings.
 */
QVector<QString> GenericTask::getArgumentNames() {
  return QVector<QString>{
      GenericArgs::DESCRIPTION_TYPE,
      GenericArgs::HEADER_TYPE
  };
}

/**
 * @warning This method is used to claim ownership of the task's arguments. Use of this method will effectively REMOVE all arguments from
 *          the task upon which it is called.
 *
 * @brief GenericTask::getTaskArguments
 * @typedef std::vector<std::unique_ptr<TaskArgument> is aliased to TaskArguments
 *
 * @return [out] {std::vector<std::unique_ptr<TaskArgument>} An R-value reference to a vector of unique pointers to the task's arguments.
 *
 */
const TaskArguments&& GenericTask::getTaskArguments() { return std::move(m_arguments); }

/**
 * @brief GenericTask::setDefaultValues
 *
 * Sets default values for the task's arguments
 */
void GenericTask::setDefaultValues() {
  setArgument("header", TypeVariant{QString{"Generic Task"}});
}

/**
 * @brief getType
 *
 * @return [out] {TaskType} The type of task
 */
Scheduler::TaskType GenericTask::getType() { return Scheduler::TaskType::GENERIC; };

/**
 * @brief getTaskCode
 *
 * @return [out] {int} The task bytecode
 */
int GenericTask::getTaskCode() { return TaskCode::GENTASKBYTE; };


/**
 * @brief GenericTask::clear
 *
 * Clears the value of each task argument
 */
void GenericTask::clear() {
  for (const auto& argument : m_arguments) {
    argument->clear();
  }
}

/**
 * @brief GenericTask::hasFiles
 *
 * @return [out] {bool} Indicates whether the task has files.
 */
bool GenericTask::hasFiles() {
  return !std::get<VariantIndex::FILEVEC>(getTaskArgumentValue("files")).empty();
}

/**
 * @brief GenericTask::hasFiles
 *
 * @return [out] {QVector<KFileData>} A vector of data structures representing file metadata and the file data as bytes.
 */
const QVector<Scheduler::KFileData> GenericTask::getFiles() {
  return std::get<VariantIndex::FILEVEC>(getTaskArgumentValue("files"));
}

/**
 * @brief GenericTask::isReady
 *
 * @return [out] {bool} A boolean value indicating whether the minimal requirements sufficient to appropriately
 *                      perform the task have been met.
 */
bool GenericTask::isReady() {
  auto header_size      = std::get<VariantIndex::QSTRING>(getTaskArgumentValue("header")).size();
  auto description_size = std::get<VariantIndex::QSTRING>(getTaskArgumentValue("description")).size();
  auto datetime_size    = std::get<VariantIndex::QSTRING>(getTaskArgumentValue("datetime")).size();
  auto hasFiles         = std::get<VariantIndex::FILEVEC>(getTaskArgumentValue("files")).size();
  auto user_size        = std::get<VariantIndex::QSTRING>(getTaskArgumentValue("user")).size();

  return header_size > 0 && description_size > 0 && datetime_size > 0 &&
         hasFiles && user_size > 0;
}

/**
 * @destructor
 */
GenericTask::~GenericTask() {
}
