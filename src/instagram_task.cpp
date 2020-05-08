#include <include/task/instagram_task.hpp>

using namespace Task;

//  std::string header = "Learn to speak like native Korean speakers 🙆‍♀️🇰🇷";
//  std::string description;
//  std::string datetime;
//  std::string promote_share = "Share the post through IG story if you enjoy the phrase 🙋‍♀️";
//  std::string link_in_bio = "Subscribe to my YouTube channel (link 🔗in bio) to learn more about Korean language and
//  culture ❤"; std::vector<std::string> hashtags; std::vector<std::string> requested_by; const char
//  *requested_by_phrase = "The phrase was requested by "; std::vector<KFile> files; std::string user; bool is_video;
const QVector<TaskArgument> instagram_task_arguments{TaskArgument{"header", "Text"},
                                                     TaskArgument{"description", "Text"},
                                                     TaskArgument{"datetime", "Datetime"},
                                                     TaskArgument{"promote_share", "Text"},
                                                     TaskArgument{"link_in_bio", "Text"},
                                                     TaskArgument{"hashtags", "StringVector"},
                                                     TaskArgument{"requested_by", "StringVector"},
                                                     TaskArgument{"requested_by_phrase", "Text"},
                                                     TaskArgument{"files", "FileVector"},
                                                     TaskArgument{"user", "Text"},
                                                     TaskArgument{"is_video", "Boolean"}};

// const TaskArguments InstagramTask::getTaskArguments() {
//  return QVector<TaskArgument>{TaskArgument{"header", "Text"},
//                               TaskArgument{"description", "Text"},
//                               TaskArgument{"datetime", "Datetime"},
//                               TaskArgument{"promote_share", "Text"},
//                               TaskArgument{"link_in_bio", "Text"},
//                               TaskArgument{"hashtags", "StringVector"},
//                               TaskArgument{"requested_by", "StringVector"},
//                               TaskArgument{"requested_by_phrase", "Text"},
//                               TaskArgument{"files", "FileVector"},
//                               TaskArgument{"user", "Text"},
//                               TaskArgument{"is_video", "Boolean"}};
//}
