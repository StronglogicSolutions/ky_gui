#include <QDialog>
#include <QComboBox>
#include <QPushButton>
#include <QMessageBox>
#include <QLineEdit>
#include <QTimer>
#include <QLabel>
#include <QString>

class Client : public QDialog
{
    Q_OBJECT

public:
    Client(QWidget *parent = nullptr);
    Client(QWidget *parent, int count, char** arguments);
    void requestNewFortune();
    int start();

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
