#include "Label.h"

Label::Label(QWidget *parent) : QLabel{parent} {}

QString Label::text(){
    return QLabel::text();
}

void Label::setText(QString text){
    QLabel::setText(text);
}

int Label::heading() {
    QFont f(font());

    if (!f.bold()) return 0;

    if (f.pointSize() == 24) return 1;
    if (f.pointSize() == 18) return 2;
    if (f.pointSize() == 14) return 3;
    if (f.pointSize() == 12) return 4;
    if (f.pointSize() == 10) return 5;
    if (f.pointSize() ==  8) return 6;

    return 0;
}

void Label::setHeading(int heading) {
    QFont f(font());
    f.setBold(true);

    if (heading == 1) {f.setPointSize(24);}
    if (heading == 2) {f.setPointSize(18);}
    if (heading == 3) {f.setPointSize(14);}
    if (heading == 4) {f.setPointSize(12);}
    if (heading == 5) {f.setPointSize(10);}
    if (heading == 6) {f.setPointSize(8);}

    setFont(f);
}
