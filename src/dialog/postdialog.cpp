#include "include/ui/postdialog.hpp"
#include "ui_postdialog.h"
#include <QStandardItem>
#include <QStandardItemModel>
#include "include/ui/status_delegate.hpp"
#include "include/ui/button_delegate.hpp"

/**
 * constructor
 *
 * @brief PostDialog::PostDialog
 * @param parent
 */
PostDialog::PostDialog(QWidget *parent)
: QDialog(parent),
  ui(new Ui::Dialog),
  m_standard_model(new QStandardItemModel{0, 6})
{
  ui->setupUi(this);  
  ui->posts->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
  ui->posts->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
  ui->posts->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
  ui->posts->setModel(m_standard_model);
//  ui->posts->setModel(&m_post_model);
  ui->posts->setEditTriggers(QAbstractItemView::AllEditTriggers);
  ui->posts->setMouseTracking(true);
  ui->posts->setItemDelegateForColumn(4, new StatusDelegate{parent, [this](auto index) { KLOG("Status Delegate Update"); }});
  ui->posts->setItemDelegateForColumn(5, new ButtonDelegate{parent, [this](auto index)
  {
    request_update(m_post_model.posts().at(index.row()));
    auto display_data = m_standard_model->data(index);
    auto edit_data =    m_standard_model->data(index, Qt::EditRole);
    KLOG("Button Delegate Update");
  }});
  ui->posts->horizontalHeader()->setStretchLastSection(true);

  QObject::connect(m_standard_model, &QStandardItemModel::itemChanged, [this](QStandardItem* item)
  {
    m_post_model.setData(item->index(), item->index().data());
    auto index = item->index();
    KLOG(QString{"Index %0 with data %1"}.arg(index_to_string(index)).arg(index.data().toString()));
  });
}


void PostDialog::ReceiveData(const QVector<QString>& data)
{
  m_post_model.set_data(data);
  size_t i = 0;
  m_standard_model->clear();
  m_standard_model->insertRows(0, m_post_model.posts().size(), QModelIndex{});
  m_standard_model->insertColumns(0, 6);
  for (const auto& post : m_post_model.posts())
    set_item_data(i++, post);
}
/**
 * destructor
 *
 * @brief PostDialog::~PostDialog
 */
PostDialog::~PostDialog()
{
  delete ui;
}

void PostDialog::set_item_data(size_t row, const Platform::Post& post)
{
  m_standard_model->setData(m_standard_model->index(row, 0, QModelIndex()), QVariant(post.name));
  m_standard_model->setData(m_standard_model->index(row, 1, QModelIndex()), QVariant(post.time));
  m_standard_model->setData(m_standard_model->index(row, 2, QModelIndex()), QVariant(post.user));
  m_standard_model->setData(m_standard_model->index(row, 3, QModelIndex()), QVariant(post.uuid));
  m_standard_model->setData(m_standard_model->index(row, 4, QModelIndex()), QVariant(post.status));
}
