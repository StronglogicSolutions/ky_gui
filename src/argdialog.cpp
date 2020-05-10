#include <include/ui/argdialog.h>
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

ArgDialog::ArgDialog(QWidget *parent) : QDialog(parent), ui(new Ui::ArgDialog), m_task(nullptr), m_ig_post(IGPost{}) {
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
      m_ig_post.files.push_back(
          KFile{.name = file_name, .path = file_path, .type = is_video ? FileType::VIDEO : FileType::IMAGE});

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
    // TODO: argType values need to be set by configuration
    // Can this somehow be known via the flatbuffer schema? I think not
    // handling of type needs to be abstracted by a class which can be
    // subclassed for various types of task: Instagram, etc
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
                   this, [this](QAbstractButton *button) {
                     if (button->text() == "Save") {
                       if (m_ig_post.isReady()) {
                         setTaskArguments();
                         QVector<KFileData> k_file_v{};
                         k_file_v.reserve(m_ig_post.files.size());

                         for (const auto &kfile : m_ig_post.files) {
                           QFile file(kfile.path);
                           if (file.open(QIODevice::ReadOnly)) {
                             k_file_v.push_back(KFileData{
                                 .name = kfile.name, .type = kfile.type, .path = kfile.path, .bytes = file.readAll()});
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
  QString hashtags{};
  for (const auto &tag : std::get<VariantIndex::STRVEC>(m_task->getTaskArgument("hashtags"))) {
    hashtags += "#" + tag + " ";
  }
  hashtags.chop(1);
  QString requested_by{};
  for (const auto &name : std::get<VariantIndex::STRVEC>(m_task->getTaskArgument("requested_by"))) {
    requested_by += "@" + name + "";
  }
  if (m_ig_post.requested_by.size() > 1) {
    requested_by.chop(1);
  }
  m_task->setArgument("hashtags", hashtags);
  m_task->setArgument("requested_by", requested_by);
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
    auto row_index = ui->argList->currentRow();
    // If deleted item is a file, we need to remove it from the task
    auto type = ui->argList->item(row_index, 0);
    if (type->text() == "file") {
      auto value = ui->argList->item(row_index, 1);
      if (!value->text().isEmpty()) {
        auto file_it = std::find_if(m_ig_post.files.begin(), m_ig_post.files.end(),
                                    [value](const KFile &file) { return file.name == value->text(); });
        if (file_it != m_ig_post.files.end()) {  // If file was matched
          qDebug() << "Removing file from task arguments";
          m_ig_post.files.erase(file_it);
        }
      }
    }
    ui->argList->removeRow(row_index);
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
  QDateTime date_time = QDateTime::currentDateTime();
  ui->dateTime->setDateTime(date_time);
  m_task->clear();
  m_task->setDefaultValues();
  m_task->setArgument("datetime", date_time.toString());
  m_task->setArgument("user", ui->user->currentText());
  ui->argType->setCurrentIndex(0);
  ui->argList->setRowCount(0);
}

void ArgDialog::clearTask() { m_task->clear(); }

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
  if (ui->user->count() > 0) {
    m_ig_post.user = ui->user->itemText(0).toUtf8().constData();
  }
}

ArgDialog::~ArgDialog()
{
    delete ui;
}

void ArgDialog::accept() { qDebug() << "Sending request to schedule a task.."; }

void ArgDialog::setArgTypes() {
  ui->argType->clear();
  for (const auto &arg : m_task->getTaskArguments()) {
    ui->argType->addItem(arg->text());
  }
}
