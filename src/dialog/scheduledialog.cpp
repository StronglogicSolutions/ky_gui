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

  QObject::connect(ui->saveTask, &QPushButton::clicked, this, [this]() {
    scheduleRequest(RequestType::UPDATE_SCHEDULE, readFields());
  });

  QObject::connect(ui->deleteTask, &QPushButton::clicked, this, [this]() {
    this->close();
  });

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

  QObject::connect(ui->fetchSchedule, &QPushButton::clicked, this, [this]() {
    updateSchedule();
  });
}

ScheduleDialog::~ScheduleDialog()
{
  delete ui;
}

void ScheduleDialog::showEvent(QShowEvent *) {
  if (!m_tasks.empty()) {
    std::sort(m_tasks.begin(), m_tasks.end(), [](ScheduledTask a, ScheduledTask b) {
      return a.id.toUInt() > b.id.toUInt();
    });
    ui->taskList->setCurrentIndex(0);
    setFields(m_tasks.front());
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
    ScheduledTask task{
      .id        = task_arguments.at(i + 0),
      .app       = task_arguments.at(i + 1),
      .time      = QDateTime::fromSecsSinceEpoch(task_arguments.at(i + 2).toLongLong()),
      .flags     = task_arguments.at(i + 3),
      .completed = task_arguments.at(i + 4),
      .recurring = task_arguments.at(i + 5),
      .notify    = task_arguments.at(i + 6),
      .runtime   = task_arguments.at(i + 7),
      .files     = QVector<QString>{task_arguments.at(i + 8)} // parse
    };
    ui->taskList->addItem(QString{task.id + ": " + task.time.toString()});
    m_tasks.push_back(task);
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

ScheduledTask ScheduleDialog::readFields() {
  return ScheduledTask {
      .id        = m_tasks.at(ui->taskList->currentIndex()).id,
      .app       = ui->appText->text(),
      .time      = QDateTime::fromString(ui->timeText->text()),
      .flags     = ui->flagsText->text(),
      .completed = ui->completedText->text(),
      .recurring = ui->recurringText->text(),
      .notify    = ui->notifyText->text(),
      .runtime   = ui->runtimeText->text(),
      .files     = {ui->filesText->text()}  // files need to be an actual array
  };
}
