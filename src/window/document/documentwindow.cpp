#include "include/ui/documentwindow.hpp"
#include "ui_documentwindow.h"
#include <QDebug>
#include <QList>
#include <QPrinter>
#include <QPrintDialog>
#include <QTextDecoder>
#include <QTextTable>
#include <QScreen>
#include <QTimer>
#include <QDesktopWidget>
#include <QCalendarWidget>
#include <QAbstractTextDocumentLayout>
#include "headers/util.hpp"

static const uint32_t DEFAULT_HEIGHT{240};

static void CenterWidget(QWidget *widget)
{
  const auto host = widget->parentWidget();

  if (host)
  {
    const auto hostRect = host->geometry();
    widget->move(hostRect.center() - widget->rect().center());
  }
  else
  {
    QScreen* screen = widget->window()->screen();
    const int x = (screen->geometry().width()  - widget->width())  / 2;
    const int y = (screen->geometry().height() - widget->height()) / 2;
    widget->move(x, y);
  }
}

static const FileWrap& FindImage(const QVector<FileWrap>& files)
{
  for (const auto& file : files)
  {
    if (file.type == "image")
      return file;
  }
}

/**
 * @brief DocumentWindow::DocumentWindow
 * @constructor
 * @param parent
 */
DocumentWindow::DocumentWindow(QWidget *parent)
: QMainWindow(parent),
  ui(new Ui::DocumentWindow),
  m_flag_index(-1),
  m_inserting(false),
  m_row_types(QList<RowType>{RowType::REPEAT}),
  m_printer(QPrinter::PrinterResolution),
  m_img_height(0),
  m_scale_images(true)
{
  ui->setupUi(this);
  SetDatePickers();
  SetTable();
  SetPrinter();
  SetListeners();
}

/**
 * ~DocumentWindow
 * @destructor
 */
DocumentWindow::~DocumentWindow()
{
  delete ui;
}

void DocumentWindow::SetDatePickers()
{
  ui->startDateTime->setEnabled(false);
  ui->endDateTime  ->setEnabled(false);
  ui->startDateTime->setDateTime(QDateTime::currentDateTime());
  ui->endDateTime  ->setDateTime(QDateTime::currentDateTime());

  QObject::connect(ui->startPick, &QPushButton::clicked, this, [this]() -> void
  {
    QCalendarWidget* widget = new QCalendarWidget();
    QObject::connect(widget, &QCalendarWidget::selectionChanged, widget, [this, widget]() -> void
    {
      ui->startDateTime->setDateTime(widget->selectedDate().startOfDay());
      widget->close();
    });
    CenterWidget(widget);
    widget->show();
  });

  QObject::connect(ui->endPick, &QPushButton::clicked, this, [this]() -> void
  {
    QCalendarWidget* widget = new QCalendarWidget();
    QObject::connect(widget, &QCalendarWidget::selectionChanged, widget, [this, widget]() -> void
    {
      ui->endDateTime->setDateTime(widget->selectedDate().startOfDay());
      widget->close();
    });
    CenterWidget(widget);
    widget->show();
  });
}

void DocumentWindow::SetTable()
{
  m_printer.setPageSize       (QPageSize{QPageSize::PageSizeId::A4});
  m_printer.setOutputFormat   (QPrinter::PdfFormat);
  m_printer.setPageOrientation(QPageLayout::Landscape);
  m_printer.setOutputFileName("kiq_document.pdf"); // TODO: Set from UI
  m_printer.setColorMode(QPrinter::Color);
  m_svg    .setFileName      ("kiq_document.svg");
  m_doc.setPageSize(m_printer.pageRect().size());
  m_doc.setDefaultStyleSheet(QString{"color: black;"});

}

void DocumentWindow::SetPrinter()
{
  ui->rowCount     ->setEnabled(false);
  m_table          .setColumnCount(ui->rowContent->columnCount());
  for (auto i = 0; i < ui->rowContent->columnCount(); i++)
  {
    ui->rowContent->setItem(0, i, new QTableWidgetItem{});
    m_table        .setHorizontalHeaderItem(i, new QTableWidgetItem{});
    ui->rowContent->item(0, i)->setBackground(GetBrushForType(RowType::REPEAT));
  }
}

