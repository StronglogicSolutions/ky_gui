#include "include/ui/postdialog.hpp"
#include "ui_postdialog.h"

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
  ui->posts->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
  ui->posts->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
  ui->posts->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
  ui->posts->setModel(&m_post_model);
}


void PostDialog::ReceiveData(const QVector<QString>& data)
{
  m_post_model.add_data(data);
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
