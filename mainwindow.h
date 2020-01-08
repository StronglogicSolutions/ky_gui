#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QList>
#include <QListView>
#include <QListWidgetItem>
#include <client.hpp>
#include <headers/ktextedit.hpp>
#include <argdialog.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(int argc = 0, char** argv = nullptr, QWidget* parent = nullptr);
    ~MainWindow();
private:
    Ui::MainWindow *ui;
    ArgDialog *arg_ui;
    void connectUi();
    void runApp();
    void updateProcessResult(int mask);
    int cli_argc;
    char** cli_argv;
    Client* q_client;
    QList<QString> m_events;

private slots:
    void connectClient();
    void updateMessages(int t, const QString& s, StringVec v);
};


#endif // MAINWINDOW_H
