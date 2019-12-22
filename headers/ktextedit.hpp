#include <QPlainTextEdit>
#include <QDebug>
#include <QKeyEvent>
#include <QtUiPlugin/QDesignerCustomWidgetInterface>


class KTextEdit : public QPlainTextEdit, public QDesignerCustomWidgetInterface {
    Q_PLUGIN_METADATA(IID "kstyleyo.kygui.ktextedit" FILE "ktextedit.json")
    Q_INTERFACES(QDesignerCustomWidgetInterface)


public:

    QString toPlainText() {
        return QPlainTextEdit::toPlainText();
    }

    void clear() {
        QPlainTextEdit::clear();
    }

protected:
    virtual void keyPressEvent(QKeyEvent* e) {
        qDebug() << "Key press: " << e->key();
        if (e->key()==Qt::Key_Enter) {
            qDebug() << "Ouch!";
        }
        QPlainTextEdit::keyPressEvent(e);
    }

};
