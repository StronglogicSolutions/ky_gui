﻿#include <include/ui/mainwindow.h>
#include <QDebug>
#include <QLayout>
#include <QScrollBar>
#include <QString>
#include <QTextEdit>
#include <QTextStream>
#include <headers/util.hpp>
#include <vector>

#include "ui_mainwindow.h"

/**
 * Helper functions
 */
namespace {
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
    if ((is_same_event =
             isSameEvent(incoming_event, existing_event.remove(0, 11)))) {
      i--;
      hits++;
    }
  } while (is_same_event && i > -1);
  return hits;
}

QString timestampPrefix() {
  return TimeUtils::getTime() + " - ";
}

/**
 * @brief createProcessListItem
 * @param process
 * @return
 */
QStandardItem* createProcessListItem(Process process) {
  return new QStandardItem(
      QString("%0 requested for execution. ID: %1\nStatus: %2\nTime: %3   "
              "Done: %4\n Errors: %5")
          .arg(process.name)
          .arg(process.id)
          .arg(ProcessNames[process.state - 1])
          .arg(process.start)
          .arg(process.end)
          .arg(process.error));
}

/**
 * @brief createEventListItem
 * @param event
 * @return
 */
QStandardItem* createEventListItem(QString event) {
  return new QStandardItem(event);
}
}  // namespace

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
  m_event_model = new QStandardItemModel(this);
  m_process_model = new QStandardItemModel(this);
  q_client = new Client(this, cli_argc, cli_argv);
  message_parser.init(this);
  ui->setupUi(this);
  this->setWindowTitle("KYGUI");
  setStyleSheet(
      "QListView { font: 87 11pt \"Noto Sans\"; background-color: #2f535f;"
      "alternate-background-color: #616161; color: rgb(131, 148, 150); "
      "font-weight: 700; background-color: rgb(29, 51, 59);"
      "color: rgb(223, 252, 255);}");
  setConnectScreen();
  connect(ui->connect, &QPushButton::clicked, this, &MainWindow::connectClient);
  ui->eventList->setModel(m_event_model);
  ui->processList->setModel(m_process_model);
}

/**
 * @brief MainWindow::~MainWindow
 */
MainWindow::~MainWindow() {
  delete q_client;
  delete ui;
  delete arg_ui;
  delete m_event_model;
}

/**
 * @brief MainWindow::setConnectScreen
 * @param visible
 */
