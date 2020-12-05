#ifndef SCHEDULEDIALOG_HPP
#define SCHEDULEDIALOG_HPP

#include <QDialog>
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

 signals:
  void updateSchedule();

 protected:
  virtual void showEvent(QShowEvent *) override;

 private:
  void         setFields(ScheduledTask task);

  Ui::ScheduleDialog*        ui;
  QVector<ScheduledTask> m_tasks;
};

#endif // SCHEDULEDIALOG_HPP
