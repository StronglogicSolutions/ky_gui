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
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDateTimeEdit>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTextEdit>

QT_BEGIN_NAMESPACE

class Ui_ArgDialog
{
public:
    QDialogButtonBox *argCommandButtons;
    QPushButton *addArgument;
    QLabel *argDialogTitle;
    QPushButton *addFile;
    QLabel *label;
    QTableWidget *argList;
    QListWidget *argType;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QTextEdit *argInput;
    QLabel *label_5;
    QDateTimeEdit *dateTime;

    void setupUi(QDialog *ArgDialog)
    {
        if (ArgDialog->objectName().isEmpty())
            ArgDialog->setObjectName(QString::fromUtf8("ArgDialog"));
        ArgDialog->resize(608, 710);
        QIcon icon;
        icon.addFile(QString::fromUtf8("favicon.ico"), QSize(), QIcon::Normal, QIcon::Off);
        ArgDialog->setWindowIcon(icon);
        ArgDialog->setStyleSheet(QString::fromUtf8("background-color: rgb(33, 33, 33);"));
        argCommandButtons = new QDialogButtonBox(ArgDialog);
        argCommandButtons->setObjectName(QString::fromUtf8("argCommandButtons"));
        argCommandButtons->setGeometry(QRect(340, 510, 201, 32));
        argCommandButtons->setLayoutDirection(Qt::LeftToRight);
        argCommandButtons->setAutoFillBackground(false);
        argCommandButtons->setStyleSheet(QString::fromUtf8("background-color: rgb(203, 0, 239);\n"
"color: rgb(16, 16, 16);"));
        argCommandButtons->setOrientation(Qt::Horizontal);
        argCommandButtons->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Save);
        argCommandButtons->setCenterButtons(true);
        addArgument = new QPushButton(ArgDialog);
        addArgument->setObjectName(QString::fromUtf8("addArgument"));
        addArgument->setGeometry(QRect(380, 190, 80, 26));
        addArgument->setStyleSheet(QString::fromUtf8("background-color: rgb(203, 0, 239);\n"
"color: rgb(16, 16, 16);"));
        argDialogTitle = new QLabel(ArgDialog);
        argDialogTitle->setObjectName(QString::fromUtf8("argDialogTitle"));
        argDialogTitle->setGeometry(QRect(210, 10, 231, 51));
        QFont font;
        font.setPointSize(22);
        font.setBold(true);
        font.setWeight(75);
        argDialogTitle->setFont(font);
        argDialogTitle->setStyleSheet(QString::fromUtf8("color: rgb(170, 0, 255);"));
        addFile = new QPushButton(ArgDialog);
        addFile->setObjectName(QString::fromUtf8("addFile"));
        addFile->setGeometry(QRect(190, 220, 80, 26));
        addFile->setStyleSheet(QString::fromUtf8("background-color: rgb(203, 0, 239);\n"
"color: rgb(16, 16, 16);"));
        label = new QLabel(ArgDialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(60, 290, 91, 18));
        argList = new QTableWidget(ArgDialog);
        if (argList->columnCount() < 2)
            argList->setColumnCount(2);
        argList->setObjectName(QString::fromUtf8("argList"));
        argList->setGeometry(QRect(60, 320, 481, 171));
        argList->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"color: rgb(5, 5, 5);\n"
"font: 87 10pt \"Noto Sans\";\n"
"selection-background-color: rgb(255, 0, 174);"));
        argList->setColumnCount(2);
        argList->horizontalHeader()->setCascadingSectionResizes(false);
        argList->horizontalHeader()->setMinimumSectionSize(100);
        argList->horizontalHeader()->setDefaultSectionSize(200);
        argList->verticalHeader()->setMinimumSectionSize(100);
        argList->verticalHeader()->setDefaultSectionSize(100);
        argType = new QListWidget(ArgDialog);
        new QListWidgetItem(argType);
        new QListWidgetItem(argType);
        new QListWidgetItem(argType);
        new QListWidgetItem(argType);
        new QListWidgetItem(argType);
        argType->setObjectName(QString::fromUtf8("argType"));
        argType->setGeometry(QRect(190, 80, 81, 41));
        argType->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"color: rgb(5, 5, 5);\n"
"font: 87 10pt \"Noto Sans\";\n"
"selection-background-color: rgb(255, 0, 174);"));
        label_2 = new QLabel(ArgDialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(130, 80, 58, 18));
        label_3 = new QLabel(ArgDialog);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(120, 130, 41, 18));
        label_4 = new QLabel(ArgDialog);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(90, 220, 71, 20));
        argInput = new QTextEdit(ArgDialog);
        argInput->setObjectName(QString::fromUtf8("argInput"));
        argInput->setGeometry(QRect(190, 130, 281, 51));
        argInput->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"color: rgb(5, 5, 5);\n"
"font: 87 10pt \"Noto Sans\";"));
        label_5 = new QLabel(ArgDialog);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(130, 260, 31, 20));
        dateTime = new QDateTimeEdit(ArgDialog);
        dateTime->setObjectName(QString::fromUtf8("dateTime"));
        dateTime->setGeometry(QRect(200, 260, 194, 27));

        retranslateUi(ArgDialog);
        QObject::connect(argCommandButtons, SIGNAL(accepted()), ArgDialog, SLOT(accept()));
        QObject::connect(argCommandButtons, SIGNAL(rejected()), ArgDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(ArgDialog);
    } // setupUi

    void retranslateUi(QDialog *ArgDialog)
    {
        ArgDialog->setWindowTitle(QCoreApplication::translate("ArgDialog", "Dialog", nullptr));
        addArgument->setText(QCoreApplication::translate("ArgDialog", "Add", nullptr));
        argDialogTitle->setText(QCoreApplication::translate("ArgDialog", "Add Arguments", nullptr));
        addFile->setText(QCoreApplication::translate("ArgDialog", "File", nullptr));
        label->setText(QCoreApplication::translate("ArgDialog", "Items to send", nullptr));

        const bool __sortingEnabled = argType->isSortingEnabled();
        argType->setSortingEnabled(false);
        QListWidgetItem *___qlistwidgetitem = argType->item(0);
        ___qlistwidgetitem->setText(QCoreApplication::translate("ArgDialog", "description", nullptr));
        QListWidgetItem *___qlistwidgetitem1 = argType->item(1);
        ___qlistwidgetitem1->setText(QCoreApplication::translate("ArgDialog", "hashtag", nullptr));
        QListWidgetItem *___qlistwidgetitem2 = argType->item(2);
        ___qlistwidgetitem2->setText(QCoreApplication::translate("ArgDialog", "link/bio", nullptr));
        QListWidgetItem *___qlistwidgetitem3 = argType->item(3);
        ___qlistwidgetitem3->setText(QCoreApplication::translate("ArgDialog", "promote/share", nullptr));
        QListWidgetItem *___qlistwidgetitem4 = argType->item(4);
        ___qlistwidgetitem4->setText(QCoreApplication::translate("ArgDialog", "requested by", nullptr));
        argType->setSortingEnabled(__sortingEnabled);

        label_2->setText(QCoreApplication::translate("ArgDialog", "Type", nullptr));
        label_3->setText(QCoreApplication::translate("ArgDialog", "Input", nullptr));
        label_4->setText(QCoreApplication::translate("ArgDialog", "Attachment", nullptr));
        label_5->setText(QCoreApplication::translate("ArgDialog", "Time", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ArgDialog: public Ui_ArgDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ARGDIALOG_H
