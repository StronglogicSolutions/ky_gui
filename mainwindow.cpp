#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QCommandLinkButton>
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
    cli_argc(argc),
    cli_argv(argv) {
    ui->setupUi(this);
    QCommandLinkButton *button = this->findChild<QCommandLinkButton*>("connect");
    connect(button, &QCommandLinkButton::clicked, this, &MainWindow::connectClient);
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

    q_client->start();

    KTextEdit* send_message_box = reinterpret_cast<KTextEdit*>(ui->inputText);
    send_message_box->show();

    QPushButton* send_message_button = this->findChild<QPushButton*>("sendMessage");
    // Handle mouse
    QObject::connect(send_message_button, &QPushButton::clicked, this, [q_client, send_message_box]() {
        q_client->sendMessage(send_message_box->toPlainText());
        send_message_box->clear();
    });
    // TODO: Handle enter key
//    QObject::connect(send_message_box, &QTextEdit::keyReleaseEvent, this, [q_client, send_message_box]() {
//        q_client->sendMessage(send_message_box->toPlainText());
//        send_message_box->clear();
//    });

    QPushButton* disconnect_button = this->findChild<QPushButton*>("disconnect");
    QObject::connect(disconnect_button, &QPushButton::clicked, q_client, &Client::closeConnection);
}

/**
 * @brief MainWindow::updateMessages
 * @param s
 */
void MainWindow::updateMessages(int t, const QString& s, StringVec v) {
    if (t == MESSAGE_UPDATE_TYPE) {
        qDebug() << "Updating message area";
        ui->messages->append(s);
    } else if (t == COMMANDS_UPDATE_TYPE) {
        qDebug() << "Updating commands";
        QListWidget* appList = ui->appList;
        appList->clear();
        for(const auto& s : v) {
            new QListWidgetItem(tr(s.toUtf8()), appList);
        }
    } else {
        qDebug() << "Unknown update type. Cannot update UI";
    }
}

