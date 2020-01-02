#ifndef ARGDIALOG_H
#define ARGDIALOG_H

#include <QDialog>
#include <QFileDialog>
#include <QPushButton>
#include <QFile>
#include <string_view>
#include <QMessageBox>

namespace Args {
    const QString DESCRIPTION_TYPE = "description";
    const QString HASHTAG_TYPE = "hashtag";
}

typedef struct KFile {
    std::string_view name;
    std::string_view path;
} KFile;

typedef struct Task{
    QString time;
    int mask;
    std::vector<std::string> args;
} Task;

typedef struct IGPost {
    std::string description;
    std::string datetime;
    std::string promote_share;
    std::string link_in_bio;
    std::vector<std::string> hashtags;
    std::vector<std::string> requested_by;
    const char* requested_by_phrase = "The phrase was requested by ";
    KFile video;
} IGPost;

namespace Ui {
class ArgDialog;
}

class ArgDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ArgDialog(QWidget *parent = nullptr);
    ~ArgDialog();

signals:
    void uploadFile(QByteArray bytes);

private:
    void clearPost();
    void clearTask();
    void addHashtag(QString tag);
    Ui::ArgDialog *ui;
    void addItem(QString value, QString type);
    Task m_task;
    IGPost m_ig_post;
};

#endif // ARGDIALOG_H
