#ifndef SAVEBUTTON_HPP
#define SAVEBUTTON_HPP

#include <QPushButton>
#include <QStyle>

class SaveButton : public QPushButton
{
  Q_OBJECT
public:
  explicit SaveButton(QWidget *parent = nullptr);
  void     OnRequest(bool complete = false);

protected:
  void paintEvent(QPaintEvent *event) final;


private:
  bool m_pending{false};

};

#endif // SAVEBUTTON_HPP
