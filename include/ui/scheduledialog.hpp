#ifndef SCHEDULEDIALOG_HPP
#define SCHEDULEDIALOG_HPP

#include <QDialog>
#include <QListView>
#include "headers/util.hpp"


struct ScheduledTask {
QString          id;
QString          app;
QDateTime        time;
QString          flags;
QString          completed;
QString          recurring;
QString          notify;
QString          runtime;
QVector<QString> files;
};

namespace constants {

}

namespace Ui {
class ScheduleDialog;
}

class ScheduleDialog : public QDialog
{
  Q_OBJECT

 public:
  explicit ScheduleDialog(QWidget *parent = nullptr);
  void     insert_tasks(QVector<QString> tasks);
  ~ScheduleDialog();
  void     clear();

// signals:
//  void appRequest(KApplication application, constants::RequestType type);

 protected:
  virtual void showEvent(QShowEvent *) override;

 private:
  void         toggleInputMode();
  void         setFields(QString app_name);
  KApplication readFields();

  Ui::ScheduleDialog*        ui;
  QVector<ScheduledTask> m_tasks;
};

#endif // SCHEDULEDIALOG_HPP
