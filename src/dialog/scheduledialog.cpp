#include "include/ui/scheduledialog.hpp"
#include "ui_scheduledialog.h"
#include <QDebug>

const QString completed_string(QString s) {
  if (s.compare("0") == 0)
    return "Scheduled";
  else
  if (s.compare("1") == 0)
    return "Success";
  else
  if (s.compare("2") == 0)
    return "Failed";
  else
  if (s.compare("3") == 0)
    return "Retry Failed";
  return s;
}

const QString recurring_string(QString s) {
  if (s.compare("0") == 0)
    return "No";
  else
  if (s.compare("1") == 0)
    return "Hourly";
  else
  if (s.compare("2") == 0)
    return "Daily";
  else
  if (s.compare("3") == 0)
    return "Weekly";
  else
  if (s.compare("4") == 0)
    return "Monthly";
  else
  if (s.compare("5") == 0)
    return "Yearly";
  return s;
}

ScheduleDialog::ScheduleDialog(QWidget *parent) :
                                        QDialog(parent),
                                        ui(new Ui::ScheduleDialog)
{
  ui->setupUi(this);

  QObject::connect(ui->close, &QPushButton::clicked, this, [this]() {
    this->close();
  });

  QObject::connect(
    ui->taskList,
    static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
    this,
    [this]() {
      if (!m_tasks.empty())
        setFields(m_tasks.at(ui->taskList->currentIndex()));
    }
  );
}

ScheduleDialog::~ScheduleDialog()
{
  delete ui;
}

void ScheduleDialog::showEvent(QShowEvent *) {
  for (const auto& task : m_tasks) {
    ui->taskList->addItem(QString{task.id + ": " + task.time.toString()});
  }
}

void ScheduleDialog::setFields(ScheduledTask task) {
  ui->appText->setText(task.app);
  ui->timeText->setText(task.time.toString());
  ui->flagsText->setText(task.flags);
  ui->completedText->setText(completed_string(task.completed));
  ui->notifyText->setText((task.notify.compare("0") == 0) ? "No" : "Yes");
  ui->recurringText->setText(recurring_string(task.recurring));
  ui->runtimeText->setText(task.runtime);
  ui->filesText->setText(task.files.at(0));
}


void ScheduleDialog::insert_tasks(QVector<QString> task_arguments) {
  uint16_t arg_num = task_arguments.size();
QVector<QString> files;
  for (uint16_t i = 1; i < arg_num; i += 9) {
    m_tasks.push_back(ScheduledTask{
      .id        = task_arguments.at(i + 0),
      .app       = task_arguments.at(i + 1),
      .time      = QDateTime::fromSecsSinceEpoch(task_arguments.at(i + 2).toLongLong()),
      .flags     = task_arguments.at(i + 3),
      .completed = task_arguments.at(i + 4),
      .recurring = task_arguments.at(i + 5),
      .notify    = task_arguments.at(i + 6),
      .runtime   = task_arguments.at(i + 7),
      .files     = QVector<QString>{task_arguments.at(i + 8)} // parse
    });
  }
}

void ScheduleDialog::clear() {
  m_tasks          .clear();
  ui->taskList     ->clear();
  ui->appText      ->clear();
  ui->timeText     ->clear();
  ui->completedText->clear();
  ui->flagsText    ->clear();
  ui->notifyText   ->clear();
  ui->recurringText->clear();
  ui->runtimeText  ->clear();
  ui->filesText    ->clear();
}




//QStandardItem* create_task_list_item(ScheduledTask task) {

//  QString task_item_text{"ID: %0 - Time: %1"};

//  return new QStandardItem{
//    task_item_text
//      .arg(task.id)
//      .arg(task.time.toString())
//  };
//}
