#ifndef __POSTDIALOG_HPP__
#define __POSTDIALOG_HPP__

#include <QDialog>
#include <QThread>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QStandardItemModel>
#include <unordered_map>
#include "headers/util.hpp"
#include "headers/kiq_types.hpp"

namespace Ui {
class PostDialog;
class Dialog;
} // ns Ui

class PostDialog : public QDialog
{
  Q_OBJECT
 public:
  explicit PostDialog(QWidget* parent = nullptr);
  ~PostDialog();

  void     ReceiveData(const QVector<QString> data);
  Ui::Dialog*         ui;
  QVector<Platform::Post> m_posts;
};

#endif // __POSTDIALOG_HPP
