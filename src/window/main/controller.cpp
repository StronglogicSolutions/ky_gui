#include <include/ui/mainwindow.h>

#include <QDateTime>
#include <QDebug>
#include <QLayout>
#include <QScrollBar>
#include <QString>
#include <QTextEdit>
#include <QTextStream>
#include <headers/util.hpp>
#include <vector>


/**
 * Controller
 *
 * \note We use the Controller class to do the heavy lifting of parsing
 * incoming messages and updating the UI accordingly
 */

/**
 * @brief MainWindow::Controller::init
 * @param window
 */
void MainWindow::Controller::init(MainWindow* window) {
  this->window = window;
}

/**
 * @brief MainWindow::MessageParser::handleCommands
 * @param commands
 * @param default_command
 */
void MainWindow::Controller::handleCommands(const StringVec& args,
                                            const QString&   default_command)
{
  uint8_t MASK_INDEX = 0;
  uint8_t NAME_INDEX = 1;
  uint8_t PATH_INDEX = 2;
  uint8_t DATA_INDEX = 3;

  QComboBox* app_list = window->ui->appList;
  app_list->clear();

  int arg_index{0};

  QVector<KApplication> k_applications{};
  KApplication application{};
  k_applications.reserve((args.size() / 4));

  for (const auto& arg : args) {
    if (arg_index == MASK_INDEX) {
      application.mask = arg;
    }
    else
    if (arg_index == NAME_INDEX) {
      application.name = arg;
    }
    else
    if (arg_index == PATH_INDEX) {
      application.path = arg;
    }
    else
    if (arg_index == DATA_INDEX) {
      application.data = arg;
      arg_index = 0;
      app_list->addItem(application.name);
      k_applications.push_back(application);

      continue;
    }
    arg_index++;
  }
  window->q_client->setCommands(k_applications);

  int app_index{0};

  for (const auto& application : k_applications)
  {
    if (application.name.toLower() == default_command.toLower()) {
      window->ui->appList->setCurrentIndex(app_index);
      std::vector<QString> selected{default_command};
      window->q_client->setSelectedApp(selected);
    }
    app_index++;
  }
  window->app_ui.setApplications(k_applications);
}
/**
 * @brief MainWindow::MessageParser::handleMessage
 * @param message
 * @param v
 */
void MainWindow::Controller::handleMessage(const QString& message, const StringVec& v)
{
  window->message_ui.append(
    utils::timestampPrefix() + parseMessage(message, v), true
  );
}

/**
 * @brief MainWindow::MessageParser::parseMessage
 * @param message
 * @param v
 * @return
 */
QString MainWindow::Controller::parseMessage(const QString&   message,
                                             const StringVec& v)
{
  QString simplified_message{};

  if      (isMessage  (message.toUtf8()))
    simplified_message += "Message: "   + getMessage(message);
  else if (isEvent    (message.toUtf8()))
    simplified_message += "Event: "     + getEvent(message);
  else if (isOperation(message.toUtf8()))
    simplified_message += "Operation: " + getOperation(message);

  if (!v.empty())
    simplified_message += "\nArguments:";

  for (const auto& arg : v) simplified_message += "\n" + arg;

  return simplified_message;
}

/**
 * @brief MainWindow::MessageParser::updateProcessResult
 * @param id
 * @param result
 * @param error
 */
void MainWindow::Controller::updateProcessResult(
    const QString& id,
    const QString& result,
    const bool     error = false)
{
  for (int i = window->m_processes.size() - 1; i >= 0; i--)
  {
    if (window->m_processes.at(i).id == id)
    {
      window->m_processes.at(i).end    = TimeUtils::getTime();
      window->m_processes.at(i).state  = !error ?
                                           ProcessState::SUCCEEDED :
                                           ProcessState::FAILED;
      window->m_processes.at(i).result = result;
      window->m_process_model->setItem(i, 0, utils::createProcessListItem(window->m_processes.at(i)));
      return;
    }
  }
}

