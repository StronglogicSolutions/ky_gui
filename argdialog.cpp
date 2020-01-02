#include "argdialog.h"
#include "ui_argdialog.h"
#include <algorithm>
#include <QDebug>
#include <QStringList>
#include <QIODevice>
#include <vector>
#include <QTableWidgetItem>
#include <QDateTime>
#include <QCalendarWidget>


ArgDialog::ArgDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ArgDialog),
    m_task(Task{}),
    m_ig_post(IGPost{})
{
    ui->setupUi(this);
    QObject::connect(ui->addFile, &QPushButton::clicked, this, [this]() {
        auto file_path = QFileDialog::getOpenFileName(this,
                                                tr("Open File"), "~", tr("All Files (*.*)"));
        qDebug() << "Selected file:" << file_path;
        if (file_path.size() > 0) {
            QFile file(file_path);
//            std::vector<char> byte_array{};
//            if (file.open(QIODevice::ReadOnly)) {
//                QByteArray bytes = file.readAll();
//                emit ArgDialog::uploadFile(bytes);
//            }
        }
        auto slash_index = file_path.lastIndexOf("/") + 1;
        QString file_name = file_path.right(file_path.size() - slash_index);

        addItem(file_name, "file");

        m_ig_post.video.name = file_name.toUtf8().constData();
        m_ig_post.video.path = file_path.toUtf8().constData();
    });

    ui->argList->setHorizontalHeaderLabels(QStringList{"Value", "Type"});
    ui->argList->setColumnWidth(0, 400);
    ui->argList->setColumnWidth(1, 40);
    ui->argList->verticalHeader()->setDefaultSectionSize(100);

    QObject::connect(ui->addArgument, &QPushButton::clicked, this, [this]() {
        QString text = ui->argInput->toPlainText();
        QList<QListWidgetItem*> types = ui->argType->selectedItems();
        auto type = types.size() > 0 ? types.at(0)->text() : "Unknown type";
        if (text.size() > 0) {
            addItem(text, type);
            if (type == Args::HASHTAG_TYPE) {
                addHashtag(text);
            } else if (type == Args::DESCRIPTION_TYPE) {
                m_ig_post.description = text.toUtf8().constData();
            }
            ui->argInput->clear();
        }
    });

    ui->argType->setItemSelected(ui->argType->item(0), true);
    QDateTime date_time = QDateTime::currentDateTime();
//    date_time.
    ui->dateTime->setDateTime(QDateTime::currentDateTime());

    QObject::connect(ui->dateTime, &QDateTimeEdit::dateTimeChanged, this, [this]() {
        auto date_time = ui->dateTime->dateTime();
        qDebug() << "Time changed to" << date_time;
    });
}

void ArgDialog::addItem(QString value, QString type) {
    QTableWidgetItem* item = new QTableWidgetItem(value);
    QTableWidgetItem* item2 = new QTableWidgetItem(type);
    auto row = ui->argList->rowCount();
    ui->argList->insertRow(row);
    ui->argList->setItem(row, 0, item);
    ui->argList->setItem(row, 1, item2);
}

void ArgDialog::clearPost() {
    m_ig_post.video = KFile{};
    m_ig_post.datetime = "";
    m_ig_post.hashtags = {};
    m_ig_post.description = "";
    m_ig_post.link_in_bio = "";
    m_ig_post.requested_by = {};
    m_ig_post.promote_share = "";
    m_ig_post.requested_by_phrase = "";
}

void ArgDialog::clearTask() {
    m_task.args = {};
    m_task.mask = -1;
    m_task.time = "";
}

void ArgDialog::addHashtag(QString tag) {
    if (std::find(m_ig_post.hashtags.begin(), m_ig_post.hashtags.end(), tag.toUtf8().constData()) == m_ig_post.hashtags.end()) {
        m_ig_post.hashtags.push_back(tag.toUtf8().constData());
    } else {
        const char* message = "Can't add the same hashtag twice";
        qDebug() << message;
        QMessageBox::warning(
            this,
            tr("Hashtags"),
            tr(message)
        );
    }
}

ArgDialog::~ArgDialog()
{
    delete ui;
}