void MainWindow::setConnectScreen(bool visible) {
  if (visible) {
    ui->startScreen->activateWindow();
    ui->startScreen->raise();
    ui->kyConfig->activateWindow();
    ui->kyConfig->raise();
    ui->startScreen->setMaximumSize(1080, 675);
    ui->startScreen->setMinimumSize(1080, 675);
    ui->connect->setMaximumSize(1080, 500);
    ui->connect->setMinimumSize(1080, 500);
    ui->kyConfig->setMaximumSize(1080, 175);
    ui->kyConfig->setMinimumSize(1080, 175);
    ui->logo->raise();
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
 * @brief MainWindow::connectClient
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
  QObject::connect(disconnect_button, &QPushButton::clicked, this, [this]() {
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

  QObject::connect(ui->openMessages, &QPushButton::clicked, this, [this]() {
    message_ui.show();
  });

  QObject::connect(
      arg_ui, &ArgDialog::taskRequestReady, this,
      [this](Task* task) {
        auto mask = q_client->getSelectedApp();
        if (mask > -1) {
            q_client->scheduleTask(task);
        }
      });

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
        UI::infoMessageBox(process_info_text, "Process");
      });

  QObject::connect(ui->eventList, &QListView::clicked, this,
                   [this](const QModelIndex& index) {
                     UI::infoMessageBox(m_event_model->item(index.row(), index.column())->text(), "Event");
                   });

  QObject::connect(m_event_model, &QAbstractItemModel::rowsAboutToBeInserted,
                   this, [this]() {
                     QScrollBar* bar = ui->eventList->verticalScrollBar();
                     if (bar->value() == bar->maximum()) {
                       m_view_states.eventViewBottom = true;
                     }
                   });

  QObject::connect(m_event_model, &QAbstractItemModel::rowsInserted, this,
                   [this]() {
                     if (m_view_states.eventViewBottom) {
                       ui->eventList->scrollToBottom();
                     }
                   });

  QTimer* timer = new QTimer(this);
  connect(timer, &QTimer::timeout, q_client, &Client::ping);
  timer->start(10000);
}

/**
 * @brief MainWindow::updateMessages
 * @param s
 */
void MainWindow::updateMessages(int t, const QString& message, StringVec v) {
  QString timestamp_prefix = timestampPrefix();
  if (t == MESSAGE_UPDATE_TYPE) {  // Normal message
    qDebug() << "Updating message area";
    message_parser.handleMessage(message, v);
    message_ui.append(message);
  } else if (t == COMMANDS_UPDATE_TYPE) {  // Received app list from server
    qDebug() << "Updating commands";
    QString default_app = configValue("defaultApp", m_config);
    message_parser.handleCommands(v, default_app);
    if (message == "New Session") {  // Session has started
      ui->led->setState(true);
      arg_ui->setConfig(configValue("instagram", m_config));
      if (configBoolValue("schedulerMode", std::ref(m_config))) {
        arg_ui->show();
      }
    }
  } else if (t == PROCESS_REQUEST_TYPE) {  // Sent execution request to server
    qDebug() << "Updating process list";
    m_processes.push_back(Process{.name = v.at(1),
                                  .state = ProcessState::PENDING,
                                  .start = TimeUtils::getTime(),
                                  .id = v.at(2)});
    int row = 0;
    for (const auto& process : m_processes) {
      m_process_model->setItem(row, createProcessListItem(process));
      row++;
    }
  } else if (t == EVENT_UPDATE_TYPE) {  // Received event from server
    QString event_message = message_parser.handleEventMessage(message, v);
    if (m_events.size() > 1) {  // Group repeating event messages
      auto last_event = m_events[m_events.size() - 1];
      if (isSameEvent(message, last_event.remove(0, 11))) {
        m_consecutive_events++;
        auto count = getLikeEventNum(event_message, m_events);
        QString clean_event_message =
            event_message + " (" + QString::number(count) + ")";
        m_events.push_back(event_message);
        m_event_model->setItem(m_event_model->rowCount() - 1,
                               createEventListItem(clean_event_message));
        return;  // It was not a unique message, we can return
      }
      m_consecutive_events = 0;
    }
    if (isKEvent<QString>(message,
                          Event::TASK_SCHEDULED)) {  // Event was scheduled task
      event_message += ". Details:\n" + parseTaskInfo(v);
      arg_ui->notifyClientSuccess(); // Update ArgDialog accordingly
    }
    m_events.push_back(event_message);
    m_event_model->setItem(m_event_model->rowCount(),
                           createEventListItem(event_message));
  } else {
    qDebug() << "Unknown update type. Cannot update UI";
  }
}

/**
 * @brief MainWindow::parseTaskInfo
 * @param v
 * @return
 */
QString MainWindow::parseTaskInfo(StringVec v) {
  QString task_info{};
  if (q_client == nullptr) {
    qDebug() << "Can't parse when not connected";
    return task_info;
  }
  auto size = v.size();
  if (size < 3) {
    qDebug() << "Not enough arguments to parse task information";
  } else {
    auto error = size == 4;
    task_info += "  UUID - " + v.at(0) + "\n  ID - " + v.at(1) + "\n  APP - " +
                 q_client->getAppName(std::stoi(v.at(2).toUtf8().constData())) +
                 "\n ENV - " + (v.at(3));
    if (error) {
      task_info += "\n !ERROR! - " + v.at(3);
    }
  }
  return task_info;
}

/**
 * MessageParser
 *
 * \note We use the MessageParser class to do the heavy lifting of parsing
 * incoming messages and updating the UI accordingly
 */

/**
 * @brief MainWindow::MessageParser::init
 * @param window
 */
void MainWindow::MessageParser::init(MainWindow* window) {
  this->window = window;
}

/**
 * @brief MainWindow::MessageParser::handleCommands
 * @param commands
 * @param default_command
 */
void MainWindow::MessageParser::handleCommands(StringVec commands,
                                               QString default_command) {
  QComboBox* app_list = window->ui->appList;
  app_list->clear();
  int app_index = 0;
  for (const auto& s : commands) {
    app_list->addItem(s);
    if (s.toLower() == default_command.toLower()) {
      window->ui->appList->setCurrentIndex(app_index);
    }
    app_index++;
  }
}
/**
 * @brief MainWindow::MessageParser::handleMessage
 * @param message
 * @param v
 */
void MainWindow::MessageParser::handleMessage(QString message, StringVec v) {
  window->message_ui.append(timestampPrefix() + parseMessage(message, v), true);
}

/**
 * @brief MainWindow::MessageParser::parseMessage
 * @param message
 * @param v
 * @return
 */
QString MainWindow::MessageParser::parseMessage(const QString& message,
                                                StringVec v) {
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

/**
 * @brief MainWindow::MessageParser::updateProcessResult
 * @param id
 * @param result
 * @param error
 */
void MainWindow::MessageParser::updateProcessResult(
    QString id, QString result,
    bool error = false) {  // We need to start matching processes with a
  // unique identifier
  for (int i = window->m_processes.size() - 1; i >= 0; i--) {
    if (window->m_processes.at(i).id == id) {
      window->m_processes.at(i).end = TimeUtils::getTime();
      window->m_processes.at(i).state =
          !error ? ProcessState::SUCCEEDED : ProcessState::FAILED;
      window->m_processes.at(i).result = result;
      window->m_process_model->setItem(
          i, 0, createProcessListItem(window->m_processes.at(i)));
      return;
    }
  }
  // If we didn't return, it's a new process:
}

/**
 * @brief MainWindow::MessageParser::handleEventMessage
 * @param message
 * @param v
 * @return
 */
QString MainWindow::MessageParser::handleEventMessage(QString message,
                                                      StringVec v) {
  QString event_message = timestampPrefix();
  if (!v.empty()) {
    if (v.size() == 1) {
      event_message += message + "\n" + v.at(0);
    } else {
      event_message += message;
      if (message == "Process Result") {
        auto error = v.size() > 3 ? true : false;
        event_message += "\n";
        auto app_name = window->q_client->getAppName(
            std::stoi(v.at(0).toUtf8().constData()));
        auto process_it = std::find_if(
            window->m_processes.begin(), window->m_processes.end(),
            [v](const Process& process) { return process.id == v.at(1); });
        if (process_it != window->m_processes.end()) {
          updateProcessResult(v.at(1), v.at(2), error);
        } else {  // new process, from scheduled task
          Process new_process{
              .name = app_name,
              .state = !error ? ProcessState::SUCCEEDED : ProcessState::FAILED,
              .start = TimeUtils::getTime(),
              .id = "Scheduled task",
              .error = error ? v.at(3) : "No errors reported"};
          if (v.count() > 2 && !v.at(2).isEmpty()) {
            new_process.result = v.at(2);
            new_process.end = new_process.start;
          }
          window->m_processes.push_back(new_process);
          window->m_process_model->setItem(window->m_process_model->rowCount(),
                                           createProcessListItem(new_process));
        }
        event_message += app_name;
        event_message += ": ";
        event_message += v.at(2);
        if (error) {
          event_message += "\n Error: " + v.at(3);
        }
      } else if (QString::compare(message, "Message Received") == 0) {
        event_message += "\n" + v.at(1) + ": " + v.at(2);
      }
    }
  } else {
    event_message += message;
  }
  return event_message;
}
