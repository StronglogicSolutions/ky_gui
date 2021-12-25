#include <include/ui/argdialog.h>
#include <ui_argdialog.h>
#include <QCalendarWidget>
#include <QDebug>
#include <QIODevice>
#include <QBuffer>
#include <QMimeDatabase>
#include <QStringList>
#include <QTableWidgetItem>
#include <algorithm>
#include <vector>

using namespace Scheduler;

static const char JPG_FORMAT[]{"JPG"};

bool isSave(QString s) {
  return (s.compare("Save") == 0 || s.compare("&Save") == 0);
}

ArgDialog::ArgDialog(QWidget *parent) : QDialog(parent), ui(new Ui::ArgDialog), m_task(nullptr) {
  ui->setupUi(this);

  m_loader = new QMovie{":/icons/loader.gif"};
  m_loader_layout.addWidget(ui->loaderMovie);
  m_loader_layout.addWidget(ui->loaderText);
  m_loader_widget.setLayout(&m_loader_layout);

  ui->argCommandButtons->button(QDialogButtonBox::Close)->setStyleSheet(QString("background:%1").arg("#2f535f"));

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
        QByteArray    bytes{};
        QMimeDatabase db{};
        QMimeType     mime_type = db.mimeTypeForFile(file_path);
        FileType      file_type{};
        auto          is_video  = mime_type.name().contains("video");
        addItem(file_name, "file");

        if (!is_video)
        {
                QBuffer buffer{&bytes};
                QImage  processed_image;
          const QImage  image{file_path};
                QSize   image_size  = image.size();
          const auto    height      = image_size.height();
          const auto    width       = image_size.width();
          const bool    is_IG       = (m_task->getType() == TaskType::INSTAGRAM);
          const char*   format      = (is_IG) ? JPG_FORMAT :
                                                mime_type.preferredSuffix().toUtf8().constData();

          if (is_IG && height != width)
          {
            processed_image = (height > width) ?
              image.copy(QRect{                   0, (height - width) / 2, width,  width}) :
              image.copy(QRect{(width - height) / 2,                    0, height, height});
          }
          else
            processed_image = image;

          processed_image.save(&buffer, format);
          addFile(file_path);
          file_type = FileType::IMAGE;
        }
        else
        {
          file_type = FileType::VIDEO;
          bytes = file.readAll();
        }

        m_task->addArgument("files", Scheduler::KFileData{
                                         .name  = file_name,
                                         .type  = file_type,
                                         .path  = file_path,
                                         .bytes = bytes});

        if (is_video)
        {
          qDebug() << "File is video";
          m_task->setArgument("is_video", true);
          QString preview_filename  = FileUtils::generatePreview(file_path, file_name);
          QString preview_file_path = QCoreApplication::applicationDirPath() + "/assets/previews/" + preview_filename;
          file.setFileName(preview_file_path);          

          if (file.open(QIODevice::ReadOnly))
          {
            // TODO: create some way of verifying preview generation was successful
            addFile("assets/previews/" + preview_filename);
            addItem(preview_filename, "file");
            addFile("assets/previews/" + preview_filename);
            m_task->addArgument("files", Scheduler::KFileData{
                                           .name  = preview_filename,
                                           .type  = is_video ? FileType::VIDEO : FileType::IMAGE,
                                           .path  = preview_file_path,
                                           .bytes = file.readAll()});
          }
          else
          {
            qDebug() << "Could not add preview image for video";
            QMessageBox::warning(this, tr("File Error"), tr("Could not add preview image for video"));
          }
        }
      }
      else
      {
        qDebug() << "Unable to open selected file";
        QMessageBox::warning(this, tr("File Error"), tr("Unable to open selected file"));
      }
    }
    else
    {
      qDebug() << "Could not read the file path";
      QMessageBox::warning(this, tr("File Error"), tr("Could not read the file path"));
    }
  });

  QObject::connect(ui->user, &QComboBox::currentTextChanged, this,
    [this](const QString &username) {
      m_task->setArgument("user", username);
    });

  ui->argList->setColumnWidth(0, 40);
  ui->argList->setColumnWidth(1, 520);
  ui->argList->setColumnWidth(2, 220);
  ui->argList->setColumnWidth(3, 30);
  ui->argList->setHorizontalHeaderLabels(QStringList{"Type", "Value", "Preview", "Delete"});
  ui->argList->verticalHeader()->setDefaultSectionSize(100);

  QObject::connect(ui->addArgument, &QPushButton::clicked, this, [this]() {
    QString text = ui->argInput->toPlainText();
    auto type = ui->argType->currentText();
    if (text.size() > 0) {
      if (type == Args::HASHTAG_TYPE) {
        addHashtag(text);
      }
      else
      if (type == Args::DESCRIPTION_TYPE) {
        addItem(text, type);
        m_task->setArgument(Args::DESCRIPTION_TYPE, text);
      }
      else
      if (type == Args::PROMOTE_TYPE) {
        addOrReplaceInArgList(text, Args::PROMOTE_TYPE);
        m_task->setArgument(Args::PROMOTE_TYPE, text);
      }
      else
      if (type == Args::LINK_BIO_TYPE) {
        addOrReplaceInArgList(text, Args::LINK_BIO_TYPE);
        m_task->setArgument(Args::LINK_BIO_TYPE, text);
      }
      else
      if (type == Args::REQUESTED_BY_TYPE) {
        addRequestedBy(text);
      }
      else
      if (type == Args::HEADER_TYPE) {
        addItem(text, type);
        m_task->setArgument(Args::HEADER_TYPE, text);
      }
      else
      if (type == Args::REQUESTED_BY_PHRASE) {
        addItem(text, type);
        m_task->setArgument(Args::REQUESTED_BY_PHRASE, text);
      }
      ui->argInput->clear();
    }
  });

  ui->dateTime->setDateTime(QDateTime::currentDateTime());

  QObject::connect(ui->dateTime, &QDateTimeEdit::dateTimeChanged, this, [this]() {
    m_task->setArgument("datetime", QString::number(ui->dateTime->dateTime().toTime_t()));
  });

  QObject::connect(ui->addRuntimeArg, &QPushButton::clicked, this, [this]() {
    if (m_task->getType() == TaskType::INSTAGRAM) {
      const char* message{"Runtime arguments are not available for Instagram tasks"};
      qDebug() << message;
      QMessageBox::warning(
        this,
        tr("Requested By"),
        tr(message)
      );
      return;
    }

    auto arg_text = ui->runtimeArgEdit->text();
    addOrReplaceInArgList(arg_text, Args::RUNTIME_ARG_TYPE);
    m_task->addArgument("runtime", arg_text);
    ui->runtimeArgEdit->clear();
    qDebug() << "Added process runtime argument: " << arg_text;
  });

  QObject::connect(ui->argCommandButtons, static_cast<void (QDialogButtonBox::*)(QAbstractButton *)>(&QDialogButtonBox::clicked), this,
    [this](QAbstractButton *button)
    {
      if (isSave(button->text()))
      {
        setTaskArguments();
        uint task_date_time = std::get<Scheduler::VariantIndex::QSTRING>(
                                 m_task->getTaskArgumentValue("datetime")).toUInt();
        if (task_date_time <= TimeUtils::unixtime())        
         UI::infoMessageBox("This was set to the past", "DateTime Warning");


        if (m_task->isReady())
        {
         emit ArgDialog::taskRequestReady(m_task);
         m_pending_task = m_task;
         m_task = nullptr;
         m_task = createTask(m_app_name);
         clearPost();
        //                          displayLoader(true);
        }
        else
         UI::infoMessageBox("Task is still missing arguments", "Task Verification Error");
      }
    }
  );

  QObject::connect(ui->clear, &QPushButton::clicked, this, [this]() {
    clearPost();
    ui->argList->setRowCount(0);
    qDebug() << "Task cleared and restored to default values";
  });

  QObject::connect(ui->recurring, &QComboBox::currentTextChanged, this, [this](const QString& text) {
    if (m_task->getType() == Scheduler::TaskType::GENERIC) {
      m_task->setArgument("recurring", findTaskFrequency(text));
    }
  });

  QObject::connect(ui->notification, &QComboBox::currentTextChanged, this, [this](const QString& text) {
    if (m_task->getType() == Scheduler::TaskType::GENERIC) {
      m_task->setArgument("notify", text.compare("Yes") == 0 ? true : false);
    }
  });
}

