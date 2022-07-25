#include <QPushButton>
#include <QPainter>
#include "include/ui/post_delegate.hpp"
#include "headers/util.hpp"

PostItemDelegate::PostItemDelegate(QObject* parent, const post_item_fn_t& fn)
: QStyledItemDelegate(parent),
  m_callback(fn)
{

}

void PostItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
  QPushButton* button = new QPushButton(index.data().toString());
  QObject::connect(button, &QPushButton::clicked, this, [](bool checked) { KLOG("WHATSUP BABY"); });
  button->setGeometry(option.rect);
  painter->save();
  painter->translate(option.rect.topLeft());
  button->render(painter);
  painter->restore();
}

QWidget* PostItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
  if (index.isValid())
    m_callback(index);
  return QStyledItemDelegate::createEditor(parent, option, index);
}

bool PostItemDelegate::eventFilter(QObject *editor, QEvent *event)
{
  if (event->type() == QEvent::MouseButtonRelease)
  {
    KLOG("mouse button release event");
  }
  return QStyledItemDelegate::eventFilter(editor, event);
}
