#include <include/argdialog.h>
#include <ui_argdialog.h>
#include <QCalendarWidget>
#include <QDateTime>
#include <QDebug>
#include <QIODevice>
#include <QMimeDatabase>
#include <QStringList>
#include <QTableWidgetItem>
#include <algorithm>
#include <vector>

ArgDialog::ArgDialog(QWidget *parent)
    : QDialog(parent),
      ui(new Ui::ArgDialog),
      m_task(Task{}),
      m_ig_post(IGPost{}) {
  ui->setupUi(this);

  ui->argCommandButtons->button(QDialogButtonBox::Close)
      ->setStyleSheet(QString("background:%1").arg("#2f535f"));
  QObject::connect(ui->addFile, &QPushButton::clicked, this, [this]() {
    KFileDialog file_dialog{};
    auto file_path = file_dialog.openFileDialog(m_file_path);
    qDebug() << "Selected file:" << file_path;
    if (file_path.size() > 0) {
      auto slash_index = file_path.lastIndexOf("/") + 1;
      QString file_name = file_path.right(file_path.size() - slash_index);
      QString dir = file_path.left(slash_index);
      qDebug() << "Dir is " << dir;
      QMimeDatabase db;
      auto is_video = db.mimeTypeForFile(file_path).name().contains("video");
      addItem(file_name, "file");
      m_ig_post.files.push_back(KFile{.name = file_name,
                                      .path = file_path,
                                      .type = is_video ? FileType::VIDEO : FileType::IMAGE});

      if (is_video) {
        qDebug() << "File discovered to be video";
        m_ig_post.is_video = true; // rename to "sending_video"
        QString preview_filename = FileUtils::generatePreview(file_path, file_name);
        // TODO: create some way of verifying preview generation was successful
        addFile("assets/previews/" + preview_filename);
        addItem(preview_filename, "file");
        addFile("assets/previews/" + preview_filename);
        m_ig_post.files.push_back(KFile{.name = preview_filename,
                                        .path = QCoreApplication::applicationDirPath()
                                                + "/assets/previews/" + preview_filename,
                                        .type = is_video ? FileType::VIDEO : FileType::IMAGE});
      } else {
        addFile(file_path);
      }
    }
  });

  QObject::connect(ui->user, &QComboBox::currentTextChanged, this,
                   [this](const QString &text) {
                     m_ig_post.user = text.toUtf8().constData();
                   });

  ui->argList->setHorizontalHeaderLabels(
      QStringList{"Type", "Value", "Preview", "Delete"});
  ui->argList->setColumnWidth(0, 40);
  ui->argList->setColumnWidth(1, 520);
  ui->argList->setColumnWidth(2, 220);
  ui->argList->setColumnWidth(3, 30);
  ui->argList->verticalHeader()->setDefaultSectionSize(100);

  QObject::connect(ui->addArgument, &QPushButton::clicked, this, [this]() {
    QString text = ui->argInput->toPlainText();
    auto type = ui->argType->currentText();
    if (text.size() > 0) {
      if (type == Args::HASHTAG_TYPE) {
        addHashtag(text);
      } else if (type == Args::DESCRIPTION_TYPE) {
        addItem(text, type);
        m_ig_post.description = escapeText(text).toUtf8().constData();
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

  ui->dateTime->setDateTime(QDateTime::currentDateTime());

  QObject::connect(ui->dateTime, &QDateTimeEdit::dateTimeChanged, this, [this]() {
    auto date_time = ui->dateTime->dateTime();
    m_ig_post.datetime = std::string{std::to_string(date_time.toTime_t())};
    qDebug() << "Time changed to" << date_time;
  });

  QObject::connect(ui->argCommandButtons,
                   static_cast<void (QDialogButtonBox::*)(QAbstractButton *)>(
                     &QDialogButtonBox::clicked),
                   this,
                   [this](QAbstractButton *button) {
                     if (button->text() == "Save") {
                       if (m_ig_post.isReady()) {
                         setTaskArguments();
                         QVector<KFileData> k_file_v{};
                         k_file_v.reserve(m_ig_post.files.size());

                         for (const auto &kfile : m_ig_post.files) {
                           QFile file(kfile.path);
                           if (file.open(QIODevice::ReadOnly)) {
                             k_file_v.push_back(KFileData{.type = kfile.type,
                                                          .name = kfile.name,
                                                          .bytes = file.readAll()});
                           } else {
                             QMessageBox::warning(this, tr("File Error"), tr("Unable to read file"));
                           }
                         }
                         if (!k_file_v.empty()) {
                           emit ArgDialog::uploadFiles(k_file_v);
                         }

                         emit ArgDialog::taskRequestReady(m_task, true);
                       }
                       clearPost(); // reset m_ig_post to default values
                     }
                   });

  QObject::connect(ui->clear, &QPushButton::clicked, this, [this]() {
    clearPost();
    ui->argList->setRowCount(0);
    qDebug() << "Task cleared and restored to default values";
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

    m_task.args.push_back(m_ig_post.datetime);
    m_task.args.push_back(m_ig_post.description);
    m_task.args.push_back(hashtags);
    m_task.args.push_back(requested_by);
    m_task.args.push_back(m_ig_post.requested_by_phrase);
    m_task.args.push_back(m_ig_post.promote_share);
    m_task.args.push_back(m_ig_post.link_in_bio);
    m_task.args.push_back(std::to_string(m_ig_post.is_video));
    m_task.args.push_back(m_ig_post.header);
    m_task.args.push_back(m_ig_post.user);
}

void ArgDialog::addItem(QString value, QString type) {
  QTableWidgetItem *item = new QTableWidgetItem(type);
  QTableWidgetItem *item2 = new QTableWidgetItem(value);
  auto row = ui->argList->rowCount();
  ui->argList->insertRow(row);
  QPushButton *q_pb = new QPushButton();
  q_pb->setText("Delete");
  q_pb->setIcon(std::move(QIcon(":/icons/icons/quit.png")));
  QObject::connect(q_pb, &QPushButton::clicked, this, [this]() {
    ui->argList->removeRow(ui->argList->currentRow());
  });
  ui->argList->setItem(row, 0, item);
  ui->argList->setItem(row, 1, item2);
  ui->argList->setCellWidget(row, 3, q_pb);
}

void ArgDialog::addFile(QString path) {
  auto row_count = ui->argList->rowCount();

  QTableWidgetItem *file_item = new QTableWidgetItem();
  QPixmap pm{path};
  file_item->setData(
      Qt::DecorationRole,
      pm.scaledToHeight(ui->argList->rowHeight(0),
                        Qt::TransformationMode::SmoothTransformation));
  ui->argList->setItem(row_count - 1, 2, file_item);
}

void ArgDialog::clearPost() {
  m_ig_post.files.clear();
  m_ig_post.header = "Learn to speak like native Korean speakers 🙆‍♀️🇰🇷";
  QDateTime date_time = QDateTime::currentDateTime();
  ui->dateTime->setDateTime(date_time);
  m_ig_post.datetime = date_time.toTime_t();
  m_ig_post.hashtags.clear();
  m_ig_post.description = "";
  m_ig_post.link_in_bio = "Subscribe to my YouTube channel (link 🔗in bio) to learn more about "
                          "Korean language and culture ❤";
  m_ig_post.requested_by.clear();
  m_ig_post.promote_share = "Share the post through IG story if you enjoy the phrase 🙋‍♀️";
  m_ig_post.requested_by_phrase = "The phrase was requested by ";
  m_ig_post.user = ui->user->currentText().toUtf8().constData();
  ui->argType->setCurrentIndex(0);
  ui->argList->setRowCount(0);
}

void ArgDialog::clearTask() {
  m_task.args.clear();
  m_task.mask = -1;
}

void ArgDialog::addRequestedBy(QString value) {
  QStringList names = value.split(" ");
  for (const auto &name : names) {
    if (std::find(m_ig_post.requested_by.begin(), m_ig_post.requested_by.end(), value.toUtf8().constData()) == m_ig_post.requested_by.end()) {
      m_ig_post.requested_by.push_back(name.toUtf8().constData());
      addToArgList(name, "requested_by");
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
}

void ArgDialog::addToArgList(QString value, QString type) {
  for (int i = 0; i < ui->argList->rowCount(); i++) {
    auto item = ui->argList->item(i, 0);
    if (item) {
      if (QString::compare(item->text(), type) == 0) {
        auto text = ui->argList->item(i, 1)->text();
        text.append("\n");
        text += value;
        ui->argList->item(i, 1)->setText(text);
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
            ui->argList->item(i, 1)->setText(value);
            return;
          }
        }
    }
    addItem(value, type);
}

void ArgDialog::addHashtag(QString tag) {
    QStringList tags = tag.split(" ");
    for (const auto& tag : tags) {
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
}

void ArgDialog::keyPressEvent(QKeyEvent *e) {
    if (Qt::ControlModifier) {
        if(e->key()==Qt::Key_Return || e->key()==Qt::Key_Enter) {
            ui->addArgument->clicked();
            auto idx = ui->argType->currentIndex();
            if (idx != (ui->argType->count() - 1)) {
                ui->argType->setCurrentIndex(idx + 1);
            }
        }
    }
}

void ArgDialog::setFilePath(QString path) { m_file_path = path; }

void ArgDialog::setConfig(QString config_string) {
  m_config_string = config_string;
  ui->user->addItems(getValueArgs(m_config_string.toUtf8(), "users"));
}

ArgDialog::~ArgDialog()
{
    delete ui;
}

void ArgDialog::accept() { qDebug() << "Sending request to schedule a task.."; }
