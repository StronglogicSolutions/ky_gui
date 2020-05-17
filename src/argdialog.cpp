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

using namespace Scheduler;

ArgDialog::ArgDialog(QWidget *parent) : QDialog(parent), ui(new Ui::ArgDialog), m_task(nullptr) {
  ui->setupUi(this);

  m_task = new InstagramTask{};
  m_task->defineTaskArguments();
  m_task->setDefaultValues();

  for (const auto& name : m_task->getArgumentNames()) {
    ui->argType->addItem(name, QVariant::String);
  }

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
      QFile file(file_path);

      if (file.open(QIODevice::ReadOnly)) {
        QMimeDatabase db;
        auto is_video = db.mimeTypeForFile(file_path).name().contains("video");
        addItem(file_name, "file");
        m_task->addArgument("files", Scheduler::KFileData{
                                         .name = file_name,
                                         .type = is_video ? FileType::VIDEO : FileType::IMAGE,
                                         .path = file_path,
                                         .bytes = file.readAll()});

        if (is_video) {
          qDebug() << "File discovered to be video";
          m_task->setArgument("is_video", true);
          QString preview_filename = FileUtils::generatePreview(file_path, file_name);

          QString preview_file_path = QCoreApplication::applicationDirPath()
                                   + "/assets/previews/" + preview_filename;
          file.setFileName(preview_file_path);
          if (file.open(QIODevice::ReadOnly)) {
            // TODO: create some way of verifying preview generation was successful
            addFile("assets/previews/" + preview_filename);
            addItem(preview_filename, "file");
            addFile("assets/previews/" + preview_filename);
            m_task->addArgument("files", Scheduler::KFileData{
                                             .name = preview_filename,
                                             .type = is_video ? FileType::VIDEO : FileType::IMAGE,
                                             .path = preview_file_path,
                                             .bytes = file.readAll()});
          } else {
            qDebug() << "Could not add preview image for video";
            QMessageBox::warning(this, tr("File Error"), tr("Could not add preview image for video"));
          }
        } else {
          addFile(file_path);
        }
      } else {
        qDebug() << "Unable to open selected file";
        QMessageBox::warning(this, tr("File Error"), tr("Unable to open selected file"));
      }
    } else {
      qDebug() << "Could not read the file path";
      QMessageBox::warning(this, tr("File Error"), tr("Could not read the file path"));
    }
  });

  QObject::connect(ui->user, &QComboBox::currentTextChanged, this,
                   [this](const QString &username) {
                     m_task->setArgument("user", username);
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
        m_task->setArgument("description", escapeText(text));
      } else if (type == Args::PROMOTE_TYPE) {
        addOrReplaceInArgList(text, "promote/share");
        m_task->setArgument("promote_share", text);
      } else if (type == Args::LINK_BIO_TYPE) {
        addOrReplaceInArgList(text, "link/bio");
        m_task->setArgument("link_in_bio", text);
      } else if (type == Args::REQUESTED_BY_TYPE) {
        addRequestedBy(text);
      } else if (type == Args::HEADER_TYPE) {
        addItem(text, type);
        m_task->setArgument("header", text);
      } else if (type == Args::REQUESTED_BY_PHRASE) {
        addItem(text, type);
        m_task->setArgument("requested_by_phrase", text);
      }
      ui->argInput->clear();
    }
  });

  ui->dateTime->setDateTime(QDateTime::currentDateTime());

  QObject::connect(ui->dateTime, &QDateTimeEdit::dateTimeChanged, this, [this]() {
    auto date_time = ui->dateTime->dateTime();
    m_task->setArgument("datetime", QString::number(date_time.toTime_t()));
    auto new_time = std::get<Scheduler::VariantIndex::QSTRING>(m_task->getTaskArgument("datetime"));
    qDebug() << "Time changed to" << date_time;
  });

  QObject::connect(ui->argCommandButtons,
                   static_cast<void (QDialogButtonBox::*)(QAbstractButton *)>(
                       &QDialogButtonBox::clicked),
                   this, [this](QAbstractButton *button) {
                     if (button->text() == "Save") {
                       setTaskArguments();
                       if (m_task->isReady()) {
                         emit ArgDialog::taskRequestReady(m_task);
                       }
                     }
                   });

  QObject::connect(ui->clear, &QPushButton::clicked, this, [this]() {
    clearPost();
    ui->argList->setRowCount(0);
    qDebug() << "Task cleared and restored to default values";
  });
}

