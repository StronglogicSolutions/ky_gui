#include <include/kfiledialog.h>

KFileDialog::KFileDialog()
{
    this->setStyleSheet("QFileDialog QWidget { background-color: rgb(0, 43, 54); color: rgb(131, 148, 150); font-weight: 700;}");
}

QString KFileDialog::openFileDialog() {
    return this->getOpenFileName(this,
                                                 tr("Open File"), "~", tr("All Files (*.*)"), nullptr, QFileDialog::DontUseNativeDialog);
}
