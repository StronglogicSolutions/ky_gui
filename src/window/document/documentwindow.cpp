#include "include/ui/documentwindow.hpp"
#include "ui_documentwindow.h"
#include <QDebug>
#include <QList>
#include <QTableWidget>

DocumentWindow::DocumentWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::DocumentWindow),
  m_inserting(-1)
{
  ui->setupUi(this);

  QObject::connect(ui->tokens, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this,
    [this](int index)
    {
      set_inserting(index);
    }
  );

  QObject::connect(ui->rowContent, &QTableWidget::clicked, this, [this](const QModelIndex& index)
  {
    auto col = index.column();
    auto row = index.row();
    auto widget = new QTableWidgetItem{};
    widget->setText(m_flags.at(m_inserting));
    ui->rowContent->setItem(row, col, widget);
  });
}

DocumentWindow::~DocumentWindow()
{
  delete ui;
}

void DocumentWindow::set_flags(const QVector<QString>& flags)
{
  qDebug() << "Setting flags in Document Window";
  m_flags = flags;
  ui->tokens->addItems(flags.toList());
  setCursor(Qt::CursorShape::ArrowCursor);
}

void DocumentWindow::set_inserting(const int32_t& index)
{
  m_inserting = index;
  setCursor(Qt::CursorShape::CrossCursor);
}


void DocumentWindow::mouseReleaseEvent(QMouseEvent* e)
{
  if (e->button() == Qt::LeftButton)
  {
    qDebug() << "Mouse pressed";
    if (m_inserting > -1)
    {
      set_inserting(-1);
      setCursor(Qt::CursorShape::ArrowCursor);
    }
  }
}
