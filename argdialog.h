#ifndef ARGDIALOG_H
#define ARGDIALOG_H

#include <QDialog>
#include <QFileDialog>
#include <QPushButton>
#include <QFile>

namespace Ui {
class ArgDialog;
}

class ArgDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ArgDialog(QWidget *parent = nullptr);
    ~ArgDialog();

signals:
    void uploadFile(QByteArray bytes);

private:
    Ui::ArgDialog *ui;
};

#endif // ARGDIALOG_H
