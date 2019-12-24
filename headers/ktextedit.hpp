#include <QTextEdit>
#include <QDebug>
#include <QKeyEvent>
#include <QtUiPlugin/QDesignerCustomWidgetInterface>


class KTextEdit : public QTextEdit, public QDesignerCustomWidgetInterface {
    Q_PLUGIN_METADATA(IID "kstyleyo.kygui.ktextedit" FILE "ktextedit.json")
    Q_INTERFACES(QDesignerCustomWidgetInterface)


public:

    QString toPlainText() {
        return QTextEdit::toPlainText();
    }

    void clear() {
        QTextEdit::clear();
    }

protected:
    virtual void keyPressEvent(QKeyEvent* e) {
        qDebug() << "Key press: " << e->key();
        if (e->key()==Qt::Key_Enter) {
            qDebug() << "Ouch!";
        }
        QTextEdit::keyPressEvent(e);
    }

};
