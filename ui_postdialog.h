/********************************************************************************
** Form generated from reading UI file 'postdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_POSTDIALOG_H
#define UI_POSTDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableView>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_Dialog
{
public:
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout;
    QFrame *frame_2;
    QVBoxLayout *verticalLayout;
    QFrame *frame;
    QLabel *label;
    QTableView *posts;
    QHBoxLayout *horizontalLayout_2;
    QTextBrowser *postText;
    QPushButton *save;
    QFrame *frame_4;
    QFrame *frame_3;

    void setupUi(QDialog *Dialog)
    {
        if (Dialog->objectName().isEmpty())
            Dialog->setObjectName(QString::fromUtf8("Dialog"));
        Dialog->resize(960, 640);
        gridLayout = new QGridLayout(Dialog);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        frame_2 = new QFrame(Dialog);
        frame_2->setObjectName(QString::fromUtf8("frame_2"));
        frame_2->setStyleSheet(QString::fromUtf8("background-color: rgb(131, 131, 131);"));
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);

        horizontalLayout->addWidget(frame_2);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        frame = new QFrame(Dialog);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setStyleSheet(QString::fromUtf8("background-color: rgb(131, 131, 131);"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);

        verticalLayout->addWidget(frame);

        label = new QLabel(Dialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setStyleSheet(QString::fromUtf8("font: 75 italic 16pt \"Noto Sans\";"));
        label->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label->setWordWrap(false);
        label->setMargin(12);

        verticalLayout->addWidget(label);

        posts = new QTableView(Dialog);
        posts->setObjectName(QString::fromUtf8("posts"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(posts->sizePolicy().hasHeightForWidth());
        posts->setSizePolicy(sizePolicy);
        posts->setMouseTracking(true);
        posts->setSelectionBehavior(QAbstractItemView::SelectRows);
        posts->setSortingEnabled(true);

        verticalLayout->addWidget(posts);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        postText = new QTextBrowser(Dialog);
        postText->setObjectName(QString::fromUtf8("postText"));
        postText->setMinimumSize(QSize(0, 0));
        postText->setMaximumSize(QSize(600, 500));

        horizontalLayout_2->addWidget(postText);

        save = new QPushButton(Dialog);
        save->setObjectName(QString::fromUtf8("save"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(save->sizePolicy().hasHeightForWidth());
        save->setSizePolicy(sizePolicy1);
        save->setMinimumSize(QSize(46, 46));
        save->setMaximumSize(QSize(300, 16777215));
        save->setStyleSheet(QString::fromUtf8("font: 87 18pt \"Noto Sans\";\n"
"color: rgb(0, 0, 0);\n"
"font-weight: 700;\n"
"padding: 4px;\n"
"border-style: outset;\n"
"border-width: 2px;\n"
"border-radius: 6px;\n"
"border-color: #00000f;\n"
"min-width: 1em;\n"
"min-height: 1em;\n"
"padding: 4px;\n"
"opacity: 0.3;"));

        horizontalLayout_2->addWidget(save);


        verticalLayout->addLayout(horizontalLayout_2);

        frame_4 = new QFrame(Dialog);
        frame_4->setObjectName(QString::fromUtf8("frame_4"));
        frame_4->setStyleSheet(QString::fromUtf8("background-color: rgb(131, 131, 131);"));
        frame_4->setFrameShape(QFrame::StyledPanel);
        frame_4->setFrameShadow(QFrame::Raised);

        verticalLayout->addWidget(frame_4);

        verticalLayout->setStretch(0, 1);
        verticalLayout->setStretch(1, 1);
        verticalLayout->setStretch(2, 10);
        verticalLayout->setStretch(4, 1);

        horizontalLayout->addLayout(verticalLayout);

        frame_3 = new QFrame(Dialog);
        frame_3->setObjectName(QString::fromUtf8("frame_3"));
        frame_3->setStyleSheet(QString::fromUtf8("background-color: rgb(131, 131, 131);"));
        frame_3->setFrameShape(QFrame::StyledPanel);
        frame_3->setFrameShadow(QFrame::Raised);

        horizontalLayout->addWidget(frame_3);

        horizontalLayout->setStretch(0, 1);
        horizontalLayout->setStretch(1, 20);
        horizontalLayout->setStretch(2, 1);

        gridLayout->addLayout(horizontalLayout, 0, 0, 1, 1);


        retranslateUi(Dialog);

        QMetaObject::connectSlotsByName(Dialog);
    } // setupUi

    void retranslateUi(QDialog *Dialog)
    {
        Dialog->setWindowTitle(QCoreApplication::translate("Dialog", "Dialog", nullptr));
        label->setText(QCoreApplication::translate("Dialog", "Platform Posts", nullptr));
        postText->setHtml(QCoreApplication::translate("Dialog", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Noto Sans'; font-size:10pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">No selection</p></body></html>", nullptr));
        save->setText(QCoreApplication::translate("Dialog", "Save", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Dialog: public Ui_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_POSTDIALOG_H
