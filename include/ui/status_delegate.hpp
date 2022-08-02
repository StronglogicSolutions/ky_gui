#ifndef __STATUS_DELEGATE_HPP__
#define __STATUS_DELEGATE_HPP__

#include <QStyledItemDelegate>
#include <QEvent>
#include <functional>

using post_item_fn_t = std::function<void(const QModelIndex &)>;

class StatusDelegate : public QStyledItemDelegate
{

  Q_OBJECT

public:
  explicit StatusDelegate(QObject* parent, const post_item_fn_t& fn);
  ~StatusDelegate() override = default;
  void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const final;
  QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const final;
  bool editorEvent(QEvent *event, QAbstractItemModel* model, const QStyleOptionViewItem &option, const QModelIndex &index) override;
  void setEditorData(QWidget *editor, const QModelIndex &index) const override;
  void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
  void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex& index) const override;

private:  
  post_item_fn_t m_callback;
};

#endif // __STATUS_DELEGATE_HPP__
