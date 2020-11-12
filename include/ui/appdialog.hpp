#ifndef APPDIALOG_HPP
#define APPDIALOG_HPP

#include <QDialog>
#include "headers/util.hpp"

namespace constants {
enum RequestType {
  REGISTER = 0x00,
  UPDATE   = 0x01,
  DELETE   = 0x02,
  GET      = 0x03
};
}

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
