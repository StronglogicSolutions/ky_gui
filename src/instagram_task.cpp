#include <include/task/instagram_task.hpp>

using namespace Scheduler;

namespace TaskIndex {
static const uint8_t HEADER = 0;
static const uint8_t DESCRIPTION = 1;
static const uint8_t DATETIME = 2;
static const uint8_t PROMOTE_SHARE = 3;
static const uint8_t LINK_IN_BIO = 4;
static const uint8_t HASHTAGS = 5;
static const uint8_t REQUESTED_BY = 6;
static const uint8_t REQUESTED_BY_PHRASE = 7;
static const uint8_t FILES = 8;
static const uint8_t USER = 9;
static const uint8_t IS_VIDEO = 10;
}  // namespace TaskIndex

/**
 * @constructor
 * @param [in] {KFileData} k_file
 */
InstagramTask::InstagramTask(KFileData k_file) : files({k_file}) {}

/**
 * @constructor
 * @param [in] {QVector<KFileData>} k_files
 */
InstagramTask::InstagramTask(QVector<KFileData> k_files) : files(k_files) {}

/**
 * @brief InstagramTask::defineTaskArguments
 */
void InstagramTask::defineTaskArguments() {
  m_arguments.clear();
  m_arguments.emplace_back(std::make_unique<TaskArgument>("header", Type::TEXT, QString{}));
  m_arguments.emplace_back(std::make_unique<TaskArgument>("description", Type::TEXT, QString{}));
  m_arguments.emplace_back(std::make_unique<TaskArgument>("datetime", Type::DATETIME, QString{}));
  m_arguments.emplace_back(std::make_unique<TaskArgument>("promote_share", Type::TEXT, QString{}));
  m_arguments.emplace_back(std::make_unique<TaskArgument>("link_in_bio", Type::TEXT, QString{}));
  m_arguments.emplace_back(std::make_unique<TaskArgument>("hashtags", Type::TEXT, QString{}));
  m_arguments.emplace_back(std::make_unique<TaskArgument>("requested_by", Type::STRINGVECTOR, QVector<QString>{}));
  m_arguments.emplace_back(std::make_unique<TaskArgument>("requested_by_phrase", Type::TEXT, QString{}));
  m_arguments.emplace_back(std::make_unique<TaskArgument>("files", Type::FILEVECTOR, QVector<KFileData>{}));
  m_arguments.emplace_back(std::make_unique<TaskArgument>("user", Type::TEXT, QString{}));
  m_arguments.emplace_back(std::make_unique<TaskArgument>("is_video", Type::BOOLEAN, bool{}));
}

/**
 * @brief InstagramTask::setArgument
 * @param name
 * @param value
 */
void InstagramTask::setArgument(QString name, TypeVariant value) {
//  TaskIterator it =
//      std::find_if(m_arguments.begin(), m_arguments.end(), [name](auto argument) { return argument.text() == name; });
for (const auto& argument : m_arguments) {
  if (argument.get()->text() == name) {
    if (isIndex(value.index(), VariantIndex::QSTRING)) {
      argument.get()->setValue(value);
    } else if (isIndex(value.index(), VariantIndex::BOOLEAN)) {
      argument.get()->setValue(QString::number(std::get<VariantIndex::BOOLEAN>(value)));
    } else if (isIndex(value.index(), VariantIndex::INTEGER)) {
      argument.get()->setValue(QString::number(std::get<VariantIndex::INTEGER>(value)));
    } else {
      // Could not set argument value
      // TODO: Log here
    }
  }
  return;
}
//  if (it != m_arguments.end()) {
//    if (isIndex(value.index(), VariantIndex::QSTRING)) {
//      it->get()->setValue(value);
//    } else if (isIndex(value.index(), VariantIndex::BOOLEAN)) {
//      it->get()->setValue(QString::number(std::get<VariantIndex::BOOLEAN>(value)));
//    } else if (isIndex(value.index(), VariantIndex::INTEGER)) {
//      it->get()->setValue(QString::number(std::get<VariantIndex::INTEGER>(value)));
//    } else {
//      // Could not set argument value
//      // TODO: Log here
//    }
//  }
}

/**
 * @brief InstagramTask::addArgument
 * @param name
 * @param file
 */
void InstagramTask::addArgument(QString name, Scheduler::KFileData file) {
  for (const auto& argument : m_arguments) {
    if (argument.get()->text() == name) {
      argument.get()->insert(file);
      return;
    }
  }
}

/**
 * @brief InstagramTask::addArgument
 * @param name
 * @param string
 */
void InstagramTask::addArgument(QString name, QString string) {
  for (const auto& argument : m_arguments) {
    if (argument.get()->text() == name) {
      argument.get()->insert(string);
      return;
    }
  }
}

/**
 * @brief InstagramTask::getTaskArgument
 * @param name
 * @return
 */
const TypeVariant InstagramTask::getTaskArgument(QString name) {
  for (const auto& argument : m_arguments) {
    if (argument.get()->text() == name) {
      return argument.get()->getValue();
    }
  }
  return "";  // Perhaps we should throw
}

/**
 * @brief InstagramTask::getArgumentValues
 * @return
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
 * @brief InstagramTask::getTaskArguments
 * @return
 */
const TaskArguments InstagramTask::getTaskArguments() { return std::move(m_arguments); }


/**
 * @brief InstagramTask::getFiles
 * @return
 */
const QVector<KFileData> InstagramTask::getFiles() { return files; }

/**
 * @brief InstagramTask::setDefaultValues
 */
void InstagramTask::setDefaultValues() {
  m_arguments.at(TaskIndex::HEADER)
      ->setValue("Learn to speak like native Korean speakers 🙆‍♀️🇰🇷");
  m_arguments.at(TaskIndex::PROMOTE_SHARE)
      ->setValue("Share the post through IG story if you enjoy the phrase 🙋‍♀️");
  m_arguments.at(TaskIndex::LINK_IN_BIO)
      ->setValue("Subscribe to my YouTube channel (link 🔗in bio) to learn more about Korean language and culture ❤");
  m_arguments.at(TaskIndex::REQUESTED_BY_PHRASE)
      ->setValue("The phrase was requested by ");
}

/**
 * @brief getType
 * @return {Scheduler::TaskType} The type of task
 */
Scheduler::TaskType getType() { return Scheduler::TaskType::INSTAGRAM; };

/**
 * @brief InstagramTask::clear
 */
void InstagramTask::clear() {
  for (const auto& argument : m_arguments) {
    argument->clear();
  }
}

/**
 * @brief InstagramTask::hasFiles
 * @return
 */
bool InstagramTask::hasFiles() {
  return !files.empty();
}

/**
 * @destructor
 */
InstagramTask::~InstagramTask() {}
