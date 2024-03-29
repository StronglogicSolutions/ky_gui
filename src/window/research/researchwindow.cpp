#include "include/ui/researchwindow.hpp"
#include "ui_researchwindow.h"
#include <QTableView>
#include <QKeyEvent>
#include <util/util.hpp>

ResearchWindow::ResearchWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::ResearchWindow)
{ 
  ui->setupUi(this);
  ui->termTable->setColumnWidth(0, 60);
  ui->termTable->setColumnWidth(1, 200);
  ui->termTable->setColumnWidth(2, 200);
  ui->termTable->setColumnWidth(3, 200);
  ui->termTable->setColumnWidth(4, 200);
  ui->termTable->setColumnWidth(5, 200);
  ui->termTable->setColumnWidth(6, 160);

  QObject::connect(ui->termTable->horizontalHeader(), &QHeaderView::sectionClicked, this,
    [this](int32_t index)
    {
      ui->termTable->sortByColumn(index, ui->termTable->horizontalHeader()->sortIndicatorOrder());
    });
}

void ResearchWindow::ReceiveData(const QVector<QString>& argv)
{
  for (auto i = 0; i < argv.size(); i++)
  {
    const auto object        = QJsonDocument::fromJson(argv[i].toUtf8()).object();
    ui->termTable->setRowCount(ui->termTable->rowCount() + 1);
    auto id_data       = new QTableWidgetItem{};
    id_data->setData(Qt::DisplayRole, object["id"].toString().toUInt());
    ui->termTable->setItem(i, 0, id_data);
//    ui->termTable->setItem(i, 0, new QTableWidgetItem{object["id"].toInt()});
    ui->termTable->setItem(i, 1, new QTableWidgetItem{object["term"].toString()});
    ui->termTable->setItem(i, 2, new QTableWidgetItem{object["type"].toString()});
    ui->termTable->setItem(i, 3, new QTableWidgetItem{object["user"].toString()});
    ui->termTable->setItem(i, 4, new QTableWidgetItem{object["person"].toString()});
    ui->termTable->setItem(i, 5, new QTableWidgetItem{object["organization"].toString()});
    ui->termTable->setItem(i, 6, new QTableWidgetItem{object["time"].toString()});
  }
}

ResearchWindow::~ResearchWindow()
{
  delete ui;
}

void ResearchWindow::keyPressEvent(QKeyEvent *e)
{
  if (e->key() == Qt::Key_Escape)
    close();
}
