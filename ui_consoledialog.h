/********************************************************************************
** Form generated from reading UI file 'consoledialog.ui'
**
** Created by: Qt User Interface Compiler version 5.13.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONSOLEDIALOG_H
#define UI_CONSOLEDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>

QT_BEGIN_NAMESPACE

class Ui_ConsoleDialog
{
public:
    QTextEdit *consoleText;
    QLabel *consoleTitle;
    QPushButton *closeConsole;

    void setupUi(QDialog *ConsoleDialog)
    {
        if (ConsoleDialog->objectName().isEmpty())
            ConsoleDialog->setObjectName(QString::fromUtf8("ConsoleDialog"));
        ConsoleDialog->resize(721, 836);
        consoleText = new QTextEdit(ConsoleDialog);
        consoleText->setObjectName(QString::fromUtf8("consoleText"));
        consoleText->setGeometry(QRect(30, 60, 661, 721));
        consoleText->setReadOnly(false);
        consoleTitle = new QLabel(ConsoleDialog);
        consoleTitle->setObjectName(QString::fromUtf8("consoleTitle"));
        consoleTitle->setGeometry(QRect(300, 20, 51, 18));
        closeConsole = new QPushButton(ConsoleDialog);
        closeConsole->setObjectName(QString::fromUtf8("closeConsole"));
        closeConsole->setGeometry(QRect(600, 800, 80, 26));

        retranslateUi(ConsoleDialog);

        QMetaObject::connectSlotsByName(ConsoleDialog);
    } // setupUi

    void retranslateUi(QDialog *ConsoleDialog)
    {
        ConsoleDialog->setWindowTitle(QCoreApplication::translate("ConsoleDialog", "Dialog", nullptr));
        consoleTitle->setText(QCoreApplication::translate("ConsoleDialog", "Console", nullptr));
        closeConsole->setText(QCoreApplication::translate("ConsoleDialog", "Close", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ConsoleDialog: public Ui_ConsoleDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONSOLEDIALOG_H
