#include "argdialog.h"
#include "ui_argdialog.h"

ArgDialog::ArgDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ArgDialog)
{
    ui->setupUi(this);
}

ArgDialog::~ArgDialog()
{
    delete ui;
}
