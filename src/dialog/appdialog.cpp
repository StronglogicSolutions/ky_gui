#include "include/ui/appdialog.hpp"
#include "ui_appdialog.h"

static bool input_mode{false};

AppDialog::AppDialog(QWidget *parent) :
                                        QDialog(parent),
                                        ui(new Ui::AppDialog)
{
  ui->setupUi(this);
  QObject::connect(ui->close, &QPushButton::clicked, this, [this]() {
    this->close();
  });

  QObject::connect(ui->addApp, &QPushButton::clicked, this, [this]() {
    toggleInputMode();
  });

  QObject::connect(ui->save, &QPushButton::clicked, this, [this]() {
    emit appRequest(readFields(), constants::RequestType::REGISTER);
  });

  QObject::connect(ui->deleteApp, &QPushButton::clicked, this, [this]() {
    emit appRequest(readFields(), constants::RequestType::DELETE);
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
  ui->appList->clear();
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

void AppDialog::toggleInputMode() {
  input_mode = !input_mode;
  if (input_mode) {
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

KApplication AppDialog::readFields() {
  return KApplication{
    .name = ui->nameText->text(),
    .path = ui->pathText->text(),
    .data = ui->dataText->text(),
    .mask = ui->maskText->text()
  };
}

void AppDialog::addApplication(KApplication application) {
  m_applications.push_back(application);
  ui->appList->addItem(application.name);
  toggleInputMode();
}

void AppDialog::removeApplication(KApplication application) {
  ui->appList->removeItem(ui->appList->findText(application.name));
  for (auto it = m_applications.begin(); it != m_applications.end(); it++) {
    if (application.name.compare(it->name) == 0) {
      m_applications.erase(it);
      return;
    }
  }
}
