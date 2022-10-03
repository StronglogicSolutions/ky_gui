#include <include/ui/mainwindow.h>
#include <QGraphicsDropShadowEffect>

/**
 * Helper functions
 */

namespace utils {
void save_config(const QString& config)
{
  QFile config_file(QCoreApplication::applicationDirPath() + "/config/config.json");
  if (!config_file.open(QIODevice::WriteOnly | QFile::WriteOnly | QIODevice::Text))
    KLOG("Unable to save config");

  QTextStream{&config_file} << config;
  config_file.close();
}

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
      m_controller(this),
      ui(new Ui::MainWindow),
      arg_ui(new ArgDialog),
      m_client_time_remaining(DEFAULT_TIMEOUT)
{
  m_event_model   = new QStandardItemModel(this);
  m_process_model = new QStandardItemModel(this);
  q_client        = new Client(this, cli_argc, cli_argv);
  auto stay_alive = [this]
  {
    static const bool reconnect = true;
    q_client->ping();
    if (m_pong_timer.elapsed() > (1000 * 60))
    {
      switch (++m_timeouts)
      {
        case (4):
          q_client->closeConnection();
          to_console("Closing connection");
        break;
        default:
          to_console(QString{"Timeouts: %0"}.arg(m_timeouts));
          if (!(m_timeouts % 5))
          {
            m_client_time_remaining = DEFAULT_TIMEOUT;
            ui->led->setState(ConnectionIndicator::State::StateWarning);
            connectClient(reconnect);
            to_console("Reconnecting");
          }
        break;
      }
      m_pong_timer.restart();
    }
  };  
  ui->setupUi(this);
  setWindowTitle("KYGUI");
  setConnectScreen();    
  ui->inputText  ->setTabChangesFocus(true);
  ui->kyConfig   ->setTabChangesFocus(true);
  ui->serverIp   ->setTabChangesFocus(true);
  ui->serverPort ->setTabChangesFocus(true);
  ui->progressBar->setRange(0, DEFAULT_TIMEOUT);
  ui->eventList  ->setModel(m_event_model);
  ui->processList->setModel(m_process_model);
  ui->serverIp   ->setText(argv[1]);
  ui->serverPort ->setText(argv[2]);
  ui->fetchToken ->setStyleSheet(GetFetchButtonTheme());
  ui->connect    ->setStyleSheet(GetConnectButtonTheme());
  QObject::connect(&m_ping_timer,    &QTimer::timeout, stay_alive);
  QObject::connect(&schedule_ui,     &ScheduleDialog::SchedulerRequest, this, [this](auto type, auto task) { q_client->request(type, task); });
  QObject::connect(&posts_ui,        &PostDialog::request_update,       this, [this](const auto& post)     { q_client->request(constants::RequestType::UPDATE_POST, post.payload()); });
  QObject::connect(ui->eventList,    &QListView::clicked,               this, [this](const auto& index)    { utils::infoMessageBox(m_event_model->item(index.row(), index.column())->text(), "Event"); });
  QObject::connect(&schedule_ui,     &ScheduleDialog::UpdateSchedule,   this, [this] { q_client->request(RequestType::FETCH_SCHEDULE); });
  QObject::connect(q_client,         &Client::onTokenReceived, this, [this](bool error) { set_connected(!error); });
  QObject::connect(q_client,         &Client::messageReceived, this, &MainWindow::onMessageReceived);
  QObject::connect(ui->connect,      &QPushButton::clicked, this, &MainWindow::connectClient);
  QObject::connect(ui->disconnect,   &QPushButton::clicked, this, [this] { exit(); });
  QObject::connect(ui->execute,      &QPushButton::clicked, this, [this] { q_client->execute(); });
  QObject::connect(ui->openMessages, &QPushButton::clicked, this, [this] { message_ui.show(); });
  QObject::connect(ui->editApps,     &QPushButton::clicked, this, [this] { app_ui.show(); });
  QObject::connect(ui->saveConfig,   &QPushButton::clicked, this, [this] { utils::save_config(ui->kyConfig->toPlainText()); });
  QObject::connect(ui->fetchTerms,   &QPushButton::clicked, this, [this] { q_client->request(RequestType::FETCH_TERM_HITS); });
  QObject::connect(ui->tasks,        &QPushButton::clicked, this, [this] { schedule_ui.show(); });
  QObject::connect(ui->addArgs,      &QPushButton::clicked, this, [this] { arg_ui->show(); });
  QObject::connect(ui->fetchToken,   &QPushButton::clicked, this, [this]()
  {
    m_config = loadJsonConfig(ui->kyConfig->toPlainText());
    if (!m_config.contains("username") || !m_config.contains("password") || !m_config.contains("auth"))
      return KLOG("Unable to connect to KIQ without credentials. Please modify config JSON");

    q_client->SetCredentials(configValue("username", m_config), configValue("password", m_config), configValue("auth",     m_config));
    arg_ui->setFilePath(configValue("fileDirectory", m_config));
  });

  QObject::connect(&doc_window, &DocumentWindow::RequestData, this, [this](QVector<QString> argv)
  {
    q_client->SetFetching();
    q_client->request(static_cast<uint8_t>(RequestType::FETCH_TASK_DATA), argv);
  });

  QObject::connect(&doc_window, &DocumentWindow::RequestFiles, this, [this](const QVector<QString>& ids)
  {
    q_client->SetFetching();
    q_client->request(static_cast<uint8_t>(RequestType::FETCH_FILE), ids);
  });

  QObject::connect(q_client, &Client::onDownload, this,
    [this](DownloadConsole::Files files) -> void
    {
      q_client->SetFetching(false);
      doc_window.ReceiveFiles(std::move(files));
    }
  );  

  QObject::connect(ui->actionDefault, &QAction::triggered, this, [this]()
  {
    ui->centralWidget->setStyleSheet(KYGUI_DEFAULT_THEME);
    ui->processList  ->setStyleSheet(KYGUI_DEFAULT_LIST_THEME);
    ui->eventList    ->setStyleSheet(KYGUI_DEFAULT_LIST_THEME);
  });

  QObject::connect(ui->actionBlack, &QAction::triggered, this, [this]()
  {
    ui->centralWidget->setStyleSheet(KYGUI_BLACK_THEME);
    ui->processList  ->setStyleSheet(KYGUI_BLACK_LIST_THEME);
    ui->eventList    ->setStyleSheet(KYGUI_BLACK_LIST_THEME);
  });

  QObject::connect(ui->actionBlue, &QAction::triggered, this, [this]()
  {
    ui->centralWidget->setStyleSheet(KYGUI_BLUE_THEME);
    ui->processList  ->setStyleSheet(KYGUI_BLUE_LIST_THEME);
    ui->eventList    ->setStyleSheet(KYGUI_BLUE_LIST_THEME);
  });

  QObject::connect(ui->sendMessage, &QPushButton::clicked, this, [this]()
  {
    q_client->sendMessage(escapeMessage(ui->inputText->toPlainText()));
    ui->inputText->clear();
  });

  QObject::connect(ui->appList, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
    this, [this]()
    {
      QString app_name = ui->appList->currentText();
      q_client->setSelectedApp(std::vector<QString>{{app_name}});      
      arg_ui  ->setConfig(configObject(app_name, m_config, true));
      arg_ui  ->setAppName(app_name);
    });

  QObject::connect(ui->platform, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
    this, [this]() { UpdateIPCOptions(); });  

  QObject::connect(arg_ui, &ArgDialog::taskRequestReady, this, [this](Task* task)
  {
    if (q_client->getSelectedApp() > -1)
      q_client->scheduleTask(task);
  });

  QObject::connect(&app_ui, &AppDialog::appRequest, this, [this](KApplication application, RequestType type)
  {
      if (type == REGISTER && q_client->hasApp(application))
      {
        QMessageBox::warning(this, tr("Application request"),
                            tr("An application with that name already exists"));
        return;
      }
      q_client->request(type, application);
  });

  QObject::connect(ui->fetchPosts, &QPushButton::clicked, this, [this]()
  {
    q_client->request(RequestType::FETCH_POSTS);
    posts_ui.show();
  });

  QObject::connect(ui->processList, &QListView::clicked, this, [this](const QModelIndex& index)
  {
    const auto process           = m_processes.at(index.row());
    QString    process_info_text = m_processes.at(index.row()).name.toUtf8() + "\n";
    process_info_text           += "Execution requested at " + process.start.toUtf8() +
                                   "\nIs currently in a state of: " + ProcessNames[process.state - 1].toUtf8();

    if (process.end.size() || process.id == "Scheduled task")
      process_info_text += "\n\nResult: \n" + process.result;
    utils::infoMessageBox(process_info_text, "Process");
  });  

  QObject::connect(m_event_model, &QAbstractItemModel::rowsAboutToBeInserted, this, [this]()
  {
   QScrollBar* bar = ui->eventList->verticalScrollBar();
   if (bar->value() == bar->maximum())
     m_view_states.eventViewBottom = true;
  });

  QObject::connect(m_event_model, &QAbstractItemModel::rowsInserted, this, [this]()
  {
    if (m_view_states.eventViewBottom)
      ui->eventList->scrollToBottom();
  });

  QObject::connect(ui->ipc, &QPushButton::clicked, this, [this]()
  {
    auto HasKey = [this](auto key) { return m_platform_map.find(key) != m_platform_map.end(); };
    auto platform = ui->platform->currentText();
    auto type = ui->ipcCommand->currentText();
    auto text = ui->inputText->toPlainText();
    auto user = defaultConfigUser(m_config);
    auto cmd  = platform.toLower() + ':' + type;
    auto arg  = QString::number(ui->ipcArg->value());
    auto opt  = (ui->ipcOption->count() && HasKey(platform)) ? m_platform_map.value(platform).at(ui->ipcOption->currentIndex()) : "";
    q_client->sendIPCMessage(cmd, text, user, opt, arg);
    ui->inputText->clear();
  });

  QObject::connect(ui->makeDoc, &QPushButton::clicked, this, [this]()
  {
    q_client->request(RequestType::FETCH_TASK_FLAGS);
    doc_window.show();
  });

  QObject::connect(&m_progress_timer, &QTimer::timeout, q_client, [this]() -> void
  {
    if (--m_client_time_remaining > 0)
    {
      ui->progressBar->setValue(m_client_time_remaining);
      m_progress_timer.start(10);
    }
  });
}

/**
 * @brief MainWindow::~MainWindow
 */
MainWindow::~MainWindow()
{
  q_client->closeConnection();
  delete q_client;
  delete ui;
  delete arg_ui;
  delete m_event_model;  
}

/**
 * @brief MainWindow::setConnectScreen
 * @param visible
 */
void MainWindow::setConnectScreen(bool visible)
{
  if (visible)
  {
    ui->tokenLED->setState(ConnectionIndicator::State::StateWarning);
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
    ui->startScreen->setMaximumSize(960, 640);
    ui->startScreen->setMinimumSize(640, 480);
    ui->kyConfig->setMaximumSize(960, 175);
    ui->kyConfig->setMinimumSize(640, 175);
    ui->serverIp->setMaximumSize(960, 30);
    ui->serverIp->setMinimumSize(640, 30);
    ui->ipLabel->setMaximumSize(120, 30);
    ui->ipLabel->setMinimumSize(120, 30);
    ui->serverPort->setMaximumSize(960, 30);
    ui->serverPort->setMinimumSize(640, 30);
    ui->portLabel->setMaximumSize(120, 30);
    ui->portLabel->setMinimumSize(120, 30);
    ui->configLabel->setMinimumSize(640, 30);
    ui->configLabel->setMaximumSize(960, 30);
    ui->centralWidget->setMaximumHeight(640);
    setMaximumHeight(640);

    QFile file(QCoreApplication::applicationDirPath() + "/config/config.json");
    file.open(QIODevice::ReadOnly | QFile::ReadOnly);
    ui->kyConfig->setText(QString::fromUtf8(file.readAll()));
    file.close();
    ui->outerLayer->setVisible(false);
  }
  else
  {
    ui->connect->hide();
    ui->fetchToken->hide();
    ui->saveConfig->hide();
    ui->kyConfig->hide();
    ui->ipLabel->hide();
    ui->configLabel->hide();
    ui->serverIp->hide();
    ui->portLabel->hide();
    ui->serverPort->hide();
    ui->tokenLED->hide();
    ui->startScreen->setVisible(false);
    ui->outerLayer->setVisible(true);
    ui->centralWidget->setMaximumHeight(QWIDGETSIZE_MAX);
    setMaximumHeight(QWIDGETSIZE_MAX);
  }
}

/**
 * @brief MainWindow::connectClient
 */
void MainWindow::connectClient(bool reconnect)
{
  using namespace constants;

  if (reconnect)
    q_client->reconnect();
  else
  {
    setConnectScreen(false);
    KLOG("Connecting to server");
    const auto& server_ip   = ui->serverIp->toPlainText();
    const auto& server_port = ui->serverPort->toPlainText();
    setWindowTitle(windowTitle() + ' ' + q_client->GetUsername() + "@kiq://" + server_ip + ":" + server_port);
    q_client->start(server_ip, server_port);
    startTimers();
  }
}

/**
 * @brief MainWindow::onMessageReceived
 * @param t
 * @param message
 * @param v
 */
void MainWindow::onMessageReceived(int t, const QString& message, StringVec v)
{
  QString timestamp_prefix = utils::timestampPrefix();
  switch (t)
  {
    case(PONG_REPLY_TYPE):
    {
      if (const auto elapsed_time = m_pong_timer.elapsed(); elapsed_time < (1000 * 60))
      {
        ui->lastPing->setText(QString::number(elapsed_time) + " ms");
        m_pong_timer.restart();
        m_client_time_remaining = DEFAULT_TIMEOUT;
      }
      else
      {
        if (m_progress_timer.isActive())
        {
          m_progress_timer.stop();
          KLOG("Heartbeat timeout");
        }
        else
          startTimers();
      }
    }
    break;
    case(MESSAGE_UPDATE_TYPE):
      KLOG("Updating message area");
      m_controller.handleMessage(message, v);
      message_ui.append(message);
    break;

    case(COMMANDS_UPDATE_TYPE):
      KLOG("Updating commands");

      m_controller.handleCommands(v, configValue("defaultApp", m_config));

      if (message == "New Session")
      {
        ui->led->setState(true);
        if (configBoolValue("schedulerMode", std::ref(m_config)))
          arg_ui->show();

        if (configBoolValue("fetchSchedule", m_config))
          q_client->request(RequestType::FETCH_SCHEDULE);
      }
    break;

    case(PROCESS_REQUEST_TYPE):
    {
      KLOG("Updating process list");
      m_processes.push_back(Process{.name = v.at(1),
                                    .state = ProcessState::PENDING,
                                    .start = TimeUtils::getTime(),
                                    .id = v.at(2)});
      int row = 0;
      for (const auto& process : m_processes)
      {
        m_process_model->setItem(row, utils::createProcessListItem(process));
        row++;
      }
    }
    break;
    case(EVENT_UPDATE_TYPE):
    {
      QString event_message = m_controller.handleEventMessage(message, v);      
      if (isKEvent<QString>(message, Event::TASK_SCHEDULED))
      {
        event_message += ". Details:\n" + parseTaskInfo(v);
        UI::infoMessageBox(event_message, "Schedule request succeeded");
        arg_ui->notifyClientSuccess(); // Update ArgDialog accordingly
      }
      to_console(message, event_message);
    }
    break;
    default:
      KLOG("Unknown update type. Cannot update UI");
    }
}

/**
 * @brief MainWindow::parseTaskInfo
 * @param v
 * @return
 */
QString MainWindow::parseTaskInfo(StringVec v)
{
  QString task_info{};
  if (!q_client)
  {
    KLOG("Can't parse when not connected");
    return task_info;
  }

  // TODO: We expect 5 arguments. Create a better verification pattern.
  auto error = v.size() < 5;
  if (error)
    task_info += "\n!ERROR! - " + v.at(TaskIndex::ERROR);
  else
  {
    task_info += "UUID - " + v.at(TaskIndex::UUID) + "\nID - " + v.at(TaskIndex::ID) + "\nAPP - " +
                 q_client->getAppName(std::stoi(v.at(TaskIndex::MASK).toUtf8().constData())) +
                 "\nENV - " + (v.at(TaskIndex::ENVFILE) + "\nFILES - " + v.at(TaskIndex::FILENUM));
    for (auto i = 5; i < v.size(); i++)
      task_info += "\nFILENAME - " + v.at(i);    
  }
  return task_info;
}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
  if (Qt::ControlModifier)
  {
    if (e->key()==Qt::Key_Return || e->key()==Qt::Key_Enter)
    {
      const auto text = ui->inputText->toPlainText();
      if (!text.isEmpty())
        q_client->sendMessage(text);      
      ui->inputText->clear();
    }
  }
}

