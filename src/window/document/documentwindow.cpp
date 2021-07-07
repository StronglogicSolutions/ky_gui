#include "include/ui/documentwindow.hpp"
#include "ui_documentwindow.h"

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
