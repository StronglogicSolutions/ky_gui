#ifndef ARGDIALOG_H
#define ARGDIALOG_H

#include <QDialog>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QPushButton>
#include <string_view>
#include <unordered_map>
#include <QKeyEvent>
#include <headers/util.hpp>
#include <include/kfiledialog.h>

namespace Args {
const QString HEADER_TYPE = "header";
const QString DESCRIPTION_TYPE = "description";
const QString HASHTAG_TYPE = "hashtag";
const QString PROMOTE_TYPE = "promote/share";
const QString LINK_BIO_TYPE = "link/bio";
const QString REQUESTED_BY_TYPE = "requested by";
}  // namespace Args

typedef std::string Str;

typedef struct KFile {
  QString name;
  QString path;
  FileType type;
} KFile;

typedef struct IGPost {
  std::string header = "Learn to speak like native Korean speakers 🙆‍♀️🇰🇷";
  std::string description;
  std::string datetime;
  std::string promote_share = "Share the post through IG story if you enjoy the phrase 🙋‍♀️";
  std::string link_in_bio = "Subscribe to my YouTube channel (link 🔗in bio) to learn more about Korean language and culture ❤";
  std::vector<std::string> hashtags;
  std::vector<std::string> requested_by;
  const char *requested_by_phrase = "The phrase was requested by ";
  std::vector<KFile> files;
  std::string user;
  bool is_video;
  bool isReady() {
    return header.size() > 0 && description.size() > 0 && datetime.size() > 0 &&
           promote_share.size() > 0 && link_in_bio.size() > 0 &&
           hashtags.size() > 0 && requested_by.size() > 0 && !files.empty() &&
           files.at(0).path.size() > 0 && user.size() > 0;
  }
} IGPost;

namespace Ui {
class ArgDialog;
}

class ArgDialog : public QDialog {
  Q_OBJECT

 public:
  explicit ArgDialog(QWidget *parent = nullptr);
  virtual void keyPressEvent(QKeyEvent* e);
  void setFilePath(QString path);
  virtual void accept() override;
  void setConfig(QString config_string);

  ~ArgDialog();

 signals:
  void uploadFiles(QVector<KFileData> files);
  void taskRequestReady(Task task, bool file_pending);

 private:
  void clearPost();
  void defaultPost();
  void clearTask();
  void addToArgList(QString value, QString type);
  void addOrReplaceInArgList(QString value, QString type);
  void addHashtag(QString tag);
  void addRequestedBy(QString value);
  void setTaskArguments();
  Ui::ArgDialog *ui;
  void addItem(QString value, QString type);
  void addFile(QString path);
  Task m_task;
  IGPost m_ig_post;
  QString m_file_path;
  QString m_config_string;
};

#endif  // ARGDIALOG_H
