#ifndef APPDIALOG_HPP
#define APPDIALOG_HPP

#include <QDialog>

namespace Ui {
class AppDialog;
}

class AppDialog : public QDialog
{
  Q_OBJECT

 public:
  explicit AppDialog(QWidget *parent = nullptr);
  void     setApps();
  ~AppDialog();

 private:
  Ui::AppDialog *ui;
};

#endif // APPDIALOG_HPP
