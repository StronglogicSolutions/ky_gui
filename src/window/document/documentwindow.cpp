#include "include/ui/documentwindow.hpp"
#include "ui_documentwindow.h"
#include <QDebug>

DocumentWindow::DocumentWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::DocumentWindow)
{
  ui->setupUi(this);
}

DocumentWindow::~DocumentWindow()
{
  delete ui;
}

void DocumentWindow::set_flags(const QVector<QString>& flags)
{
  qDebug() << "Setting flags in Document Window";
  m_flags = flags;
}
