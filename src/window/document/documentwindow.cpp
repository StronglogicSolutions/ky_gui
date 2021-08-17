#include "include/ui/documentwindow.hpp"
#include "ui_documentwindow.h"
#include <QDebug>
#include <QList>
#include <QPrinter>
#include <QTextDecoder>
#include <QTextCursor>
#include <QTimer>
#include "headers/util.hpp"

/**
 * @brief GetBrushForType
 * @param type
 * @return
 */
static QBrush GetBrushForType(const RowType& type)
{
  return (type == RowType::REPEAT) ?
    QBrush{QColor{"grey"}} :
    QBrush{QColor{"red"}};
}

/**
 * @brief IsImage
 * @param s
 * @return
 */
static bool IsImage(const QString& s)
{
  return (s == "--media=$FILE_TYPE");
}

/**
 * @brief CloseTable
 * @param doc_ptr
 */
static void CloseTable(QTextDocument* doc_ptr)
{
  QTextCursor cursor{doc_ptr};
  cursor.movePosition(QTextCursor::End);
  cursor.insertHtml("</tbody></table>");
}

/**
 * @brief DocumentWindow::SaveSection
 */
void DocumentWindow::SaveSection()
{
  QString text;
  QTableWidget* t = &m_table;

  if (m_doc.isEmpty())
  {
    text = QString{"<table><thead>"};
    text.append("<tr>");

    for (int i = 0; i < t->columnCount(); i++)
      text.append("<th>")
          .append(t->horizontalHeaderItem(i)->data(Qt::DisplayRole).toString())
          .append("</th>");
    text.append("</tr></thead>");
    text.append("<tbody>");
  }

  for (int i = 0; i < t->rowCount(); i++) {
    text.append("<tr>");
    for (int j = 0; j < t->columnCount(); j++)
    {
      if (ImageAtCell(i, j))
      {
        /** Rendering Image (see example at end of method)
         *
         * 1. QCursor
         * 2. Inject HTML thus far
         * 3. Render QPixMap
         * 4. Inject rendered QPixMap
         * 4. clear text and resume
         *
         */
      }
      else
      {
        QTableWidgetItem* item = t->item(i, j);
        if (!item || item->text().isEmpty())
          t->setItem(i, j, new QTableWidgetItem("0"));

        text.append("<td>")
          .append(t->item(i, j)->text())
          .append("</td>");
      }
    }
    text.append("</tr>");
  }

  QTextCursor cursor{&m_doc};
  cursor.movePosition(QTextCursor::End);
  cursor.insertHtml(text);
  /** Adding image to QTextDocument
    *
    *           QTextCursor cursor(&document);
                QPalette palette;
                palette.setColor(backgroundRole(), QColor(255, 255, 255));
                this->ui->tab_3->setPalette(palette);
                this->ui->tab_3->setAutoFillBackground(true);

                QPixmap pm = this->ui->tab_3->grab();
                pm = pm.transformed(QTransform().rotate(270));

                cursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor);

                QImage img = pm.toImage();
                img = img.scaledToHeight(img.height()-60);
                cursor.insertImage(img);

                this->ui->tab_3->setAutoFillBackground(false);
  */

  /**
    * Getting QPixMap from a QTableWidgetItem (cell)
    *
    * QPixmap m = item->data(Qt::DecorationRole).value<QPixmap>();
    *
  */
}

/**
 * @brief DocumentWindow::DocumentWindow
 * @constructor
 * @param parent
 */
DocumentWindow::DocumentWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::DocumentWindow),
  m_flag_index(-1),
  m_inserting(false),
  m_row_types(QList<RowType>{RowType::REPEAT}),
  m_printer(QPrinter::PrinterResolution)
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

  m_printer.setOutputFormat(   QPrinter::PdfFormat);
  m_printer.setPageSize(       QPageSize{QPageSize::PageSizeId::A4});
  m_printer.setPageOrientation(QPageLayout::Landscape);
  m_printer.setOutputFileName("test.pdf");

  m_doc.setPageSize(m_printer.pageRect().size());
