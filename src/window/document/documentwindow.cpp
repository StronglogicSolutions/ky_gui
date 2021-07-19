#include "include/ui/documentwindow.hpp"
#include "ui_documentwindow.h"
#include <QDebug>
#include <QList>
#include <QTableWidget>
#include <QTextDocument>
#include <QPrinter>
#include <QTimer>
#include "headers/util.hpp"

static QBrush GetBrushForType(const RowType& type)
{
  return (type == RowType::REPEAT) ?
    QBrush{QColor{"grey"}} :
    QBrush{QColor{"red"}};
}

static void SaveTableAsPDF(QTableWidget* t)
{
  QTextDocument doc;

  QString text("<table><thead>");
  text.append("<tr>");
  for (int i = 0; i < t->columnCount(); i++)
    text.append("<th>")
        .append(t->horizontalHeaderItem(i)->data(Qt::DisplayRole).toString())
        .append("</th>");

  text.append("</tr></thead>");
  text.append("<tbody>");

  for (int i = 0; i < t->rowCount(); i++) {
    text.append("<tr>");
    for (int j = 0; j < t->columnCount(); j++) {
      QTableWidgetItem *item = t->item(i, j);
      if (!item || item->text().isEmpty())
        t->setItem(i, j, new QTableWidgetItem("0"));

      text.append("<td>")
          .append(t->item(i, j)->text())
          .append("</td>");
    }
    text.append("</tr>");
  }

  text.append("</tbody></table>");
  doc.setHtml(text);

  QPrinter      printer{QPrinter::PrinterResolution};

  printer.setOutputFormat(QPrinter::PdfFormat);
  printer.setPaperSize(QPrinter::A4);
  printer.setOrientation(QPrinter::Landscape);
  printer.setOutputFileName("test.pdf");

  doc.setPageSize(printer.pageRect().size());

  doc.print(&printer);
}

DocumentWindow::DocumentWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::DocumentWindow),
  m_flag_index(-1),
  m_inserting(false),
  m_row_types(QList<RowType>{RowType::REPEAT})
{
  ui->setupUi(this);

  ui->startDateTime->setEnabled(false);
  ui->endDateTime  ->setEnabled(false);
  ui->rowCount     ->setEnabled(false);

  for (auto i = 0; i < ui->rowContent->columnCount(); i++)
  {
    ui->rowContent->setItem(0, i, new QTableWidgetItem{});
    ui->rowContent->item(0, i)->setBackground(GetBrushForType(RowType::REPEAT));
  }

  QObject::connect(ui->rowCountActive, &QCheckBox::clicked, this, [this](const bool checked) -> void
  {
    ui->rowCount->setEnabled(checked);
  });

  QObject::connect(ui->dateRangeActive, &QCheckBox::clicked, this, [this](const bool checked) -> void
  {
    ui->startDateTime->setEnabled(checked);
    ui->endDateTime  ->setEnabled(checked);
  });

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
      ui->rowContent->item(row, col)->setBackground(GetBrushForType(m_row_types.at(row)));
      set_inserting(false);
    }
  );

  QObject::connect(ui->rowContent->verticalHeader(), &QHeaderView::sectionClicked, this,
    [this](int32_t index)
  {
    RowType row_type = m_row_types.at(index);
    for (int i = 0; i < ui->rowContent->columnCount(); i++)
      if (row_type == RowType::HEADER)
      {
        ui->rowContent->item(index, i)->setBackground(GetBrushForType(RowType::REPEAT));
        m_row_types[index] = RowType::REPEAT;
      }
      else
      {
        ui->rowContent->item(index, i)->setBackground(GetBrushForType(RowType::HEADER));
        m_row_types[index] = RowType::HEADER;
      }

  });

  QObject::connect(ui->rowContent, &QTableWidget::itemChanged, this,
    [this](QTableWidgetItem* item)
  {
    auto type = item->Type;
    auto text = item->text();
  });

  QObject::connect(ui->createPDF, &QPushButton::clicked, this,
    [this]()
  {
    if (!ui->rowCountActive->isChecked() && !ui->dateRangeActive->isChecked())
    {
      ui->mandatoryLabel->setStyleSheet("color: red");
      UI::infoMessageBox("Results must be filtered by date or rowcount");
      QTimer::singleShot(5000, this, [this]() -> void { ui->mandatoryLabel->setStyleSheet("color: #FFF;"); });
    }
    else
      SaveTableAsPDF(ui->rowContent);
  });

  QObject::connect(ui->addRow, &QPushButton::clicked, this,
    [this]()
  {
    add_row();
  });

  QObject::connect(ui->removeRow, &QPushButton::clicked, this,
    [this]()
  {
    auto count = ui->rowContent->rowCount();
    if (count)
    {
      ui->rowContent->setRowCount((count - 1));
      m_row_types.pop_back();
    }
  });

  QObject::connect(ui->addColumn, &QPushButton::clicked, this,
    [this]()
  {
    add_column();
  });

  QObject::connect(ui->removeColumn, &QPushButton::clicked, this,
    [this]()
  {
    if (ui->rowContent->columnCount() > 0)
      ui->rowContent->setColumnCount(ui->rowContent->columnCount() - 1);
  });
}

DocumentWindow::~DocumentWindow()
{
  delete ui;
}

void DocumentWindow::set_flags(const QList<QString>& flags)
{
  qDebug() << "Setting flags in Document Window";
  m_flags = flags;
  ui->tokens->addItems(flags);
  setCursor(Qt::CursorShape::ArrowCursor);
}

void DocumentWindow::set_inserting(const bool inserting, const int32_t& index)
{
  m_flag_index = index;
  m_inserting  = inserting;
  setCursor(m_inserting ? Qt::CursorShape::CrossCursor : Qt::CursorShape::ArrowCursor);
}

void DocumentWindow::add_row()
{
  auto count = ui->rowContent->rowCount();
  ui->rowContent->setRowCount(count + 1);
  for (auto i = 0; i < ui->rowContent->columnCount(); i++)
  {
    ui->rowContent->setItem(count, i, new QTableWidgetItem{});
    ui->rowContent->item(count, i)->setBackground(GetBrushForType(RowType::REPEAT));
  }
  m_row_types.push_back(RowType::REPEAT);
}

void DocumentWindow::add_column()
{
  auto count = ui->rowContent->columnCount();
  ui->rowContent->setColumnCount(count + 1);
  for (auto i = 0; i < ui->rowContent->rowCount(); i++)
  {
    ui->rowContent->setItem(i, count, new QTableWidgetItem{});
    ui->rowContent->item(i, count)->setBackground(GetBrushForType(m_row_types.at(i)));
  }
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