void MainWindow::startTimers()
{
  m_progress_timer.start(10);
  m_pong_timer    .start();
  m_ping_timer    .start(10000);
}

void MainWindow::SetPlatformOptions(const QString& platform, const QList<QString>& options)
{
  m_platform_map[platform] = options;
  if (ui->platform->currentText() == platform)
    UpdateIPCOptions();
}

void MainWindow::UpdateIPCOptions()
{ 
  ui->ipcOption->clear();
  auto platform = ui->platform->currentText();
  auto list     = m_platform_map.value(platform);  
  for (const auto& option : list)
    ui->ipcOption->addItem(option.right(option.size() - (option.indexOf(',') + 1)));
}

void MainWindow::exit()
{
  q_client->closeConnection();
  QApplication::exit(CLIENT_EXIT);
}

void MainWindow::to_console(const QString& msg, const QString& event_msg)
{
  auto group_event_messages = [this] (const auto& message, const auto& event_message)
  {
    if (m_events.size() > 1)    // Group repeating event messages
    {
      auto last_event = m_events[m_events.size() - 1];
      if (utils::isSameEvent(message, last_event.remove(0, 11)))
      {
        auto count = utils::getLikeEventNum(event_message, m_events);
        auto clean_event_message = event_message + " (" + QString::number(count) + ")";
        m_events.push_back(event_message);

        m_event_model->setItem(m_event_model->rowCount() - 1,
                               utils::createEventListItem(clean_event_message));
        return;  // It was not a unique message, we can return
      }
    }
  };
  const auto event_message = (event_msg.isEmpty()) ?
    utils::timestampPrefix() + msg :
    utils::timestampPrefix() + event_msg;

  group_event_messages(msg, event_message);
  m_events.push_back(event_message);
  m_event_model->setItem(m_event_model->rowCount(), utils::createEventListItem(event_message));
}

void MainWindow::set_connected(bool connected)
{
  if (connected)
    m_timeouts = 0;
  ui->tokenLED->setState(connected);
}
