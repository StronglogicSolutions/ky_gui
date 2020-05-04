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
#include <include/argdialog.h>
#include <include/consoledialog.h>
#include <QTableView>
#include <QTimer>

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

struct Event {};

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
    /** Process arguments */
    int cli_argc;
    char** cli_argv;
    /** UI & Messages */
    void connectUi();
    void setConnectScreen(bool visible = true);
    QString parseMessage(const QString& s, StringVec v);
    void updateProcessResult(QString request_id, QString result);
    /** UI Members */
    Ui::MainWindow *ui;
    ArgDialog *arg_ui;
    ConsoleDialog console_ui;
    /** Client member */
    Client* q_client;
    /** Models */
    std::vector<Process> m_processes;
    QList<QString> m_events;
    QStandardItemModel* m_process_model;
    QStandardItemModel* m_event_model;
    /** Misc */
    ConfigJson m_config;
    uint16_t m_consecutive_events;

   private slots:
    /** Receivers */
    void connectClient();
    void updateMessages(int t, const QString& s, StringVec v);
    void handleKey();
};


#endif // MAINWINDOW_H
