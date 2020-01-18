#include <include/mainwindow.h>
#include "ui_mainwindow.h"
#include <QDebug>
#include <QTextEdit>
#include <QTextStream>
#include <QString>
#include <QLayout>
#include <headers/ktextedit.hpp>
#include <QDateTime>
#include <vector>
#include <headers/util.hpp>

QString getTime() {
    return QDateTime::currentDateTime().toString("hh:mm:ss");
}

/**
 * @brief MainWindow::MainWindow
 * @param argc
 * @param argv
 * @param parent
 */
MainWindow::MainWindow(int argc, char** argv, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    arg_ui(new ArgDialog),
    cli_argc(argc),
    cli_argv(argv),
    q_client(nullptr) {
    m_process_model = new QStandardItemModel(this);
    q_client = new Client(this, cli_argc, cli_argv);
    ui->setupUi(this);
    this->setWindowTitle("KYGUI");
    QPushButton *button = this->findChild<QPushButton*>("connect");
    connect(button, &QPushButton::clicked, this, &MainWindow::connectClient);
    ui->processList->setModel(m_process_model);
}

/**
 * @brief MainWindow::~MainWindow
 */
MainWindow::~MainWindow()
{
    delete q_client;
    delete ui;
}

/**
 * @brief MainWindow::buttonClicked
 */
void MainWindow::connectClient() {
    qDebug() << "Connecting to KServer";

    QObject::connect(q_client, &Client::messageReceived, this, &MainWindow::updateMessages);

    QProgressBar* progressBar = ui->progressBar;
    q_client->start();

    for (int i = 1; i < 101; i++) {
        progressBar->setValue(i);
    }

    KTextEdit* send_message_box = reinterpret_cast<KTextEdit*>(ui->inputText);
    send_message_box->show();

    QPushButton* send_message_button = this->findChild<QPushButton*>("sendMessage");
    // Handle mouse
    QObject::connect(send_message_button, &QPushButton::clicked, this, [this, send_message_box]() {
        q_client->sendMessage(send_message_box->toPlainText());
        send_message_box->clear();
    });

    QObject::connect(ui->appList,  static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, [this]() {
        QString app_name = ui->appList->currentText();
        // TODO: I know, it's awful. Fix this
        q_client->setSelectedApp(std::vector<QString>{{app_name}});

    });
    QPushButton* disconnect_button = this->findChild<QPushButton*>("disconnect");
    QObject::connect(disconnect_button, &QPushButton::clicked, this, [this, progressBar]() {
        q_client->closeConnection();
        progressBar->setValue(0);
        ui->appList->clear();
        ui->messages->clear();
        ui->led->setState(false);
    });

    QObject::connect(ui->execute, &QPushButton::clicked, this, [this]() {
        q_client->execute();
    });

    QObject::connect(ui->addArgs, &QPushButton::clicked, this, [this]() {
        if (ui->appList->count() == 0) {
            QMessageBox::warning(this, tr("Args"), tr("Please connect to the KServer and retrieve a list of available processes."));
        } else {
            arg_ui->show();
        }
    });

    QObject::connect(arg_ui, &ArgDialog::uploadFile, this, [this](QByteArray bytes) {
        q_client->sendFile(bytes);
    });

    QObject::connect(arg_ui, &ArgDialog::taskRequestReady, this, [this](Task task, bool file_pending) {
        // TODO: Maybe this should be handled by the Client class directly
        auto mask = q_client->getSelectedApp();
        if (mask > -1) {
            if (q_client->getAppName(mask) == "Instagram") {
                auto datetime = task.args.at(1);
                auto current_datetime = QDateTime::currentDateTime().toTime_t();
                auto seconds_diff = std::stoi(datetime) - current_datetime;
                qDebug() << "Time difference: " << seconds_diff;
                if (seconds_diff > 3600) {
                    qDebug() << "Scheduling a task";
                    task.args.push_back(std::to_string(mask));
                    q_client->scheduleTask(task.args, file_pending);
                }
            }
        }
    });

    QObject::connect(ui->tasks, &QPushButton::clicked, this, [this]() {
        // TODO: Change this to a complete implementation
        q_client->sendMessage("scheduler");
    });

    QObject::connect(ui->viewConsole, &QPushButton::clicked, this, [this]() {
        m_console.show();
    });

    // TODO: Handle enter key
//    QObject::connect(static_cast<KTextEdit*>(ui->inputText), &KTextEdit::textInputEnter, this, &MainWindow::handleInputEnterKey);
    QObject::connect(static_cast<KTextEdit*>(ui->inputText), &KTextEdit::textInputEnter, this, &MainWindow::handleInputEnterKey);

    QObject::connect(ui->processList, &QListView::clicked, this, [this](const QModelIndex &index) {
        QMessageBox::information(
            this, tr("Process"),
            tr(m_processes.at(index.row()).name.toUtf8() + "\n" +
               "Execution requested at " + m_processes.at(index.row()).start.toUtf8() + "\n" +
               "Is currently in a state of: " + ProcessNames[m_processes.at(index.row()).state - 1].toUtf8()),
            tr("Close"));
    });
}

void MainWindow::handleInputEnterKey() {
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

return new QStandardItem(QString("%0 requested for execution. ID: %1\nStatus: %2\nTime: %3   Done: %4").arg(process.name).arg(process.id).arg(ProcessNames[process.state - 1]).arg(process.start).arg(process.end));
}



/**
 * @brief MainWindow::updateMessages
 * @param s
 */
void MainWindow::updateMessages(int t, const QString& message, StringVec v) {
    QString timestamp_prefix = QDateTime::currentDateTime().toString("hh:mm:ss") + " - ";
    if (t == MESSAGE_UPDATE_TYPE) {
        qDebug() << "Updating message area";
        auto simple_message = timestamp_prefix + parseMessage(message, v);
        ui->messages->append(simple_message);
        m_console.updateText(message);
    } else if (t == COMMANDS_UPDATE_TYPE) {
        if (message == "New Session") {
            ui->led->setState(true);
        }
        qDebug() << "Updating commands";
        QComboBox* app_list = ui->appList;
        app_list->clear();
        for (const auto& s : v) {
            app_list->addItem(s);
        }
        //TODO: We do this because a CommandLinkButton turns transparent by default, except when hovered or checked
        ui->connect->setChecked(true);
    } else if (t == PROCESS_REQUEST_TYPE) {
        qDebug() << "Updating process list";
        m_processes.push_back(Process{ .name=v.at(1), .state=ProcessState::PENDING, .start=getTime(), .id=v.at(2) });
        int row = 0;
        for (const auto& process : m_processes) {
            m_process_model->setItem(row, createProcessListItem(process));
            row++;
        }

        //TODO: We do this because a CommandLinkButton turns transparent by default, except when hovered or checked
        ui->connect->setChecked(true);
    } else if (t == EVENT_UPDATE_TYPE) {
        QString event_message{timestamp_prefix};
        if (!v.empty()) {
            // TODO: extract process result handling from here. This should handle any event
            if (v.size() == 1) {
                event_message += message + "\n" + v.at(0);
            } else {
                event_message += message;
                if (message == "Process Result") {
                    event_message += "\n";
                    auto app_name = q_client->getAppName(std::stoi(v.at(0).toUtf8().constData()));
                    if (v.at(1).length() > 0) {
                        updateProcessResult(v.at(1));
                    } else { // new process, from scheduled task
                        Process new_process{ .name=app_name, .state=ProcessState::SUCCEEDED, .start=getTime(), .id="Scheduled task" };
                        m_processes.push_back(new_process);
                        m_process_model->setItem(m_process_model->rowCount(), createProcessListItem(new_process));
                    }
                    event_message += app_name;
                    event_message += ": ";
                    event_message += v.at(2);
                } else {
                    event_message += ": ";
                    event_message += v.at(1);
                }
            }
        } else {
            event_message += message;
        }
        m_events.push_front(event_message);
        ui->eventList->clear();

        for (const auto& i : Kontainer::ReverseIterator(m_events)) {
            ui->eventList->addItem(i);
        }
    } else {
        qDebug() << "Unknown update type. Cannot update UI";
    }
}

void MainWindow::updateProcessResult(QString id) { // We need to start matching processes with a unique identifier
    for (int i = m_processes.size() - 1; i >= 0; i--) {
        if (m_processes.at(i).id == id) {
            m_processes.at(i).end = getTime();
            m_processes.at(i).state = ProcessState::SUCCEEDED;
            m_process_model->setItem(i, 0, createProcessListItem(m_processes.at(i)));
            return;
        }
    }
}

void MainWindow::keyPressEvent(QKeyEvent *e) {
    qDebug() << "Key press: " << e->key();
    if(e->key()==Qt::Key_0)
    {
        qDebug() << "Ok";
    }
}
