#include <QPushButton>
#include <QGraphicsColorizeEffect>
#include <QPainter>
#include "include/ui/button_delegate.hpp"
#include "headers/util.hpp"

static const int RequestRole = (Qt::DisplayRole + 1);

ButtonDelegate::ButtonDelegate(QObject* parent, const post_item_fn_t& fn)
: QStyledItemDelegate(parent),
  m_callback(fn)
{}

void ButtonDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
  QPushButton button{};
  if (bool request_pending = index.data(RequestRole).toBool())
  {
    button.setStyleSheet("background-color: purple");
    button.setText("Requested");
  }
  else
  {
    button.setStyleSheet("background-color: green");
    button.setText("Save");
  }

  button.setGeometry(option.rect);
  painter->save();
  painter->translate(option.rect.topLeft());
  button.render(painter);
  painter->restore();
}

void ButtonDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
  QPushButton* button = qobject_cast<QPushButton*>(editor);
  KLOG("setEditorData");
  button->setText("Requested");
  button->setStyleSheet("background-color: purple");
}

QWidget* ButtonDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
  if (index.isValid())
    m_callback(index);  

  QPushButton* button = new QPushButton{parent};
  button->setGeometry(option.rect);
  button->setText("Save");
  button->setStyleSheet("background-color: green");
  QObject::connect(button, &QPushButton::clicked, [this, &index] ()
  {
    m_callback(index);
  });

  return button;
}

void ButtonDelegate::setModelData(QWidget*, QAbstractItemModel* model, const QModelIndex &index) const
{
  KLOG("setModelData");
  model->setData(index, true, RequestRole);
}

void ButtonDelegate::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem &option, const QModelIndex&) const
{
  editor->setGeometry(option.rect);
}
