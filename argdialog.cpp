#include "argdialog.h"
#include "ui_argdialog.h"
#include <QDebug>
#include <QIODevice>
#include <vector>

ArgDialog::ArgDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ArgDialog)
{
    ui->setupUi(this);
    QObject::connect(ui->addFile, &QPushButton::clicked, this, [this]() {
        auto file_name = QFileDialog::getOpenFileName(this,
                                                tr("Open File"), "~", tr("Image Files (*.png *.jpg *.bmp)"));
        qDebug() << "Selected file:" << file_name;
        if (file_name.size() > 0) {
            QFile file(file_name);
            std::vector<char> byte_array{};
            if (file.open(QIODevice::ReadOnly)) {
                QByteArray bytes = file.readAll();
                emit ArgDialog::uploadFile(bytes);
            }
        }
    });
}

ArgDialog::~ArgDialog()
{
    delete ui;
}
