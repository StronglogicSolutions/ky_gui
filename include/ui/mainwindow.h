#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QList>
#include <QListView>
#include <QListWidgetItem>
#include <QMainWindow>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QTableView>
#include <QTimer>
#include <QElapsedTimer>
#include <QDateTime>
#include <QLayout>
#include <QScrollBar>
#include <QTextEdit>
#include <QTextStream>

#include <include/ui/documentwindow.hpp>
#include <include/ui/argdialog.h>
#include <include/ui/messagedialog.hpp>
#include <include/ui/appdialog.hpp>
#include <include/ui/scheduledialog.hpp>

#include "ui_mainwindow.h"

#include <headers/kiq_types.hpp>
#include <include/client/client.hpp>

static const uint8_t  CLIENT_EXIT{9};
static const uint32_t DEFAULT_TIMEOUT{1000};
static const char* stylesheet_path{"style/style.css"};

const QString KYGUI_DEFAULT_THEME{
"border-color: rgb(0, 0, 0);"
"background-color: rgb(0, 43, 54);"
"QPushButton::hover:"
"{"
"   opacity: 0.7;"
"}"
};

const QString KYGUI_BLACK_THEME{
"border-color: rgb(0, 0, 0);"
"background-color: rgb(30, 30, 30);"
"QPushButton::hover:"
"{"
"   opacity: 0.7;"
"}"
};

const QString KYGUI_BLUE_THEME{
"border-color: rgb(0, 0, 0);"
"background-color: rgb(4, 0, 29);"
"QPushButton::hover:"
"{"
"   opacity: 0.7;"
"}"
};

const QString KYGUI_DEFAULT_LIST_THEME{
"font: 87 11pt \"Noto Sans\";"
"font-weight: 700;"
"background-color: rgb(29, 51, 59);"
"color: rgb(223, 252, 255);"
"padding: 4px;"
"item { border-bottom: 1px solid black; padding: 16px;} "
};

const QString KYGUI_BLACK_LIST_THEME{
"font: 87 11pt \"Noto Sans\";"
"font-weight: 700;"
"background-color: rgb(75, 75, 75);"
"color: rgb(223, 252, 255);"
"padding: 4px;"
"item { border-bottom: 1px solid black; padding: 16px;} "
};

const QString KYGUI_BLUE_LIST_THEME{
"font: 87 11pt \"Noto Sans\";"
"font-weight: 700;"
"background-color: rgb(0, 6, 120);"
"color: rgb(223, 252, 255);"
"padding: 4px;"
"item { border-bottom: 1px solid black; padding: 16px;} "
};

const auto get_stylesheet = []() -> QString
{
    QFile file{stylesheet_path};
    file.open(QFile::ReadOnly |
              QFile::Text);

    return QString{file.readAll()};
};

namespace ProcessState {
    static constexpr int READY = 1;
    static constexpr int PENDING = 2;
    static constexpr int SUCCEEDED = 3;
    static constexpr int FAILED = 4;
}

struct KListViewsStates {
  bool eventViewBottom;
  bool processViewBottom;
  bool historyViewBottom;
};

const QString ProcessNames[4] = { "READY", "PENDING", "SUCCEEDED", "FAILED" };

struct Process {
    QString name;
    int state;
    QString start;
    QString end;
    QString id;
    QString result = "";
    QString error = "";

    bool operator==(const Process &other) const {
        return name == other.name && state == other.state;
    }
};

namespace utils {
QString getTime();
QString timestampPrefix();
QStandardItem* createProcessListItem(Process process);
QStandardItem* createEventListItem(QString event);
} // namespace utils

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(int argc = 0, char** argv = nullptr, QWidget* parent = nullptr);
    ~MainWindow();

    class Controller {
     public:
      void init(MainWindow* window);
      void handleCommands(const StringVec& commands, const QString& default_app);
      void handleMessage(const QString& message, const StringVec& v);
      QString handleEventMessage(const QString& message, const StringVec& v);

     private:
      QString parseMessage(const QString& message, const StringVec& v);
      void updateProcessResult(const QString& id, const QString& result, const bool error);
      MainWindow* window;
    };

   protected:
    void keyPressEvent(QKeyEvent *e);

   private:
    /** UI & Messages */
    void connectUi();
    void setConnectScreen(bool visible = true);
    QString parseTaskInfo(StringVec v);

    /** Process arguments */
    int                   cli_argc;
    char**                cli_argv;
    /** UI Members */
    Controller            m_controller;
    Ui::MainWindow*       ui;
    ArgDialog*            arg_ui;
    AppDialog             app_ui;
    ScheduleDialog        schedule_ui;
    MessageDialog         message_ui;
    DocumentWindow        doc_window;

    /** Client member */
    Client*               q_client;
    QElapsedTimer         m_pong_timer;
    /** Models */
    std::vector<Process>  m_processes;
    QList<QString>        m_events;
    QStandardItemModel*   m_process_model;
    QStandardItemModel*   m_event_model;
    KListViewsStates      m_view_states;
    /** Misc */
    QJsonObject           m_config;
    uint16_t              m_consecutive_events;
    quint64               m_client_time_remaining;
    QTimer                m_progress_timer;

   private slots:
    /** Receivers */
    void connectClient();
    void onMessageReceived(int t, const QString& s, StringVec v);
};


#endif // MAINWINDOW_H
