#ifndef SCHEDULEDIALOG_HPP
#define SCHEDULEDIALOG_HPP

#include <QDialog>
#include <QThread>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QStandardItemModel>
#include <unordered_map>
#include "headers/util.hpp"

namespace Ui {
class ScheduleDialog;
}
using AppMap = std::unordered_map<std::string, uint32_t>;
using namespace constants;
class ScheduleDialog : public QDialog
{
  Q_OBJECT
 public:
  explicit ScheduleDialog(QWidget* parent = nullptr);
  void     insert_tasks(QVector<QString> tasks);
  void     SetApps(const CommandMap& map);
  void     receive_response(RequestType type, QVector<QString> v);
  ~ScheduleDialog();
  void     clear();

 signals:
  void updateSchedule();
  void scheduleRequest(constants::RequestType type, ScheduledTask task);

 protected:
  virtual void showEvent(QShowEvent* e)     override;

 private:
  void           setFields(ScheduledTask task);
  ScheduledTask  readFields();
  void           refreshUI();
  ScheduledTask  GetTask() const;

  Ui::ScheduleDialog*        ui;
  QVector<ScheduledTask> m_tasks;
  QVector<size_t>        m_task_indexes;
  QStandardItemModel     m_task_model;
  QStandardItemModel     m_task_filter_model;
  AppMap                 m_apps;
  int32_t                m_mask;
  bool                   m_refreshing;
};

#endif // SCHEDULEDIALOG_HPP
