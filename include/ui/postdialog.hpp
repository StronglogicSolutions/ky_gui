#ifndef __POSTDIALOG_HPP__
#define __POSTDIALOG_HPP__

#include <QDialog>
#include <QThread>
#include <QAbstractTableModel>
#include <QStandardItemModel>
#include <unordered_map>
#include "headers/util.hpp"
#include "headers/kiq_types.hpp"

namespace Ui {
class Dialog;
} // ns Ui

static QString index_to_string(const QModelIndex& index)
{
  return QString("Row: %0 | Column %1").arg(index.row()).arg(index.column());
}
class PostModel : public QAbstractTableModel
{
  Q_OBJECT

public:

  QVariant data(const QModelIndex & index, int role) const final
  {
//    KLOG(QString{"PostModel::data() index is %0 and role is %1"}.arg(index_to_string(index)).arg(role));
    auto post_to_string = [this](auto row, auto col)
    {
      switch (col)
      {
        case (0): return QString{"%0"}.arg(posts().at(row).name);   break;
        case (1): return QString{"%0"}.arg(posts().at(row).time);   break;
        case (2): return QString{"%0"}.arg(posts().at(row).user);   break;
        case (3): return QString{"%0"}.arg(posts().at(row).uuid);   break;
        case (4): return QString{"%0"}.arg(posts().at(row).status); break;
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
    {
      return m_posts.at(i).active;
    }
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
  QVariant headerData(int section, Qt::Orientation orientation, int role) const override
  {
//    qDebug() << "Header Data";
    Q_UNUSED(orientation);
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole &&
        !posts().empty())
    {
      // Read the column names from the first line
      return QString{"This header"};
    }
//    if (role == Qt::DisplayRole)
//    {
//      return QString("Header #%1").arg(section);
//    }
//    else
//    if (role == Qt::FontRole)
//    {
//      QFont serifFont("Times", 10, QFont::Bold, true);
//      return serifFont;
//    }
//    else
//    if (role == Qt::TextAlignmentRole)
//    {
//      return Qt::AlignRight;
//    }
//    else
//    if (role == Qt::BackgroundRole)
//    {
//      return QBrush(Qt::blue);
//    }
//    else
//    if (role == Qt::ForegroundRole)
//    {
//      return QBrush(Qt::red);
//    }
//    else
//      qDebug() << "THIS ROLE IS " << role;
    return "Unknown role";
  }

  bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override
  {
    if (index.isValid() && role == Qt::EditRole)
    {
      if (index.column() == 4)
      {
        m_posts[index.row()].active = (value.toInt() == 1);
        KLOG(QString{"Set to %0"}.arg(value.toUInt()));
        emit dataChanged(index, index);
        return true;
      }
    }
    return QAbstractItemModel::setData(index, value, role);
  }

  QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex{}) const final
  {
    Q_UNUSED(parent);
    return createIndex(row, column);
  }

  QModelIndex parent(const QModelIndex &index) const final
  {
    if (index.isValid())
      return index;
    return QModelIndex{};
  }

  void set_data(const QVector<QString>& data)
  {
    m_posts.clear();
    auto post_count = data.size() / Platform::ARGSIZE;
    beginInsertRows(QModelIndex{}, 0, post_count - 1);

    for (int i = 0; i < data.size(); i += Platform::ARGSIZE)
      m_posts.push_back(Platform::Post{
        .name    = data[i + Platform::NAME_INDEX],
        .uuid    = data[i + Platform::UUID_INDEX],
        .user    = data[i + Platform::USER_INDEX],
        .time    = data[i + Platform::TIME_INDEX],
        .content = data[i + Platform::CONTENT_INDEX],
        .urls    = data[i + Platform::URLS_INDEX],
        .repost  = data[i + Platform::REPOST_INDEX],
        .method  = data[i + Platform::METHOD_INDEX],
        .args    = data[i + Platform::ARGS_INDEX],
        .cmd     = data[i + Platform::CMD_INDEX],
        .status  = data[i + Platform::STATUS_INDEX]});

    endInsertRows();
    emit dataChanged(index(0, 0), index(posts().size() - 1, 3));
  }

  const QVector<Platform::Post>& posts() const { return m_posts; }

  bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value,
                     int role = Qt::EditRole) override
  {
    KLOG("setHeaderData");
    QAbstractTableModel::setHeaderData(section, orientation, value, role);
    emit headerDataChanged(orientation, 0, 1);
    return true;
  }

  Qt::ItemFlags flags(const QModelIndex& index) const final
  {
    return QAbstractTableModel::flags(index)| Qt::ItemIsEditable | Qt::ItemIsSelectable;
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

signals:
  void     request_update(const Platform::Post& post);

private:
  void set_item_data(size_t row, const Platform::Post& post);

  Ui::Dialog* ui;
  PostModel   m_post_model;
  QStandardItemModel* m_standard_model;
};

#endif // __POSTDIALOG_HPP
