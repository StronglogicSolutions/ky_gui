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
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDateTimeEdit>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ArgDialog
{
public:
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QLabel *argDialogTitle;
    QHBoxLayout *horizontalLayout;
    QLabel *label_2;
    QComboBox *argType;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_3;
    QTextEdit *argInput;
    QPushButton *addArgument;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_4;
    QPushButton *addFile;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_5;
    QDateTimeEdit *dateTime;
    QLabel *label;
    QHBoxLayout *horizontalLayout_6;
    QSpacerItem *horizontalSpacer;
    QTableWidget *argList;
    QVBoxLayout *verticalLayout_2;
    QSpacerItem *horizontalSpacer_2;
    QHBoxLayout *horizontalLayout_5;
    QPushButton *devTestButton;
    QPushButton *clear;
    QDialogButtonBox *argCommandButtons;

    void setupUi(QDialog *ArgDialog)
    {
        if (ArgDialog->objectName().isEmpty())
            ArgDialog->setObjectName(QString::fromUtf8("ArgDialog"));
        ArgDialog->resize(724, 948);
        QIcon icon;
        icon.addFile(QString::fromUtf8("favicon.ico"), QSize(), QIcon::Normal, QIcon::Off);
        ArgDialog->setWindowIcon(icon);
        ArgDialog->setStyleSheet(QString::fromUtf8("border-color: rgb(0, 0, 0);\n"
"background-color: rgb(0, 43, 54);"));
        verticalLayoutWidget = new QWidget(ArgDialog);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(10, 10, 701, 921));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        argDialogTitle = new QLabel(verticalLayoutWidget);
        argDialogTitle->setObjectName(QString::fromUtf8("argDialogTitle"));
        QFont font;
        font.setPointSize(22);
        font.setBold(true);
        font.setWeight(75);
        argDialogTitle->setFont(font);
        argDialogTitle->setStyleSheet(QString::fromUtf8("color: rgb(170, 0, 255);\n"
"color:  rgb(255, 85, 0);\n"
" qproperty-alignment: 'AlignHCenter';\n"
""));

        verticalLayout->addWidget(argDialogTitle);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label_2 = new QLabel(verticalLayoutWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(2);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy);
        label_2->setMinimumSize(QSize(108, 0));
        label_2->setStyleSheet(QString::fromUtf8("font: 75 11pt \"Noto Sans\";\n"
"color: rgb(131, 148, 150);\n"
"font-weight: 700;"));

        horizontalLayout->addWidget(label_2);

        argType = new QComboBox(verticalLayoutWidget);
        argType->addItem(QString());
        argType->addItem(QString());
        argType->addItem(QString());
        argType->addItem(QString());
        argType->addItem(QString());
        argType->setObjectName(QString::fromUtf8("argType"));
        QSizePolicy sizePolicy1(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(argType->sizePolicy().hasHeightForWidth());
        argType->setSizePolicy(sizePolicy1);
        argType->setStyleSheet(QString::fromUtf8("font: 87 11pt \"Noto Sans\";\n"
"background-color: #2f535f;\n"
"color: rgb(131, 148, 150);\n"
"font-weight: 700;\n"
""));

        horizontalLayout->addWidget(argType);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_3 = new QLabel(verticalLayoutWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setMinimumSize(QSize(108, 0));
        label_3->setStyleSheet(QString::fromUtf8("font: 75 11pt \"Noto Sans\";\n"
"color: rgb(131, 148, 150);\n"
"font-weight: 700;\n"
" qproperty-alignment: 'AlignVLeft';"));

        horizontalLayout_2->addWidget(label_3);

        argInput = new QTextEdit(verticalLayoutWidget);
        argInput->setObjectName(QString::fromUtf8("argInput"));
        argInput->setStyleSheet(QString::fromUtf8("font: 87 11pt \"Noto Sans\";\n"
"background-color: #2f535f;\n"
"color: rgb(131, 148, 150);\n"
"font-weight: 700;\n"
""));

        horizontalLayout_2->addWidget(argInput);


        verticalLayout->addLayout(horizontalLayout_2);

        addArgument = new QPushButton(verticalLayoutWidget);
        addArgument->setObjectName(QString::fromUtf8("addArgument"));
        addArgument->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 85, 0);\n"
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

        verticalLayout->addWidget(addArgument);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label_4 = new QLabel(verticalLayoutWidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(label_4->sizePolicy().hasHeightForWidth());
        label_4->setSizePolicy(sizePolicy2);
        label_4->setMinimumSize(QSize(196, 0));
        label_4->setStyleSheet(QString::fromUtf8("font: 75 11pt \"Noto Sans\";\n"
"color: rgb(131, 148, 150);\n"
"font-weight: 700;"));

        horizontalLayout_3->addWidget(label_4, 0, Qt::AlignVCenter);

        addFile = new QPushButton(verticalLayoutWidget);
        addFile->setObjectName(QString::fromUtf8("addFile"));
        addFile->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 85, 0);\n"
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

        horizontalLayout_3->addWidget(addFile);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        label_5 = new QLabel(verticalLayoutWidget);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        sizePolicy2.setHeightForWidth(label_5->sizePolicy().hasHeightForWidth());
        label_5->setSizePolicy(sizePolicy2);
        label_5->setMinimumSize(QSize(52, 0));
        label_5->setStyleSheet(QString::fromUtf8("font: 75 11pt \"Noto Sans\";\n"
"color: rgb(131, 148, 150);\n"
"font-weight: 700;\n"
" qproperty-alignment: 'AlignHCenter';"));

        horizontalLayout_4->addWidget(label_5, 0, Qt::AlignVCenter);

        dateTime = new QDateTimeEdit(verticalLayoutWidget);
        dateTime->setObjectName(QString::fromUtf8("dateTime"));
        sizePolicy1.setHeightForWidth(dateTime->sizePolicy().hasHeightForWidth());
        dateTime->setSizePolicy(sizePolicy1);
        dateTime->setMinimumSize(QSize(380, 0));
        dateTime->setStyleSheet(QString::fromUtf8("background-color: #00000f;\n"
"color: #2f535f;\n"
"font-weight: 700;\n"
"font-size: 16pt;"));

        horizontalLayout_4->addWidget(dateTime, 0, Qt::AlignRight);


        verticalLayout->addLayout(horizontalLayout_4);

        label = new QLabel(verticalLayoutWidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setStyleSheet(QString::fromUtf8("font: 75 11pt \"Noto Sans\";\n"
"color: rgb(131, 148, 150);\n"
"font-weight: 700;\n"
"padding-left: 8px;"));

        verticalLayout->addWidget(label, 0, Qt::AlignLeft);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Maximum, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer);

        argList = new QTableWidget(verticalLayoutWidget);
        if (argList->columnCount() < 4)
            argList->setColumnCount(4);
        argList->setObjectName(QString::fromUtf8("argList"));
        argList->setStyleSheet(QString::fromUtf8("font: 87 11pt \"Noto Sans\";\n"
"background-color: #2f535f;\n"
"color: rgb(131, 148, 150);\n"
"font-weight: 700;\n"
"padding-left: 2px;"));
        argList->setShowGrid(true);
        argList->setColumnCount(4);
        argList->horizontalHeader()->setCascadingSectionResizes(false);
        argList->horizontalHeader()->setMinimumSectionSize(100);
        argList->horizontalHeader()->setDefaultSectionSize(200);
        argList->verticalHeader()->setMinimumSectionSize(100);
        argList->verticalHeader()->setDefaultSectionSize(100);

        horizontalLayout_6->addWidget(argList);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Maximum, QSizePolicy::Minimum);

        verticalLayout_2->addItem(horizontalSpacer_2);


        horizontalLayout_6->addLayout(verticalLayout_2);


        verticalLayout->addLayout(horizontalLayout_6);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        devTestButton = new QPushButton(verticalLayoutWidget);
        devTestButton->setObjectName(QString::fromUtf8("devTestButton"));
        devTestButton->setStyleSheet(QString::fromUtf8("background-color: rgb(130, 255, 121);\n"
"background-color: rgb(2, 180, 43);\n"
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

        horizontalLayout_5->addWidget(devTestButton);

        clear = new QPushButton(verticalLayoutWidget);
        clear->setObjectName(QString::fromUtf8("clear"));
        clear->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 85, 0);\n"
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
        QIcon icon1;
        icon1.addFile(QString::fromUtf8("icons/erase.png"), QSize(), QIcon::Normal, QIcon::Off);
        clear->setIcon(icon1);

        horizontalLayout_5->addWidget(clear);

        argCommandButtons = new QDialogButtonBox(verticalLayoutWidget);
        argCommandButtons->setObjectName(QString::fromUtf8("argCommandButtons"));
        QSizePolicy sizePolicy3(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(argCommandButtons->sizePolicy().hasHeightForWidth());
        argCommandButtons->setSizePolicy(sizePolicy3);
        argCommandButtons->setLayoutDirection(Qt::LeftToRight);
        argCommandButtons->setAutoFillBackground(false);
        argCommandButtons->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 85, 0);\n"
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
        argCommandButtons->setOrientation(Qt::Horizontal);
        argCommandButtons->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Save);
        argCommandButtons->setCenterButtons(true);

        horizontalLayout_5->addWidget(argCommandButtons);


        verticalLayout->addLayout(horizontalLayout_5);

        verticalLayout->setStretch(2, 1);
        verticalLayout->setStretch(7, 3);

        retranslateUi(ArgDialog);
        QObject::connect(argCommandButtons, SIGNAL(accepted()), ArgDialog, SLOT(accept()));
        QObject::connect(argCommandButtons, SIGNAL(rejected()), ArgDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(ArgDialog);
    } // setupUi

    void retranslateUi(QDialog *ArgDialog)
    {
        ArgDialog->setWindowTitle(QCoreApplication::translate("ArgDialog", "Dialog", nullptr));
        argDialogTitle->setText(QCoreApplication::translate("ArgDialog", "Add Arguments", nullptr));
        label_2->setText(QCoreApplication::translate("ArgDialog", "Type", nullptr));
        argType->setItemText(0, QCoreApplication::translate("ArgDialog", "description", nullptr));
        argType->setItemText(1, QCoreApplication::translate("ArgDialog", "hashtag", nullptr));
        argType->setItemText(2, QCoreApplication::translate("ArgDialog", "requested by", nullptr));
        argType->setItemText(3, QCoreApplication::translate("ArgDialog", "promote/share", nullptr));
        argType->setItemText(4, QCoreApplication::translate("ArgDialog", "link/bio", nullptr));

        label_3->setText(QCoreApplication::translate("ArgDialog", "Input", nullptr));
        addArgument->setText(QCoreApplication::translate("ArgDialog", "Add", nullptr));
        label_4->setText(QCoreApplication::translate("ArgDialog", "Add file attachment", nullptr));
        addFile->setText(QCoreApplication::translate("ArgDialog", "Choose", nullptr));
        label_5->setText(QCoreApplication::translate("ArgDialog", "Time", nullptr));
        label->setText(QCoreApplication::translate("ArgDialog", "Items to send", nullptr));
        devTestButton->setText(QCoreApplication::translate("ArgDialog", "DEV TEST", nullptr));
        clear->setText(QCoreApplication::translate("ArgDialog", "Clear", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ArgDialog: public Ui_ArgDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ARGDIALOG_H
