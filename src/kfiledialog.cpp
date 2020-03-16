#include <include/kfiledialog.h>

KFileDialog::KFileDialog()
{
    this->setStyleSheet("QFileDialog QWidget { background-color: rgb(0, 43, 54); color: rgb(131, 148, 150); font-weight: 700;}");
    this->setFileMode(QFileDialog::ExistingFiles);
}

QString KFileDialog::openFileDialog(QString file_path) {
  if (!file_path.isEmpty()) {
    this->setDirectory(file_path);
  }
  this->exec();
  QStringList filenames = this->selectedFiles();
  if (!filenames.empty()) {
    return filenames.at(0);
  }
  return "";
}
