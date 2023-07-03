#include "include/ui/scheduledialog.hpp"
#include "ui_scheduledialog.h"
#include <QStandardItem>
#include <QTimer>

static const char    ARG_DELIM  {'\x1f'};
static const QString R_ARGS_FLAG{"R_ARGS"};

/**
*  ┌──────────────────────────────────────────────────┐
*  │░░░░░░░░░░░░░░░ HELPERS ░░░░░░░░░░░░░░░░░│
*  └──────────────────────────────────────────────────┘
*/
static QString completed_string(QString s)
{
  if (s.compare("0") == 0)
    return "Scheduled";
  else
  if (s.compare("1") == 0)
    return "Success  ";
  else
  if (s.compare("2") == 0)
    return "Failed   ";
  else
  if (s.compare("3") == 0)
    return "RetryFail";
  return s;
}

static QString completed_num_string(QString s)
{
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

uint8_t completed_integer(QString s)
{
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

static QString recurring_string(QString s)
{
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

static QString recurring_num_string(QString s)
{
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

static uint8_t recurring_integer(QString s)
{
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
*  ┌─────────────────────────────────────────────────────────────────────────┐
*  │░░░░░░░░░░░░░░░░ ScheduleDialog Definitions ░░░░░░░░░░░░░░░░░│
*  └─────────────────────────────────────────────────────────────────────────┘
*/

/**
 * constructor
 *
 * @brief ScheduleDialog::ScheduleDialog
 * @param parent
 */
ScheduleDialog::ScheduleDialog(QWidget *parent)
: QDialog(parent),
  ui(new Ui::ScheduleDialog),
  m_mask(0x00),
  m_refreshing(false)
{
  ui->setupUi(this);
  ui->dateTime->setDateTime(QDateTime::currentDateTime());
  ui->paramTable->setHorizontalHeaderLabels(QStringList{"Param Map", "Value"});
  ui->paramTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
  ui->taskList->setModel(&m_task_model);
  ui->appList->setModel(&m_task_filter_model);

/**
*  ┌──────────────────────────────────────────────────┐
*  │░░░░░░░░░░░░░░░░ SLOTS ░░░░░░░░░░░░░░░░░│
*  └──────────────────────────────────────────────────┘
*/
  /** Fetch **/
  QObject::connect(ui->fetchSchedule, &QPushButton::clicked, this, [this]()
  {
    UpdateSchedule();
  });
  /** Save **/
  QObject::connect(ui->saveTask, &QPushButton::clicked, this, [this]()
  {
    SchedulerRequest(kiq::Request::RequestType::UPDATE_SCHEDULE, ReadFields());
  });
  /** Delete **/
  QObject::connect(ui->deleteTask, &QPushButton::clicked, this, [this]()
  {
    this->close();
  });
  /** Close **/
  QObject::connect(ui->close, &QPushButton::clicked, this, [this]()
  {
    this->close();
  });
  /** Select **/
  QObject::connect(
    ui->taskList,
    static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
    this,
    [this](int index)
    {
      if ((index > -1) && !m_tasks.empty() && !m_task_indexes.empty())
      {
        SchedulerRequest(kiq::Request::RequestType::FETCH_SCHEDULE_TOKENS, ReadFields());
        SetFields(GetTask(index));
      }
    }
  );
  /** Date select **/
  QObject::connect(ui->dateTime, &QDateTimeEdit::dateTimeChanged, this, [this]()
  {
    ui->timeText->setText(ui->dateTime->dateTime().toString());
  });
  /** Task Filter **/
  QObject::connect(&m_task_filter_model, &QStandardItemModel::itemChanged, this, [this](QStandardItem* item)
  {
               m_refreshing = false;
    const bool checked      = item->checkState() == Qt::CheckState::Checked;
    const auto text         = item->text();
    const auto it           = m_apps.find(text.toStdString());
    if (it != m_apps.end())
    {
      int mask = it->second;
      if (checked)
        m_mask |= mask;
      else
        m_mask &= ~mask;
    }
    RefreshUI();
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
void ScheduleDialog::showEvent(QShowEvent*)
{
  RefreshUI();
}

/**
 * @brief ScheduleDialog::setFields
 * @param task
 */
void ScheduleDialog::SetFields(ScheduledTask task) {
  ui->appText->setText(task.app);
  ui->timeText->setText(task.time.toString());
  ui->flagsText->setText(task.flags);
  ui->completed->setCurrentIndex(task.completed.toUInt());
  ui->notifyCheck->setChecked((task.notify.compare("0") == 0) ? false : true);
  ui->recurring->setCurrentIndex(task.recurring.toUInt());
  ui->runtimeText->setText(task.runtime);
  ui->filesText->setText(task.files.at(0));
  ui->dateTime->setDateTime(task.time);
}

/**
 * @brief ScheduleDialog::insert_tasks
 * @param task_arguments
 */
void ScheduleDialog::InsertTasks(QVector<QString> task_arguments)
{
  size_t           arg_num = task_arguments.size();
  QVector<QString> files;
  for (size_t i = 1; i < arg_num; i += 9)
  {
    ScheduledTask task{
      .id        = task_arguments.at(i + 0),
      .app       = task_arguments.at(i + 1),
      .time      = QDateTime::fromSecsSinceEpoch(task_arguments.at(i + 2).toLongLong()),
      .flags     = task_arguments.at(i + 3),
      .completed = task_arguments.at(i + 4),
      .recurring = task_arguments.at(i + 5),
      .notify    = task_arguments.at(i + 6),
      .runtime   = task_arguments.at(i + 7),
      .files     = QVector<QString>{task_arguments.at(i + 8)}}; // TODO: this can be a bug
    m_tasks.push_back(task);
  }
  std::sort(m_tasks.begin(), m_tasks.end(), [](ScheduledTask a, ScheduledTask b){ return a.id.toUInt() > b.id.toUInt(); });
  m_tasks.erase(std::unique(m_tasks.begin(), m_tasks.end()), m_tasks.end());
}

/**
 * @brief ScheduleDialog::clear
 */
void ScheduleDialog::Clear() {
  m_tasks           .clear();
  m_task_indexes    .clear();
  ui->taskList     ->clear();
  ui->appText      ->clear();
  ui->timeText     ->clear();
  ui->completed->setCurrentText("Unknown");
  ui->flagsText    ->clear();
  ui->notifyCheck->setChecked(false);
  ui->recurring->setCurrentText("Unknown");
  ui->runtimeText  ->clear();
  ui->filesText    ->clear();
  ui->dateTime->setDateTime(QDateTime::currentDateTime());
}

/**
 * @brief ScheduleDialog::readFields
 * @return
 */
ScheduledTask ScheduleDialog::ReadFields() {
  struct EnvData
  {
    QString file;
    QString flags;
  };

  const auto read_env_data = [&]() -> EnvData
  {
    EnvData               env{};
    const QTableWidget&   table        = *(ui->paramTable);
    const auto            row_count    = table.rowCount();
    const auto            r_args       = ui->runtimeText->text();

    for (int i = 0; i < row_count; i++)
    {
      const auto flag  = table.item(i, 0)->text();
      const auto value = table.item(i, 1)->text();

      if (value.isEmpty()) continue;

      const auto index = flag.indexOf('=');
      const auto key   = flag.rightRef(flag.size() - index - 2);
      env.file  += '\n' + key + '=' + '"' + value.trimmed() + '"' + ARG_DELIM;
      env.flags += flag + ' ';
    }

    if (!r_args.isEmpty())
      env.file += '\n' + R_ARGS_FLAG + '=' + '"' + r_args + '"' + ARG_DELIM;

    return env;
  };

  const EnvData env_data = read_env_data();

  return ScheduledTask {
      .id        = GetTask(ui->taskList->currentIndex()).id,
      .app       = ui->appText->text(),
      .time      = QDateTime::fromString(ui->timeText->text()),
      .flags     = env_data.flags.trimmed(),
      .completed = completed_num_string(ui->completed->currentText()),
      .recurring = recurring_num_string(ui->recurring->currentText()),
      .notify    = ui->notifyCheck->isChecked() ? "1" : "0",
      .runtime   = ui->runtimeText->text(),
      .files     = {ui->filesText->text()},  // array
      .envfile   = env_data.file
  };
}

/**
 * @brief ScheduleDialog::receive_response
 * @param v
 */
void ScheduleDialog::ReceiveResponse(kiq::Request::RequestType type, QVector<QString> v)
{
  if (type == kiq::Request::RequestType::UPDATE_SCHEDULE)
  {
    QString display_s{};
    for (const auto& e : v) display_s += e + "\n";
    UI::infoMessageBox(display_s, "Schedule Response");
  }
  else
  if (type == kiq::Request::RequestType::FETCH_SCHEDULE)
    RefreshUI();
  else
  if (type == kiq::Request::RequestType::FETCH_SCHEDULE_TOKENS)
  {
    const auto&    task = GetTask(ui->taskList->currentIndex());
    QList<QString> keys = task.flags.split(' ');
    ui->paramTable->setRowCount(0);
    auto row_count = (keys.size() < v.size()) ? keys.size() : (v.size() - 1);
    for (int i = 0; i < row_count; i++)
    {
      auto key = keys.at(i);
      if (key.isEmpty()) continue;

      auto row = ui->paramTable->rowCount();
      auto value = v.at(i + 1);
      ui->paramTable->insertRow(row);
      QTableWidgetItem *item  = new QTableWidgetItem(key);
      QTableWidgetItem *item2 = new QTableWidgetItem(value);
      ui->paramTable->setItem(row, 0, item);
      ui->paramTable->setItem(row, 1, item2);
    }
  }
}

static QStandardItem* CreateTaskModelItem(const ScheduledTask& task)
{
  QString template_text{"%0: %1 - %2 - %3"};
  QStandardItem* item = new QStandardItem{
    template_text
    .arg(task.id)
    .arg(task.time.toString())
    .arg(task.app)
    .arg(completed_string(task.completed))};
  item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsSelectable | Qt::ItemIsEnabled);
  item->setSelectable(true);
  item->setCheckState(Qt::CheckState::Unchecked);
  return item;
}

static QStandardItem* CreateFilterModelItem(const QString& app)
{
  QStandardItem* item = new QStandardItem{app};
  item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsSelectable | Qt::ItemIsEnabled);
  item->setSelectable(true);
  item->setCheckState(Qt::CheckState::Unchecked);
  return item;
}

void ScheduleDialog::RefreshUI()
{
  m_refreshing = true;
  QTimer::singleShot(1000, Qt::TimerType::VeryCoarseTimer, this, [this]()
  {
    if (m_refreshing)
    {
      uint32_t bitmask;
      if (m_mask)
      {
        bitmask = m_mask;
        m_task_indexes.clear();
      }
      else
      {
        m_task_indexes.clear();
        bitmask = 0xFFFFFFFF;
      }

      if (m_tasks.size())
      {
        SetFields(m_tasks.front());

        ui->taskList->clear();
        int count{};
        for (auto i = 0; i < m_tasks.size(); i++)
        {
          const auto mask = m_apps.at(m_tasks[i].app.toStdString());
          if ((mask & bitmask) == mask)
          {
            m_task_model.setItem(count++, CreateTaskModelItem(m_tasks[i]));
            m_task_indexes.push_back(i);
          }
        }

        ui->taskList->setCurrentIndex(0);
      }
    }
  });
}

void ScheduleDialog::SetApps(const CommandMap& map)
{
  int32_t i{};
  for (const auto& [mask, name] : map)
  {
    m_apps.insert({name, mask});
    m_task_filter_model.setItem(i++, CreateFilterModelItem(name.c_str()));
  }
  ui->appList->setCurrentIndex(0);
}

ScheduledTask  ScheduleDialog::GetTask(int index) const
{
  return (m_tasks.at(m_task_indexes.at(index)));
}
