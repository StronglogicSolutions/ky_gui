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
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ScheduleDialog
{
public:
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QLabel *appsLabel;
    QComboBox *taskList;
    QHBoxLayout *horizontalLayout_4;
    QLabel *nameLabel;
    QLineEdit *appText;
    QHBoxLayout *horizontalLayout_10;
    QLabel *maskLabel_4;
    QLineEdit *flagsText;
    QHBoxLayout *horizontalLayout_3;
    QLabel *pathLabel;
    QLineEdit *timeText;
    QHBoxLayout *horizontalLayout_6;
    QLabel *dataLabel;
    QLineEdit *completedText;
    QHBoxLayout *horizontalLayout_5;
    QLabel *maskLabel;
    QLineEdit *recurringText;
    QHBoxLayout *horizontalLayout_9;
    QLabel *maskLabel_3;
    QLineEdit *notifyText;
    QHBoxLayout *horizontalLayout_8;
    QLabel *maskLabel_2;
    QLineEdit *runtimeText;
    QHBoxLayout *horizontalLayout_11;
    QLabel *maskLabel_5;
    QLineEdit *filesText;
    QHBoxLayout *horizontalLayout_7;
    QPushButton *save;
    QPushButton *deleteApp;
    QHBoxLayout *horizontalLayout;
    QPushButton *close;

    void setupUi(QDialog *ScheduleDialog)
    {
        if (ScheduleDialog->objectName().isEmpty())
            ScheduleDialog->setObjectName(QString::fromUtf8("ScheduleDialog"));
        ScheduleDialog->resize(970, 864);
        ScheduleDialog->setStyleSheet(QString::fromUtf8("background-color: rgb(36, 41, 46);\n"
"color: rgb(161, 164, 167);"));
        verticalLayoutWidget = new QWidget(ScheduleDialog);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(10, 10, 951, 851));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setSpacing(18);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(16, 16, 16, 16);
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        appsLabel = new QLabel(verticalLayoutWidget);
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

        horizontalLayout_2->addWidget(appsLabel);

        taskList = new QComboBox(verticalLayoutWidget);
        taskList->setObjectName(QString::fromUtf8("taskList"));

        horizontalLayout_2->addWidget(taskList);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        nameLabel = new QLabel(verticalLayoutWidget);
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

        appText = new QLineEdit(verticalLayoutWidget);
        appText->setObjectName(QString::fromUtf8("appText"));

        horizontalLayout_4->addWidget(appText);


        verticalLayout->addLayout(horizontalLayout_4);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setObjectName(QString::fromUtf8("horizontalLayout_10"));
        maskLabel_4 = new QLabel(verticalLayoutWidget);
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

        flagsText = new QLineEdit(verticalLayoutWidget);
        flagsText->setObjectName(QString::fromUtf8("flagsText"));

        horizontalLayout_10->addWidget(flagsText);


        verticalLayout->addLayout(horizontalLayout_10);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        pathLabel = new QLabel(verticalLayoutWidget);
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

        timeText = new QLineEdit(verticalLayoutWidget);
        timeText->setObjectName(QString::fromUtf8("timeText"));

        horizontalLayout_3->addWidget(timeText);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        dataLabel = new QLabel(verticalLayoutWidget);
        dataLabel->setObjectName(QString::fromUtf8("dataLabel"));
        sizePolicy.setHeightForWidth(dataLabel->sizePolicy().hasHeightForWidth());
        dataLabel->setSizePolicy(sizePolicy);
        dataLabel->setMinimumSize(QSize(196, 0));
        dataLabel->setMaximumSize(QSize(196, 16777215));
        dataLabel->setStyleSheet(QString::fromUtf8("font: 75 11pt \"Noto Sans\";\n"
"color: rgb(131, 148, 150);\n"
"font-weight: 700;"));
        dataLabel->setAlignment(Qt::AlignCenter);

        horizontalLayout_6->addWidget(dataLabel);

        completedText = new QLineEdit(verticalLayoutWidget);
        completedText->setObjectName(QString::fromUtf8("completedText"));

        horizontalLayout_6->addWidget(completedText);


        verticalLayout->addLayout(horizontalLayout_6);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        maskLabel = new QLabel(verticalLayoutWidget);
        maskLabel->setObjectName(QString::fromUtf8("maskLabel"));
        sizePolicy.setHeightForWidth(maskLabel->sizePolicy().hasHeightForWidth());
        maskLabel->setSizePolicy(sizePolicy);
        maskLabel->setMinimumSize(QSize(196, 0));
        maskLabel->setMaximumSize(QSize(196, 16777215));
        maskLabel->setStyleSheet(QString::fromUtf8("font: 75 11pt \"Noto Sans\";\n"
"color: rgb(131, 148, 150);\n"
"font-weight: 700;"));
        maskLabel->setAlignment(Qt::AlignCenter);

        horizontalLayout_5->addWidget(maskLabel);

        recurringText = new QLineEdit(verticalLayoutWidget);
        recurringText->setObjectName(QString::fromUtf8("recurringText"));

        horizontalLayout_5->addWidget(recurringText);


        verticalLayout->addLayout(horizontalLayout_5);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        maskLabel_3 = new QLabel(verticalLayoutWidget);
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

        notifyText = new QLineEdit(verticalLayoutWidget);
        notifyText->setObjectName(QString::fromUtf8("notifyText"));

        horizontalLayout_9->addWidget(notifyText);


        verticalLayout->addLayout(horizontalLayout_9);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        maskLabel_2 = new QLabel(verticalLayoutWidget);
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

        runtimeText = new QLineEdit(verticalLayoutWidget);
        runtimeText->setObjectName(QString::fromUtf8("runtimeText"));

        horizontalLayout_8->addWidget(runtimeText);


        verticalLayout->addLayout(horizontalLayout_8);

        horizontalLayout_11 = new QHBoxLayout();
        horizontalLayout_11->setObjectName(QString::fromUtf8("horizontalLayout_11"));
        maskLabel_5 = new QLabel(verticalLayoutWidget);
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

        filesText = new QLineEdit(verticalLayoutWidget);
        filesText->setObjectName(QString::fromUtf8("filesText"));

        horizontalLayout_11->addWidget(filesText);


        verticalLayout->addLayout(horizontalLayout_11);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        save = new QPushButton(verticalLayoutWidget);
        save->setObjectName(QString::fromUtf8("save"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(save->sizePolicy().hasHeightForWidth());
        save->setSizePolicy(sizePolicy1);
        save->setMinimumSize(QSize(100, 25));
        save->setMaximumSize(QSize(420, 16777215));
        save->setStyleSheet(QString::fromUtf8("background-color: rgb(2, 180, 43);\n"
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
        icon.addFile(QString::fromUtf8(":/icons/send-button.png"), QSize(), QIcon::Normal, QIcon::Off);
        save->setIcon(icon);

        horizontalLayout_7->addWidget(save);

        deleteApp = new QPushButton(verticalLayoutWidget);
        deleteApp->setObjectName(QString::fromUtf8("deleteApp"));
        sizePolicy1.setHeightForWidth(deleteApp->sizePolicy().hasHeightForWidth());
        deleteApp->setSizePolicy(sizePolicy1);
        deleteApp->setMinimumSize(QSize(100, 25));
        deleteApp->setMaximumSize(QSize(420, 16777215));
        deleteApp->setStyleSheet(QString::fromUtf8("background-color: rgb(170, 0, 0);\n"
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
        deleteApp->setIcon(icon1);

        horizontalLayout_7->addWidget(deleteApp);


        verticalLayout->addLayout(horizontalLayout_7);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        close = new QPushButton(verticalLayoutWidget);
        close->setObjectName(QString::fromUtf8("close"));
        close->setMinimumSize(QSize(100, 30));
        close->setMaximumSize(QSize(420, 16777215));
        close->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 85, 0);\n"
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
        icon2.addFile(QString::fromUtf8(":/icons/icons/cancel.png"), QSize(), QIcon::Normal, QIcon::Off);
        close->setIcon(icon2);

        horizontalLayout->addWidget(close);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(ScheduleDialog);

        QMetaObject::connectSlotsByName(ScheduleDialog);
    } // setupUi

    void retranslateUi(QDialog *ScheduleDialog)
    {
        ScheduleDialog->setWindowTitle(QCoreApplication::translate("ScheduleDialog", "Dialog", nullptr));
        appsLabel->setText(QCoreApplication::translate("ScheduleDialog", "Tasks", nullptr));
        nameLabel->setText(QCoreApplication::translate("ScheduleDialog", "App", nullptr));
        maskLabel_4->setText(QCoreApplication::translate("ScheduleDialog", "Flags", nullptr));
        pathLabel->setText(QCoreApplication::translate("ScheduleDialog", "Time", nullptr));
        dataLabel->setText(QCoreApplication::translate("ScheduleDialog", "Completed", nullptr));
        maskLabel->setText(QCoreApplication::translate("ScheduleDialog", "Recurring", nullptr));
        maskLabel_3->setText(QCoreApplication::translate("ScheduleDialog", "Notify", nullptr));
        maskLabel_2->setText(QCoreApplication::translate("ScheduleDialog", "Runtime Arguments", nullptr));
        maskLabel_5->setText(QCoreApplication::translate("ScheduleDialog", "Files", nullptr));
        save->setText(QCoreApplication::translate("ScheduleDialog", "Update", nullptr));
        deleteApp->setText(QCoreApplication::translate("ScheduleDialog", "Delete", nullptr));
        close->setText(QCoreApplication::translate("ScheduleDialog", "Close", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ScheduleDialog: public Ui_ScheduleDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SCHEDULEDIALOG_H
