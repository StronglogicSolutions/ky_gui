#include "include/ui/scheduledialog.hpp"
#include "ui_scheduledialog.h"

static bool input_mode{false};

ScheduleDialog::ScheduleDialog(QWidget *parent) :
                                        QDialog(parent),
                                        ui(new Ui::ScheduleDialog)
{
  ui->setupUi(this);
  QObject::connect(ui->close, &QPushButton::clicked, this, [this]() {
    this->close();
  });

//  QObject::connect(ui->addApp, &QPushButton::clicked, this, [this]() {
//    toggleInputMode();
//  });

  /*QObject::connect(ui->save, &QPushButton::clicked, this, [this]() {
    emit appRequest(readFields(), constants::RequestType::REGISTER);
  });

  QObject::connect(ui->deleteApp, &QPushButton::clicked, this, [this]() {
    emit appRequest(readFields(), constants::RequestType::DELETE);
  });*/

  QObject::connect(ui->taskList, &QListView::clicked,
    this, [this](const QModelIndex& index) {
//                     setFields(ui->taskList->currentIndex());
    }
  );
}

ScheduleDialog::~ScheduleDialog()
{
  delete ui;
}

void ScheduleDialog::showEvent(QShowEvent *) {
  ui->taskList->reset();
//  for (const auto& app : m_applications) {
//    ui->appList->addItem(app.name);
//  }
}

void ScheduleDialog::setFields(QString id) {
  for (const auto& task : m_tasks) {
    if (task.id.compare(id) == 0) {

    }
  }
}


void ScheduleDialog::insert_tasks(QVector<QString> task_arguments) {
  uint16_t arg_num = task_arguments.size();
  /*
   * QString          id;
     QString          app;
     QDateTime        time;
     QString          flags;
     QString          completed;
     QString          recurring;
     QString          notify;
     QString          runtime;*/
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
  m_tasks.clear();
  ui->taskList->reset();
  ui->dataText->clear();
  ui->maskText->clear();
  ui->nameText->clear();
  ui->pathText->clear();
}

void ScheduleDialog::toggleInputMode() {
  input_mode = !input_mode;
  if (input_mode) {
    ui->taskList->setStyleSheet("QComboBox { color: #33383c }");

    ui->nameText->setText("");
    ui->pathText->setText("");
    ui->dataText->setText("");
    ui->maskText->setText("");
  } else {
//    setFields(ui->appList->currentText());
//    ui->appList->setStyleSheet("QComboBox { color: rgb(131, 148, 150); background-color: #2f535f; }");
//    ui->addApp->setStyleSheet(
//        "QPushButton { "
//        "background-color: rgb(0, 58, 70);"
//        "font: 87 11pt \"Noto Sans\";"
//        "color: rgb(0, 0, 0);"
//        "font-weight: 700;"
//        "padding: 4px;"
//        "border-style: outset;"
//        "border-width: 2px;"
//        "border-radius: 4px;"
//        "border-color: #00000f;"
//        "min-width: 4em;"
//        "padding: 4px;"
//        "}"
//        );
  }
}

KApplication ScheduleDialog::readFields() {
  return KApplication{
    .name = ui->nameText->text(),
    .path = ui->pathText->text(),
    .data = ui->dataText->text(),
    .mask = ui->maskText->text()
  };
}


