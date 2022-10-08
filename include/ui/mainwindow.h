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
#include <include/ui/researchwindow.hpp>
#include <include/ui/argdialog.h>
#include <include/ui/messagedialog.hpp>
#include "include/ui/postdialog.hpp"
#include <include/ui/appdialog.hpp>
#include <include/ui/scheduledialog.hpp>

#include "ui_mainwindow.h"

#include <headers/kiq_types.hpp>
#include <include/client/client.hpp>

static const uint8_t  CLIENT_EXIT{9};
static const int      DEFAULT_TIMEOUT{1000};
static const char*    stylesheet_path{"style/style.css"};
static const QString  nonce_event = "";

const QString KYGUI_DEFAULT_THEME{
"border-color: rgb(0, 0, 0);"
"background-color: rgb(0, 43, 54);"
"QPushButton::hover:"
"{"
"   opacity: 1;"
"}"
};

const QString KYGUI_BLACK_THEME{
"border-color: rgb(0, 0, 0);"
"background-color: rgb(30, 30, 30);"
"QPushButton::hover:"
"{"
"   opacity: 1;"
"}"
};

const QString KYGUI_BLUE_THEME{
"border-color: rgb(0, 0, 0);"
"background-color: rgb(4, 0, 29);"
"QPushButton::hover:"
"{"
"   opacity: 1;"
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

static const QString KYGUI_FETCH_BUTTON_THEME{
"QPushButton{"
"font: 87 18pt \"Noto Sans\";"
"color: rgb(0, 0, 0);"
"background-color: rgb(255, 71, 0);"
"font-weight: 700;"
"padding: 4px;"
"border-style: outset;"
"border-width: 2px;"
"border-radius: 6px;"
"border-color: #00000f;"
"min-width: 1em;"
"min-height: 1em;"
"padding: 4px;"
"opacity: 1;"
"}"
"QPushButton:hover { background-color: #ff5607a3;}"
"QPushButton:pressed { opacity: 0.2; }"
};

static const QString KYGUI_CONNECT_BUTTON_THEME{
"QPushButton{"
"font: 87 18pt \"Noto Sans\";"
"color: rgb(0, 0, 0);"
"background-color: rgb(2, 180, 43);"
"font-weight: 700;"
"padding: 4px;"
"border-style: outset;"
"border-width: 2px;"
"border-radius: 6px;"
"border-color: #00000f;"
"min-width: 1em;"
"min-height: 1em;"
"padding: 4px;"
"opacity: 1;"
"}"
"QPushButton:hover { background-color: #33ff00;}"
"QPushButton:pressed { opacity: 0.2; }"
};


[[ maybe_unused ]]
static QString GetFetchButtonTheme()
{
  return KYGUI_FETCH_BUTTON_THEME;
}

[[ maybe_unused ]]
static QString GetConnectButtonTheme()
{
  return KYGUI_CONNECT_BUTTON_THEME;
}

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

using PlatformMap = QHash<QString, QList<QString>>;
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

    void SetPlatformOptions(const QString& platform, const QList<QString>& options);
    void set_connected(bool connected = true);

    class Controller {
     public:
      explicit Controller(MainWindow* window);
      void handleCommands(const StringVec& commands, const QString& default_app);
      void handleMessage(const QString& message, const StringVec& v);
      QString handleEventMessage(const QString& message, const StringVec& v = {});

     private:
      QString parseMessage(const QString& message, const StringVec& v);
      void updateProcessResult(const QString& id, const QString& result, const bool error);
      MainWindow* window;
    };

   protected:
    virtual void keyPressEvent(QKeyEvent* e) override;

   private:
    /** UI & Messages */
    void connectUi();
    void reconnect();
    void to_console(const QString& msg, const QStringRef event_msg = &nonce_event);
    void setConnectScreen(bool visible = true);
    void UpdateIPCOptions();
    QString parseTaskInfo(StringVec v);

    /** Process arguments */
    int                   cli_argc;
    char**                cli_argv;
    /** UI Members */
    Controller            m_controller;
    Ui::MainWindow*       ui;
    ArgDialog*            arg_ui;
    AppDialog             app_ui;
    PostDialog            posts_ui;
    ScheduleDialog        schedule_ui;
    MessageDialog         message_ui;
    DocumentWindow        doc_window;
    ResearchWindow        research_window;

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
    int                   m_client_time_remaining;
    int                   m_timeouts{0};
    QTimer                m_progress_timer;
    QTimer                m_ping_timer;
    PlatformMap           m_platform_map;

   private slots:
    /** Receivers */
    void connectClient(bool reconnect = false);
    void onMessageReceived(int t, const QString& s, StringVec v);
    void startTimers();
    void exit();
};


#endif // MAINWINDOW_H
