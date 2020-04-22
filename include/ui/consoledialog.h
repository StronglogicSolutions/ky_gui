#ifndef CONSOLEDIALOG_H
#define CONSOLEDIALOG_H

#include <QDialog>

namespace Ui {
class ConsoleDialog;
}

class ConsoleDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConsoleDialog(QWidget *parent = nullptr);
    void updateText(const QString& s);
    ~ConsoleDialog();

private:
    Ui::ConsoleDialog *ui;
};

#endif // CONSOLEDIALOG_H
