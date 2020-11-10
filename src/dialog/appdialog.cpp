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

  QObject::connect(ui->addApp, &QPushButton::clicked, this, [this]() {
    emit appRequest(KApplication{.name = "Mugatu"});
  });

  QObject::connect(ui->appList, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
   this, [this]() {
     QString app_name = ui->appList->currentText();
     for (const auto& app : m_applications) {
       if (app_name.compare(app.name) == 0) {
         ui->nameText->setText(app.name);
         ui->maskText->setText(app.mask);
         ui->dataText->setText(app.data);
         ui->pathText->setText(app.path);
       }
     }
   });
}

AppDialog::~AppDialog()
{
  delete ui;
}

void AppDialog::showEvent(QShowEvent *) {
  for (const auto& app : m_applications) {
    ui->appList->addItem(app.name);
  }
}

