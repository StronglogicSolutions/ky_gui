#include <include/ui/mainwindow.h>
#include <QApplication>

int main(int argc, char **argv) {
    QApplication a(argc, argv);
    QFont font("Deja Sans Mono");
    font.setStyleHint(QFont::Monospace);
    a.setFont(font);

    int code = 0;

    do {
      MainWindow w(argc, argv);
      w.show();

      code = a.exec();
    } while (code == 9);

    return code;
}
