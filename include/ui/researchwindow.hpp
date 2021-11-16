#ifndef RESEARCHWINDOW_H
#define RESEARCHWINDOW_H

#include <QMainWindow>

namespace Ui {
class ResearchWindow;
}

class ResearchWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit ResearchWindow(QWidget *parent = nullptr);
  ~ResearchWindow();

  void ReceiveData(const QVector<QString>& argv);

protected:
  virtual void keyPressEvent(QKeyEvent* e) override;
private:
  Ui::ResearchWindow* ui;
};

#endif // RESEARCHWINDOW_H
