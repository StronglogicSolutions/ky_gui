#ifndef SCHEDULEDIALOG_HPP
#define SCHEDULEDIALOG_HPP

#include <QDialog>
#include <QThread>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QStandardItemModel>
#include "headers/util.hpp"

namespace Ui {
class ScheduleDialog;
}
using AppMap = std::vector<std::string, int32_t>;
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
  virtual void showEvent(QShowEvent *)     override;
//  virtual void hideEvent(QHideEvent *e)    override;
  virtual void keyPressEvent(QKeyEvent* e) override;

 private:
  void           setFields(ScheduledTask task);
  void           SetApps(const CommandMap& map);
  ScheduledTask  readFields();
  void           refreshUI();

  Ui::ScheduleDialog*        ui;
  QVector<ScheduledTask> m_tasks;
  QStandardItemModel     m_task_model;
  QStandardItemModel     m_task_filter_model;
  AppMap                 m_apps;
};

#endif // SCHEDULEDIALOG_HPP
