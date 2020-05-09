#include <include/task/instagram_task.hpp>

using namespace Scheduler;

void InstagramTask::defineTaskArguments() {
  std::vector<std::unique_ptr<TaskArgumentBase>> args{};
  args.emplace_back(std::make_unique<TaskArgument<QString>>("header", Type::TEXT, QString{}));
  args.emplace_back(std::make_unique<TaskArgument<QString>>("description", Type::TEXT, QString{}));
  args.emplace_back(std::make_unique<TaskArgument<QString>>("datetime", Type::DATETIME, QString{}));
  args.emplace_back(std::make_unique<TaskArgument<QString>>("promote_share", Type::TEXT, QString{}));
  args.emplace_back(std::make_unique<TaskArgument<QString>>("link_in_bio", Type::TEXT, QString{}));
  args.emplace_back(
      std::make_unique<TaskArgument<std::vector<QString>>>("hashtags", Type::STRINGVECTOR, std::vector<QString>{}));
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

template <typename T>
void InstagramTask::setArgument(QString name, T value) {
  if constexpr (std::is_same_v<T, QString>) {
    auto it = std::find_if(m_arguments.begin(), m_arguments.end(),
                           [name, value](auto argument) { return argument.text() == name; });
    if (it != m_arguments.end()) {
      it->value = value;
    }
  }
}

void InstagramTask::clear() {
  // remove all arg values
}

InstagramTask::~InstagramTask() {}
