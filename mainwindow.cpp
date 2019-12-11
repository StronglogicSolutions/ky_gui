#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QCommandLinkButton>
#include <QProcess>
#include <QDebug>
#include <QTextStream>
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
    ui->textEdit->setText("KSupStyle YO?");
    this->connectUi();
}

/**
 * @brief MainWindow::buttonClicked
 */
void MainWindow::buttonClicked() {
    qDebug() << "You clicked the button!!";

//    QVBoxLayout *layout = new QVBoxLayout;
    Client* q_client = new Client(this, cli_argc, cli_argv);
//    layout->addWidget(q_client);
//    setLayout(layout);
//    q_client->exec();
//    q_client->show();
    q_client->start();

//    runApp();
}

void MainWindow::connectUi() {
    QCommandLinkButton *button = this->findChild<QCommandLinkButton*>("kbutton");
    connect(button, &QCommandLinkButton::clicked, this, &MainWindow::buttonClicked);
}

void MainWindow::runApp() {
    QString appString = "python /data/www/kiq/requests/test.py";
    process->start(appString);

    QVector<QString> app_result{};

    qDebug() << "trying to run app";

    connect(process, &QProcess::readyReadStandardOutput, [=] () {
        this->process->setProcessChannelMode(QProcess::MergedChannels);
        QTextStream reader(this->process->readAllStandardOutput());
        do {
            QString s;
            reader.readLineInto(&s);
            qDebug() << s;
        } while (!reader.readLine().isNull());
    });

    for (auto qString: app_result) {
        qDebug() << qString;
    }
}


MainWindow::~MainWindow()
{
    delete ui;
}
