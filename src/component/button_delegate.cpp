#include <QPushButton>
#include <QGraphicsColorizeEffect>
#include <QPainter>
#include <QStandardItemModel>
#include "include/ui/button_delegate.hpp"
#include "headers/util.hpp"

ButtonDelegate::ButtonDelegate(QObject* parent, const post_item_fn_t& fn)
: QStyledItemDelegate(parent),
  m_callback(fn)
{}

bool has_been_purple = false;
void ButtonDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
  QPushButton button{};
//  auto item_data = qobject_cast<QStandardItemModel*>(const_cast<QAbstractItemModel*>(index.model()))->item(index.row(), index.column());
  if (bool request_pending = index.data(RequestRole).toBool())
  {
    button.setStyleSheet("background-color: purple");
    button.setText("Requested");
    if (index.row() == 165)
      KLOG("paint() 165 is pending");
  }
  else
  {
    auto row = index.row();
    if (row == 165)
    {
      KLOG("paint() 165 is NOT pending");
      button.setText("This one will save");
    }
    else
      button.setText("Save");
    button.setStyleSheet("background-color: green");
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

QWidget* ButtonDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex& index) const
{
  if (index.isValid())
    m_callback(index);  

  QPushButton* button = new QPushButton{parent};
  button->setGeometry(option.rect);
  button->setText("Save");
  button->setStyleSheet("background-color: green");
  QObject::connect(button, &QPushButton::clicked, [this, &index]
  {
    if (index.isValid())
      m_callback(index);
  });

  return button;
}

void ButtonDelegate::setModelData(QWidget*, QAbstractItemModel* model, const QModelIndex &index) const
{
  KLOG("setModelData");

  const auto request_pending = index.data(RequestRole).toBool();
  KLOG(QString{"Request pending was %0 and will become %1 for %3 : %4"}.arg(request_pending).arg(!request_pending).arg(index.row()).arg(index.column()));
  model->setData(index, !request_pending, RequestRole);
}

void ButtonDelegate::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem &option, const QModelIndex&) const
{
  editor->setGeometry(option.rect);
}