void DocumentWindow::SetListeners()
{
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
    });

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
        file_item     ->setData(Qt::DecorationRole, QPixmap{m_file_path}.scaledToHeight(240));
        ui->rowContent->setItem(row, col, file_item);
        m_image_coords.insert(QPair{row, col}, QMimeDatabase().mimeTypeForName(m_file_path));
        m_file_path.clear();
      }
      else
      {
        auto flag   = m_flags.at(m_flag_index);
        auto widget = new QTableWidgetItem{};
        widget->setText(flag.right(flag.size() - flag.indexOf('=') - 1));
        ui->rowContent->setItem(row, col, widget);
        ui->rowContent->item(row, col)->setBackground(GetBrushForType(m_row_types.at(row)));
      }
      SetInserting(false);
    });

  /**
   * setRowRepeating
   * @lambda
   */
  QObject::connect(ui->rowContent->verticalHeader(), &QHeaderView::sectionClicked, this,
    [this](int32_t index)
    {
      ToggleRow(index);
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
   *  createPDF
   *  @lambda
   */
  QObject::connect(ui->createPDF, &QPushButton::clicked, this,
    [this]()
    {
      SavePDF();
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
       QString file_name   = file_path.right(file_path.size() - slash_index);
       QString dir         = file_path.left(slash_index);
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
    });

  /**
   * saveTableSection
   * @lambda
   */
  QObject::connect(ui->saveTable, &QPushButton::clicked, this,
    [this]() -> void
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
      {
        m_fetch_files                     = FindInTable(ui->rowContent, "$FILE_TYPE");
        const auto             date_range = (date_range_active) ? DatetimesToRange(ui->startDateTime, ui->endDateTime) : "0TO0";
        const auto             row_count  = (row_count_active) ? ui->rowCount->text() : "0";
        const auto             max_id     = QString::number(ui->lastID->intValue());
        const auto             order      = ui->order->currentText();
        const QVector<QString> argv{date_range, row_count, max_id, order};

        emit RequestData(argv);
      }
    });

  QObject::connect(ui->runTest, &QPushButton::clicked, this,
    [this]() -> void
    {
      AddColumn();
      AddRow();
      ToggleRow(0);
      ui->rowContent->setItem(0, 0, new QTableWidgetItem{"User"});
      ui->rowContent->setItem(0, 1, new QTableWidgetItem{"Description"});
      ui->rowContent->setItem(0, 2, new QTableWidgetItem{"Image"});
      ui->rowContent->setItem(1, 0, new QTableWidgetItem{"$USER"});
      ui->rowContent->setItem(1, 1, new QTableWidgetItem{"$DESCRIPTION"});
      ui->rowContent->setItem(1, 2, new QTableWidgetItem{"$FILE_TYPE"});
      ui->rowCountActive->toggle();
      ui->rowCount->setValue(3);
    });

  /**
   * scaleImages
   * @lambda
   */
  QObject::connect(ui->scaleImages, &QCheckBox::clicked, this, [this](const bool checked) -> void
  {
    ui->imageHeight->setEnabled(checked);
    m_scale_images = checked;
  });

  /**
   * setImageHeight
   * @lambda
   */
  QObject::connect(ui->imageHeight, &QLineEdit::textChanged, this,
    [this](const QString& s) -> void
    {
      if (s == "auto")
      {
        m_img_height = 0;
      }
      else
      if (s == "0")
      {
        m_img_height = 0;
        ui->imageHeight->setText("auto");
      }
      else
        m_img_height = s.toUInt();
    });
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
    m_table.setVerticalHeaderItem(i, new QTableWidgetItem{});
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
  const auto count = ui->rowContent->columnCount();
  ui->rowContent->setColumnCount(count + 1);
  m_table        .setColumnCount(count + 1);
  m_table        .setHorizontalHeaderItem(count, new QTableWidgetItem{});
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
 * @brief DocumentWindow::SaveSection
 * Uses the intermediary table (m_table) to build our final render table.
 * Renders media in cells as needed.
 *
 * TODO: render non-task images
 */
void DocumentWindow::SaveSection()
{
  KLOG("Saving section");
  QTextCursor   cursor{&m_doc};
  int32_t       row_idx{};
  const auto    row_count    = m_table.rowCount();
  const auto    col_count    = m_table.columnCount();
  const auto    top_count    = (row_count - m_tasks.size());
  QTableWidget* t            = &m_table;
  QTextTable*   render_table = cursor.insertTable(row_count, col_count);

  for (; row_idx < top_count; row_idx++)
    for (auto col = 0; col < col_count; col++)
    {
      QTextTableCell cell    = render_table->cellAt(row_idx, col);
      if (ImageAtCell(row_idx, col))
      {
        const auto image = ui->rowContent->item(row_idx, col)->data(Qt::DecorationRole);
        cell.firstCursorPosition().insertImage((m_scale_images) ?
                image.value<QPixmap>().toImage().scaledToHeight((m_img_height) ? m_img_height : DEFAULT_HEIGHT) :
                image.value<QPixmap>().toImage());
      }
      else
        cell.firstCursorPosition().insertText(t->item(row_idx, col)->text());
    }

  for (TaskMap::Iterator it = m_tasks.begin(); it != m_tasks.end(); it++)
  {
    for (auto i = top_count; i < ui->rowContent->rowCount(); i++, row_idx++)
    {
      for (auto j = 0; j < col_count; j++)
      {
        const auto     col_idx = j;
        QTextTableCell cell    = render_table->cellAt(row_idx, col_idx);

        if (ImageAtCell(i, j))
        {
          if (it->files.isEmpty()) continue;

          const FileWrap& file = FindImage(it->files);
          QPixmap         pm{};
          pm.loadFromData(file.buffer, QMimeDatabase{}.mimeTypeForName(file.name).preferredSuffix().toUtf8());
          cell.firstCursorPosition().insertImage(pm.toImage().scaledToHeight(240));
        }
        else
          cell.firstCursorPosition().insertText(t->item(row_idx, col_idx)->text());
      }
    }
  }
}

/**
 * @brief DocumentWindow::RenderSection
 *
 * Uses the template table and task data to build our intermediary table.
 * The intermediary has header rows at the top, and all content rows (REPEAT type) below.
 *
 */
void DocumentWindow::RenderSection()
{
  KLOG("Rendering section");
  int32_t rows_add{};
  int32_t rows_mul{};
  for (const auto& row_type : m_row_types)
    if (row_type == RowType::HEADER)
      rows_add++;
  rows_mul = (m_row_types.size() - rows_add);
  m_table.setRowCount((m_tasks.size() * rows_mul) + rows_add);

  QTableWidget* t       = ui->rowContent;
  int32_t       row_idx = 0;
  const auto    rows    = t->rowCount();
  const auto    cols    = t->columnCount();

  for (auto row = 0; row < m_row_types.size(); row++)
  {
    if (m_row_types.at(row) == RowType::HEADER)
    { // TODO: Check if text or media
      for (auto col = 0; col < cols; col++)

        m_table.setItem(row_idx, col, new QTableWidgetItem{t->item(row, col)->text()});
     row_idx++;
    }
  }

  for (TaskMap::iterator it = m_tasks.begin(); it != m_tasks.end(); it++, row_idx++)
  {
    const auto task = *it;
    for (auto row = 0; row < rows; row++)
    {
      const bool should_insert = m_row_types.at(row) == RowType::REPEAT;
      if (should_insert)
      {
        for (auto col = 0; col < cols; col++)
        {
          const auto item   = t->item(row, col);
          auto       widget = new QTableWidgetItem{};
          const bool image  = IsImage(item->text());
          if (image && !task.files.empty())
            m_image_coords.insert({row, col}, QMimeDatabase{}.mimeTypeForName(FindImage(task.files).name));
          else
          {
            const auto text  = item->text().isEmpty() ? "" : item->text().remove(0, 1);
            const auto f_it  = task.flags.find(text);
            const bool found = f_it != task.flags.cend();

            if (found)
              widget->setText(f_it.value());
          }
          m_table.setItem(row_idx, col, widget);
        }
      }
    }
  }
  SaveSection();
}

/**
 * @brief DocumentWindow::ReceiveData
 * @param [in] message {QString}
 * @param [in] data    {QVector<QString>}
 */
void DocumentWindow::ReceiveData(const QString& message, const QVector<QString>& data)
{
  KLOG("Received data");
  if (message == "Task Data")
  {
    const int32_t task_count = data.front().toInt();
    const int32_t item_count = data.size();
    const int32_t arg_count  = ((item_count - (1 + task_count)) / task_count);

    for (int32_t i = 1; i < item_count; i += (arg_count + 1))
    {
      const auto id = data.at(i);
      TaskFlags  flags{};

      for (int32_t j = 0; j < (arg_count / 2); j++)
        flags.insert(data.at(i + (j * 2) + 1), data.at(i + (j * 2) + 2));

      m_tasks.insert(id, TaskData{.flags = flags});
      m_task_index++;
    }
  }
  else
  if (message == "Task Data Final")
  {
    if (m_fetch_files)
      emit RequestFiles(m_tasks.keys().toVector());
    else
      RenderSection();
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
  const auto it = m_image_coords.find(QPair{row, col});
  return (it != m_image_coords.cend());
}

/**
 * @brief DocumentWindow::SavePDF
 */
void DocumentWindow::SavePDF()
{
  KLOG("Saving PDF");
  QFont font = ui->font->font();
  font .setPixelSize(11);
  m_doc.setDefaultFont(font);

  if (QPrintDialog(&m_printer, this).exec() != QDialog::Accepted) return;

  m_doc.print(&m_printer);
  QPainter painter{&m_svg};
  m_svg.setSize(QSize{-1, -1});
  m_doc.documentLayout()->setPaintDevice(&m_svg);
  m_doc.drawContents(&painter);
  m_doc  .clear();
  m_table.clear();
}

/**
 * @brief DocumentWindow::ReceiveFiles
 * @param [in] files {QVector<FileWrap>}
 */
void DocumentWindow::ReceiveFiles(QVector<FileWrap>&& files)
{
  KLOG("Received ", files.size(), " files");
  for (auto&& file : files)
  {
    auto it = m_tasks.find(file.task_id);
    if (it != m_tasks.end())
     it->files.push_back(std::move(file));
    else
      throw std::invalid_argument{"Task with matching ID not found"};
  }
  RenderSection();
}

void DocumentWindow::keyPressEvent(QKeyEvent* e)
{
  if (e->key() == Qt::Key_Escape)
    close();
}

void DocumentWindow::ToggleRow(const int32_t& index)
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
}
