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
#include <include/task/generic_task.hpp>
#include <include/task/task.hpp>
#include <string_view>
#include <unordered_map>

#include <QVBoxLayout>
#include <QLabel>
#include <QMovie>

using namespace Scheduler;

inline Task* createTask(QString task_name = GENERIC_NAME) {
  Task* task;
  if (task_name == INSTAGRAM_NAME) {
     task = new InstagramTask{};
  } else {
    task = new GenericTask{};
  }
  task->defineTaskArguments();
  task->setDefaultValues();

  return task;
}

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
  void setAppName(QString task_name);
  void setConfig(QJsonObject config);
  void notifyClientSuccess();

  ~ArgDialog();

 signals:
  void taskRequestReady(Scheduler::Task *task);

 protected:
  void showEvent(QShowEvent* event) override;

 private:
  void clearPost();
  void defaultPost();
  void clearTask();
  void displayLoader(bool visible);
  void addToArgList(QString value, QString type);
  void addOrReplaceInArgList(QString value, QString type);
  void addHashtag(QString tag);
  void addRequestedBy(QString value);
  void setTaskArguments();
  void setArgTypes();
  Ui::ArgDialog *ui;
  void addItem(QString value, QString type);
  void addFile(QString path);

  Task*         m_task;
  Task*         m_pending_task;
  QString       m_file_path;
  QJsonObject   m_config;
  QString       m_app_name;
  QMovie*       m_loader;
  QWidget      m_loader_widget;
  QVBoxLayout  m_loader_layout;
};

#endif  // ARGDIALOG_H
