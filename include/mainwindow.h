#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QList>
#include <QListView>
#include <QListWidgetItem>
#include <QStandardItemModel>
#include <QStandardItem>
#include <include/client.hpp>
#include <headers/ktextedit.hpp>
#include <include/argdialog.h>
#include <include/consoledialog.h>
#include <QTableView>

namespace ProcessState {
    static constexpr int READY = 1;
    static constexpr int PENDING = 2;
    static constexpr int SUCCEEDED = 3;
    static constexpr int FAILED = 4;
}

const QString ProcessNames[4] = { "READY", "PENDING", "SUCCEEDED", "FAILED" };

struct Process {
    QString name;
    int state;
    QString start;
    QString end;
    QString id;
    QString result;

    bool operator==(const Process &other) const {
        return name == other.name && state == other.state;
    }
};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(int argc = 0, char** argv = nullptr, QWidget* parent = nullptr);
    virtual void keyPressEvent(QKeyEvent* e);
    ~MainWindow();
private:
    Ui::MainWindow *ui;
    ArgDialog *arg_ui;
    void connectUi();
    void runApp();
    void updateProcessResult(QString request_id, QString result);
    QString parseMessage(const QString& s, StringVec v);
    int cli_argc;
    char** cli_argv;
    Client* q_client;
    std::vector<Process> m_processes;
    QStandardItemModel* m_process_model;
    QList<QString> m_events;
    ConsoleDialog m_console;

private slots:
    void connectClient();
    void updateMessages(int t, const QString& s, StringVec v);
    void handleInputEnterKey();
};


#endif // MAINWINDOW_H
