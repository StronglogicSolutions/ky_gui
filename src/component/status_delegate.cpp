#include <QComboBox>
#include <QPainter>
#include <QTableView>
#include <QKeyEvent>
#include <map>
#include "include/ui/status_delegate.hpp"
#include "include/ui/postdialog.hpp"
#include <util/util.hpp>
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
  if (index.column() != 4)
    return parent;

  QComboBox* box = new QComboBox{parent};
  box->addItems({"Scheduled", "Complete", "Error"});
  box->setCurrentIndex(index.data().toInt());
  box->setGeometry(option.rect);

  QObject::connect(box, QOverload<int>::of(&QComboBox::currentIndexChanged), [](int index)
  {
    KLOG("Combobox value {}", index);
  });

  return box;
}

void StatusDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
  QComboBox* box   = static_cast<QComboBox*>(editor);
  if (!box || !index.isValid())
  {
    ELOG("Failed to set model data.");
    return;
  }
  QString value = g_status_strings.at(box->itemText(box->currentIndex()));
  model->setData(index, value, Qt::EditRole);
}

void StatusDelegate::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem &option, const QModelIndex&) const
{  
  editor->setGeometry(option.rect);
}
