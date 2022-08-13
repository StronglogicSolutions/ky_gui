#ifndef __POSTDIALOG_HPP__
#define __POSTDIALOG_HPP__

#include <QDialog>
#include <QThread>
#include <QAbstractTableModel>
#include "headers/util.hpp"
#include "headers/kiq_types.hpp"
#include "include/ui/status_delegate.hpp"

namespace Ui {
class Dialog;
} // ns Ui


class PostModel : public QAbstractTableModel
{
  Q_OBJECT

public:

  QVariant data(const QModelIndex& index, int role) const final
  {
    auto post_to_string = [this](auto row, auto col)
    {
      switch (col)
      {
        case (0): return QString{"%0"}.arg(posts().at(row).name);   break;
        case (1): return QString{"%0"}.arg(posts().at(row).time);   break;
        case (2): return QString{"%0"}.arg(posts().at(row).user);   break;
        case (3): return QString{"%0"}.arg(posts().at(row).uuid);   break;
        case (4):
        {
          auto status = posts().at(row).status;
          return (g_status_names.find(status) != g_status_names.end()) ?
            QString{"%0"}.arg(g_status_names.at(status)) : "";
        }
        break;
        default:  return QString{};
      }
    };
    const int i = index.row();
    const int j = index.column();
    if (role == Qt::DisplayRole)
      return m_posts.empty() || i >= m_posts.size() ? QString{"Loading data..."} : post_to_string(i, j);
    if (role == Qt::EditRole)
      return posts()[i].active;
    else
    if (role == 257)    
      return m_posts.at(i).active;   

    return QVariant{};
  }

  int rowCount(const QModelIndex & parent = QModelIndex{}) const final
  {
    Q_UNUSED(parent);
    return m_posts.empty() ? 0 : m_posts.size() - 1;
  }

  int columnCount(const QModelIndex &parent) const final
  {
    Q_UNUSED(parent);
    return 5;
  }

  bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override
  {
    if (index.isValid() && (role == Qt::EditRole || role == Qt::DisplayRole))
    {
      const auto column = index.column();
      const auto row    = index.row();
      switch(column)
      {
        case 4:
          m_posts[row].active = value.toInt() == 1;
          m_posts[row].status = value.toString();
          emit dataChanged(index, index);
          return true;
        break;
        default:
          return false;
        break;
      }
    }
    KLOG(QString{"Didn't handle setData for role %0"}.arg(role));
  }

  void set_data(const QVector<QString>& data)
  {
    if (data.empty())
      return;

    const size_t count = (data.size() / Platform::ARGSIZE);
    beginInsertRows(QModelIndex{}, 0, count - 1);
    m_posts = Platform::Post::from_vector(data);
    endInsertRows();
    emit dataChanged(index(0, 0), index(posts().size() - 1, 3));
  }

  const QVector<Platform::Post>& posts() const       { return m_posts; }
        QVector<Platform::Post>& get_mutable_posts() { return m_posts; }


  Qt::ItemFlags flags(const QModelIndex& index) const final
  {
    auto flags = Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsDragEnabled| Qt::ItemIsDropEnabled;
    if (index.column() == 4)
      flags |= Qt::ItemIsEditable;
    return flags;
  }

  void Update(const QModelIndex& index)
  {
    bool editable = flags(index) & Qt::ItemIsEditable;
    KLOG(QString{"Hit with index %0, %1. Editable: %2"}.arg(index.row()).arg(index.column()).arg(editable));
  }

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

signals:
  void     request_update(const Platform::Post& post);

private:
  Ui::Dialog* ui;
  PostModel   m_post_model;
  int         m_selected{-1};
};

#endif // __POSTDIALOG_HPP
