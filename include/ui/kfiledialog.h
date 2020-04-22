#ifndef KFILEDIALOG_H
#define KFILEDIALOG_H

#include <QFileDialog>

class KFileDialog : QFileDialog {
Q_OBJECT
public:
    KFileDialog();
    QString openFileDialog(QString file_path = "");
};

#endif // KFILEDIALOG_H
