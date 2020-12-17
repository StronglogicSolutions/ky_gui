#include "include/ui/scheduledialog.hpp"
#include "ui_scheduledialog.h"
#include <QDebug>

/**
*   ┌──────────────────────────────────────────────────┐
*   │░░░░░░░░░░░░░░░ HELPERS ░░░░░░░░░░░░░░░░░│
*   └──────────────────────────────────────────────────┘
*/
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

const QString completed_num_string(QString s) {
  if (s.compare("Scheduled") == 0)
    return "0";
  else
      if (s.compare("Success") == 0)
    return "1";
  else
      if (s.compare("Failed") == 0)
    return "2";
  else
      if (s.compare("Retry Failed") == 0)
    return "3";
  return s;
}

uint8_t completed_integer(QString s) {
  if (s.compare("Scheduled") == 0)
    return 0;
  else
      if (s.compare("Success") == 0)
    return 1;
  else
      if (s.compare("Failed") == 0)
    return 2;
  else
      if (s.compare("Retry Failed") == 0)
    return 3;
  return 10;
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

const QString recurring_num_string(QString s) {
  if (s.compare("No") == 0)
    return "0";
  else
      if (s.compare("Hourly") == 0)
    return "1";
  else
      if (s.compare("Daily") == 0)
    return "2";
  else
      if (s.compare("Weekly") == 0)
    return "3";
  else
      if (s.compare("Monthly") == 0)
    return "4";
  else
      if (s.compare("Yearly") == 0)
    return "5";
  return s;
}

uint8_t recurring_integer(QString s) {
  if (s.compare("No") == 0)
    return 0;
  else
      if (s.compare("Hourly") == 0)
    return 1;
  else
      if (s.compare("Daily") == 0)
    return 2;
  else
      if (s.compare("Weekly") == 0)
    return 3;
  else
      if (s.compare("Monthly") == 0)
    return 4;
  else
      if (s.compare("Yearly") == 0)
    return 5;
  return 10;
}

/**
*   ┌─────────────────────────────────────────────────────────────────────────┐
*   │░░░░░░░░░░░░░░░░ ScheduleDialog Definitions ░░░░░░░░░░░░░░░░░│
*   └─────────────────────────────────────────────────────────────────────────┘
*/

/**
 * constructor
 *
 * @brief ScheduleDialog::ScheduleDialog
 * @param parent
 */
ScheduleDialog::ScheduleDialog(QWidget *parent)
: QDialog(parent),
  ui(new Ui::ScheduleDialog)
{
  ui->setupUi(this);
  ui->dateTime->setDateTime(QDateTime::currentDateTime());

  ui->paramTable->setHorizontalHeaderLabels(QStringList{"Param Map", "Value"});
  ui->paramTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);


/**
*   ┌──────────────────────────────────────────────────┐
*   │░░░░░░░░░░░░░░░░ SLOTS ░░░░░░░░░░░░░░░░░│
*   └──────────────────────────────────────────────────┘
*/
  /** Fetch **/
  QObject::connect(ui->fetchSchedule, &QPushButton::clicked, this, [this]() {
    updateSchedule();
  });
  /** Save **/
  QObject::connect(ui->saveTask, &QPushButton::clicked, this, [this]() {
    scheduleRequest(RequestType::UPDATE_SCHEDULE, readFields());
  });
  /** Delete **/
  QObject::connect(ui->deleteTask, &QPushButton::clicked, this, [this]() {
    this->close();
  });
  /** Close **/
  QObject::connect(ui->close, &QPushButton::clicked, this, [this]() {
    this->close();
  });
  /** Select **/
  QObject::connect(
    ui->taskList,
    static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
    this,
    [this](int index) {
        if ((index > -1) && !m_tasks.empty()) {
        scheduleRequest(RequestType::FETCH_SCHEDULE_TOKENS, readFields());
        setFields(m_tasks.at(index));
      }
    }
  );
  /** Date select **/
  QObject::connect(ui->dateTime, &QDateTimeEdit::dateTimeChanged, this, [this]() {
    ui->timeText->setText(ui->dateTime->dateTime().toString());
  });
}