//  doc.setPageSize(printer.pageLayout().paintRectPixels());

  /**
   * activateRowCount
   * @lambda
   */
  QObject::connect(ui->rowCountActive, &QCheckBox::clicked, this, [this](const bool checked) -> void
  {
    ui->rowCount->setEnabled(checked);
  });

  /**
   * activateDateRange
   * @lambda
   */
  QObject::connect(ui->dateRangeActive, &QCheckBox::clicked, this, [this](const bool checked) -> void
  {
    ui->startDateTime->setEnabled(checked);
    ui->endDateTime  ->setEnabled(checked);
  });

  /**
   * onTokenSelect
   * @lambda
   */
  QObject::connect(ui->tokens, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this,
    [this](int index)
    {
      SetInserting(true, index);
    }
  );

  /**
   * setCellValue
   * @lambda
   */
  QObject::connect(ui->rowContent, &QTableWidget::clicked, this,
    [this](const QModelIndex& index)
    {
      if (!m_inserting) return;
      auto col    = index.column();
      auto row    = index.row();
      if (!m_file_path.isEmpty())
      {
        QTableWidgetItem* file_item = new QTableWidgetItem();
        QPixmap pm{m_file_path};
        file_item->setData(
            Qt::DecorationRole,
            pm.scaledToHeight(ui->rowContent->rowHeight(0),
                              Qt::TransformationMode::SmoothTransformation));
        ui->rowContent->setItem(row, col, file_item);
        m_file_path.clear();
      }
      else
      {
        auto flag   = m_flags.at(m_flag_index);
        auto widget = new QTableWidgetItem{};
        widget->setText(flag.right(flag.size() - flag.indexOf('=') - 1));
        ui->rowContent->setItem(row, col, widget);
        ui->rowContent->item(row, col)->setBackground(GetBrushForType(m_row_types.at(row)));
        SetInserting(false);
      }
    }
  );

  /**
   * setRowRepeating
   * @lambda
   */
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

  /**
   * onCellValueChanged
   * @lambda
   */
  QObject::connect(ui->rowContent, &QTableWidget::itemChanged, this,
    [this](QTableWidgetItem* item)
  {
    auto type = item->Type;
    auto text = item->text();
  });

  /**
   *  savePDF
   *  @lambda
   */
  QObject::connect(ui->createPDF, &QPushButton::clicked, this,
    [this]()
  {
    CloseTable(&m_doc);
  });

  /**
   * addRow
   * @lambda
   */
  QObject::connect(ui->addRow, &QPushButton::clicked, this,
    [this]()
  {
    AddRow();
  });

  /**
   * removeRow
   * @lambda
   */
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

  /**
   * addColumn
   * @lambda
   */
  QObject::connect(ui->addColumn, &QPushButton::clicked, this,
    [this]()
  {
    AddColumn();
  });

  /**
   * removeColumn
   * @lambda
   */
  QObject::connect(ui->removeColumn, &QPushButton::clicked, this,
    [this]()
    {
      if (ui->rowContent->columnCount())
        ui->rowContent->setColumnCount(ui->rowContent->columnCount() - 1);
    });

  /**
   * addFile
   * @lambda
   */
  QObject::connect(ui->addFile, &QPushButton::clicked, this,
    [this]() -> void
    {
      KFileDialog file_dialog{};
      auto file_path = file_dialog.openFileDialog(m_file_path);
      qDebug() << "Selected file:" << file_path;

      if (file_path.size())
      {
       auto    slash_index = file_path.lastIndexOf("/") + 1;
       QString file_name = file_path.right(file_path.size() - slash_index);
       QString dir = file_path.left(slash_index);
       QFile   file{file_path};

       if (file.open(QIODevice::ReadOnly)) {
         if (QMimeDatabase{}.mimeTypeForFile(file_path).name().contains("video"))
         {
           UI::infoMessageBox("Only pictures can be added to the table");
           return;
         }
         m_file_path = file_path;
         SetInserting(true);
       } else {
         qDebug() << "Unable to open selected file";
         QMessageBox::warning(this, tr("File Error"), tr("Unable to open selected file"));
       }
      } else {
       qDebug() << "Could not read the file path";
       QMessageBox::warning(this, tr("File Error"), tr("Could not read the file path"));
      }
    }
  );

  /**
   * saveTableSection
   * @lambda
   */
  QObject::connect(ui->saveTable, &QPushButton::clicked, this,
    [this]()
    {
    /**
     * DateTimesToRange
     * @lambda
     *
     * Produces a KIQ-compatible date range string from two pointers to QDateTimeEdit objects
     */
      const auto DatetimesToRange = [](const QDateTimeEdit* s, const QDateTimeEdit* e) -> QString
      {
        return QString{QString::number(s->dateTime().toTime_t()) + 'TO' + QString::number(e->dateTime().toTime_t())};
      };

      const bool row_count_active  = ui->rowCountActive ->isChecked();
      const bool date_range_active = ui->dateRangeActive->isChecked();

      if (!row_count_active && !date_range_active)
      {
        ui->mandatoryLabel->setStyleSheet("color: red");
        UI::infoMessageBox("Results must be filtered by date or rowcount");
        QTimer::singleShot(5000, this, [this]() -> void { ui->mandatoryLabel->setStyleSheet("color: #FFF;"); });
      }
      else
        // Row count
        // Date range
        // send request for data / files
        // onDataArriva => add to vector of QString html
      {
        const auto             date_range = (date_range_active) ? DatetimesToRange(ui->startDateTime, ui->endDateTime) : "0TO0";
        const auto             row_count  = (row_count_active) ? ui->rowCount->text() : "0";
        const auto             max_id     = QString::number(ui->lastID->intValue());
        const auto             order      = ui->order->currentText();
        const QVector<QString> argv{date_range, row_count, max_id, order};

        emit RequestData(argv);
      }
    }
  );

}

