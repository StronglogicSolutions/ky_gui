/********************************************************************************
** Form generated from reading UI file 'appdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.13.0
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
    QLabel *label_3;
    QComboBox *argType_2;
    QPushButton *pushButton;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_5;
    QLineEdit *lineEdit_3;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_4;
    QLineEdit *lineEdit_2;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_6;
    QLineEdit *lineEdit_4;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButton_3;
    QPushButton *pushButton_4;
    QPushButton *close;

    void setupUi(QDialog *AppDialog)
    {
        if (AppDialog->objectName().isEmpty())
            AppDialog->setObjectName(QString::fromUtf8("AppDialog"));
        AppDialog->resize(970, 864);
        AppDialog->setStyleSheet(QString::fromUtf8("background-color: rgb(36, 41, 46);\n"
"color: rgb(161, 164, 167);"));
        verticalLayoutWidget = new QWidget(AppDialog);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(10, 10, 951, 851));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_3 = new QLabel(verticalLayoutWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(2);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label_3->sizePolicy().hasHeightForWidth());
        label_3->setSizePolicy(sizePolicy);
        label_3->setMinimumSize(QSize(196, 0));
        label_3->setMaximumSize(QSize(196, 16777215));
        label_3->setStyleSheet(QString::fromUtf8("font: 75 11pt \"Noto Sans\";\n"
"color: rgb(131, 148, 150);\n"
"font-weight: 700;"));
        label_3->setAlignment(Qt::AlignCenter);

        horizontalLayout_2->addWidget(label_3);

        argType_2 = new QComboBox(verticalLayoutWidget);
        argType_2->setObjectName(QString::fromUtf8("argType_2"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(argType_2->sizePolicy().hasHeightForWidth());
        argType_2->setSizePolicy(sizePolicy1);
        argType_2->setStyleSheet(QString::fromUtf8("font: 87 11pt \"Noto Sans\";\n"
"background-color: #2f535f;\n"
"color: rgb(131, 148, 150);\n"
"font-weight: 700;\n"
""));

        horizontalLayout_2->addWidget(argType_2);

        pushButton = new QPushButton(verticalLayoutWidget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/icons/add.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton->setIcon(icon);

        horizontalLayout_2->addWidget(pushButton);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        label_5 = new QLabel(verticalLayoutWidget);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        sizePolicy.setHeightForWidth(label_5->sizePolicy().hasHeightForWidth());
        label_5->setSizePolicy(sizePolicy);
        label_5->setMinimumSize(QSize(196, 0));
        label_5->setMaximumSize(QSize(196, 16777215));
        label_5->setStyleSheet(QString::fromUtf8("font: 75 11pt \"Noto Sans\";\n"
"color: rgb(131, 148, 150);\n"
"font-weight: 700;"));
        label_5->setAlignment(Qt::AlignCenter);

        horizontalLayout_4->addWidget(label_5);

        lineEdit_3 = new QLineEdit(verticalLayoutWidget);
        lineEdit_3->setObjectName(QString::fromUtf8("lineEdit_3"));

        horizontalLayout_4->addWidget(lineEdit_3);


        verticalLayout->addLayout(horizontalLayout_4);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label_4 = new QLabel(verticalLayoutWidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        sizePolicy.setHeightForWidth(label_4->sizePolicy().hasHeightForWidth());
        label_4->setSizePolicy(sizePolicy);
        label_4->setMinimumSize(QSize(196, 0));
        label_4->setMaximumSize(QSize(196, 16777215));
        label_4->setStyleSheet(QString::fromUtf8("font: 75 11pt \"Noto Sans\";\n"
"color: rgb(131, 148, 150);\n"
"font-weight: 700;"));
        label_4->setAlignment(Qt::AlignCenter);

        horizontalLayout_3->addWidget(label_4);

        lineEdit_2 = new QLineEdit(verticalLayoutWidget);
        lineEdit_2->setObjectName(QString::fromUtf8("lineEdit_2"));

        horizontalLayout_3->addWidget(lineEdit_2);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        label_6 = new QLabel(verticalLayoutWidget);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        sizePolicy.setHeightForWidth(label_6->sizePolicy().hasHeightForWidth());
        label_6->setSizePolicy(sizePolicy);
        label_6->setMinimumSize(QSize(196, 0));
        label_6->setMaximumSize(QSize(196, 16777215));
        label_6->setStyleSheet(QString::fromUtf8("font: 75 11pt \"Noto Sans\";\n"
"color: rgb(131, 148, 150);\n"
"font-weight: 700;"));
        label_6->setAlignment(Qt::AlignCenter);

        horizontalLayout_5->addWidget(label_6);

        lineEdit_4 = new QLineEdit(verticalLayoutWidget);
        lineEdit_4->setObjectName(QString::fromUtf8("lineEdit_4"));

        horizontalLayout_5->addWidget(lineEdit_4);


        verticalLayout->addLayout(horizontalLayout_5);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        pushButton_3 = new QPushButton(verticalLayoutWidget);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/icons/icons/checkmark.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_3->setIcon(icon1);

        horizontalLayout->addWidget(pushButton_3);

        pushButton_4 = new QPushButton(verticalLayoutWidget);
        pushButton_4->setObjectName(QString::fromUtf8("pushButton_4"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/icons/icons/revert.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_4->setIcon(icon2);

        horizontalLayout->addWidget(pushButton_4);

        close = new QPushButton(verticalLayoutWidget);
        close->setObjectName(QString::fromUtf8("close"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/icons/icons/cancel.png"), QSize(), QIcon::Normal, QIcon::Off);
        close->setIcon(icon3);

        horizontalLayout->addWidget(close);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(AppDialog);

        QMetaObject::connectSlotsByName(AppDialog);
    } // setupUi

    void retranslateUi(QDialog *AppDialog)
    {
        AppDialog->setWindowTitle(QCoreApplication::translate("AppDialog", "Dialog", nullptr));
        label_3->setText(QCoreApplication::translate("AppDialog", "Apps", nullptr));
        pushButton->setText(QCoreApplication::translate("AppDialog", "Add", nullptr));
        label_5->setText(QCoreApplication::translate("AppDialog", "Name", nullptr));
        label_4->setText(QCoreApplication::translate("AppDialog", "Path", nullptr));
        label_6->setText(QCoreApplication::translate("AppDialog", "Mask", nullptr));
        pushButton_3->setText(QCoreApplication::translate("AppDialog", "Save", nullptr));
        pushButton_4->setText(QCoreApplication::translate("AppDialog", "Revert", nullptr));
        close->setText(QCoreApplication::translate("AppDialog", "Close", nullptr));
    } // retranslateUi

};

namespace Ui {
    class AppDialog: public Ui_AppDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_APPDIALOG_H
