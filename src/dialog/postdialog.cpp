#include "include/ui/postdialog.hpp"
#include "ui_postdialog.h"

static const QString save_button_style{"QPushButton{"
  "font: 87 18pt \"Noto Sans\""
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
  "opacity: 0.3;}"};

static const QString request_button_style{"QPushButton{"
  "font: 87 18pt \"Noto Sans\""
  "color: rgb(0, 0, 0);"
  "background-color: purple;"
  "font-weight: 700;"
  "padding: 4px;"
  "border-style: outset;"
  "border-width: 2px;"
  "border-radius: 6px;"
  "border-color: #00000f;"
  "min-width: 1em;"
  "min-height: 1em;"
  "padding: 4px;"
  "opacity: 0.3;}"};

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
  ui->posts->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
  ui->posts->horizontalHeader()->setStretchLastSection(true);
  ui->posts->setModel(&m_post_model);
  ui->posts->setItemDelegateForColumn(4, new StatusDelegate{parent, [this](auto index)
  {
    if (index.isValid())
      SelectRow(index.row());
  }});

  QObject::connect(ui->save, &QPushButton::clicked, [this]
  {
    if (m_selected > m_post_model.posts().size())
      return;

    request_update(m_post_model.posts()[m_selected]);
    KLOG("Request to update post");
    ui->save->OnRequest();
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
  auto unselect = [this] { ui->save->OnRequest(true); ui->selectionLabel->setText("No selection"); };
  for (int i = 0; i < m_post_model.posts().size(); i++)
  {
    auto& post = m_post_model.get_mutable_posts()[i];
    if (post.uuid == data[Platform::UUID_INDEX] && post.name == data[Platform::NAME_INDEX])
    {
      post = Platform::Post::from_vector(data).front();
      unselect();
      break;
    }
  }
}

void PostDialog::SelectRow(int row)
{
  auto get_selected_content = [this]
  {
    return m_post_model.posts()[m_selected].content;
//    return (text.size() < 60) ? text : text.left(60);
  };

  m_selected = row;
  ui->selectionLabel->setText(QString{"Row %0 selected: %1"}.arg(m_selected)
                                                            .arg(get_selected_content()));
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
