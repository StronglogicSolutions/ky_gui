/********************************************************************************
** Form generated from reading UI file 'argdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.13.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ARGDIALOG_H
#define UI_ARGDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListView>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_ArgDialog
{
public:
    QDialogButtonBox *argButtonBox;
    QListView *argList;
    QPushButton *addArgument;
    QLineEdit *argInput;
    QLabel *argDialogTitle;
    QPushButton *addFile;
    QLabel *label;

    void setupUi(QDialog *ArgDialog)
    {
        if (ArgDialog->objectName().isEmpty())
            ArgDialog->setObjectName(QString::fromUtf8("ArgDialog"));
        ArgDialog->resize(490, 314);
        argButtonBox = new QDialogButtonBox(ArgDialog);
        argButtonBox->setObjectName(QString::fromUtf8("argButtonBox"));
        argButtonBox->setGeometry(QRect(80, 260, 341, 32));
        argButtonBox->setStyleSheet(QString::fromUtf8("background-color: rgb(203, 0, 239);\n"
"color: rgb(16, 16, 16);"));
        argButtonBox->setOrientation(Qt::Horizontal);
        argButtonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        argList = new QListView(ArgDialog);
        argList->setObjectName(QString::fromUtf8("argList"));
        argList->setGeometry(QRect(60, 140, 361, 91));
        argList->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"color: rgb(5, 5, 5);\n"
"font: 87 10pt \"Noto Sans\";\n"
"selection-background-color: rgb(255, 0, 174);"));
        addArgument = new QPushButton(ArgDialog);
        addArgument->setObjectName(QString::fromUtf8("addArgument"));
        addArgument->setGeometry(QRect(340, 70, 80, 26));
        addArgument->setStyleSheet(QString::fromUtf8("background-color: rgb(203, 0, 239);\n"
"color: rgb(16, 16, 16);"));
        argInput = new QLineEdit(ArgDialog);
        argInput->setObjectName(QString::fromUtf8("argInput"));
        argInput->setGeometry(QRect(80, 70, 231, 26));
        argInput->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"color: rgb(5, 5, 5);\n"
"font: 87 10pt \"Noto Sans\";"));
        argDialogTitle = new QLabel(ArgDialog);
        argDialogTitle->setObjectName(QString::fromUtf8("argDialogTitle"));
        argDialogTitle->setGeometry(QRect(130, 0, 231, 51));
        QFont font;
        font.setPointSize(22);
        font.setBold(true);
        font.setWeight(75);
        argDialogTitle->setFont(font);
        argDialogTitle->setStyleSheet(QString::fromUtf8("color: rgb(170, 0, 255);"));
        addFile = new QPushButton(ArgDialog);
        addFile->setObjectName(QString::fromUtf8("addFile"));
        addFile->setGeometry(QRect(340, 100, 80, 26));
        addFile->setStyleSheet(QString::fromUtf8("background-color: rgb(203, 0, 239);\n"
"color: rgb(16, 16, 16);"));
        label = new QLabel(ArgDialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(70, 120, 81, 18));

        retranslateUi(ArgDialog);
        QObject::connect(argButtonBox, SIGNAL(accepted()), ArgDialog, SLOT(accept()));
        QObject::connect(argButtonBox, SIGNAL(rejected()), ArgDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(ArgDialog);
    } // setupUi

    void retranslateUi(QDialog *ArgDialog)
    {
        ArgDialog->setWindowTitle(QCoreApplication::translate("ArgDialog", "Dialog", nullptr));
        addArgument->setText(QCoreApplication::translate("ArgDialog", "Add", nullptr));
        argDialogTitle->setText(QCoreApplication::translate("ArgDialog", "Add Arguments", nullptr));
        addFile->setText(QCoreApplication::translate("ArgDialog", "File", nullptr));
        label->setText(QCoreApplication::translate("ArgDialog", "Output", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ArgDialog: public Ui_ArgDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ARGDIALOG_H
