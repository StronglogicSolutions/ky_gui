#include <include/ui/mainwindow.h>

/**
 * Helper functions
 */

namespace utils {
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

  QString processResultText{
    "%0 requested for execution. "
    "ID: %1\n"
    "Status: %2\n"
    "Time: %3   "
    "Done: %4\n"
  };

  auto error = !process.error.isEmpty();

  if (error) {
    processResultText += "Errors: %5";
    return new QStandardItem{
      processResultText
      .arg(process.name)
      .arg(process.id)
      .arg(ProcessNames[process.state - 1])
      .arg(process.start)
      .arg(process.end)
      .arg(process.error)
    };
  }
  return new QStandardItem{
    processResultText
    .arg(process.name)
    .arg(process.id)
    .arg(ProcessNames[process.state - 1])
    .arg(process.start)
    .arg(process.end)
  };
}

/**
 * @brief createEventListItem
 * @param event
 * @return
 */
QStandardItem* createEventListItem(QString event) {
  return new QStandardItem(event);
}
}  // namespace utils

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
      q_client(nullptr),
      m_client_time_remaining(DEFAULT_TIMEOUT)
{
  m_event_model = new QStandardItemModel(this);
  m_process_model = new QStandardItemModel(this);
  q_client = new Client(this, cli_argc, cli_argv);
  m_controller.init(this);
  ui->setupUi(this);
  this->setWindowTitle("KYGUI");
  setStyleSheet(get_stylesheet());
  setConnectScreen();
  connect(ui->connect, &QPushButton::clicked, this, &MainWindow::connectClient);
  ui->progressBar->setMinimum(0);
  ui->progressBar->setMaximum(DEFAULT_TIMEOUT);
  ui->eventList->setModel(m_event_model);
  ui->processList->setModel(m_process_model);
  ui->serverIp->setText(argv[1]);
  ui->serverPort->setText(argv[2]);
}

/**
 * @brief MainWindow::~MainWindow
 */
MainWindow::~MainWindow() {
  q_client->closeConnection();
  usleep(100000);
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
    ui->serverIp->activateWindow();
    ui->serverIp->raise();
    ui->serverPort->activateWindow();
    ui->serverPort->raise();
    ui->configLabel->activateWindow();
    ui->configLabel->raise();
    ui->ipLabel->activateWindow();
    ui->ipLabel->raise();
    ui->portLabel->activateWindow();
    ui->portLabel->raise();
    ui->startScreen->setMaximumSize(1080, 675);
    ui->startScreen->setMinimumSize(1080, 675);
    ui->connect->setMaximumSize(1080, 500);
    ui->connect->setMinimumSize(1080, 500);
    ui->kyConfig->setMaximumSize(1080, 175);
    ui->kyConfig->setMinimumSize(1080, 175);
    ui->serverIp->setMaximumSize(960, 30);
    ui->serverIp->setMinimumSize(960, 30);
    ui->ipLabel->setMaximumSize(120, 30);
    ui->ipLabel->setMinimumSize(120, 30);
    ui->serverPort->setMaximumSize(960, 30);
    ui->serverPort->setMinimumSize(960, 30);
    ui->portLabel->setMaximumSize(120, 30);
    ui->portLabel->setMinimumSize(120, 30);
    ui->configLabel->setMinimumSize(1080, 30);
    ui->configLabel->setMaximumSize(1080, 30);

    QFile file(QCoreApplication::applicationDirPath() + "/config/config.json");
    file.open(QIODevice::ReadOnly | QFile::ReadOnly);

    QString config_json = QString::fromUtf8(file.readAll());
    ui->kyConfig->setText(config_json);

    qDebug() << "Set config json: \n" << ui->kyConfig->toPlainText();

    file.close();
    ui->outerLayer->setVisible(false);
  } else {
    ui->connect->hide();
    ui->kyConfig->hide();
    ui->ipLabel->hide();
    ui->configLabel->hide();
    ui->serverIp->hide();
    ui->portLabel->hide();
    ui->serverPort->hide();
    ui->startScreen->setVisible(false);
    ui->outerLayer->setVisible(true);
  }
}

/**
 * @brief MainWindow::connectClient
 */
