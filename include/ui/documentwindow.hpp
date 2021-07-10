#ifndef DOCUMENTWINDOW_H
#define DOCUMENTWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>

namespace Ui {
class DocumentWindow;
}

class DocumentWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit DocumentWindow(QWidget *parent = nullptr);
  ~DocumentWindow();

  void set_flags(const QVector<QString>& flags);

protected:
  void mouseReleaseEvent(QMouseEvent* e);

private:
  void set_inserting(const int32_t& index);
  Ui::DocumentWindow *ui;
  QVector<QString>   m_flags;
  int32_t            m_inserting;
};

#endif // DOCUMENTWINDOW_H
