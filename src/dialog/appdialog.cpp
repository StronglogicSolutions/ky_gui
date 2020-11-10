#include "include/ui/appdialog.hpp"
#include "ui_appdialog.h"

bool input_mode{false};

AppDialog::AppDialog(QWidget *parent) :
                                        QDialog(parent),
                                        ui(new Ui::AppDialog)
{
  ui->setupUi(this);
  QObject::connect(ui->close, &QPushButton::clicked, this, [this]() {
    this->close();
  });

  QObject::connect(ui->addApp, &QPushButton::clicked, this, [this]() {
//    emit appRequest(KApplication{.name = "Mugatu"});
    input_mode = !input_mode;
    setInputMode(input_mode);
  });

  QObject::connect(ui->appList, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
    this, [this]() {
      setFields(ui->appList->currentText());
    }
  );
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

void AppDialog::setFields(QString app_name) {
  for (const auto& app : m_applications) {
    if (app_name.compare(app.name) == 0) {
      ui->nameText->setText(app.name);
      ui->maskText->setText(app.mask);
      ui->dataText->setText(app.data);
      ui->pathText->setText(app.path);
    }
  }
}

void AppDialog::setInputMode(bool mode) {
  if (mode) {
    ui->appList->setStyleSheet("QComboBox { color: #33383c }");
    ui->addApp->setStyleSheet(
        "QPushButton { "
        "background-color: #8e00bd; "
        "font: 87 11pt \"Noto Sans\";"
        "color: rgb(0, 0, 0);"
        "font-weight: 700;"
        "padding: 4px;"
        "border-style: outset;"
        "border-width: 2px;"
        "border-radius: 4px;"
        "border-color: #00000f;"
        "min-width: 4em;"
        "padding: 4px;"
        "}"
        );
    ui->nameText->setText("");
    ui->pathText->setText("");
    ui->dataText->setText("");
    ui->maskText->setText("");
  } else {
    setFields(ui->appList->currentText());
    ui->appList->setStyleSheet("QComboBox { color: rgb(131, 148, 150); background-color: #2f535f; }");
    ui->addApp->setStyleSheet(
        "QPushButton { "
        "background-color: rgb(0, 58, 70);"
        "font: 87 11pt \"Noto Sans\";"
        "color: rgb(0, 0, 0);"
        "font-weight: 700;"
        "padding: 4px;"
        "border-style: outset;"
        "border-width: 2px;"
        "border-radius: 4px;"
        "border-color: #00000f;"
        "min-width: 4em;"
        "padding: 4px;"
        "}"
        );
  }

}
