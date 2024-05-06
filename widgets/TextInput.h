#ifndef TEXTINPUT_H
#define TEXTINPUT_H

#include <QWidget>

namespace Ui {class TextInput;}

class TextInput : public QWidget {
    Q_OBJECT
    Q_PROPERTY(QString className__ READ className WRITE setClassName)
    Q_PROPERTY(QString text__ READ text WRITE setText)

public:
    explicit TextInput(QWidget *parent = nullptr);
    ~TextInput();

    QString className();
    void setClassName(QString className);

    QString text();
    void setText(QString text);

private:
    Ui::TextInput *ui;
    void paintEvent(QPaintEvent *event);
    QString m_className = "text";
};

#endif // TEXTINPUT_H
