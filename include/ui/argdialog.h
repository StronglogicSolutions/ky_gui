#ifndef ARGDIALOG_H
#define ARGDIALOG_H

#include <include/ui/kfiledialog.h>
#include <QDialog>
#include <QFile>
#include <QFileDialog>
#include <QKeyEvent>
#include <QMessageBox>
#include <QPushButton>
#include <headers/util.hpp>
#include <include/task/instagram_task.hpp>
#include <include/task/task.hpp>
#include <string_view>
#include <unordered_map>

using namespace Scheduler;

typedef std::string Str;

namespace Ui {
class ArgDialog;
}

class ArgDialog : public QDialog {
  Q_OBJECT

 public:
  explicit ArgDialog(QWidget *parent = nullptr);
  virtual void keyPressEvent(QKeyEvent* e) override;
  void setFilePath(QString path);
  virtual void accept() override;
  void setConfig(QString config_string);
  void notifyClientSuccess();

  ~ArgDialog();

 signals:
  void taskRequestReady(Scheduler::Task *task);

 private:
  void clearPost();
  void defaultPost();
  void clearTask();
  void addToArgList(QString value, QString type);
  void addOrReplaceInArgList(QString value, QString type);
  void addHashtag(QString tag);
  void addRequestedBy(QString value);
  void setTaskArguments();
  void setArgTypes();
  Ui::ArgDialog *ui;
  void addItem(QString value, QString type);
  void addFile(QString path);
  Task *m_task;
  QString m_file_path;
  QString m_config_string;
};

#endif  // ARGDIALOG_H