/**
 * ~DocumentWindow
 * @destructor
 */
DocumentWindow::~DocumentWindow()
{
  delete ui;
}

/**
 * @brief DocumentWindow::SetFlags
 * @param[in] flags {QList<QString>
 */
void DocumentWindow::SetFlags(const QList<QString>& flags)
{
  qDebug() << "Setting flags in Document Window";
  m_flags = flags;
  ui->tokens->addItems(flags);
  setCursor(Qt::CursorShape::ArrowCursor);
}

/**
 * @brief DocumentWindow::SetInserting
 * @param[in] inserting {bool}
 * @param[in] index     {int32_t}
 */
void DocumentWindow::SetInserting(const bool inserting, const int32_t& index)
{
  if (m_file_path.isEmpty())
  {
    m_flag_index = index;
  }
  m_inserting  = inserting;
  setCursor(m_inserting ? Qt::CursorShape::CrossCursor : Qt::CursorShape::ArrowCursor);
}

/**
 * @brief DocumentWindow::AddRow
 */
void DocumentWindow::AddRow()
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

/**
 * @brief DocumentWindow::AddColumn
 */
void DocumentWindow::AddColumn()
{
  auto count = ui->rowContent->columnCount();
  ui->rowContent->setColumnCount(count + 1);
  for (auto i = 0; i < ui->rowContent->rowCount(); i++)
  {
    ui->rowContent->setItem(i, count, new QTableWidgetItem{});
    ui->rowContent->item(i, count)->setBackground(GetBrushForType(m_row_types.at(i)));
  }
}

/**
 * @brief DocumentWindow::mouseReleaseEvent
 * @param e
 *
 * Toggles insert mode
 */
void DocumentWindow::mouseReleaseEvent(QMouseEvent* e)
{
  if (e->button() == Qt::LeftButton)
  {
    qDebug() << "Mouse pressed";
    if (m_inserting)
      SetInserting(false);
  }
}

/**
 * @brief DocumentWindow::ReceiveData
 * @param [in] message {QString}
 * @param [in] data    {QVector<QString>}
 */
void DocumentWindow::ReceiveData(const QString& message, const QVector<QString>& data)
{
  if (message == "Task Data")
  {
    const int32_t task_count = data.front().toInt();
    const int32_t arg_count  = ((data.size() - 1) / task_count);

    for (int32_t i = 1; i < task_count; i += arg_count)
    {
      const auto id = data.at(i);
      TaskFlags  flags{};

      for (int32_t j = 0; j < arg_count; j++)
        flags.insert(data.at(i + j), data.at(i + j + 1));

      m_tasks.insert(id, TaskData{.flags = flags});
      m_task_index++;
    }
  }
  else
  if (message == "Task Data Final")
  {
    qDebug() << "Still waiting for files before rendering";
  }
}

/**
 * @brief DocumentWindow::ImageAtCell
 * @param row
 * @param col
 * @return
 */
bool DocumentWindow::ImageAtCell(int32_t row, int32_t col)
{
  const auto it = m_image_coords.find(row);
  return (it != m_image_coords.cend() && it.value() == col);
}

/**
 * @brief DocumentWindow::SavePDF
 */
void DocumentWindow::SavePDF()
{
  m_doc.print(&m_printer);
}

/**
 * @brief DocumentWindow::ReceiveFiles
 * @param [in] files {QVector<FileWrap>}
 */
void DocumentWindow::ReceiveFiles(QVector<FileWrap>&& files)
{

  for (auto&& file : files)
  {
    auto it = m_tasks.find(file.id);
    if (it != m_tasks.end())
     it->files.push_back(std::move(file));
    else
      throw std::invalid_argument{"Task with matching ID not found"};
  }

  QTableWidget* t = ui->rowContent;
  for (TaskMap::iterator it = m_tasks.begin(); it != m_tasks.end(); it++)
  {
    const auto task = *it;
    for (auto row = 0; row < t->rowCount(); row++)
    {
      const bool should_insert = m_row_types.at(row) == RowType::REPEAT;
      if (should_insert)
      {
        for (auto col = 0; col < t->columnCount(); col++)
        {
          const auto item = t->item(row, col);
          if (!item->text().isEmpty())
          {
            const bool image = IsImage(item->text());
            if (image)
              m_image_coords.insert(row, col);
              // TODO: Render here, or when saving section (or both)
            else
            {
              const auto f_it    = task.flags.find(item->text());
              const bool found = f_it != task.flags.cend();

              if (found)
              {
                const auto value = f_it.value();
                auto widget = new QTableWidgetItem{};
                widget->setText(value);
                m_table.setItem(row, col, widget);
              }
            }
          }
        }
      }
    }
  }

  SaveSection();
}

void DocumentWindow::keyPressEvent(QKeyEvent* e)
{
  if (e->key() == Qt::Key_Escape)
    close();
}
