#include <include/mainwindow.h>

#include <QDateTime>
#include <QDebug>
#include <QLayout>
#include <QString>
#include <QTextEdit>
#include <QTextStream>
#include <headers/util.hpp>
#include <vector>

#include "ui_mainwindow.h"

void infoMessageBox(QString text, QString title = "KYGUI") {
  QMessageBox box;
  box.setWindowTitle(title);
  box.setText(text);
  box.setButtonText(0, "Close");
  box.exec();
}

bool isSameEvent(QString event1, QString event2) {
  auto event_size =
      event1.size() > event2.size() ? event2.size() : event1.size();
  auto similarity_standard = event_size * 0.67;
  auto similarity_index = 0;
  for (auto i = 0; i < event_size; i++) {
    if (event1[i] == event2[i]) {
      similarity_index++;
    }
  }
  return similarity_index > similarity_standard;
}

int getLikeEventNum(QString event, QList<QString> events) {
  auto i = events.size() - 1;
  auto hits = 0;
  bool is_same_event = false;
  auto incoming_event = event.remove(0, 11);
  do {
    auto existing_event = events[i];
    if (is_same_event =
            isSameEvent(incoming_event, existing_event.remove(0, 11))) {
      i--;
      hits++;
    }
  } while (is_same_event && i > -1);
  return hits;
}

QString getTime() { return QDateTime::currentDateTime().toString("hh:mm:ss"); }

/**
 *\mainpage The KYGUI application interface begins with the MainWindow
 * @brief MainWindow::MainWindow
 * @param argc
 * @param argv
 * @param parent
 */
MainWindow::MainWindow(int argc, char** argv, QWidget* parent)
    : QMainWindow(parent),
      cli_argc(argc),
      cli_argv(argv),
      ui(new Ui::MainWindow),
      arg_ui(new ArgDialog),
      q_client(nullptr) {
  m_process_model = new QStandardItemModel(this);
  m_event_model = new QStandardItemModel(this);
  q_client = new Client(this, cli_argc, cli_argv);
  ui->setupUi(this);
  this->setWindowTitle("KYGUI");
  setConnectScreen();
  connect(ui->connect, &QPushButton::clicked, this, &MainWindow::connectClient);
  ui->processList->setModel(m_process_model);
  ui->eventList->setModel(m_event_model);
}

/**
 * @brief MainWindow::~MainWindow
 */
MainWindow::~MainWindow() {
  delete q_client;
  delete ui;
}

void MainWindow::setConnectScreen(bool visible) {
  if (visible) {
    ui->startScreen->setMaximumSize(1366, 825);
    ui->startScreen->setMinimumSize(1366, 825);
    ui->connect->setMaximumSize(1366, 725);
    ui->connect->setMinimumSize(1366, 725);
    ui->kyConfig->setMaximumSize(1366, 75);
    ui->kyConfig->setMinimumSize(1366, 75);
    QFile file(QCoreApplication::applicationDirPath() + "/config/config.json");
    file.open(QIODevice::ReadOnly | QFile::ReadOnly);
    QString config_json = QString::fromUtf8(file.readAll());
    ui->kyConfig->setText(config_json);
    qDebug() << "Set config json: \n" << ui->kyConfig->toPlainText();
    file.close();
  } else {
    ui->connect->hide();
    ui->kyConfig->hide();
    ui->startScreen->setVisible(false);
  }
}

/**
 * @brief MainWindow::buttonClicked
 */
