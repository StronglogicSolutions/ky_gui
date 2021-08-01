#ifndef DOCUMENTWINDOW_H
#define DOCUMENTWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>
#include <QDialog>
#include <QFile>
#include <QFileDialog>
#include <QBuffer>
#include <QMimeDatabase>

#include <include/ui/kfiledialog.h>
#include <headers/util.hpp>

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

  void SetFlags(const QList<QString>& flags);
  void SetFilePath(QString path) { m_file_path = path; }
  void Receive(const QVector<FileWrap>& files);

signals:
  void RequestFiles(const uint32_t id);

protected:
  virtual void mouseReleaseEvent(QMouseEvent* e) override;

private:
  void SetInserting(const bool inserting, const int32_t& index = -1);
  void AddRow();
  void AddColumn();
  Ui::DocumentWindow *ui;
  QList<QString>     m_flags;
  int32_t            m_flag_index;
  bool               m_inserting;
  QList<RowType>     m_row_types;
  QString            m_file_path;
};

#endif // DOCUMENTWINDOW_H
