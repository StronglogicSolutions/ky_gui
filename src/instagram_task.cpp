#include <include/task/instagram_task.hpp>
#include <QDebug>
using namespace Scheduler;

/**
 * These values contained inside the TaskIndex namespace represent the order in which the tasks are to be stored.
 */
namespace TaskIndex {
static const uint8_t HEADER = 0;
static const uint8_t DESCRIPTION = 1;
static const uint8_t DATETIME = 2;
static const uint8_t PROMOTE_SHARE = 3;
static const uint8_t LINK_IN_BIO = 4;
static const uint8_t HASHTAGS = 5;
static const uint8_t HASHTAGS_STRING = 6;
static const uint8_t REQUESTED_BY = 7;
static const uint8_t REQUESTED_BY_STRING = 8;
static const uint8_t REQUESTED_BY_PHRASE = 9;
static const uint8_t FILES = 10;
static const uint8_t USER = 11;
static const uint8_t IS_VIDEO = 12;
}  // namespace TaskIndex

/**
 * @constructor
 */
InstagramTask::InstagramTask() {}

/**
 * @brief InstagramTask::defineTaskArguments
 *
 * This method defines all of the arguments that are available to be set for the Task
 *
 */
void InstagramTask::defineTaskArguments() {
  m_arguments.clear();
  m_arguments.emplace_back(std::move(new TaskArgument{"header", Type::TEXT, QString{}}));
  m_arguments.emplace_back(std::move(new TaskArgument{"description", Type::TEXT, TypeVariant{QString{}}}));
  m_arguments.emplace_back(std::move(new TaskArgument{"datetime", Type::TEXT, QString{}}));
  m_arguments.emplace_back(std::move(new TaskArgument{"promote_share", Type::TEXT, QString{}}));
  m_arguments.emplace_back(std::move(new TaskArgument{"link_in_bio", Type::TEXT, QString{}}));
  m_arguments.emplace_back(std::move(new TaskArgument{"hashtags", Type::STRINGVECTOR, QVector<QString>{}}));
  m_arguments.emplace_back(std::move(new TaskArgument{"hashtags_string", Type::TEXT, QString{}}));
  m_arguments.emplace_back(std::move(new TaskArgument{"requested_by", Type::STRINGVECTOR, QVector<QString>{}}));
  m_arguments.emplace_back(std::move(new TaskArgument{"requested_by_string", Type::TEXT, QString{}}));
  m_arguments.emplace_back(std::move(new TaskArgument{"requested_by_phrase", Type::TEXT, QString{}}));
  m_arguments.emplace_back(std::move(new TaskArgument{"files", Type::FILEVECTOR, QVector<KFileData>{}}));
  m_arguments.emplace_back(std::move(new TaskArgument{"user", Type::TEXT, QString{}}));
  m_arguments.emplace_back(std::move(new TaskArgument{"is_video", Type::BOOLEAN, bool{}}));
}

/**
 * @brief InstagramTask::setArgument
 * @param [in] {QString}      name   The name of the argment
 * @param [in] {TypeVariant}  value  The value of the argument
 */
void InstagramTask::setArgument(QString name, TypeVariant value) {
  for (auto&& argument : m_arguments) {
    if (argument->text() == name) {
      if (isIndex(value.index(), VariantIndex::STRVEC)) {
        argument->setValue(std::get<VariantIndex::STRVEC>(value));
        return;
      }
    }
  }
}

/**
 * @warning This method is used to add values to an argument, and can only be used on arguments whose type is a form of container.
 *
 * @brief InstagramTask::addArgument
 *
 * @param [in] {QString}    name  The name of the argument
 * @param [in] {KFileData}  file  A data structure to be added to a container of files.
 *                                The KFileData structure contains metadata about a file and
 *                                its data as a byte array
 */