void MainWindow::connectClient() {
  m_config = getConfigObject(ui->kyConfig->toPlainText());
  QString file_path = m_config.at("fileDirectory");
  if (file_path != NULL) {
    arg_ui->setFilePath(file_path);
  }
  setConnectScreen(false);
  qDebug() << "Connecting to KServer";
  QObject::connect(q_client, &Client::messageReceived, this,
                   &MainWindow::updateMessages);

  QProgressBar* progressBar = ui->progressBar;
  q_client->start();

  for (int i = 1; i < 101; i++) {
    progressBar->setValue(i);
  }

  QPushButton* send_message_button =
      this->findChild<QPushButton*>("sendMessage");
  // Handle mouse
  QObject::connect(send_message_button, &QPushButton::clicked, this, [this]() {
    q_client->sendMessage(escapeMessage(ui->inputText->toPlainText()));
    ui->inputText->clear();
  });

  QObject::connect(
      ui->appList,
      static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
      this, [this]() {
        QString app_name = ui->appList->currentText();
        // TODO: I know, it's awful. Fix this
        q_client->setSelectedApp(std::vector<QString>{{app_name}});
      });
  QPushButton* disconnect_button = this->findChild<QPushButton*>("disconnect");
  QObject::connect(disconnect_button, &QPushButton::clicked, this,
                   [this, progressBar]() {
                     q_client->closeConnection();
                     QApplication::exit(9);
                   });

  QObject::connect(ui->execute, &QPushButton::clicked, this,
                   [this]() { q_client->execute(); });

  QObject::connect(ui->addArgs, &QPushButton::clicked, this, [this]() {
    if (ui->appList->count() == 0) {
      QMessageBox::warning(this, tr("Args"),
                           tr("Please connect to the KServer and retrieve a "
                              "list of available processes."));
    } else {
      arg_ui->show();
    }
  });

  QObject::connect(
      arg_ui, &ArgDialog::uploadFiles, this,
      [this](QVector<KFileData> files) { q_client->sendFiles(files); });

  QObject::connect(
      arg_ui, &ArgDialog::taskRequestReady, this,
      [this](Task task, bool file_pending) {
        auto mask = q_client->getSelectedApp();
        if (mask > -1) {
          if (q_client->getAppName(mask) == "Instagram") {
            qDebug() << "Scheduling a task";
            task.args.push_back(std::to_string(mask));
            q_client->scheduleTask(task.args, file_pending);
          }
        }
      });

  QObject::connect(ui->tasks, &QPushButton::clicked, this, [this]() {
    // TODO: Change this to a complete implementation
    q_client->sendMessage("scheduler");
  });

  QObject::connect(ui->viewConsole, &QPushButton::clicked, this,
                   [this]() { console_ui.show(); });

  QObject::connect(
      ui->processList, &QListView::clicked, this,
      [this](const QModelIndex& index) {
        auto process = m_processes.at(index.row());
        QString process_info_text =
            m_processes.at(index.row()).name.toUtf8() + "\n";
        process_info_text += "Execution requested at " +
                             process.start.toUtf8() + "\n" +
                             "Is currently in a state of: " +
                             ProcessNames[process.state - 1].toUtf8();
        if (process.end.size() > 0 || process.id == "Scheduled task") {
          process_info_text += "\n\nResult: \n" + process.result;
        }
        infoMessageBox(process_info_text, "Process");
      });

  QObject::connect(ui->eventList, &QListView::clicked, this,
                   [this](const QModelIndex& index) {
                     auto event = m_events.at(index.row());
                     infoMessageBox(event, "Event");
                   });

  QTimer* timer = new QTimer(this);
  connect(timer, &QTimer::timeout, q_client, &Client::ping);
  timer->start(10000);
}

void MainWindow::handleKey() {
  q_client->sendMessage(ui->inputText->toPlainText());
  ui->inputText->clear();
}

QString MainWindow::parseMessage(const QString& message, StringVec v) {
  QString simplified_message{};
  if (isMessage(message.toUtf8())) {
    simplified_message += "Message: " + getMessage(message.toUtf8());
  } else if (isEvent(message.toUtf8())) {
    simplified_message += "Event: " + getEvent(message.toUtf8());
  } else if (isOperation(message.toUtf8())) {
    simplified_message += "Operation: ";
    simplified_message += getOperation(message.toUtf8()).c_str();
  }
  return simplified_message;
}

QStandardItem* createProcessListItem(Process process) {
  return new QStandardItem(
      QString(
          "%0 requested for execution. ID: %1\nStatus: %2\nTime: %3   Done: %4")
          .arg(process.name)
          .arg(process.id)
          .arg(ProcessNames[process.state - 1])
          .arg(process.start)
          .arg(process.end));
}

QStandardItem* createEventListItem(QString event) {
  return new QStandardItem(event);
}

/**
 * @brief MainWindow::updateMessages
 * @param s
 */
