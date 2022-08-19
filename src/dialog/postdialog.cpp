#include "include/ui/postdialog.hpp"
#include "ui_postdialog.h"

static const QString save_button_style{"QPushButton{"
  "font: 87 18pt \"Noto Sans\";"
  "color: rgb(0, 0, 0);"
  "background-color: rgb(2, 180, 43);"
  "font-weight: 700;"
  "padding: 4px;"
  "border-style: outset;"
  "border-width: 2px;"
  "border-radius: 6px;"
  "border-color: #00000f;"
  "min-width: 1em;"
  "min-height: 1em;"
  "padding: 4px;"
  "opacity: 0.3;}"
  "QPushButton:hover {"
    "background-color: #328930;"
  "}"};

static const QString request_button_style{"QPushButton{"
  "font: 87 18pt \"Noto Sans\";"
  "color: rgb(0, 0, 0);"
  "background-color: magenta;"
  "font-weight: 700;"
  "padding: 4px;"
  "border-style: outset;"
  "border-width: 2px;"
  "border-radius: 6px;"
  "border-color: #00000f;"
  "min-width: 1em;"
  "min-height: 1em;"
  "padding: 4px;"
  "opacity: 0.3;}"
  "QPushButton:hover {"
    "background-color: red;"
  "}"};

PostDialog::PostDialog(QWidget *parent)
: QDialog(parent),
  ui(new Ui::Dialog)
{
  ui->setupUi(this);  
  ui->save->setStyleSheet(save_button_style);
  ui->posts->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
  ui->posts->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
  ui->posts->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
  ui->posts->horizontalHeader()->setStretchLastSection(true);
  ui->posts->setModel(&m_post_model);
  ui->posts->setItemDelegateForColumn(4, new StatusDelegate{parent, [this](const auto index)
  {
    if (index.isValid())
      SelectRow(index.row());
  }});

  QObject::connect(ui->save, &QPushButton::clicked, [this]
  {
    if (m_selected > m_post_model.posts().size())
      return;

    request_update(m_post_model.posts()[m_selected]);
    ui->save->setStyleSheet(request_button_style);
  });

  QObject::connect(ui->posts, &QTableView::clicked, [this](const QModelIndex& index)
  {
    SelectRow(index.row());
  });
}

void PostDialog::ReceiveData(const QVector<QString>& data)
{
  m_post_model.set_data(data);
}

void PostDialog::Update(const QVector<QString>& data)
{
  auto unselect = [this] { ui->postText->setText("No selection"); };
  for (int i = 0; i < m_post_model.posts().size(); i++)
  {
    auto& post = m_post_model.get_mutable_posts()[i];
    if (post.uuid == data[Platform::UUID_INDEX] && post.name == data[Platform::NAME_INDEX])
    {
      post = Platform::Post::from_vector(data).front();
      ui->save->setStyleSheet(save_button_style);
      unselect();
      break;
    }
  }
}

void PostDialog::SelectRow(int row)
{
  m_selected = row;
  ui->postText->setText(QString{"Row %0 selected: %1"}.arg(row).arg(m_post_model.posts()[row].content));
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
