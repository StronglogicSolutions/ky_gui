#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QTextEdit>
#include <QTextStream>
#include <QString>
#include <QLayout>
#include <vector>

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
    cli_argv(argv) {
    ui->setupUi(this);
    this->setWindowTitle("KYGUI");
    QPushButton *button = this->findChild<QPushButton*>("connect");
    connect(button, &QPushButton::clicked, this, &MainWindow::connectClient);
}

/**
 * @brief MainWindow::~MainWindow
 */
MainWindow::~MainWindow()
{
    delete ui;
}

/**
 * @brief MainWindow::buttonClicked
 */
void MainWindow::connectClient() {
    qDebug() << "Connecting to KServer";

    Client* q_client = new Client(this, cli_argc, cli_argv);
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
    QObject::connect(send_message_button, &QPushButton::clicked, this, [q_client, send_message_box]() {
        q_client->sendMessage(send_message_box->toPlainText());
        send_message_box->clear();
    });

    QObject::connect(ui->appList,  static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, [this, q_client]() {
        QString app_name = ui->appList->currentText();
        // TODO: I know, it's awful. Fix this
        q_client->setSelectedApp(std::vector<QString>{{app_name}});

    });
    QPushButton* disconnect_button = this->findChild<QPushButton*>("disconnect");
    QObject::connect(disconnect_button, &QPushButton::clicked, this, [this, q_client, progressBar]() {
        q_client->closeConnection();
        progressBar->setValue(0);
        ui->appList->clear();
        ui->messages->clear();
    });

    QObject::connect(ui->execute, &QPushButton::clicked, this, [this, q_client]() {
        q_client->execute();
    });

    QObject::connect(ui->addArgs, &QPushButton::clicked, this, [this]() {
        if (ui->appList->count() == 0) {
            QMessageBox::warning(this, tr("Args"), tr("Please connect to the KServer and retrieve a list of available processes."));
        } else {
            arg_ui->show();
        }
    });

    QObject::connect(arg_ui, &ArgDialog::uploadFile, this, [q_client](QByteArray bytes) {
        q_client->sendFile(bytes);
    });

    // TODO: Handle enter key
    //    QObject::connect(send_message_box, &QTextEdit::keyReleaseEvent, this, [q_client, send_message_box]() {
    //        q_client->sendMessage(send_message_box->toPlainText());
    //        send_message_box->clear();
    //    });
}

/**
 * @brief MainWindow::updateMessages
 * @param s
 */
void MainWindow::updateMessages(int t, const QString& message, StringVec v) {
    if (t == MESSAGE_UPDATE_TYPE) {
        qDebug() << "Updating message area";
        ui->messages->append(message);
    } else if (t == COMMANDS_UPDATE_TYPE) {
        qDebug() << "Updating commands";
        QComboBox* app_list = ui->appList;
        app_list->clear();
        for (const auto& s : v) {
            app_list->addItem(s);
        }
        //TODO: We do this because a CommandLinkButton turns transparent by default, except when hovered or checked
        ui->connect->setChecked(true);
    } else {
        qDebug() << "Unknown update type. Cannot update UI";
    }
}

