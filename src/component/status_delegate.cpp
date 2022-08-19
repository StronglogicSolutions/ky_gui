#include <QComboBox>
#include <QPainter>
#include <QTableView>
#include <QKeyEvent>
#include <map>
#include "include/ui/status_delegate.hpp"
#include "include/ui/postdialog.hpp"
#include "headers/util.hpp"
#include <QApplication>

StatusDelegate::StatusDelegate(QObject* parent, const post_item_fn_t& fn)
: QStyledItemDelegate(parent),
  m_callback(fn)
{}

void StatusDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
  if (QComboBox* box = qobject_cast<QComboBox*>(editor))
  {
    box->setCurrentIndex(index.data().toInt());    
    box->showPopup();
  }
}

QWidget* StatusDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
  if (index.isValid())
    m_callback(index);

  QComboBox* box = new QComboBox{parent};
  box->addItems({"Scheduled", "Complete", "Error"});
  box->setCurrentIndex(index.data().toInt());
  box->setGeometry(option.rect);

  QObject::connect(box, QOverload<int>::of(&QComboBox::currentIndexChanged), [](int index)
  {
    KLOG(QString{"Combobox value %0"}.arg(index));
  });

  QObject::connect(box, static_cast<void (QComboBox::*)(int)>(&QComboBox::activated), [box]
  {
    QApplication::sendEvent(box, new QKeyEvent(QEvent::KeyPress, Qt::Key_Enter, Qt::NoModifier));
  });

  return box;
}

void StatusDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
  QComboBox* box   = static_cast<QComboBox*>(editor);
  QString    value = g_status_strings.at(box->itemText(box->currentIndex()));
  model->setData(index, value, Qt::EditRole);
}

void StatusDelegate::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem &option, const QModelIndex&) const
{  
  editor->setGeometry(option.rect);
}
