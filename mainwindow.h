#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>

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
    void updateMessages(const QString& s);
};


#endif // MAINWINDOW_H
