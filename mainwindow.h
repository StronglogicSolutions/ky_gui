#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QListWidget>
#include <QListWidgetItem>
#include <client.hpp>
#include <headers/ktextedit.hpp>

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
    void connectUi();
    void runApp();
    int cli_argc;
    char** cli_argv;

private slots:
    void connectClient();
    void updateMessages(int t, const QString& s, StringVec v);
};


#endif // MAINWINDOW_H
