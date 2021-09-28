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


using TaskFlags = QHash<QString, QString>;
using Coord     = QPair<qint32, qint32>;
using Coords    = QHash<Coord, QMimeType>;

struct TaskData
{
  TaskFlags         flags;
  QVector<FileWrap> files;
};

using TaskMap = QHash<QString, TaskData>;

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
  bool ImageAtCell(int32_t row, int32_t col);
  void SetDatePickers();
  void SetPrinter();
  void SetTable();
  void SetListeners();


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
  Coords             m_image_coords;
  bool               m_fetch_files;
};
