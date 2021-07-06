#ifndef DOCUMENTDIALOG_H
#define DOCUMENTDIALOG_H

#include <QDialog>

namespace Ui {
class DocumentDialog;
}

class DocumentDialog : public QDialog
{
  Q_OBJECT

public:
  explicit DocumentDialog(QWidget *parent = nullptr);
  ~DocumentDialog();

private:
  Ui::DocumentDialog *ui;
};

#endif // DOCUMENTDIALOG_H
