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

 signals:
  void appRequest(KApplication application);

 protected:
  virtual void showEvent(QShowEvent *) override;

 private:
  void setInputMode(bool mode = true);
  void setFields(QString app_name);

  Ui::AppDialog*        ui;
  QVector<KApplication> m_applications;
};

#endif // APPDIALOG_HPP
