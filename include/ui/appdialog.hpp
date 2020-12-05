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
  void     addApplication(KApplication application);
  void     removeApplication(KApplication application);
  ~AppDialog();

 signals:
  void appRequest(KApplication application, constants::RequestType type);

 protected:
  virtual void showEvent(QShowEvent *) override;

 private:
  void         toggleInputMode();
  void         setFields(QString app_name);
  KApplication readFields();

  Ui::AppDialog*        ui;
  QVector<KApplication> m_applications;
};

#endif // APPDIALOG_HPP
