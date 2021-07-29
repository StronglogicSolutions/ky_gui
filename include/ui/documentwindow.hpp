#ifndef DOCUMENTWINDOW_H
#define DOCUMENTWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>
#include <include/ui/kfiledialog.h>
#include <QDialog>
#include <QFile>
#include <QFileDialog>
#include <QBuffer>
#include <QMimeDatabase>

enum class RowType
{
  HEADER = 0x00,
  REPEAT = 0x01
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

  void set_flags(const QList<QString>& flags);
  void setFilePath(QString path) { m_file_path = path; }

protected:
  void mouseReleaseEvent(QMouseEvent* e);

private:
  void set_inserting(const bool inserting, const int32_t& index = -1);
  void add_row();
  void add_column();
  Ui::DocumentWindow *ui;
  QList<QString>     m_flags;
  int32_t            m_flag_index;
  bool               m_inserting;
  QList<RowType>     m_row_types;
  QString            m_file_path;
};

#endif // DOCUMENTWINDOW_H
