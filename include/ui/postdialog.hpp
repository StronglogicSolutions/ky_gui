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


class PostModel : public QAbstractTableModel
{
  Q_OBJECT
using posts_t = QVector<Platform::Post>;
public:

  QVariant data(const QModelIndex& index, int role) const final
  {
    if (!index.isValid())
      return QVariant{};

    auto post_to_string = [this](auto row, auto col)
    {
      switch (col)
      {
        case (0): return posts().at(row).name;   break;
        case (1): return posts().at(row).time;   break;
        case (2): return posts().at(row).user;   break;
        case (3): return posts().at(row).uuid;   break;
        case (4):
        {
          const auto status = posts().at(row).status;
          if (g_status_names.find(status) != g_status_names.end())
            return g_status_names.at(status);
        }
        default:  return QString{};
      }
    };
    const int i = index.row();
    const int j = index.column();
    if (role == Qt::DisplayRole)
      return post_to_string(i, j);
    else
    if (role == Qt::EditRole)
      return posts()[i].active;
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
    if (!index.isValid() || index.column() != 4)
      return false;

    if (role == Qt::EditRole || role == Qt::DisplayRole)
    {
      const auto row      = index.row();
      m_posts[row].active = value.toInt() == 1;
      m_posts[row].status = value.toString();
      emit dataChanged(index, index);
      return true;
    }

    return false;
  }

  QVariant headerData(int section, Qt::Orientation orientation, int role) const final
  {
    if (role != Qt::DisplayRole)
        return QVariant{};

    if (orientation == Qt::Horizontal)
    {
      switch (section)
      {
        case 0: return "Platform"; case 1:  return "Time";
        case 2: return "User";     case 3:  return "UUID";
        case 4: return "Status";   default: return QVariant{};
      }
    }
    return section + 1;
  }

  void sort(int column, Qt::SortOrder order = Qt::AscendingOrder) final
  {
    using sort_fn = std::function<bool(const Platform::Post&, const Platform::Post&)>;
    auto predicate = sort_fn{};
    bool ascending = (order == Qt::AscendingOrder);
    switch (column)
    {
      case 0:
        predicate = [ascending](const auto& a, const auto& b)
        {
          return (ascending) ?
                   (a.name.front() != b.name.front()) ? a.name.front().digitValue() > b.name.front().digitValue() :
                                                        a.name.at(1)  .digitValue() > b.name.at(1)  .digitValue()   :
                   (a.name.at(1)   != b.name.at(1))   ? a.name.front().digitValue() < b.name.front().digitValue() :
                                                        a.name.at(1)  .digitValue() < b.name.at(1)  .digitValue();
        };
      break;
      case 1:
        predicate = [ascending](const auto& a, const auto& b) { return (ascending) ? a.time.toInt() > b.time.toInt() :
                                                                                     a.time.toInt() < b.time.toInt(); };
      break;
      case 2:
        predicate = [ascending](const auto& a, const auto& b)
        {
          return (ascending) ?
                   (a.user.front() != b.user.front()) ? a.user.front().digitValue() > b.user.front().digitValue() :
                                                        a.user.at(1)  .digitValue() > b.user.at(1)  .digitValue()   :
                   (a.user.at(1)   != b.user.at(1))   ? a.user.front().digitValue() < b.user.front().digitValue() :
                                                        a.user.at(1)  .digitValue() < b.user.at(1)  .digitValue();
        };
      break;
      case 3:
        predicate = [ascending](const auto& a, const auto& b)
        {
          return (ascending) ?
                   (a.uuid.front() != b.uuid.front()) ? a.uuid.front().digitValue() > b.uuid.front().digitValue() :
                                                        a.uuid.at(1)  .digitValue() > b.uuid.at(1)  .digitValue()   :
                   (a.uuid.at(1)   != b.uuid.at(1))   ? a.uuid.front().digitValue() < b.uuid.front().digitValue() :
                                                        a.uuid.at(1)  .digitValue() < b.uuid.at(1)  .digitValue();
        };
      break;
      case 4:
        predicate = [ascending](const auto& a, const auto& b) { return (ascending) ? a.status.toInt() > b.status.toInt() :
                                                                                     a.status.toInt() < b.status.toInt(); };
      break;
      default:
        KLOG("Can't sort with column ", std::to_string(column));
        return;
    }
    std::sort(m_posts.begin(), m_posts.end(), predicate);
  }

  void set_data(const QVector<QString>& data)
  {
    if (data.empty())
      return;

    removeRows(0, m_posts.size());

    const size_t count = (data.size() / Platform::ARGSIZE);
    beginInsertRows(QModelIndex{}, 0, count - 1);
    m_posts = Platform::Post::from_vector(data);
    endInsertRows();
    emit dataChanged(index(0, 0), index(posts().size() - 1, 3));
  }

  const posts_t& posts()             const { return m_posts; }
        posts_t& get_mutable_posts()       { return m_posts; }

  bool removeRows(int row, int count, const QModelIndex& parent = QModelIndex()) final
  {
    const auto last_row = (row + count) + 1;
    QAbstractItemModel::beginRemoveRows(parent, row, last_row);
    m_posts.erase((m_posts.begin() + row), m_posts.begin() + last_row - 1);
    QAbstractItemModel::endRemoveRows();
    emit dataChanged(index(0, 0), index(m_posts.size() - 1, 3));
    return true;
  }

  Qt::ItemFlags flags(const QModelIndex& index) const final
  {
    Q_UNUSED(index);
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsDragEnabled| Qt::ItemIsDropEnabled | Qt::ItemIsEditable;
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
