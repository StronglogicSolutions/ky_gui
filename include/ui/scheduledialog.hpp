#ifndef SCHEDULEDIALOG_HPP
#define SCHEDULEDIALOG_HPP

#include <QDialog>
#include <QThread>
#include <QTableWidget>
#include <QTableWidgetItem>
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
  void     receive_response(RequestType type, QVector<QString> v);
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
  void           run_thread(QString string);

  Ui::ScheduleDialog*        ui;
  QVector<ScheduledTask> m_tasks;
};

#endif // SCHEDULEDIALOG_HPP