void MainWindow::connectClient() {
  using namespace constants;
  m_config = loadJsonConfig(ui->kyConfig->toPlainText());
  QString file_path = configValue("fileDirectory", m_config);

  if (!file_path.isEmpty())
  {
    arg_ui    ->setFilePath(file_path);
    doc_window .setFilePath(file_path);
  }

  setConnectScreen(false);

  qDebug() << "Connecting to KServer";

  QObject::connect(q_client, &Client::messageReceived, this,
                   &MainWindow::onMessageReceived);

  auto server_ip   = ui->serverIp->toPlainText();
  auto server_port = ui->serverPort->toPlainText();
  setWindowTitle(windowTitle() + " kiq://" + server_ip + ":" + server_port);
  q_client->start(server_ip, server_port);

  QObject::connect(ui->actionDefault, &QAction::triggered, this, [this]()
  {
    ui->centralWidget->setStyleSheet(KYGUI_DEFAULT_THEME);
    ui->processList->setStyleSheet(KYGUI_DEFAULT_LIST_THEME);
    ui->eventList->setStyleSheet(KYGUI_DEFAULT_LIST_THEME);
  });

  QObject::connect(ui->actionBlack, &QAction::triggered, this, [this]()
  {
    ui->centralWidget->setStyleSheet(KYGUI_BLACK_THEME);
    ui->processList->setStyleSheet(KYGUI_BLACK_LIST_THEME);
    ui->eventList->setStyleSheet(KYGUI_BLACK_LIST_THEME);
  });

  QObject::connect(ui->actionBlue, &QAction::triggered, this, [this]()
  {
    ui->centralWidget->setStyleSheet(KYGUI_BLUE_THEME);
    ui->processList->setStyleSheet(KYGUI_BLUE_LIST_THEME);
    ui->eventList->setStyleSheet(KYGUI_BLUE_LIST_THEME);
  });


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
        q_client->setSelectedApp(std::vector<QString>{{app_name}});
        arg_ui->setAppName(app_name);
        auto json_object = configObject(app_name, m_config, true);
        arg_ui->setConfig(json_object);
      });
  QPushButton* disconnect_button = this->findChild<QPushButton*>("disconnect");
  QObject::connect(disconnect_button, &QPushButton::clicked, this, [this]() {
    q_client->closeConnection();
    QApplication::exit(CLIENT_EXIT);
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

  QObject::connect(arg_ui, &ArgDialog::taskRequestReady, this,
    [this](Task* task) {
      auto mask = q_client->getSelectedApp();
      if (mask > -1) {
          q_client->scheduleTask(task);
      }
    }
  );

  QObject::connect(&app_ui, &AppDialog::appRequest, this,
    [this](KApplication application, RequestType type) {
      if (type == REGISTER && q_client->hasApp(application)) {
        QMessageBox::warning(this, tr("Application request"),
                            tr("An application with that name already exists"));
        return;
      }
      q_client->request(type, application);
    }
  );

  QObject::connect(&schedule_ui, &ScheduleDialog::scheduleRequest, this,
    [this](RequestType type, ScheduledTask task) {
      q_client->request(type, task);
  });

  QObject::connect(&schedule_ui, &ScheduleDialog::updateSchedule, this,
    [this]() {
      q_client->request(RequestType::FETCH_SCHEDULE);
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
        utils::infoMessageBox(process_info_text, "Process");
      });

  QObject::connect(ui->eventList, &QListView::clicked, this,
    [this](const QModelIndex& index) {
      utils::infoMessageBox(
        m_event_model->item(index.row(), index.column())->text(), "Event"
      );
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

  QObject::connect(ui->editApps, &QPushButton::clicked, this, [this]() {
    app_ui.show();
  });

  QObject::connect(ui->tasks, &QPushButton::clicked, this, [this]() {
    schedule_ui.show();
  });

  QObject::connect(ui->ipc, &QPushButton::clicked, this, [this]() {
    q_client->sendIPCMessage(ui->ipcList->currentText(), ui->inputText->toPlainText(), defaultConfigUser(m_config));
    ui->inputText->clear();
  });

  QObject::connect(ui->makeDoc, &QPushButton::clicked, this,
    [this]()
    {
      q_client->request(RequestType::FETCH_TASK_FLAGS);
      doc_window.show();
    }
  );

  QTimer* ping_timer = new QTimer(this);

  QObject::connect(&m_progress_timer, &QTimer::timeout, q_client, [this]() -> void
  {
    if (--m_client_time_remaining > 0)
    {
      ui->progressBar->setValue(m_client_time_remaining);
      m_progress_timer.start(10);
    }
    else
      ui->led->setState(false);
  }
  );
  connect(ping_timer, &QTimer::timeout, q_client, &Client::ping);

  m_progress_timer.start(10);
  m_pong_timer    .start();
  ping_timer     ->start(10000);
}

/**
 * @brief MainWindow::onMessageReceived
 * @param t
 * @param message
 * @param v
 */
void MainWindow::onMessageReceived(int t, const QString& message, StringVec v) {
  QString timestamp_prefix = utils::timestampPrefix();
  switch (t)
  {
  case(PONG_REPLY_TYPE):
  {

    qint64 elapsed_time = m_pong_timer.elapsed();
    if (elapsed_time < (1000 * 60))
    {
      ui->lastPing->setText(QString::number(elapsed_time) + " ms");
      m_pong_timer.restart();
      m_client_time_remaining = DEFAULT_TIMEOUT;
    }
    else
    {
      m_progress_timer.stop();
      qDebug() << "Server timeout";
    }
    break;
  }

  case(MESSAGE_UPDATE_TYPE):      // Normal message
    qDebug() << "Updating message area";
    m_controller.handleMessage(message, v);
    message_ui.append(message);
    break;

  case(COMMANDS_UPDATE_TYPE):     // Received apps from server
    qDebug() << "Updating commands";

    m_controller.handleCommands(v, configValue("defaultApp", m_config));

    if (message == "New Session") // Session has started
    {
      ui->led->setState(true);
      if (configBoolValue("schedulerMode", std::ref(m_config)))
        arg_ui->show();

      if (configBoolValue("fetchSchedule", m_config))
        q_client->request(RequestType::FETCH_SCHEDULE);      
    }
    break;

  case(PROCESS_REQUEST_TYPE):   // Sent execution request to server
  {
    qDebug() << "Updating process list";
    m_processes.push_back(Process{.name = v.at(1),
                                  .state = ProcessState::PENDING,
                                  .start = TimeUtils::getTime(),
                                  .id = v.at(2)});

    int row = 0;
    for (const auto& process : m_processes) {
      m_process_model->setItem(row, utils::createProcessListItem(process));
      row++;
    }
    break;
  }

  case(EVENT_UPDATE_TYPE):      // Received event from server
  {
    QString event_message = m_controller.handleEventMessage(message, v);

    if (m_events.size() > 1)    // Group repeating event messages
    {
      auto last_event = m_events[m_events.size() - 1];
      if (utils::isSameEvent(message, last_event.remove(0, 11))) {
        m_consecutive_events++;
        auto count = utils::getLikeEventNum(event_message, m_events);
        auto clean_event_message = event_message + " (" + QString::number(count) + ")";
        m_events.push_back(event_message);

        m_event_model->setItem(m_event_model->rowCount() - 1,
                               utils::createEventListItem(clean_event_message));
        return;  // It was not a unique message, we can return
      }
      m_consecutive_events = 0;
    }
    if (isKEvent<QString>(message, Event::TASK_SCHEDULED))    // Event was scheduled task
    {
      event_message += ". Details:\n" + parseTaskInfo(v);
      UI::infoMessageBox(event_message, "Schedule request succeeded");
      arg_ui->notifyClientSuccess(); // Update ArgDialog accordingly
    }
    m_events.push_back(event_message);
    m_event_model->setItem(m_event_model->rowCount(),
                           utils::createEventListItem(event_message));
    break;
  }
  default:
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
  // TODO: We expect 5 arguments. Create a better verification pattern.
  auto error = v.size() < 5;
  if (error) {
    task_info += "\n!ERROR! - " + v.at(TaskIndex::ERROR);
  } else {
    task_info += "UUID - " + v.at(TaskIndex::UUID) + "\nID - " + v.at(TaskIndex::ID) + "\nAPP - " +
                 q_client->getAppName(std::stoi(v.at(TaskIndex::MASK).toUtf8().constData())) +
                 "\nENV - " + (v.at(TaskIndex::ENVFILE) + "\nFILES - " + v.at(TaskIndex::FILENUM));
    for (auto i = 5; i < v.size(); i++) {
      task_info += "\nFILENAME - " + v.at(i);
    }
  }
  return task_info;
}

/**
 * @brief ArgDialog::keyPressEvent
 * @param e
 */
void MainWindow::keyPressEvent(QKeyEvent *e) {
  if (Qt::ControlModifier) {
    if (e->key()==Qt::Key_Return || e->key()==Qt::Key_Enter) {
      const auto text = ui->inputText->toPlainText();
      if (!text.isEmpty()) {
        q_client->sendMessage(text);
      }
      ui->inputText->clear();
    }
  }
}
