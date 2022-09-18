#ifndef CONNECTIONINDICATOR_H
#define CONNECTIONINDICATOR_H

#include <QLabel>

class ConnectionIndicator : public QLabel
{
  Q_OBJECT
public:
  explicit ConnectionIndicator(QWidget *parent = nullptr);

  enum State{
      StateConnected,
      StateOkBlue,
      StateWarning,
      StateDisconnected
  };

  void setState(bool state);
  void setState(State state);

private:
    State m_state;
};

#endif // CONNECTIONINDICATOR_H
