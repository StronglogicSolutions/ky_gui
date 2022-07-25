#include "include/ui/postdialog.hpp"
#include "ui_postdialog.h"
#include <QStandardItem>
#include "include/ui/post_delegate.hpp"


/**
 * constructor
 *
 * @brief PostDialog::PostDialog
 * @param parent
 */
PostDialog::PostDialog(QWidget *parent)
: QDialog(parent),
  ui(new Ui::Dialog)
{
  ui->setupUi(this);  
//  m_post_model.insertColumn(0);
//  m_post_model.insertColumn(1);
  ui->posts->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
  ui->posts->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
  ui->posts->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
  ui->posts->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectItems);
  ui->posts->setModel(&m_post_model);
  ui->posts->setItemDelegateForColumn(4, new PostItemDelegate{parent, [this](auto index) { m_post_model.Update(index); }});
  /*m_post_model.setHeaderData(0, Qt::Orientation::Vertical, "Platf
   * orm", Qt::DisplayRole);
  m_post_model.setHeaderData(1, Qt::Orientation::Vertical, "Time",     Qt::DisplayRole);
  m_post_model.setHeaderData(2, Qt::Orientation::Vertical, "User",     Qt::DisplayRole);
  m_post_model.setHeaderData(3, Qt::Orientation::Vertical, "UUID",     Qt::DisplayRole);*/
}


void PostDialog::ReceiveData(const QVector<QString>& data)
{
  m_post_model.set_data(data);
  m_post_model.setHeaderData(0, Qt::Orientation::Horizontal, "Platform", Qt::DisplayRole);
  m_post_model.setHeaderData(1, Qt::Orientation::Horizontal, "Time",     Qt::DisplayRole);
  m_post_model.setHeaderData(2, Qt::Orientation::Horizontal, "User",     Qt::DisplayRole);
  m_post_model.setHeaderData(3, Qt::Orientation::Horizontal, "UUID",     Qt::DisplayRole);
  m_post_model.setHeaderData(4, Qt::Orientation::Horizontal, "Status",   Qt::DisplayRole);
  ui->posts->horizontalHeader()->show();
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
