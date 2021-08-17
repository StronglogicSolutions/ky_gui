#ifndef DOCUMENTWINDOW_HPP
#define DOCUMENTWINDOW_HPP

#include <QMainWindow>
#include <QMouseEvent>
#include <QTextDocument>
#include <QTableWidget>
#include <QPrinter>
#include <QDialog>
#include <QFile>
#include <QFileDialog>
#include <QBuffer>
#include <QMimeDatabase>
#include <QHash>

#include <include/ui/kfiledialog.h>
#include <headers/util.hpp>

enum class RowType
{
  HEADER = 0x00,
  REPEAT = 0x01
};
using TaskFlags = QHash<QString, QString>;
using Coords    = QHash<qint32, qint32>;

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

protected:
  virtual void mouseReleaseEvent(QMouseEvent* e) override;
  virtual void keyPressEvent(QKeyEvent* e) override;

private:
  void SetInserting(const bool inserting, const int32_t& index = -1);
  void AddRow();
  void AddColumn();
  void SaveSection();
  void SavePDF();
  bool ImageAtCell(int32_t row, int32_t col);


  Ui::DocumentWindow *ui;
  QList<QString>     m_flags;
  int32_t            m_flag_index;
  bool               m_inserting;
  QList<RowType>     m_row_types;
  QString            m_file_path;
  QTextDocument      m_doc;
  QPrinter           m_printer;
  TaskMap            m_tasks;
  int32_t            m_task_index;
  QTableWidget       m_table;
  Coords             m_image_coords;
};

#endif // DOCUMENTWINDOW_HPP
