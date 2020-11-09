#ifndef APPDIALOG_HPP
#define APPDIALOG_HPP

#include <QDialog>
#include "headers/util.hpp"

namespace Ui {
class AppDialog;
}

class AppDialog : public QDialog
{
  Q_OBJECT

 public:
  explicit AppDialog(QWidget *parent = nullptr);
  void     setApplications(QVector<KApplication> applications) { m_applications = applications; }
  ~AppDialog();

 protected:
  virtual void showEvent(QShowEvent *) override;

 private:
  Ui::AppDialog*        ui;
  QVector<KApplication> m_applications;
};

#endif // APPDIALOG_HPP
