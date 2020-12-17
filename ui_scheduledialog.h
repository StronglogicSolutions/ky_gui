/********************************************************************************
** Form generated from reading UI file 'scheduledialog.ui'
**
** Created by: Qt User Interface Compiler version 5.13.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SCHEDULEDIALOG_H
#define UI_SCHEDULEDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDateTimeEdit>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_ScheduleDialog
{
public:
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_12;
    QLabel *appsLabel;
    QComboBox *taskList;
    QPushButton *fetchSchedule;
    QHBoxLayout *horizontalLayout_4;
    QLabel *nameLabel;
    QLineEdit *appText;
    QHBoxLayout *horizontalLayout_10;
    QLabel *maskLabel_4;
    QLineEdit *flagsText;
    QHBoxLayout *horizontalLayout_3;
    QLabel *pathLabel;
    QLineEdit *timeText;
    QDateTimeEdit *dateTime;
    QHBoxLayout *horizontalLayout_9;
    QLabel *dataLabel;
    QComboBox *completed;
    QLabel *maskLabel;
    QComboBox *recurring;
    QLabel *maskLabel_3;
    QCheckBox *notifyCheck;
    QHBoxLayout *horizontalLayout_8;
    QLabel *maskLabel_2;
    QLineEdit *runtimeText;
    QHBoxLayout *horizontalLayout_11;
    QLabel *maskLabel_5;
    QLineEdit *filesText;
    QTableWidget *paramTable;
    QHBoxLayout *horizontalLayout_7;
    QPushButton *deleteTask;
    QPushButton *saveTask;
    QPushButton *close;

    void setupUi(QDialog *ScheduleDialog)
    {
        if (ScheduleDialog->objectName().isEmpty())
            ScheduleDialog->setObjectName(QString::fromUtf8("ScheduleDialog"));
        ScheduleDialog->resize(970, 864);
        ScheduleDialog->setStyleSheet(QString::fromUtf8("background-color: rgb(36, 41, 46);\n"
"color: rgb(161, 164, 167);"));
        gridLayout = new QGridLayout(ScheduleDialog);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(36);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(16, 16, 16, 16);
        horizontalLayout_12 = new QHBoxLayout();
        horizontalLayout_12->setObjectName(QString::fromUtf8("horizontalLayout_12"));
        horizontalLayout_12->setContentsMargins(0, -1, -1, -1);
        appsLabel = new QLabel(ScheduleDialog);
        appsLabel->setObjectName(QString::fromUtf8("appsLabel"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(2);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(appsLabel->sizePolicy().hasHeightForWidth());
        appsLabel->setSizePolicy(sizePolicy);
        appsLabel->setMinimumSize(QSize(196, 0));
        appsLabel->setMaximumSize(QSize(196, 16777215));
        appsLabel->setStyleSheet(QString::fromUtf8("font: 75 11pt \"Noto Sans\";\n"
"color: rgb(131, 148, 150);\n"
"font-weight: 700;"));
        appsLabel->setAlignment(Qt::AlignCenter);

        horizontalLayout_12->addWidget(appsLabel);

        taskList = new QComboBox(ScheduleDialog);
        taskList->setObjectName(QString::fromUtf8("taskList"));

        horizontalLayout_12->addWidget(taskList);

        fetchSchedule = new QPushButton(ScheduleDialog);
        fetchSchedule->setObjectName(QString::fromUtf8("fetchSchedule"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(fetchSchedule->sizePolicy().hasHeightForWidth());
        fetchSchedule->setSizePolicy(sizePolicy1);
        fetchSchedule->setMinimumSize(QSize(100, 25));
        fetchSchedule->setMaximumSize(QSize(210, 16777215));
        fetchSchedule->setLayoutDirection(Qt::LeftToRight);
        fetchSchedule->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 85, 0);\n"
"font: 87 11pt \"Noto Sans\";\n"
"color: rgb(0, 0, 0);\n"
"font-weight: 700;\n"
"padding: 4px;\n"
"border-style: outset;\n"
"border-width: 2px;\n"
"border-radius: 4px;\n"
"border-color: #00000f;\n"
"min-width: 4em;\n"
"padding: 4px;"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/icons/refresh.png"), QSize(), QIcon::Normal, QIcon::Off);
        fetchSchedule->setIcon(icon);

        horizontalLayout_12->addWidget(fetchSchedule);


        verticalLayout->addLayout(horizontalLayout_12);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        nameLabel = new QLabel(ScheduleDialog);
        nameLabel->setObjectName(QString::fromUtf8("nameLabel"));
        sizePolicy.setHeightForWidth(nameLabel->sizePolicy().hasHeightForWidth());
        nameLabel->setSizePolicy(sizePolicy);
        nameLabel->setMinimumSize(QSize(196, 0));
        nameLabel->setMaximumSize(QSize(196, 16777215));
        nameLabel->setStyleSheet(QString::fromUtf8("font: 75 11pt \"Noto Sans\";\n"
"color: rgb(131, 148, 150);\n"
"font-weight: 700;"));
        nameLabel->setAlignment(Qt::AlignCenter);

        horizontalLayout_4->addWidget(nameLabel);

        appText = new QLineEdit(ScheduleDialog);
        appText->setObjectName(QString::fromUtf8("appText"));

        horizontalLayout_4->addWidget(appText);


        verticalLayout->addLayout(horizontalLayout_4);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setObjectName(QString::fromUtf8("horizontalLayout_10"));
        maskLabel_4 = new QLabel(ScheduleDialog);
        maskLabel_4->setObjectName(QString::fromUtf8("maskLabel_4"));
        sizePolicy.setHeightForWidth(maskLabel_4->sizePolicy().hasHeightForWidth());
        maskLabel_4->setSizePolicy(sizePolicy);
        maskLabel_4->setMinimumSize(QSize(196, 0));
        maskLabel_4->setMaximumSize(QSize(196, 16777215));
        maskLabel_4->setStyleSheet(QString::fromUtf8("font: 75 11pt \"Noto Sans\";\n"
"color: rgb(131, 148, 150);\n"
"font-weight: 700;"));
        maskLabel_4->setAlignment(Qt::AlignCenter);

        horizontalLayout_10->addWidget(maskLabel_4);

        flagsText = new QLineEdit(ScheduleDialog);
        flagsText->setObjectName(QString::fromUtf8("flagsText"));

        horizontalLayout_10->addWidget(flagsText);


        verticalLayout->addLayout(horizontalLayout_10);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        pathLabel = new QLabel(ScheduleDialog);
        pathLabel->setObjectName(QString::fromUtf8("pathLabel"));
        sizePolicy.setHeightForWidth(pathLabel->sizePolicy().hasHeightForWidth());
        pathLabel->setSizePolicy(sizePolicy);
        pathLabel->setMinimumSize(QSize(196, 0));
        pathLabel->setMaximumSize(QSize(196, 16777215));
        pathLabel->setStyleSheet(QString::fromUtf8("font: 75 11pt \"Noto Sans\";\n"
"color: rgb(131, 148, 150);\n"
"font-weight: 700;"));
        pathLabel->setAlignment(Qt::AlignCenter);

        horizontalLayout_3->addWidget(pathLabel);

        timeText = new QLineEdit(ScheduleDialog);
        timeText->setObjectName(QString::fromUtf8("timeText"));
        timeText->setMaximumSize(QSize(0, 0));

        horizontalLayout_3->addWidget(timeText);

        dateTime = new QDateTimeEdit(ScheduleDialog);
        dateTime->setObjectName(QString::fromUtf8("dateTime"));

        horizontalLayout_3->addWidget(dateTime);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        horizontalLayout_9->setContentsMargins(-1, -1, 18, -1);
        dataLabel = new QLabel(ScheduleDialog);
        dataLabel->setObjectName(QString::fromUtf8("dataLabel"));
        sizePolicy.setHeightForWidth(dataLabel->sizePolicy().hasHeightForWidth());
        dataLabel->setSizePolicy(sizePolicy);
        dataLabel->setMinimumSize(QSize(196, 0));
        dataLabel->setMaximumSize(QSize(196, 16777215));
        dataLabel->setStyleSheet(QString::fromUtf8("font: 75 11pt \"Noto Sans\";\n"
"color: rgb(131, 148, 150);\n"
"font-weight: 700;"));
        dataLabel->setAlignment(Qt::AlignCenter);

        horizontalLayout_9->addWidget(dataLabel);

        completed = new QComboBox(ScheduleDialog);
        completed->addItem(QString());
        completed->addItem(QString());
        completed->addItem(QString());
        completed->addItem(QString());
        completed->setObjectName(QString::fromUtf8("completed"));

        horizontalLayout_9->addWidget(completed);

        maskLabel = new QLabel(ScheduleDialog);
        maskLabel->setObjectName(QString::fromUtf8("maskLabel"));
        sizePolicy.setHeightForWidth(maskLabel->sizePolicy().hasHeightForWidth());
        maskLabel->setSizePolicy(sizePolicy);
        maskLabel->setMinimumSize(QSize(196, 0));
        maskLabel->setMaximumSize(QSize(196, 16777215));
        maskLabel->setStyleSheet(QString::fromUtf8("font: 75 11pt \"Noto Sans\";\n"
"color: rgb(131, 148, 150);\n"
"font-weight: 700;"));
        maskLabel->setAlignment(Qt::AlignCenter);

        horizontalLayout_9->addWidget(maskLabel);

        recurring = new QComboBox(ScheduleDialog);
        recurring->addItem(QString());
        recurring->addItem(QString());
        recurring->addItem(QString());
        recurring->addItem(QString());
        recurring->addItem(QString());
        recurring->addItem(QString());
        recurring->setObjectName(QString::fromUtf8("recurring"));

        horizontalLayout_9->addWidget(recurring);

        maskLabel_3 = new QLabel(ScheduleDialog);
        maskLabel_3->setObjectName(QString::fromUtf8("maskLabel_3"));
        sizePolicy.setHeightForWidth(maskLabel_3->sizePolicy().hasHeightForWidth());
        maskLabel_3->setSizePolicy(sizePolicy);
        maskLabel_3->setMinimumSize(QSize(196, 0));
        maskLabel_3->setMaximumSize(QSize(196, 16777215));
        maskLabel_3->setStyleSheet(QString::fromUtf8("font: 75 11pt \"Noto Sans\";\n"
"color: rgb(131, 148, 150);\n"
"font-weight: 700;"));
        maskLabel_3->setAlignment(Qt::AlignCenter);

        horizontalLayout_9->addWidget(maskLabel_3);

        notifyCheck = new QCheckBox(ScheduleDialog);
        notifyCheck->setObjectName(QString::fromUtf8("notifyCheck"));
        notifyCheck->setStyleSheet(QString::fromUtf8("border: 3px solid #5A5A5A;\n"
"border: 3px solid red;\n"
"border: 3px solid rgb(255, 120, 100);\n"
"border: 3px solid rgba(255,120,100, 50);"));

        horizontalLayout_9->addWidget(notifyCheck);

        horizontalLayout_9->setStretch(0, 1);
        horizontalLayout_9->setStretch(1, 3);
        horizontalLayout_9->setStretch(2, 1);
        horizontalLayout_9->setStretch(3, 3);
        horizontalLayout_9->setStretch(4, 1);

        verticalLayout->addLayout(horizontalLayout_9);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        maskLabel_2 = new QLabel(ScheduleDialog);
        maskLabel_2->setObjectName(QString::fromUtf8("maskLabel_2"));
        sizePolicy.setHeightForWidth(maskLabel_2->sizePolicy().hasHeightForWidth());
        maskLabel_2->setSizePolicy(sizePolicy);
        maskLabel_2->setMinimumSize(QSize(196, 0));
        maskLabel_2->setMaximumSize(QSize(196, 16777215));
        maskLabel_2->setStyleSheet(QString::fromUtf8("font: 75 11pt \"Noto Sans\";\n"
"color: rgb(131, 148, 150);\n"
"font-weight: 700;"));
        maskLabel_2->setAlignment(Qt::AlignCenter);

        horizontalLayout_8->addWidget(maskLabel_2);

        runtimeText = new QLineEdit(ScheduleDialog);
        runtimeText->setObjectName(QString::fromUtf8("runtimeText"));

        horizontalLayout_8->addWidget(runtimeText);


        verticalLayout->addLayout(horizontalLayout_8);

        horizontalLayout_11 = new QHBoxLayout();
        horizontalLayout_11->setObjectName(QString::fromUtf8("horizontalLayout_11"));
        maskLabel_5 = new QLabel(ScheduleDialog);
        maskLabel_5->setObjectName(QString::fromUtf8("maskLabel_5"));
        sizePolicy.setHeightForWidth(maskLabel_5->sizePolicy().hasHeightForWidth());
        maskLabel_5->setSizePolicy(sizePolicy);
        maskLabel_5->setMinimumSize(QSize(196, 0));
        maskLabel_5->setMaximumSize(QSize(196, 16777215));
        maskLabel_5->setStyleSheet(QString::fromUtf8("font: 75 11pt \"Noto Sans\";\n"
"color: rgb(131, 148, 150);\n"
"font-weight: 700;"));
        maskLabel_5->setAlignment(Qt::AlignCenter);

        horizontalLayout_11->addWidget(maskLabel_5);

        filesText = new QLineEdit(ScheduleDialog);
        filesText->setObjectName(QString::fromUtf8("filesText"));

        horizontalLayout_11->addWidget(filesText);


        verticalLayout->addLayout(horizontalLayout_11);

        paramTable = new QTableWidget(ScheduleDialog);
        if (paramTable->columnCount() < 2)
            paramTable->setColumnCount(2);
        paramTable->setObjectName(QString::fromUtf8("paramTable"));
        paramTable->setStyleSheet(QString::fromUtf8("font: 87 11pt \"Noto Sans\";\n"
"background-color: #2f535f;\n"
"color: rgb(131, 148, 150);\n"
"font-weight: 700;\n"
"padding-left: 2px;"));
        paramTable->setColumnCount(2);

        verticalLayout->addWidget(paramTable);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        deleteTask = new QPushButton(ScheduleDialog);
        deleteTask->setObjectName(QString::fromUtf8("deleteTask"));
        sizePolicy1.setHeightForWidth(deleteTask->sizePolicy().hasHeightForWidth());
        deleteTask->setSizePolicy(sizePolicy1);
        deleteTask->setMinimumSize(QSize(100, 25));
        deleteTask->setMaximumSize(QSize(420, 16777215));
        deleteTask->setStyleSheet(QString::fromUtf8("background-color: rgb(170, 0, 0);\n"
"font: 87 11pt \"Noto Sans\";\n"
"color: rgb(0, 0, 0);\n"
"font-weight: 700;\n"
"padding: 4px;\n"
"border-style: outset;\n"
"border-width: 2px;\n"
"border-radius: 4px;\n"
"border-color: #00000f;\n"
"min-width: 4em;\n"
"padding: 4px;"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/icons/icons/trash.png"), QSize(), QIcon::Normal, QIcon::Off);
        deleteTask->setIcon(icon1);

        horizontalLayout_7->addWidget(deleteTask);

        saveTask = new QPushButton(ScheduleDialog);
        saveTask->setObjectName(QString::fromUtf8("saveTask"));
        sizePolicy1.setHeightForWidth(saveTask->sizePolicy().hasHeightForWidth());
        saveTask->setSizePolicy(sizePolicy1);
        saveTask->setMinimumSize(QSize(100, 25));
        saveTask->setMaximumSize(QSize(420, 16777215));
        saveTask->setStyleSheet(QString::fromUtf8("background-color: rgb(2, 180, 43);\n"
"font: 87 11pt \"Noto Sans\";\n"
"color: rgb(0, 0, 0);\n"
"font-weight: 700;\n"
"padding: 4px;\n"
"border-style: outset;\n"
"border-width: 2px;\n"
"border-radius: 4px;\n"
"border-color: #00000f;\n"
"min-width: 4em;\n"
"padding: 4px;"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/icons/send-button.png"), QSize(), QIcon::Normal, QIcon::Off);
        saveTask->setIcon(icon2);

        horizontalLayout_7->addWidget(saveTask);

        close = new QPushButton(ScheduleDialog);
        close->setObjectName(QString::fromUtf8("close"));
        close->setMinimumSize(QSize(100, 30));
        close->setMaximumSize(QSize(420, 16777215));
        close->setStyleSheet(QString::fromUtf8("background-color: rgb(141, 141, 141);\n"
"font: 87 11pt \"Noto Sans\";\n"
"color: rgb(0, 43, 54);\n"
"font-weight: 700;\n"
"padding: 4px;\n"
"border-style: outset;\n"
"border-width: 2px;\n"
"border-radius: 4px;\n"
"border-color: #00000f;\n"
"min-width: 4em;\n"
"padding: 4px;"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/icons/icons/cancel.png"), QSize(), QIcon::Normal, QIcon::Off);
        close->setIcon(icon3);

        horizontalLayout_7->addWidget(close);


        verticalLayout->addLayout(horizontalLayout_7);


        gridLayout->addLayout(verticalLayout, 1, 0, 1, 1);


        retranslateUi(ScheduleDialog);

        QMetaObject::connectSlotsByName(ScheduleDialog);
    } // setupUi

    void retranslateUi(QDialog *ScheduleDialog)
    {
        ScheduleDialog->setWindowTitle(QCoreApplication::translate("ScheduleDialog", "Dialog", nullptr));
        appsLabel->setText(QCoreApplication::translate("ScheduleDialog", "Tasks", nullptr));
        fetchSchedule->setText(QCoreApplication::translate("ScheduleDialog", "Fetch", nullptr));
        nameLabel->setText(QCoreApplication::translate("ScheduleDialog", "App", nullptr));
        maskLabel_4->setText(QCoreApplication::translate("ScheduleDialog", "Flags", nullptr));
        pathLabel->setText(QCoreApplication::translate("ScheduleDialog", "Time", nullptr));
        dataLabel->setText(QCoreApplication::translate("ScheduleDialog", "Status", nullptr));
        completed->setItemText(0, QCoreApplication::translate("ScheduleDialog", "Completed", nullptr));
        completed->setItemText(1, QCoreApplication::translate("ScheduleDialog", "Scheduled", nullptr));
        completed->setItemText(2, QCoreApplication::translate("ScheduleDialog", "Failed", nullptr));
        completed->setItemText(3, QCoreApplication::translate("ScheduleDialog", "Retry Failed", nullptr));

        maskLabel->setText(QCoreApplication::translate("ScheduleDialog", "Recurring", nullptr));
        recurring->setItemText(0, QCoreApplication::translate("ScheduleDialog", "No", nullptr));
        recurring->setItemText(1, QCoreApplication::translate("ScheduleDialog", "Hourly", nullptr));
        recurring->setItemText(2, QCoreApplication::translate("ScheduleDialog", "Daily", nullptr));
        recurring->setItemText(3, QCoreApplication::translate("ScheduleDialog", "Weekly", nullptr));
        recurring->setItemText(4, QCoreApplication::translate("ScheduleDialog", "Monthly", nullptr));
        recurring->setItemText(5, QCoreApplication::translate("ScheduleDialog", "Yearly", nullptr));

        maskLabel_3->setText(QCoreApplication::translate("ScheduleDialog", "Notify", nullptr));
        notifyCheck->setText(QString());
        maskLabel_2->setText(QCoreApplication::translate("ScheduleDialog", "Runtime Arguments", nullptr));
        maskLabel_5->setText(QCoreApplication::translate("ScheduleDialog", "Files", nullptr));
        deleteTask->setText(QCoreApplication::translate("ScheduleDialog", "Delete", nullptr));
        saveTask->setText(QCoreApplication::translate("ScheduleDialog", "Update", nullptr));
        close->setText(QCoreApplication::translate("ScheduleDialog", "Close", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ScheduleDialog: public Ui_ScheduleDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SCHEDULEDIALOG_H