void MainWindow::updateMessages(int t, const QString& message, StringVec v) {
  QString timestamp_prefix =
      QDateTime::currentDateTime().toString("hh:mm:ss") + " - ";
  if (t == MESSAGE_UPDATE_TYPE) {
    qDebug() << "Updating message area";
    auto simple_message = timestamp_prefix + parseMessage(message, v);
    ui->messages->append(simple_message);
    console_ui.updateText(message);
  } else if (t == COMMANDS_UPDATE_TYPE) {
    qDebug() << "Updating commands";
    QComboBox* app_list = ui->appList;
    app_list->clear();
    int app_index = 0;
    QString default_app = configValue("defaultApp", m_config);
    for (const auto& s : v) {
      app_list->addItem(s);
      if (s.toLower() == default_app.toLower()) {
        q_client->setSelectedApp(std::vector<QString>{default_app});
        ui->appList->setCurrentIndex(app_index);
      }
      app_index++;
    }
    if (message == "New Session") {
      ui->led->setState(true);
      arg_ui->setConfig(configValue("instagram", m_config));
      if (configBoolValue("schedulerMode", std::ref(m_config))) {
        arg_ui->show();
      }
    }
  } else if (t == PROCESS_REQUEST_TYPE) {
    qDebug() << "Updating process list";
    m_processes.push_back(Process{.name = v.at(1),
                                  .state = ProcessState::PENDING,
                                  .start = getTime(),
                                  .id = v.at(2)});
    int row = 0;
    for (const auto& process : m_processes) {
      m_process_model->setItem(row, createProcessListItem(process));
      row++;
    }
  } else if (t == EVENT_UPDATE_TYPE) {
    QString event_message{timestamp_prefix};
    if (!v.empty()) {
      // TODO: extract process result handling from here. This should handle any
      // event
      if (v.size() == 1) {
        event_message += message + "\n" + v.at(0);
      } else {
        event_message += message;
        if (message == "Process Result") {
          event_message += "\n";
          auto app_name =
              q_client->getAppName(std::stoi(v.at(0).toUtf8().constData()));
          auto process_it = std::find_if(
              m_processes.begin(), m_processes.end(),
              [v](const Process& process) { return process.id == v.at(1); });
          if (process_it != m_processes.end()) {
            updateProcessResult(v.at(1), v.at(2));
          } else {  // new process, from scheduled task
            Process new_process{.name = app_name,
                                .state = ProcessState::SUCCEEDED,
                                .start = getTime(),
                                .id = "Scheduled task"};
            if (v.count() > 2 && !v.at(2).isEmpty()) {
              new_process.result = v.at(2);
              new_process.end = new_process.start;
            }
            m_processes.push_back(new_process);
            m_process_model->setItem(m_process_model->rowCount(),
                                     createProcessListItem(new_process));
          }
          event_message += app_name;
          event_message += ": ";
          event_message += v.at(2);
        } else if (QString::compare(message, "Message Received") == 0) {
          event_message += "\n" + v.at(1) + ": " + v.at(2);
        }
      }
    } else {
      event_message += message;
    }
    if (m_events.size() > 1) {
      auto last_event = m_events[m_events.size() - 1];
      if (isSameEvent(message, last_event.remove(0, 11))) {
        m_consecutive_events++;
        auto count = getLikeEventNum(event_message, m_events);
        QString clean_event_message =
            event_message + " (" + QString::number(count) + ")";
        m_events.push_back(event_message);
        m_event_model->setItem(m_event_model->rowCount() - 1,
                               createEventListItem(clean_event_message));
        return;
      }
      m_consecutive_events = 0;
    }
    m_events.push_back(event_message);
    m_event_model->setItem(m_event_model->rowCount(),
                           createEventListItem(event_message));
  } else {
    qDebug() << "Unknown update type. Cannot update UI";
  }
}

void MainWindow::updateProcessResult(
    QString id, QString result) {  // We need to start matching processes with a
                                   // unique identifier
  for (int i = m_processes.size() - 1; i >= 0; i--) {
    if (m_processes.at(i).id == id) {
      m_processes.at(i).end = getTime();
      m_processes.at(i).state = ProcessState::SUCCEEDED;
      m_processes.at(i).result = result;
      m_process_model->setItem(i, 0, createProcessListItem(m_processes.at(i)));
      return;
    }
  }
  // If we didn't return, it's a new process:
}

void MainWindow::keyPressEvent(QKeyEvent* e) {
  qDebug() << "Key press: " << e->key();
  if (e->key() == Qt::Key_0) {
    qDebug() << "Ok";
  }
}
