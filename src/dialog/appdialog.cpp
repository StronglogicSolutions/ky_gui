#include "include/ui/appdialog.hpp"
#include "ui_appdialog.h"

AppDialog::AppDialog(QWidget *parent) :
                                        QDialog(parent),
                                        ui(new Ui::AppDialog)
{
  ui->setupUi(this);
  QObject::connect(ui->close, &QPushButton::clicked, this, [this]() {
    this->close();
  });
}

AppDialog::~AppDialog()
{
  delete ui;
}
