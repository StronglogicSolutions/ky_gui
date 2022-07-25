#ifndef __POST_DELEGATE_HPP__
#define __POST_DELEGATE_HPP__

#include <QStyledItemDelegate>
#include <QEvent>
#include <functional>

using post_item_fn_t = std::function<void(const QModelIndex &)>;

class PostItemDelegate : public QStyledItemDelegate
{

  Q_OBJECT

public:
  explicit PostItemDelegate(QObject* parent, const post_item_fn_t& fn);
  ~PostItemDelegate() override = default;
  void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const final;
  QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const final;
  bool eventFilter(QObject *editor, QEvent *event) override;

private:
  post_item_fn_t m_callback;
};

#endif // __POST_DELEGATE_HPP__