/**
 * @brief MainWindow::MessageParser::handleEventMessage
 * @param message
 * @param v
 * @return
 */
QString MainWindow::Controller::handleEventMessage(const QString&   message,
                                                   const StringVec& v)
{
  KLOG("Event: ", message);

  QString event_message = utils::timestampPrefix();
  if (v.size() == 1)
    event_message += message + "\n" + v.at(0);
  else
  {
    event_message += message;

    if (message == "Process Result")
    {
      const auto error = v.size() > 3;
      const auto app_name = window->q_client->getAppName(std::stoi(v.at(0).toUtf8().constData()));
      event_message += '\n';
      event_message += app_name;
      event_message += ": ";
      event_message += v.at(2);

      const auto process_it = std::find_if(window->m_processes.begin(), window->m_processes.end(),
        [v](const Process& process) { return process.id == v.at(1); });

      if (process_it != window->m_processes.end())
        updateProcessResult(v.at(1), v.at(2), error);
      else // new process, from scheduled task
      {
        Process new_process{
          .name  = app_name,
          .state = !error ? ProcessState::SUCCEEDED : ProcessState::FAILED,
          .start = TimeUtils::getTime(),
          .id    = "Scheduled task",
          .error = error ? v.at(3) : "No errors reported"};

          if (v.count() > 2 && !v.at(2).isEmpty())
          {
            new_process.result = v.at(2);
            new_process.end = new_process.start;
          }

        window->m_processes.push_back(new_process);
        window->m_process_model->setItem(window->m_process_model->rowCount(),
                                         utils::createProcessListItem(new_process));
      }

      if (error)
        event_message += "\n Error: " + v.at(3);
    }
    else
    if (message == "Platform Post")
    {
      if (v.size() < 6)
        event_message += "\nEvent occurred, but data is missing";
      else
        event_message += "\nContent: " + v.at(3) + "\nCompleted: " + v.at(5);
    }
    else
    if (QString::compare(message, "Application was registered") == 0) {
      const KApplication application{.name = v.at(0), .path = v.at(1), .data = v.at(2), .mask = v.at(3)};
      window->app_ui.      addApplication(application);
      window->q_client->   addCommand    (application);
      window->ui->appList->addItem       (application.name);
    }
    else
    if (message == "Application was deleted")
    {
      const auto name = v.at(0);
      const auto i   = window->ui->appList->findText(name);
      window->app_ui.removeApplication(KApplication{.name = name});
      if (i != -1)
        window->ui->appList->removeItem(i);
    }
    else
    if (message =="Scheduled Tasks")
    {
      const auto details = v.at(0);
      if (details == "Schedule")
      {
        window->schedule_ui.clear();
        window->schedule_ui.insert_tasks(v);
      }
      else
      if (details == "Schedule more")
        window->schedule_ui.insert_tasks(v);
      else
      if (details == "Schedule end")
        window->schedule_ui.receive_response(RequestType::FETCH_SCHEDULE, v);
    }
    else
    if (message == "Schedule PUT")
      window->schedule_ui.receive_response(RequestType::UPDATE_SCHEDULE, v);
    else
    if (message == "Schedule Tokens")
      window->schedule_ui.receive_response(RequestType::FETCH_SCHEDULE_TOKENS, v);
    else
    if (message == "Application Flags")
    {
      if (v.front().toInt() != window->q_client->getSelectedApp())
        qDebug() << "do not match";
      else
        window->doc_window.SetFlags(QList<QString>{v.begin() + 1, v.end()});
    }
    else
    if (message == "Task Data")
      window->doc_window.ReceiveData(message, v);
    else
    if (message == "Task Data Final")
      window->doc_window.ReceiveData(message, v);
    else
    if (message == "File Upload")
      window->q_client->setIncomingFile(v);
    else
    if (message == "File Upload Meta")
      window->q_client->setMetadata(v);
    else
    if (message == "Message Received")
      event_message += "\n" + v.at(1) + ": " + v.at(2);
  }
  return event_message;
}
