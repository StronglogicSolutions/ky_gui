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

InstagramTask::InstagramTask(KFileData k_file) : files({k_file}) {}
InstagramTask::InstagramTask(QVector<KFileData> k_files) : files(k_files) {}

void InstagramTask::defineTaskArguments() {
  std::vector<std::unique_ptr<TaskArgumentBase>> args{};
  args.emplace_back(std::make_unique<TaskArgument<QString>>("header", Type::TEXT, QString{}));
  args.emplace_back(std::make_unique<TaskArgument<QString>>("description", Type::TEXT, QString{}));
  args.emplace_back(std::make_unique<TaskArgument<QString>>("datetime", Type::DATETIME, QString{}));
  args.emplace_back(std::make_unique<TaskArgument<QString>>("promote_share", Type::TEXT, QString{}));
  args.emplace_back(std::make_unique<TaskArgument<QString>>("link_in_bio", Type::TEXT, QString{}));
  args.emplace_back(std::make_unique<TaskArgument<QString>>("hashtags", Type::TEXT, QString{}));
  args.emplace_back(
      std::make_unique<TaskArgument<std::vector<QString>>>("requested_by", Type::STRINGVECTOR, std::vector<QString>{}));
  args.emplace_back(std::make_unique<TaskArgument<QString>>("requested_by_phrase", Type::TEXT, QString{}));
  args.emplace_back(
      std::make_unique<TaskArgument<std::vector<KFileData>>>("files", Type::FILEVECTOR, std::vector<KFileData>{})),
      args.emplace_back(std::make_unique<TaskArgument<QString>>("user", Type::TEXT, QString{}));
  args.emplace_back(std::make_unique<TaskArgument<bool>>("is_video", Type::BOOLEAN, bool{}));
  m_arguments = args;
}

const TaskArguments InstagramTask::getTaskArguments() { return m_arguments; }

void InstagramTask::setArgument(QString name, TypeVariant value) {
  TaskIterator it =
      std::find_if(m_arguments.begin(), m_arguments.end(), [name](auto argument) { return argument.text() == name; });
  if (it != m_arguments.end()) {
    if (isIndex(value.index(), VariantIndex::QSTRING)) {
      it->get()->setValue(value);
    } else if (isIndex(value.index(), VariantIndex::BOOLEAN)) {
      it->get()->setValue(QString::number(std::get<VariantIndex::BOOLEAN>(value)));
    } else if (isIndex(value.index(), VariantIndex::INTEGER)) {
      it->get()->setValue(QString::number(std::get<VariantIndex::INTEGER>(value)));
    } else {
      // Could not set argument value
      // TODO: Log here
    }
  }
}

TypeVariant InstagramTask::getTaskArgument(QString name) {
  TaskIterator it =
      std::find_if(m_arguments.begin(), m_arguments.end(), [name](auto argument) { return argument.text() == name; });
  if (it != m_arguments.end()) {
    return it->get()->getValue();
  }
  return "";  // Perhaps we should throw
}

ArgumentValues InstagramTask::getArgumentValues() {
  ArgumentValues values{static_cast<int>(m_arguments.size())};
  for (const auto& argument : m_arguments) {
    if (!argument->isContainer()) {
      values.push_back(argument->getStringValue());
    }
  }
  return values;
}

const QVector<KFileData> InstagramTask::getFiles() { return files; }

void InstagramTask::setDefaultValues() {
  m_arguments.at(TaskIndex::HEADER)->setValue("Learn to speak like native Korean speakers 🙆‍♀️🇰🇷");
  m_arguments.at(TaskIndex::PROMOTE_SHARE)
      ->setValue("Share the post through IG story if you enjoy the phrase 🙋‍♀️");
  m_arguments.at(TaskIndex::LINK_IN_BIO)
      ->setValue("Subscribe to my YouTube channel (link 🔗in bio) to learn more about Korean language and culture ❤");
  m_arguments.at(TaskIndex::REQUESTED_BY_PHRASE)->setValue("The phrase was requested by ");
}

Scheduler::TaskType getType() { return Scheduler::TaskType::INSTAGRAM; };

void InstagramTask::clear() {
  for (const auto& argument : m_arguments) {
    argument->clear();
  }
}

InstagramTask::~InstagramTask() {}
