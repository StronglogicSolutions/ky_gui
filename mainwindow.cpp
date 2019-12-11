#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QCommandLinkButton>
#include <QProcess>
#include <QDebug>
#include <QLabel>
#include <QTextStream>
#include <QString>
#include <QVector>
#include <QLayout>
#include <client.hpp>
#include <vector>

MainWindow::MainWindow(int argc, char** argv, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    cli_argc(argc),
    cli_argv(argv) {
    this->process = new QProcess(this);
    ui->setupUi(this);
    ui->inputText->setText("KSupStyle YO?");
    this->connectUi();
}

/**
 * @brief MainWindow::buttonClicked
 */
void MainWindow::connectClient() {
    qDebug() << "You clicked the button!!";

    Client* q_client = new Client(this, cli_argc, cli_argv);
    QObject::connect(q_client, &Client::messageReceived, this, &MainWindow::updateMessages);
    q_client->start();
    QTextEdit* send_message_box = this->findChild<QTextEdit*>("inputText");

    QPushButton* send_message_button = this->findChild<QPushButton*>("sendMessage");
    QObject::connect(send_message_button, &QPushButton::clicked, this, [&q_client, &send_message_box]() {
        q_client->sendMessage(send_message_box->toPlainText());
    });
}

void MainWindow::updateMessages(const QString& s) {
    QLabel* message_display = this->findChild<QLabel*>("messages");
    message_display->setText(s);
}

void MainWindow::connectUi() {
    QCommandLinkButton *button = this->findChild<QCommandLinkButton*>("connect");
    connect(button, &QCommandLinkButton::clicked, this, &MainWindow::connectClient);
}


MainWindow::~MainWindow()
{
    delete ui;
}
