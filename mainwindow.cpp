#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QCommandLinkButton>
#include <QDebug>
#include <QLabel>
#include <QTextStream>
#include <QString>
#include <QLayout>
#include <client.hpp>
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

    QTextEdit* send_message_box = this->findChild<QTextEdit*>("inputText");
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
void MainWindow::updateMessages(const QString& s) {
    QLabel* message_display = this->findChild<QLabel*>("messages");
    message_display->setText(message_display->text() + "\n" + s);
}
