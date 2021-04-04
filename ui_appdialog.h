/********************************************************************************
** Form generated from reading UI file 'appdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_APPDIALOG_H
#define UI_APPDIALOG_H

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

class Ui_AppDialog
{
public:
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QLabel *appsLabel;
    QComboBox *appList;
    QPushButton *addApp;
    QHBoxLayout *horizontalLayout_4;
    QLabel *nameLabel;
    QLineEdit *nameText;
    QHBoxLayout *horizontalLayout_3;
    QLabel *pathLabel;
    QLineEdit *pathText;
    QHBoxLayout *horizontalLayout_6;
    QLabel *dataLabel;
    QLineEdit *dataText;
    QHBoxLayout *horizontalLayout_5;
    QLabel *maskLabel;
    QLineEdit *maskText;
    QHBoxLayout *horizontalLayout_7;
    QPushButton *save;
    QPushButton *deleteApp;
    QHBoxLayout *horizontalLayout;
    QPushButton *close;

    void setupUi(QDialog *AppDialog)
    {
        if (AppDialog->objectName().isEmpty())
            AppDialog->setObjectName(QString::fromUtf8("AppDialog"));
        AppDialog->resize(970, 864);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/favicon.ico"), QSize(), QIcon::Normal, QIcon::Off);
        AppDialog->setWindowIcon(icon);
        AppDialog->setStyleSheet(QString::fromUtf8("background-color: rgb(36, 41, 46);\n"
"color: rgb(161, 164, 167);"));
        verticalLayoutWidget = new QWidget(AppDialog);
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

        appList = new QComboBox(verticalLayoutWidget);
        appList->setObjectName(QString::fromUtf8("appList"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(appList->sizePolicy().hasHeightForWidth());
        appList->setSizePolicy(sizePolicy1);
        appList->setStyleSheet(QString::fromUtf8("font: 87 11pt \"Noto Sans\";\n"
"background-color: #2f535f;\n"
"color: rgb(131, 148, 150);\n"
"font-weight: 700;\n"
""));

        horizontalLayout_2->addWidget(appList);

        addApp = new QPushButton(verticalLayoutWidget);
        addApp->setObjectName(QString::fromUtf8("addApp"));
        addApp->setMinimumSize(QSize(100, 30));
        addApp->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 58, 70);\n"
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
        icon1.addFile(QString::fromUtf8(":/icons/icons/add.png"), QSize(), QIcon::Normal, QIcon::Off);
        addApp->setIcon(icon1);

        horizontalLayout_2->addWidget(addApp);


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

        nameText = new QLineEdit(verticalLayoutWidget);
        nameText->setObjectName(QString::fromUtf8("nameText"));

        horizontalLayout_4->addWidget(nameText);


        verticalLayout->addLayout(horizontalLayout_4);

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

        pathText = new QLineEdit(verticalLayoutWidget);
        pathText->setObjectName(QString::fromUtf8("pathText"));

        horizontalLayout_3->addWidget(pathText);


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

        dataText = new QLineEdit(verticalLayoutWidget);
        dataText->setObjectName(QString::fromUtf8("dataText"));

        horizontalLayout_6->addWidget(dataText);


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

        maskText = new QLineEdit(verticalLayoutWidget);
        maskText->setObjectName(QString::fromUtf8("maskText"));

        horizontalLayout_5->addWidget(maskText);


        verticalLayout->addLayout(horizontalLayout_5);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        save = new QPushButton(verticalLayoutWidget);
        save->setObjectName(QString::fromUtf8("save"));
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(save->sizePolicy().hasHeightForWidth());
        save->setSizePolicy(sizePolicy2);
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
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/icons/send-button.png"), QSize(), QIcon::Normal, QIcon::Off);
        save->setIcon(icon2);

        horizontalLayout_7->addWidget(save);

        deleteApp = new QPushButton(verticalLayoutWidget);
        deleteApp->setObjectName(QString::fromUtf8("deleteApp"));
        sizePolicy2.setHeightForWidth(deleteApp->sizePolicy().hasHeightForWidth());
        deleteApp->setSizePolicy(sizePolicy2);
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
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/icons/icons/trash.png"), QSize(), QIcon::Normal, QIcon::Off);
        deleteApp->setIcon(icon3);

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
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/icons/icons/cancel.png"), QSize(), QIcon::Normal, QIcon::Off);
        close->setIcon(icon4);

        horizontalLayout->addWidget(close);


        verticalLayout->addLayout(horizontalLayout);

        verticalLayout->setStretch(0, 2);
        verticalLayout->setStretch(1, 1);
        verticalLayout->setStretch(2, 1);
        verticalLayout->setStretch(3, 1);
        verticalLayout->setStretch(4, 1);

        retranslateUi(AppDialog);

        QMetaObject::connectSlotsByName(AppDialog);
    } // setupUi

    void retranslateUi(QDialog *AppDialog)
    {
        AppDialog->setWindowTitle(QCoreApplication::translate("AppDialog", "Dialog", nullptr));
        appsLabel->setText(QCoreApplication::translate("AppDialog", "Apps", nullptr));
        addApp->setText(QCoreApplication::translate("AppDialog", "Add", nullptr));
        nameLabel->setText(QCoreApplication::translate("AppDialog", "Name", nullptr));
        pathLabel->setText(QCoreApplication::translate("AppDialog", "Path", nullptr));
        dataLabel->setText(QCoreApplication::translate("AppDialog", "Data", nullptr));
        maskLabel->setText(QCoreApplication::translate("AppDialog", "Mask", nullptr));
        save->setText(QCoreApplication::translate("AppDialog", "Submit", nullptr));
        deleteApp->setText(QCoreApplication::translate("AppDialog", "Delete", nullptr));
        close->setText(QCoreApplication::translate("AppDialog", "Close", nullptr));
    } // retranslateUi

};

namespace Ui {
    class AppDialog: public Ui_AppDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_APPDIALOG_H
