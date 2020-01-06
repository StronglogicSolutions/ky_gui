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
            auto slash_index = file_path.lastIndexOf("/") + 1;
            QString file_name = file_path.right(file_path.size() - slash_index);

            addItem(file_name, "file");

            m_ig_post.video.name = file_name;
            m_ig_post.video.path = file_path;
        }
    });

    ui->argList->setHorizontalHeaderLabels(QStringList{"Value", "Type"});
    ui->argList->setColumnWidth(0, 400);
    ui->argList->setColumnWidth(1, 40);
    ui->argList->verticalHeader()->setDefaultSectionSize(100);

    QObject::connect(ui->addArgument, &QPushButton::clicked, this, [this]() {
        QString text = ui->argInput->toPlainText();
        auto type = ui->argType->currentText();
        if (text.size() > 0) {
            if (type == Args::HASHTAG_TYPE) {
                addHashtag(text);
            } else if (type == Args::DESCRIPTION_TYPE) {
                addItem(text, type);
                m_ig_post.description = text.toUtf8().constData();
            } else if (type == Args::PROMOTE_TYPE) {
                addOrReplaceInArgList(text, "promote/share");
                m_ig_post.promote_share = text.toUtf8().constData();
            } else if (type == Args::LINK_BIO_TYPE) {
                addOrReplaceInArgList(text, "link/bio");
                m_ig_post.link_in_bio = text.toUtf8().constData();
            } else if (type == Args::REQUESTED_BY_TYPE) {
                addRequestedBy(text);
            }
            ui->argInput->clear();
        }
    });

    QDateTime date_time = QDateTime::currentDateTime();
//    date_time.
    ui->dateTime->setDateTime(QDateTime::currentDateTime());

    QObject::connect(ui->dateTime, &QDateTimeEdit::dateTimeChanged, this, [this]() {
        auto date_time = ui->dateTime->dateTime();
        m_ig_post.datetime = std::string{date_time.toString().toUtf8().data()};
        qDebug() << "Time changed to" << date_time;
    });

    QObject::connect(ui->argCommandButtons, static_cast<void (QDialogButtonBox::*)(QAbstractButton*)>(&QDialogButtonBox::clicked), this, [this](QAbstractButton* button) {
        if (button->text() == "Save") {
            if (m_ig_post.isReady()) {
                setTaskArguments();
                QFile file(m_ig_post.video.path);
                std::vector<char> byte_array{};
                if (file.open(QIODevice::ReadOnly)) {
                    QByteArray bytes = file.readAll();
                    emit ArgDialog::uploadFile(bytes);
                    qDebug() << "Would be sending file..";
                } else {
                    QMessageBox::warning(
                        this,
                        tr("File Error"),
                        tr("Unable to read file")
                    );
                }
                emit ArgDialog::taskRequestReady(m_task, true);
            }
        }
    });

    QObject::connect(ui->devTestButton, &QPushButton::clicked, this, [this]() {
        clearPost();
        KFile file = KFile{.name="videofile", .path="videopath"};
        m_ig_post = IGPost{
            .description = "asdasdas",
            .datetime = "sdasadasd",
            .promote_share = "dfgdfg",
            .link_in_bio = "asdasd",
            .hashtags = {"tag1", "tag2"},
            .requested_by = {"person"},
            .video = file
        };
    });
}

void ArgDialog::setTaskArguments() {
    m_task.args.clear();
    std::string hashtags{};
    for (const auto & tag : m_ig_post.hashtags) {
        hashtags += "#" + tag + " ";
    }
    hashtags.pop_back();

    std::string requested_by{};
    for (const auto & name : m_ig_post.requested_by) {
        requested_by += "@" + name + "";
    }
    if (m_ig_post.requested_by.size() > 1) {
    requested_by.pop_back();
    }

    m_task.args.push_back(m_ig_post.video.name.toUtf8().constData());
    m_task.args.push_back(m_ig_post.datetime);
    m_task.args.push_back(m_ig_post.description);
    m_task.args.push_back(hashtags);
    m_task.args.push_back(requested_by);
    m_task.args.push_back(m_ig_post.requested_by_phrase);
    m_task.args.push_back(m_ig_post.promote_share);
    m_task.args.push_back(m_ig_post.link_in_bio);
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
}

void ArgDialog::addRequestedBy(QString value) {
    if (std::find(m_ig_post.requested_by.begin(), m_ig_post.requested_by.end(), value.toUtf8().constData()) == m_ig_post.requested_by.end()) {
        m_ig_post.requested_by.push_back(value.toUtf8().constData());
        addToArgList(value, "requested_by");
    } else {
        const char* message = "You have already inputed this name under \"requested_by\"";
        qDebug() << message;
        QMessageBox::warning(
            this,
            tr("Requested By"),
            tr(message)
            );
    }
}

void ArgDialog::addToArgList(QString value, QString type) {
    for (int i = 0; i < ui->argList->rowCount(); i++) {
        auto item = ui->argList->item(i, 1);
        if (item) {
            if (QString::compare(item->text(), type) == 0) {
                auto text = ui->argList->item(i, 0)->text();
                text.append("\n");
                text += value;
                ui->argList->item(i, 0)->setText(text);
                return;
            }
        }
    }
    addItem(value, type);
}

void ArgDialog::addOrReplaceInArgList(QString value, QString type) {
    for (int i = 0; i < ui->argList->rowCount(); i++) {
        auto item = ui->argList->item(i, 1);
        if (item) {
            if (QString::compare(item->text(), type) == 0) {
                ui->argList->item(i, 0)->setText(value);
                return;
            }
        }
    }
    addItem(value, type);
}

void ArgDialog::addHashtag(QString tag) {
    if (std::find(m_ig_post.hashtags.begin(), m_ig_post.hashtags.end(), tag.toUtf8().constData()) == m_ig_post.hashtags.end()) {
        m_ig_post.hashtags.push_back(tag.toUtf8().constData());
        addToArgList(tag, "hashtag");
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
