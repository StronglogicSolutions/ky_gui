#ifndef SCHEDULEDIALOG_HPP
#define SCHEDULEDIALOG_HPP

#include <QDialog>
#include "headers/util.hpp"

namespace Ui {
class ScheduleDialog;
}

using namespace constants;
class ScheduleDialog : public QDialog
{
  Q_OBJECT
 public:
  explicit ScheduleDialog(QWidget *parent = nullptr);
  void     insert_tasks(QVector<QString> tasks);
  void     receive_response(QVector<QString> v);
  ~ScheduleDialog();
  void     clear();

 signals:
  void updateSchedule();
  void scheduleRequest(constants::RequestType type, ScheduledTask task);

 protected:
  virtual void showEvent(QShowEvent *) override;

 private:
  void           setFields(ScheduledTask task);
  ScheduledTask  readFields();

  Ui::ScheduleDialog*        ui;
  QVector<ScheduledTask> m_tasks;
};

#endif // SCHEDULEDIALOG_HPP
