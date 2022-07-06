#ifndef __POSTDIALOG_HPP__
#define __POSTDIALOG_HPP__

#include <QDialog>
#include <QThread>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QStandardItemModel>
#include <QAbstractListModel>
#include <unordered_map>
#include "headers/util.hpp"
#include "headers/kiq_types.hpp"

namespace Ui {
class Dialog;
} // ns Ui

class PostModel : public QAbstractItemModel
{
public:

  QVariant data(const QModelIndex & index, int role) const final
  {
    auto post_to_string = [](const Platform::Post& post)
        {
          return QString{"%0 - %1 - %2 - %3"}.arg(post.name).arg(post.time).arg(post.user).arg(post.uuid);
        };
    int i = index.row();
        if (role == Qt::DisplayRole)
    //      return QString{"Row%1, Column%2"}.arg(index.row() + 1).arg(index.column() + 1);
          return m_posts.empty() || i >= m_posts.size() ? QString{"This is your mom"} : post_to_string(m_posts[i]);//post_to_string(m_posts[index.row()]);
        if (role == Qt::EditRole)
          return QString{"This is your mom"};
        return QVariant{};
  }

  int      rowCount(const QModelIndex & parent = QModelIndex{}) const final
  {
    Q_UNUSED(parent);
    return m_posts.empty() ? 1 : m_posts.size();
  }

  int      columnCount(const QModelIndex &parent) const final
  {
    Q_UNUSED(parent);
    return 1;
  }
  QVariant headerData(int section, Qt::Orientation orientation, int role) const override
  {
    Q_UNUSED(orientation)
    if (role == Qt::DisplayRole)
    {
        return QString("Header #%1").arg(section);
    }
    else
    if (role == Qt::FontRole)
    {
        QFont serifFont("Times", 10, QFont::Bold, true);
        return serifFont;
    }
    else
    if (role == Qt::TextAlignmentRole)
    {
        return Qt::AlignRight;
    }
    else
    if (role == Qt::BackgroundRole)
    {
        return QBrush(Qt::blue);
    }
    else
    if (role == Qt::ForegroundRole)
    {
        return QBrush(Qt::red);
    }
    else
      qDebug() << "THIS ROLE IS " << role;
    return "Unknown role";
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

  void add_data(const QVector<QString>& data)
  {
    auto post_count = data.size() / Platform::ARGSIZE;
    beginInsertRows(QModelIndex{}, 0, post_count - 1);

    for (int i = 0; i < data.size(); i += Platform::ARGSIZE)
      m_posts.push_back(Platform::Post{
        .name    = data[i + Platform::NAME_INDEX],
        .uuid    = data[i + Platform::UUID_INDEX],
        .user    = data[i + Platform::USER_INDEX],
        .time    = data[i + Platform::TIME_INDEX],
        .content = data[i + Platform::CONTENT_INDEX],
        .urls    = data[i + Platform::URLS_INDEX]});

    endInsertRows();
    emit dataChanged(index(0, 0), index(posts().size() - 1, 0));
  }

  QVector<Platform::Post>& posts() { return m_posts; }

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
  Ui::Dialog*         ui;
  PostModel               m_post_model;
};

#endif // __POSTDIALOG_HPP
