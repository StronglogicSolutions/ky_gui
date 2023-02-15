#ifndef SCHEDULEDIALOG_HPP
#define SCHEDULEDIALOG_HPP

#include <QDialog>
#include <QThread>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QStandardItemModel>
#include <unordered_map>
#include <util/util.hpp>

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
  void     InsertTasks(QVector<QString> tasks);
  void     SetApps(const CommandMap& map);
  void     ReceiveResponse(RequestType type, QVector<QString> v);
  ~ScheduleDialog();
  void     Clear();

 signals:
  void UpdateSchedule();
  void SchedulerRequest(constants::RequestType type, ScheduledTask task);

 protected:
  virtual void showEvent(QShowEvent* e)     override;

 private:
  void           SetFields(ScheduledTask task);
  ScheduledTask  ReadFields();
  void           RefreshUI();
  ScheduledTask  GetTask(int index) const;

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
