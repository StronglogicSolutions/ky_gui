/********************************************************************************
** Form generated from reading UI file 'researchwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RESEARCHWINDOW_H
#define UI_RESEARCHWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ResearchWindow
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout_2;
    QGridLayout *gridLayout;
    QTableWidget *termTable;
    QLabel *label;
    QMenuBar *menubar;
    QMenu *menuResearch_Window;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *ResearchWindow)
    {
        if (ResearchWindow->objectName().isEmpty())
            ResearchWindow->setObjectName(QString::fromUtf8("ResearchWindow"));
        ResearchWindow->resize(720, 580);
        centralwidget = new QWidget(ResearchWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        gridLayout_2 = new QGridLayout(centralwidget);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        termTable = new QTableWidget(centralwidget);
        if (termTable->columnCount() < 7)
            termTable->setColumnCount(7);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        termTable->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        termTable->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        termTable->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        termTable->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        termTable->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        termTable->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        termTable->setHorizontalHeaderItem(6, __qtablewidgetitem6);
        if (termTable->rowCount() < 1)
            termTable->setRowCount(1);
        termTable->setObjectName(QString::fromUtf8("termTable"));
        termTable->setRowCount(1);
        termTable->setColumnCount(7);
        termTable->horizontalHeader()->setDefaultSectionSize(100);
        termTable->verticalHeader()->setDefaultSectionSize(60);

        gridLayout->addWidget(termTable, 1, 0, 1, 1);

        label = new QLabel(centralwidget);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);


        gridLayout_2->addLayout(gridLayout, 0, 0, 1, 1);

        ResearchWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(ResearchWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 720, 23));
        menuResearch_Window = new QMenu(menubar);
        menuResearch_Window->setObjectName(QString::fromUtf8("menuResearch_Window"));
        ResearchWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(ResearchWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        ResearchWindow->setStatusBar(statusbar);

        menubar->addAction(menuResearch_Window->menuAction());

        retranslateUi(ResearchWindow);

        QMetaObject::connectSlotsByName(ResearchWindow);
    } // setupUi

    void retranslateUi(QMainWindow *ResearchWindow)
    {
        ResearchWindow->setWindowTitle(QCoreApplication::translate("ResearchWindow", "MainWindow", nullptr));
        QTableWidgetItem *___qtablewidgetitem = termTable->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("ResearchWindow", "ID", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = termTable->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("ResearchWindow", "Term", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = termTable->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("ResearchWindow", "Type", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = termTable->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("ResearchWindow", "User", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = termTable->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("ResearchWindow", "Person", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = termTable->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("ResearchWindow", "Organization", nullptr));
        QTableWidgetItem *___qtablewidgetitem6 = termTable->horizontalHeaderItem(6);
        ___qtablewidgetitem6->setText(QCoreApplication::translate("ResearchWindow", "Time", nullptr));
        label->setText(QCoreApplication::translate("ResearchWindow", "Term Hit Events", nullptr));
        menuResearch_Window->setTitle(QCoreApplication::translate("ResearchWindow", "Research Window", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ResearchWindow: public Ui_ResearchWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RESEARCHWINDOW_H
