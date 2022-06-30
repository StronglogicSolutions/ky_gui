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
}


void PostDialog::ReceiveData(const QVector<QString> data)
{
  for (int i = 0; i < data.size(); i += Platform::ARGSIZE)
    m_posts.push_back(Platform::Post{
      .name    = data[i + Platform::NAME_INDEX],
      .uuid    = data[i + Platform::UUID_INDEX],
      .user    = data[i + Platform::USER_INDEX],
      .time    = data[i + Platform::TIME_INDEX],
      .content = data[i + Platform::CONTENT_INDEX],
      .urls    = data[i + Platform::URLS_INDEX]});
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
