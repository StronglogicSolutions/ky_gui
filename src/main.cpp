#include <include/mainwindow.h>
#include <QApplication>

int main(int argc, char **argv)
{
    QApplication a(argc, argv);
    QFont font("Deja Sans Mono");
    font.setStyleHint(QFont::Monospace);
    a.setFont(font);
    MainWindow w(argc, argv);
    w.show();

    return a.exec();
}
