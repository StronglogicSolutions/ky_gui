#ifndef __BUTTON_DELEGATE_HPP__
#define __BUTTON_DELEGATE_HPP__

#include <QStyledItemDelegate>
#include <QEvent>
#include <functional>
#include <map>

using post_item_fn_t = std::function<void(const QModelIndex &)>;

class ButtonDelegate : public QStyledItemDelegate
{
  Q_OBJECT

public:
  explicit ButtonDelegate(QObject* parent, const post_item_fn_t& fn);
  ~ButtonDelegate() override = default;
  void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const final;
  QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const final;
  void setEditorData(QWidget *editor, const QModelIndex &index) const override;
  void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
  void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex& index) const override;

private:  
  post_item_fn_t m_callback;

};

#endif // __BUTTON_DELEGATE_HPP__
