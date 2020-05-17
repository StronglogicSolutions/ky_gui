#include "include/ui/messagedialog.hpp"
#include "ui_messagedialog.h"

MessageDialog::MessageDialog(QWidget *parent) :
                                                QDialog(parent),
                                                ui(new Ui::MessageDialog) {
  ui->setupUi(this);
  QObject::connect(ui->close, &QPushButton::clicked, this, [this]() {
    this->close();
  });
}

void MessageDialog::append(QString message) {
  ui->messages->append(message);
}

MessageDialog::~MessageDialog() {
  delete ui;
}
