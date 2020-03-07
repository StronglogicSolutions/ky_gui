#ifndef KFILEDIALOG_H
#define KFILEDIALOG_H

#include <QFileDialog>

class KFileDialog : QFileDialog {
Q_OBJECT
public:
    KFileDialog();
    QString openFileDialog();
};

#endif // KFILEDIALOG_H
