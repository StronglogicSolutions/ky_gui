#ifndef ARGDIALOG_H
#define ARGDIALOG_H

#include <QDialog>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QPushButton>
#include <string_view>
#include <unordered_map>

namespace Args {
const QString DESCRIPTION_TYPE = "description";
const QString HASHTAG_TYPE = "hashtag";
const QString PROMOTE_TYPE = "promote/share";
const QString LINK_BIO_TYPE = "link/bio";
const QString REQUESTED_BY_TYPE = "requested by";
}  // namespace Args
typedef std::string Str;
//namespace Emoji {
//    Str SmilingFaceEyes{"U+1F60A"};
//    const char* SmilingFaceEyesBytes = "😊";
//}
typedef struct Task {
    int mask;
    std::vector<std::string> args;
} Task;

typedef struct KFile {
  QString name;
  QString path;
} KFile;

typedef struct IGPost {
  std::string description;
  std::string datetime;
  std::string promote_share = "Promote share";
//  std::string link_in_bio = u8"Download a FREE PDF of basic 245 verbs (link 🔗 in bio 👆)";
  std::string link_in_bio = "Link inbio";
  std::vector<std::string> hashtags;
  std::vector<std::string> requested_by;
  const char *requested_by_phrase = "The phrase was requested by ";
  KFile video;
  bool isReady() {
    return description.size() > 0 && datetime.size() > 0 &&
           promote_share.size() > 0 && link_in_bio.size() > 0 &&
           hashtags.size() > 0 && requested_by.size() > 0 &&
           video.path.size() > 0;
  }
} IGPost;

namespace Ui {
class ArgDialog;
}

class ArgDialog : public QDialog {
  Q_OBJECT

 public:
  explicit ArgDialog(QWidget *parent = nullptr);
  ~ArgDialog();

 signals:
  void uploadFile(QByteArray bytes);
  void taskRequestReady(Task task, bool file_pending);

 private:
  void clearPost();
  void clearTask();
  void addToArgList(QString value, QString type);
  void addOrReplaceInArgList(QString value, QString type);
  void addHashtag(QString tag);
  void addRequestedBy(QString value);
  void setTaskArguments();
  Ui::ArgDialog *ui;
  void addItem(QString value, QString type);
  Task m_task;
  IGPost m_ig_post;
};

#endif  // ARGDIALOG_H