/**
 * destructor
 *
 * @brief ScheduleDialog::~ScheduleDialog
 */
ScheduleDialog::~ScheduleDialog()
{
  delete ui;
}

/**
 * @brief ScheduleDialog::showEvent
 */
void ScheduleDialog::showEvent(QShowEvent *) {
  if (!m_tasks.empty()) {
    std::sort(m_tasks.begin(), m_tasks.end(), [](ScheduledTask a, ScheduledTask b) {
      return a.id.toUInt() > b.id.toUInt();
    });

    setFields(m_tasks.front());
    ui->taskList->clear();

    for (const auto& task : m_tasks)
      ui->taskList->addItem(QString{task.id + ": " + task.time.toString()});

    ui->taskList->setCurrentIndex(0);
  }
}

/**
 * @brief ScheduleDialog::setFields
 * @param task
 */
void ScheduleDialog::setFields(ScheduledTask task) {
  ui->appText->setText(task.app);
  ui->timeText->setText(task.time.toString());
  ui->flagsText->setText(task.flags);
  ui->completed->setCurrentIndex(task.completed.toUInt());
  ui->notifyCheck->setChecked((task.notify.compare("0") == 0) ? false : true);
  ui->recurring->setCurrentIndex(task.recurring.toUInt());
  ui->runtimeText->setText(task.runtime);
  ui->filesText->setText(task.files.at(0));
}

/**
 * @brief ScheduleDialog::insert_tasks
 * @param task_arguments
 */
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
//    ui->taskList->addItem(QString{task.id + ": " + task.time.toString()});
    m_tasks.push_back(task);
  }
}

/**
 * @brief ScheduleDialog::clear
 */
void ScheduleDialog::clear() {
  m_tasks          .clear();
  ui->taskList     ->clear();
  ui->appText      ->clear();
  ui->timeText     ->clear();
  ui->completed->setCurrentText("Unknown");
  ui->flagsText    ->clear();
  ui->notifyCheck->setChecked(false);
  ui->recurring->setCurrentText("Unknown");
  ui->runtimeText  ->clear();
  ui->filesText    ->clear();
}

/**
 * @brief ScheduleDialog::readFields
 * @return
 */
ScheduledTask ScheduleDialog::readFields() {
  return ScheduledTask {
      .id        = m_tasks.at(ui->taskList->currentIndex()).id,
      .app       = ui->appText->text(),
      .time      = QDateTime::fromString(ui->timeText->text()),
      .flags     = ui->flagsText->text(),
      .completed = completed_num_string(ui->completed->currentText()),
      .recurring = recurring_num_string(ui->recurring->currentText()),
      .notify    = ui->notifyCheck->isChecked() ? "1" : "0",
      .runtime   = ui->runtimeText->text(),
      .files     = {ui->filesText->text()}  // files need to be an actual array
  };
}

/**
 * @brief ScheduleDialog::receive_response
 * @param v
 */
void ScheduleDialog::receive_response(RequestType type, QVector<QString> v) {
  if (type == RequestType::UPDATE) {
    QString display_s{};
    for (const auto& e : v) display_s += e + "\n";
    UI::infoMessageBox(display_s, "Schedule Response");
  }
  else
  if (type == RequestType::FETCH_SCHEDULE_TOKENS) {
    QString display_s{};
    for (const auto& e : v) display_s += e + "\n";
    UI::infoMessageBox(display_s, "Schedule Response");

    QList<QString> keys = m_tasks.at(ui->taskList->currentIndex()).flags.split(' ');
//    m_model->clear();
    ui->paramTable->setRowCount(0);
    for (int i = 0; i < keys.size(); i++) {
      auto row = ui->paramTable->rowCount(); // insert row
      auto key = keys.at(i);
      auto value = v.at(i + 1);
      ui->paramTable->insertRow(row);
      QTableWidgetItem *item = new QTableWidgetItem(key);
      QTableWidgetItem *item2 = new QTableWidgetItem(value);
      ui->paramTable->setItem(row, 0, item);
      ui->paramTable->setItem(row, 1, item2);
    }
  }
}