void ArgDialog::showEvent(QShowEvent* event) {
  if (event->type() == QEvent::Show) {
    if (m_task == nullptr || m_task->getTaskCode() != findTaskCode(m_app_name)) {
      m_task = createTask(m_app_name);
      if (m_task->getType() == INSTAGRAM) {   // recurring and notification should
        ui->recurring->hide();                // only be visible for generic tasks
        ui->recurringLabel->hide();
        ui->notification->hide();
        ui->notificationLabel->hide();
      } else if (ui->recurring->isHidden()) {
        ui->recurring->show();
        ui->recurringLabel->show();
        ui->notification->show();
        ui->notificationLabel->show();
      }
    }

    ui->argType->clear();

    for (const auto& name : m_task->getArgumentNames())
      ui->argType->addItem(name, QVariant::String);

    if (m_config.contains("users"))
    {
      ui->user->clear();
      ui->user->addItems(configValueToQList("users", m_config));
    }

    if (ui->user->count() > 0)
      m_task->setArgument("user", ui->user->itemText(0));

    m_task->setArgument("datetime", QString::number(ui->dateTime->dateTime().toTime_t()));
  }
}

/**
 * @brief ArgDialog::setTaskArguments
 */
void ArgDialog::setTaskArguments() {
  auto type = m_task->getType();
  if (type == TaskType::INSTAGRAM) {
    QString hashtags{};
    for (const auto &tag : std::get<VariantIndex::STRVEC>(m_task->getTaskArgumentValue("hashtags"))) {
      hashtags += '#' + tag + ' ';
    }
    hashtags.chop(1);
    QString requested_by{};
    for (const auto &name : std::get<VariantIndex::STRVEC>(m_task->getTaskArgumentValue("requested_by"))) {
      requested_by += '@' + name + ' ';
    }
    m_task->setArgument("hashtags_string", hashtags);
    m_task->setArgument("requested_by_string", requested_by);
  }
  else
  if (type == TaskType::GENERIC) {
    QString runtime_string{};
    for (const auto& arg : std::get<VariantIndex::STRVEC>(m_task->getTaskArgumentValue("runtime"))) {
      runtime_string += arg + " ";
    }
    runtime_string.chop(1);
    m_task->setArgument("runtime_string", runtime_string);
  }
}