/**
 * @brief ArgDialog::setTaskArguments
 */
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
  if (requested_by.size() > 1) {
    requested_by.chop(1);
  }
  m_task->setArgument("hashtags_string", hashtags);
  m_task->setArgument("requested_by_string", requested_by);
}

/**
 * @brief ArgDialog::addItem
 * @param value
 * @param type
 */
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
        // All of the following should be done by the task
        QVector<Scheduler::KFileData> task_files = std::get<VariantIndex::FILEVEC>(m_task->getTaskArgument("files"));
        auto file_it = std::find_if(task_files.begin(), task_files.end(),
                                    [value](const Scheduler::KFileData &file) { return file.name == value->text(); });
        if (file_it != task_files.end()) {  // If file was matched
          qDebug() << "Removing file from task arguments";
          task_files.erase(file_it);
          m_task->setArgument("files", task_files); // Probably not necessary. Try without doing this
        }
      }
    }
    ui->argList->removeRow(row_index);
  });
  ui->argList->setItem(row, 0, item);
  ui->argList->setItem(row, 1, item2);
  ui->argList->setCellWidget(row, 3, q_pb);
}

/**
 * @brief ArgDialog::addFile
 * @param path
 */
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

/**
 * @brief ArgDialog::clearPost
 */
void ArgDialog::clearPost() {
  QDateTime date_time = QDateTime::currentDateTime();
  ui->dateTime->setDateTime(date_time);
  m_task->clear();
  m_task->setDefaultValues();
  m_task->setArgument("datetime", QString::number(date_time.toTime_t()));
  m_task->setArgument("user", ui->user->currentText());
  ui->argType->setCurrentIndex(0);
  ui->argList->setRowCount(0);
}

/**
 * @brief ArgDialog::clearTask
 */
void ArgDialog::clearTask() { m_task->clear(); }

/**
 * @brief ArgDialog::addRequestedBy
 * @param value
 */
void ArgDialog::addRequestedBy(QString value) {
  QStringList names = value.split(" ");
  QVector<QString> requested_by_names = std::get<VariantIndex::STRVEC>(m_task->getTaskArgument("requested_by"));
  for (const auto &name : names) {
    if (std::find(requested_by_names.begin(), requested_by_names.end(), value.toUtf8().constData()) == requested_by_names.end()) {
      m_task->addArgument("requested_by", name);
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

/**
 * @brief ArgDialog::addToArgList
 * @param value
 * @param type
 */
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

/**
 * @brief ArgDialog::addOrReplaceInArgList
 * @param value
 * @param type
 */
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

/**
 * @brief ArgDialog::addHashtag
 * @param tag
 */
void ArgDialog::addHashtag(QString tag) {
    QStringList tags = tag.split(" ");
    for (const auto& tag : tags) {
      QVector<QString> hashtags = std::get<VariantIndex::STRVEC>(m_task->getTaskArgument("hashtags"));
        if (std::find(hashtags.begin(), hashtags.end(), tag.toUtf8().constData()) == hashtags.end()) {
          m_task->addArgument("hashtags", tag);
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

/**
 * @brief ArgDialog::keyPressEvent
 * @param e
 */
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

/**
 * @brief ArgDialog::setFilePath
 * @param path
 */
void ArgDialog::setFilePath(QString path) { m_file_path = path; }

/**
 * @brief ArgDialog::setConfig
 * @param config_string
 */
void ArgDialog::setConfig(QString config_string) {
  m_config_string = config_string;
  ui->user->addItems(getValueArgs(m_config_string.toUtf8(), "users"));
  if (ui->user->count() > 0) {
    m_task->setArgument("user", ui->user->itemText(0));
  }
}

/**
 * @brief ArgDialog::~ArgDialog
 */
ArgDialog::~ArgDialog() {
  delete m_task;
  delete ui;
}

void ArgDialog::accept() { qDebug() << "Sending request to schedule a task.."; }

void ArgDialog::setArgTypes() {
  ui->argType->clear();
  for (const auto &arg : m_task->getTaskArguments()) {
    ui->argType->addItem(arg->text());
  }
}

void ArgDialog::notifyClientSuccess() {
  clearPost();
}
