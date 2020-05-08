#include <include/task/instagram_task.hpp>

using namespace Task;

const TaskArguments InstagramTask::getTaskArguments() {
  return QVector<TaskArgument>{TaskArgument{"header", Type::TEXT},
                               TaskArgument{"description", Type::TEXT},
                               TaskArgument{"datetime", Type::DATETIME},
                               TaskArgument{"promote_share", Type::TEXT},
                               TaskArgument{"link_in_bio", Type::TEXT},
                               TaskArgument{"hashtags", Type::STRINGVECTOR},
                               TaskArgument{"requested_by", Type::STRINGVECTOR},
                               TaskArgument{"requested_by_phrase", Type::TEXT},
                               TaskArgument{"files", Type::FILEVECTOR},
                               TaskArgument{"user", Type::TEXT},
                               TaskArgument{"is_video", Type::BOOLEAN}};
}

InstagramTask::~InstagramTask() {}