/**
 * @brief ArgDialog::addItem
 * @param value
 * @param type
 */
void ArgDialog::addItem(QString value, QString type) {
  auto row = ui->argList->rowCount(); // insert row
  ui->argList->insertRow(row);
  // Create UI elements
  QTableWidgetItem *item = new QTableWidgetItem(type);
  QTableWidgetItem *item2 = new QTableWidgetItem(value);
  QPushButton *q_pb = new QPushButton();
  q_pb->setText("Delete");
  q_pb->setIcon(std::move(QIcon(":/icons/icons/quit.png")));
  // Delete listener
  QObject::connect(q_pb, &QPushButton::clicked, this, [this]() {
    auto row_index = ui->argList->currentRow();
    auto name = ui->argList->item(row_index, 0)->text();
    if (name == "file") { // UI displays files as type "file", but the argument is named "files"
      name = Args::FILE_TYPE;
    }
    auto value = ui->argList->item(row_index, 1)->text();
    for (auto&& s : value.split("\n")) { // If there are multiple values, they are separated by line breaks
      m_task->removeArgument(name, s);
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
  m_task->clear();
  m_task->setDefaultValues();
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
  QVector<QString> requested_by_names =
      std::get<VariantIndex::STRVEC>(
          m_task->getTaskArgumentValue("requested_by"));

  for (const auto &name : names) {
    if (std::find(
          requested_by_names.begin(), requested_by_names.end(),  value.toUtf8().constData()) == requested_by_names.end()) {
      m_task->addArgument("requested_by", name);
      addToArgList(name, "requested_by");
    } else {
        const char* message =
          "You have already inputed this name under \"requested_by\"";
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

static const char findSplitChar(const QString& s)
{
  if (s.contains('\n'))
    return '\n';
  else if (s.contains(' '))
    return ' ';
  else
    return 'x';
}

static bool noSplit(const char& c)
{
  return c == 'x';
}

/**
 * @brief ArgDialog::addHashtag
 * @param tag
 */
void ArgDialog::addHashtag(QString tag) {
  const char split_char = findSplitChar(tag);
    QStringList tags = noSplit(split_char) ? QStringList{tag} : tag.split(split_char);
    for (const auto& tag : tags) {
      QVector<QString> hashtags = std::get<VariantIndex::STRVEC>(m_task->getTaskArgumentValue(Args::HASHTAG_TYPE));
        if (std::find(hashtags.begin(), hashtags.end(), tag.toUtf8().constData()) == hashtags.end()) {
          m_task->addArgument(Args::HASHTAG_TYPE, tag);
          addToArgList(tag, Args::HASHTAG_TYPE);
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
    if (e->key()==Qt::Key_Return || e->key()==Qt::Key_Enter) {
      if (ui->argInput->hasFocus()) {
        ui->addArgument->clicked();
        auto idx = ui->argType->currentIndex();
        if (idx != (ui->argType->count() - 1)) {
          ui->argType->setCurrentIndex(idx + 1);
        }
      }
      else
      if (ui->runtimeArgEdit->hasFocus()) {
        (*(ui->addRuntimeArg)).clicked();
      }
    }
  }
}

/**
 * @brief ArgDialog::setFilePath
 * @param path
 */
void ArgDialog::setFilePath(QString path) { m_file_path = path; }

void ArgDialog::setAppName(QString app_name) {
  m_app_name = app_name;
}

/**
 * @brief ArgDialog::setConfig
 * @param config_string
 */
void ArgDialog::setConfig(QJsonObject config) {
  m_config = config;
}

/**
 * @brief ArgDialog::~ArgDialog
 */
ArgDialog::~ArgDialog() {
  delete m_task;
  if (m_pending_task == nullptr) {
    delete m_pending_task;
  }
  delete m_loader;
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
//   displayLoader(false);
  if (m_pending_task != nullptr) {
    delete m_pending_task;
  } else {
    clearPost();
  }
}

void ArgDialog::displayLoader(bool visible) {
  if (visible) {
    auto height = 400;
    auto width = 480;
    m_loader_widget.setMaximumSize(width, height);
    m_loader_widget.setMinimumSize(width, height);
    m_loader_widget.show();
    m_loader_widget.activateWindow();
    m_loader_widget.raise();
    ui->loaderMovie->setMovie(m_loader);
    ui->loaderMovie->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ui->loaderMovie->show();
    ui->loaderMovie->activateWindow();
    ui->loaderMovie->raise();
    ui->loaderMovie->setMaximumSize(width, height);
    ui->loaderMovie->setMinimumSize(width, height / 2);
    ui->loaderText->setMaximumSize(width, height);
    ui->loaderText->setMinimumSize(width, height / 2);
    m_loader->start();
  } else {
    m_loader_widget.hide();
    ui->loaderMovie->hide();
    ui->loaderMovie->setVisible(false);
    ui->loaderText->hide();
    ui->loaderText->setVisible(false);
  }
}
