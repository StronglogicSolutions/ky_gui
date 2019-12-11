#include <QDialog>
#include <QComboBox>
#include <QPushButton>
#include <QMessageBox>
#include <QLineEdit>
#include <QTimer>
#include <QLabel>

class Client : public QDialog
{
    Q_OBJECT

public:
    Client(QWidget *parent = nullptr);
    Client(QWidget *parent, int count, char** arguments);
    void requestNewFortune();
    int start();

private:
    int argc;
    char** argv;
};
