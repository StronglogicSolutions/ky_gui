#include "documentdialog.h"
#include "ui_documentdialog.h"

DocumentDialog::DocumentDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::DocumentDialog)
{
  ui->setupUi(this);
}

DocumentDialog::~DocumentDialog()
{
  delete ui;
}
