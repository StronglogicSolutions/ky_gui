#ifndef ARGDIALOG_H
#define ARGDIALOG_H

#include <QDialog>

namespace Ui {
class ArgDialog;
}

class ArgDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ArgDialog(QWidget *parent = nullptr);
    ~ArgDialog();

private:
    Ui::ArgDialog *ui;
};

#endif // ARGDIALOG_H
