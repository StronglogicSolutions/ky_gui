#include "include/ui/documentwindow.hpp"
#include "ui_documentwindow.h"
#include <QDebug>
#include <QList>
#include <QTableWidget>

DocumentWindow::DocumentWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::DocumentWindow),
  m_flag_index(-1),
  m_inserting(false)
{
  ui->setupUi(this);

  QObject::connect(ui->tokens, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this,
    [this](int index)
    {
      set_inserting(true, index);
    }
  );

  QObject::connect(ui->rowContent, &QTableWidget::clicked, this,
    [this](const QModelIndex& index)
    {
      if (!m_inserting) return;

      auto col    = index.column();
      auto row    = index.row();
      auto flag   = m_flags.at(m_flag_index);
      auto widget = new QTableWidgetItem{};
      widget->setText(flag.right(flag.size() - flag.indexOf('=') - 1));
      ui->rowContent->setItem(row, col, widget);
      set_inserting(false);
    }
  );
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

void DocumentWindow::set_inserting(const bool inserting, const int32_t& index)
{
  m_flag_index = index;
  m_inserting  = inserting;
  setCursor(m_inserting ? Qt::CursorShape::CrossCursor : Qt::CursorShape::ArrowCursor);
}


void DocumentWindow::mouseReleaseEvent(QMouseEvent* e)
{
  if (e->button() == Qt::LeftButton)
  {
    qDebug() << "Mouse pressed";
    if (m_inserting)

      set_inserting(false);

  }
}
