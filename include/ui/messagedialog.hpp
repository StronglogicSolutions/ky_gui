#ifndef MESSAGEDIALOG_HPP
#define MESSAGEDIALOG_HPP

#include <QDialog>
#include <QPushButton>

namespace Ui {
class MessageDialog;
}

class MessageDialog : public QDialog
{
  Q_OBJECT

 public:
  explicit MessageDialog(QWidget *parent = nullptr);
  void append(QString message, bool formatted=false);
  ~MessageDialog();

 private:
  Ui::MessageDialog *ui;

};

#endif // MESSAGEDIALOG_HPP
