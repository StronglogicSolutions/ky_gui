#pragma once

#include <QMainWindow>
#include <QMouseEvent>
#include <QPrinter>
#include <QDialog>
#include <QSvgGenerator>
#include <QFile>
#include <QFileDialog>
#include <QBuffer>
#include <QMimeDatabase>
#include <QHash>
#include <QPair>
#include "src/window/document/helpers.hpp"
#include <include/ui/kfiledialog.h>
#include <headers/util.hpp>

using TaskFlags  = QHash<QString, QString>;
using Coord      = QPair<qint32, qint32>;
using FileCoords = QHash<Coord, QMimeType>;
using Coords     = QHash<Coord, bool>;

struct TaskData
{
  TaskFlags         flags;
  QVector<FileWrap> files;
};

using TaskMap = QHash<QString, TaskData>;

struct CellRange
{
  using QSelectionRanges = QList<QTableWidgetSelectionRange>;
  using CellRanges = std::vector<CellRange>;
  CellRange(const QTableWidgetSelectionRange& r) { xy1[0] = r.topRow(); xy1[1] = r.leftColumn(); xy2[0] = r.bottomRow(); xy2[1] = r.rightColumn(); }
  static CellRange  FromQRange (const QTableWidgetSelectionRange& r) { return CellRange{r}; }
  static CellRanges FromQRanges(const QSelectionRanges&           r) { return CellRanges{r.begin(), r.end()}; }

  int32_t xy1[2];
  int32_t xy2[2];

  int32_t XStart()  const { return xy1[0]; }
  int32_t YStart()  const { return xy1[1]; }
  int32_t XEnd()    const { return xy2[0]; }
  int32_t YEnd()    const { return xy2[1]; }
  int32_t XLength() const { return xy2[0] - xy1[0] + 1; }
  int32_t YLength() const { return xy2[1] - xy1[1] + 1; }

  void SetCoords(Coords& coords) const
  {
    for (auto row = XStart(); row != XEnd(); row++)
      for (auto col = YStart(); col != YEnd(); col++)
        coords.insert({row, col}, true);
  }
};

namespace Ui {
class DocumentWindow;
}

class DocumentWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit DocumentWindow(QWidget *parent = nullptr);
  ~DocumentWindow();

  void SetFlags(const QList<QString>& flags);
  void SetFilePath(QString path) { m_file_path = path; }
  void ReceiveFiles(QVector<FileWrap>&& files);
  void ReceiveData (const QString& message, const QVector<QString>& data);

signals:
  void RequestData(QVector<QString> argv);
  void RequestFiles(const QVector<QString>& id);

protected:
  virtual void mouseReleaseEvent(QMouseEvent* e) override;
  virtual void keyPressEvent(QKeyEvent* e) override;

private:
  void SetInserting(const bool inserting, const int32_t& index = -1);
  void AddRow();
  void AddColumn();
  void ToggleRow(const int32_t& index);
  void RenderSection();
  void SaveSection();
  void SavePDF();
  bool ImageAtCell(const int32_t& row, const int32_t& col) const;
  void SetDatePickers();
  void SetPrinter();
  void SetTable();
  void SetListeners();
  bool CellIsOpen(const int32_t& row, const int32_t& col) const;
  void SetCoord(const int32_t& row, const int32_t& col, bool is_set = true);


  Ui::DocumentWindow *ui;
  QList<QString>     m_flags;
  int32_t            m_flag_index;
  bool               m_inserting;
  QList<RowType>     m_row_types;
  QString            m_file_path;
  QTextDocument      m_doc;
  QSvgGenerator      m_svg;
  QPrinter           m_printer;
  TaskMap            m_tasks;
  int32_t            m_task_index;
  QTableWidget       m_table;
  FileCoords         m_image_coords;
  bool               m_fetch_files;
  int32_t            m_img_height;
  bool               m_scale_images;
  Coords             m_coords;
};