void InstagramTask::addArgument(QString name, Scheduler::KFileData file) {
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
 * @brief InstagramTask::addArgument
 *
 * @param [in] {QString}      name  The name of the argument
 * @param [in] {TypeVariant}  file  A data structure to be added to a container of files.
 *                                The KFileData structure contains metadata about a file and
 *                                its data as a byte array
 */
void InstagramTask::removeArgument(QString name, Scheduler::TypeVariant value) {
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
 * @brief InstagramTask::addArgument
 *
 * @param [in] {QString} name     The name of the argument
 * @param [in] {QString} string   A string value intended to be added to a container of strings
 */
void InstagramTask::addArgument(QString name, QString string) {
  for (const auto& argument : m_arguments) {
    if (argument->text() == name) {
      argument->insert(string);
      return;
    }
  }
}


/**
 * @brief InstagramTask::getTaskArgument
 *
 * @param [in] {QString} name   The name of the argument to retrieve
 * @return [out] {TaskArgument}  The argument
 */
TaskArgument&& InstagramTask::getTaskArgument(QString name) {
  for (const auto& argument : m_arguments) {
    if (argument->text() == name) {
      return std::move(*argument);
    }
  }
  throw std::invalid_argument("Argument not found");
}

/**
 * @brief InstagramTask::getTaskArgumentValue
 *
 * @param [in] {QString} name   The name of the argument to retrieve
 * @return [out] {TypeVariant}  The value of the argument
 */
const TypeVariant InstagramTask::getTaskArgumentValue(QString name) {
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
 * @brief InstagramTask::getArgumentValues
 * @typedef QVector<QString> is aliased to ArgumentValues
 *
 * @return [out] {ArgumentValues} A vector of strings for all of the arguments that can be represented as a string.
 */
ArgumentValues InstagramTask::getArgumentValues() {
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
 * @brief InstagramTask::getArgumentNames
 *
 * @return [out] {QVector<QString>} A vector of argument names as strings.
 */
QVector<QString> InstagramTask::getArgumentNames() {
  return QVector<QString>{
      Scheduler::Args::DESCRIPTION_TYPE,
      Scheduler::Args::HASHTAG_TYPE,
      Scheduler::Args::REQUESTED_BY_TYPE,
      Scheduler::Args::PROMOTE_TYPE,
      Scheduler::Args::LINK_BIO_TYPE,
      Scheduler::Args::HEADER_TYPE,
      Scheduler::Args::REQUESTED_BY_PHRASE
  };
}

/**
 * @warning This method is used to claim ownership of the task's arguments. Use of this method will effectively REMOVE all arguments from
 *          the task upon which it is called.
 *
 * @brief InstagramTask::getTaskArguments
 * @typedef std::vector<std::unique_ptr<TaskArgument> is aliased to TaskArguments
 *
 * @return [out] {std::vector<std::unique_ptr<TaskArgument>} An R-value reference to a vector of unique pointers to the task's arguments.
 *
 */
const TaskArguments&& InstagramTask::getTaskArguments() { return std::move(m_arguments); }

/**
 * @brief InstagramTask::setDefaultValues
 *
 * Sets default values for the task's arguments
 */
void InstagramTask::setDefaultValues() {
  setArgument("header", TypeVariant{QString{"Learn to speak like native Korean speakers 🙆‍♀️🇰🇷"}});
  setArgument("promote_share", TypeVariant{QString{"Share the post through IG story if you enjoy the phrase 🙋‍♀️"}});
  setArgument("link_in_bio", TypeVariant{QString{"Subscribe to my YouTube channel (link 🔗in bio) to learn more about Korean language and culture ❤"}});
  setArgument("requested_by_phrase", TypeVariant{QString{"The phrase was requested by "}});
}

/**
 * @brief getType
 *
 * @return [out] {TaskType} The type of task
 */
Scheduler::TaskType InstagramTask::getType() { return Scheduler::TaskType::INSTAGRAM; };

/**
 * @brief InstagramTask::clear
 *
 * Clears the value of each task argument
 */
void InstagramTask::clear() {
  for (const auto& argument : m_arguments) {
    argument->clear();
  }
}

/**
 * @brief InstagramTask::hasFiles
 *
 * @return [out] {bool} Indicates whether the task has files.
 */
bool InstagramTask::hasFiles() {
  return !std::get<VariantIndex::FILEVEC>(getTaskArgumentValue("files")).empty();
}

/**
 * @brief InstagramTask::hasFiles
 *
 * @return [out] {QVector<KFileData>} A vector of data structures representing file metadata and the file data as bytes.
 */
const QVector<Scheduler::KFileData> InstagramTask::getFiles() {
  return std::get<VariantIndex::FILEVEC>(getTaskArgumentValue("files"));
}

/**
 * @brief InstagramTask::isReady
 *
 * @return [out] {bool} A boolean value indicating whether the minimal requirements sufficient to appropriately
 *                      perform the task have been met.
 */
bool InstagramTask::isReady() {
  auto header_size = std::get<VariantIndex::QSTRING>(getTaskArgumentValue("header")).size();
  auto description_size = std::get<VariantIndex::QSTRING>(getTaskArgumentValue("description")).size();
  auto datetime_size = std::get<VariantIndex::QSTRING>(getTaskArgumentValue("datetime")).size();
  auto promote_share_size = std::get<VariantIndex::QSTRING>(getTaskArgumentValue("promote_share")).size();
  auto link_in_bio_size = std::get<VariantIndex::QSTRING>(getTaskArgumentValue("link_in_bio")).size();
  auto hashtags_size = std::get<VariantIndex::QSTRING>(getTaskArgumentValue("hashtags_string")).size();
  auto requested_by_size = std::get<VariantIndex::QSTRING>(getTaskArgumentValue("requested_by_string")).size();
  auto hasFiles = std::get<VariantIndex::FILEVEC>(getTaskArgumentValue("files")).size();
  auto user_size = std::get<VariantIndex::QSTRING>(getTaskArgumentValue("user")).size();

  return header_size > 0 && description_size > 0 && datetime_size > 0 &&
         promote_share_size > 0 && link_in_bio_size > 0 &&
         hashtags_size > 0 && requested_by_size > 0 && hasFiles && user_size > 0;
}

/**
 * @destructor
 */
InstagramTask::~InstagramTask() {
}
