#include "include/ui/savebutton.hpp"
#include "headers/util.hpp"
#include <QStyleOptionButton>
#include <QPaintEvent>
#include <QPainter>

static const QString save_button_style{"QPushButton{"
  "font: 87 18pt \"Noto Sans\";"
  "color: rgb(0, 0, 0);"
  "background-color: rgb(2, 180, 43);"
  "font-weight: 700;"
  "padding: 4px;"
  "border-style: outset;"
  "border-width: 2px;"
  "border-radius: 6px;"
  "border-color: #00000f;"
  "min-width: 1em;"
  "min-height: 1em;"
  "padding: 4px;"
  "opacity: 0.3;}"
  "QPushButton:hover {"
    "background-color: #328930;"
  "}"};

static const QString request_button_style{"QPushButton{"
  "font: 87 18pt \"Noto Sans\";"
  "color: rgb(0, 0, 0);"
  "background-color: magenta;"
  "font-weight: 700;"
  "padding: 4px;"
  "border-style: outset;"
  "border-width: 2px;"
  "border-radius: 6px;"
  "border-color: #00000f;"
  "min-width: 1em;"
  "min-height: 1em;"
  "padding: 4px;"
  "opacity: 0.3;}"
  "QPushButton:hover {"
    "background-color: red;"
  "}"};


SaveButton::SaveButton(QWidget *parent)
: QPushButton{parent}
{
  setAttribute(Qt::WA_StyledBackground);
  setAutoFillBackground(true);
}


void SaveButton::paintEvent(QPaintEvent* event)
{
  QPainter           painter(this);
  QStyleOptionButton option;

  option.initFrom(this);
  if (m_pending)
  {
    setStyleSheet(request_button_style);
    option.text = "Requested";
  }
  else
  {
    setStyleSheet(save_button_style);
    option.text = "Save";
  }

  style()->drawControl(QStyle::CE_PushButton, &option, &painter, this);
  setPalette(option.palette);
  palette();
}

void SaveButton::OnRequest(bool complete)
{
    m_pending = !complete;
}
