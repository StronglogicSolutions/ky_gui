#ifndef KTEXTEDIT_HPP
#define KTEXTEDIT_HPP

#include <QTextEdit>
#include <QDebug>
#include <QKeyEvent>
#include <QtUiPlugin/QDesignerCustomWidgetInterface>


class KTextEdit : public QTextEdit, public QDesignerCustomWidgetInterface {
    Q_OBJECT
//    Q_PLUGIN_METADATA(IID "kstyleyo.kygui.ktextedit" FILE "ktextedit.json")
    Q_INTERFACES(QDesignerCustomWidgetInterface)


public:

    QString toPlainText() {
        return QTextEdit::toPlainText();
    }

    void clear() {
        QTextEdit::clear();
    }


signals:
    void textInputEnter();

protected:
    virtual void keyPressEvent(QKeyEvent* e) {
        qDebug() << "Key press: " << e->key();

        if (e->key()==Qt::Key_Enter) {
            emit textInputEnter();
        }
        QTextEdit::keyPressEvent(e);
    }

};
#endif // KTEXTEDIT_HPP
