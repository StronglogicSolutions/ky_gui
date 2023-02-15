#pragma once

#include <QDialog>
#include <QThread>
#include <QAbstractTableModel>
#include <algorithm>
#include <util/util.hpp>
#include "headers/kiq_types.hpp"
#include "include/ui/status_delegate.hpp"

namespace Ui {
class Dialog;
} // ns Ui

using posts_t = QVector<Platform::Post>;
class PostModel : public QAbstractTableModel
{
  Q_OBJECT

public:

  QVariant data(const QModelIndex& index, int role) const final;

  int rowCount(const QModelIndex & parent = QModelIndex{}) const final;

  int columnCount(const QModelIndex &parent) const final;

  bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;

  QVariant headerData(int section, Qt::Orientation orientation, int role) const final;

  void sort(int column, Qt::SortOrder order = Qt::AscendingOrder) final;

  void set_data(const QVector<QString>& data);

  const posts_t& posts()             const;
        posts_t& get_mutable_posts();

  bool removeRows(int row, int count, const QModelIndex& parent = QModelIndex()) final;

  Qt::ItemFlags flags(const QModelIndex& index) const final;

  void Update(const QModelIndex& index);

private:

  QVector<Platform::Post> m_posts;
};

class PostDialog : public QDialog
{
  Q_OBJECT
public:
  explicit PostDialog(QWidget* parent = nullptr);
  ~PostDialog();

  void     ReceiveData(const QVector<QString>& data);
  void     Update(const QVector<QString>& data);
  void     SelectRow(int row);
  QString  GetLastUpdated() const;

signals:
  void     request_update(const Platform::Post& post) const;
  void     refresh() const;

protected:
  void     showEvent(QShowEvent *) final;
private:
  Ui::Dialog* ui;
  PostModel   m_post_model;
  QString     m_last_updated;
  int         m_selected{-1};
};
