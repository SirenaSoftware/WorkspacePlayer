#ifndef BUTTON_H
#define BUTTON_H

#include <QPushButton>

class Button : public QPushButton {
    Q_OBJECT
public:
    explicit Button(QWidget *parent = nullptr);

signals:

};

#endif // BUTTON_H
