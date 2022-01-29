#include <include/ui/mainwindow.h>
#include <QApplication>

int main(int argc, char **argv)
{
  QApplication a(argc, argv);
  QFont font("Deja Sans Mono");
  font.setStyleHint(QFont::Monospace);
  a.setFont(font);

  int8_t code = 0;

  do
  {
    MainWindow w(argc, argv);
    w.show();
    code = a.exec();
  }
  while (code == CLIENT_EXIT);

  return code;
}
