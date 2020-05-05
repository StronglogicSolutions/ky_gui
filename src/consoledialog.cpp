#include <include/ui/consoledialog.h>
#include <QPushButton>
#include "ui_consoledialog.h"

ConsoleDialog::ConsoleDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConsoleDialog)
{
    ui->setupUi(this);

    QObject::connect(ui->closeConsole, &QPushButton::clicked, this, [this]() {
        this->close();
    });
}

void ConsoleDialog::updateText(const QString& s) {
    ui->consoleText->append(s);
}

ConsoleDialog::~ConsoleDialog()
{
    delete ui;
}
