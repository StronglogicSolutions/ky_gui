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
void MainWindow::Controller::handleCommands(StringVec args,
                                            QString default_command) {
  uint8_t MASK_INDEX = 0;
  uint8_t NAME_INDEX = 1;
  uint8_t PATH_INDEX = 2;
  uint8_t DATA_INDEX = 3;

  QComboBox* app_list = window->ui->appList;
  app_list->clear();
  int app_index{0};
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
      app_index++;
      arg_index = 0;
      app_list->addItem(application.name);
      if (application.name.toLower() == default_command.toLower()) {
        window->ui->appList->setCurrentIndex(app_index);
        std::vector<QString> selected{default_command};
        window->q_client->setSelectedApp(selected);
      }
      k_applications.push_back(application);
      continue;
    }
    arg_index++;
  }
  window->q_client->setCommands(k_applications);
}
/**
 * @brief MainWindow::MessageParser::handleMessage
 * @param message
 * @param v
 */
void MainWindow::Controller::handleMessage(QString message, StringVec v) {
  window->ui->messages->append(
    utils::timestampPrefix() + parseMessage(message, v)
  );
}

/**
 * @brief MainWindow::MessageParser::parseMessage
 * @param message
 * @param v
 * @return
 */
QString MainWindow::Controller::parseMessage(const QString& message,
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
void MainWindow::Controller::updateProcessResult(
    QString id, QString result,
    bool error = false) {  // We need to start matching processes with a
  // unique identifier
  for (int i = window->m_processes.size() - 1; i >= 0; i--) {
    if (window->m_processes.at(i).id == id) {
      window->m_processes.at(i).end = utils::getTime();
      window->m_processes.at(i).state =
          !error ? ProcessState::SUCCEEDED : ProcessState::FAILED;
      window->m_processes.at(i).result = result;
      window->m_process_model->setItem(
          i, 0, utils::createProcessListItem(window->m_processes.at(i)));
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
QString MainWindow::Controller::handleEventMessage(QString message,
                                                      StringVec v) {
  QString event_message = utils::timestampPrefix();
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
              .name  = app_name,
              .state = !error ? ProcessState::SUCCEEDED : ProcessState::FAILED,
              .start = utils::getTime(),
              .id    = "Scheduled task",
              .error = error ? v.at(3) : "No errors reported"};
          if (v.count() > 2 && !v.at(2).isEmpty()) {
            new_process.result = v.at(2);
            new_process.end = new_process.start;
          }
          window->m_processes.push_back(new_process);
          window->m_process_model->setItem(window->m_process_model->rowCount(),
                                           utils::createProcessListItem(new_process));
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
