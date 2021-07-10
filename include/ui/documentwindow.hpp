#ifndef DOCUMENTWINDOW_H
#define DOCUMENTWINDOW_H

#include <QMainWindow>

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

private:
  Ui::DocumentWindow *ui;
  QVector<QString>   m_flags;
};

#endif // DOCUMENTWINDOW_H
