﻿#include <include/task/instagram_task.hpp>

using namespace Scheduler;

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
static const uint8_t MASK = 13;
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
  m_arguments.emplace_back(std::make_unique<TaskArgument>("header", Type::TEXT, QString{}));
  m_arguments.emplace_back(std::make_unique<TaskArgument>("description", Type::TEXT, TypeVariant{QString{}}));
  m_arguments.emplace_back(std::make_unique<TaskArgument>("datetime", Type::TEXT, QString{}));
  m_arguments.emplace_back(std::make_unique<TaskArgument>("promote_share", Type::TEXT, QString{}));
  m_arguments.emplace_back(std::make_unique<TaskArgument>("link_in_bio", Type::TEXT, QString{}));
  m_arguments.emplace_back(std::make_unique<TaskArgument>("hashtags", Type::STRINGVECTOR, QVector<QString>{}));
  m_arguments.emplace_back(std::make_unique<TaskArgument>("hashtags_string", Type::TEXT, QString{}));
  m_arguments.emplace_back(std::make_unique<TaskArgument>("requested_by", Type::STRINGVECTOR, QVector<QString>{}));
  m_arguments.emplace_back(std::make_unique<TaskArgument>("requested_by_string", Type::TEXT, QString{}));
  m_arguments.emplace_back(std::make_unique<TaskArgument>("requested_by_phrase", Type::TEXT, QString{}));
  m_arguments.emplace_back(std::make_unique<TaskArgument>("files", Type::FILEVECTOR, QVector<KFileData>{}));
  m_arguments.emplace_back(std::make_unique<TaskArgument>("user", Type::TEXT, QString{}));
  m_arguments.emplace_back(std::make_unique<TaskArgument>("is_video", Type::BOOLEAN, bool{}));
  m_arguments.emplace_back(std::make_unique<TaskArgument>("mask", Type::INTEGER, -1));
}

/**
 * @brief InstagramTask::setArgument
 * @param [in] {QString} name The name of the argment
 * @param [in] {TypeVariant} value The value of the argument
 */
void InstagramTask::setArgument(QString name, TypeVariant value) {
  for (const auto& argument : m_arguments) {
    if (argument->text() == name) {
      argument->setValue(value);
      return;
    }
  }
}

/**
 * @brief InstagramTask::addArgument
 * @param [in] {QString} name The name of the argument
 * @param [in] {KFileData} file A data structure to be added to a container of files.
 *                         The KFileData structure contains metadata about a file and
 *                         its data as a byte array
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
 * @brief InstagramTask::addArgument
 * @param [in] {QString} name The name of the argument
 * @param [in] {QString} string A string value intended to be added to a container of strings
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
 * @param name
 * @return
 */
const TypeVariant InstagramTask::getTaskArgument(QString name) {
  for (const auto& argument : m_arguments) {
    if (argument->text() == name) {
      return argument->getValue();
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
 * @brief InstagramTask::setDefaultValues
 */
void InstagramTask::setDefaultValues() {
  setArgument("header", TypeVariant{QString{"Learn to speak like native Korean speakers 🙆‍♀️🇰🇷"}});
  setArgument("promote_share", TypeVariant{QString{"Share the post through IG story if you enjoy the phrase 🙋‍♀️"}});
  setArgument("link_in_bio", TypeVariant{QString{"Subscribe to my YouTube channel (link 🔗in bio) to learn more about Korean language and culture ❤"}});
  setArgument("requested_by_phrase", TypeVariant{QString{"The phrase was requested by "}});
}

/**
 * @brief getType
 * @return {Scheduler::TaskType} The type of task
 */
Scheduler::TaskType InstagramTask::getType() { return Scheduler::TaskType::INSTAGRAM; };

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
  return !std::get<VariantIndex::FILEVEC>(getTaskArgument("files")).empty();
}

/**
 * @brief InstagramTask::hasFiles
 * @return
 */
const QVector<Scheduler::KFileData> InstagramTask::getFiles() {
  return std::get<VariantIndex::FILEVEC>(getTaskArgument("files"));
}

/**
 * @brief InstagramTask::isReady
 * @return
 */
bool InstagramTask::isReady() {
  auto header_size = std::get<VariantIndex::QSTRING>(getTaskArgument("header")).size();
  auto description_size = std::get<VariantIndex::QSTRING>(getTaskArgument("description")).size();
  auto datetime_size = std::get<VariantIndex::QSTRING>(getTaskArgument("datetime")).size();
  auto promote_share_size = std::get<VariantIndex::QSTRING>(getTaskArgument("promote_share")).size();
  auto link_in_bio_size = std::get<VariantIndex::QSTRING>(getTaskArgument("link_in_bio")).size();
  auto hashtags_size = std::get<VariantIndex::QSTRING>(getTaskArgument("hashtags_string")).size();
  auto requested_by_size = std::get<VariantIndex::QSTRING>(getTaskArgument("requested_by_string")).size();
  auto hasFiles = std::get<VariantIndex::FILEVEC>(getTaskArgument("files")).size();
  auto user_size = std::get<VariantIndex::QSTRING>(getTaskArgument("user")).size();

  return header_size > 0 && description_size > 0 && datetime_size > 0 &&
         promote_share_size > 0 && link_in_bio_size > 0 &&
         hashtags_size > 0 && requested_by_size > 0 && hasFiles && user_size > 0;
}

/**
 * @destructor
 */
InstagramTask::~InstagramTask() {
}
