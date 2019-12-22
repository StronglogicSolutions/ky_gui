#include <QDialog>
#include <QComboBox>
#include <QPushButton>
#include <QMessageBox>
#include <QLineEdit>
#include <QTimer>
#include <QLabel>
#include <QString>
#include <QThread>
#include <thread>

class Client : public QDialog
{
    Q_OBJECT
    QThread workerThread;

public:

    class MessageHandler {
    public:
        MessageHandler(std::function<void()> cb) : m_cb(cb) {}

        void operator()() { m_cb(); }

    private:
        std::function<void()> m_cb;
    };

    Client(QWidget *parent = nullptr);
    Client(QWidget *parent, int count, char** arguments);
    ~Client();
    void start();
    void closeConnection();
    MessageHandler createMessageHandler(std::function<void()> cb);

public slots:
    void sendMessage(const QString& s);

signals:
    void messageReceived(QString s);

private:
    void handleMessages();
    int argc;
    char** argv;
    int m_client_socket_fd;
};
