#include "TextInput.h"
#include "ui_TextInput.h"
#include <QStyleOption>
#include <QPainter>

TextInput::TextInput(QWidget *parent) : QWidget(parent),ui(new Ui::TextInput){
    ui->setupUi(this);
}

TextInput::~TextInput(){
    delete ui;
}

QString TextInput::className(){
    return m_className;
}

void TextInput::setClassName(QString className){
    if (className.toLower() == "numeric") {
        m_className = className.toLower();
        ui->pushButton->setText("123");
        return;
    }

    if (className.toLower() == "text") {
        m_className = className.toLower();
        ui->pushButton->setText("aB c");
        return;
    }

    if (className.toLower() == "capital") {
        m_className = className.toLower();
        ui->pushButton->setText("A Bc");
        return;
    }

    if (className.toLower() == "literal") {
        m_className = className.toLower();
        ui->pushButton->setText("0123");
        return;
    }

    if (className.toLower() == "integer") {
        m_className = className.toLower();
        ui->pushButton->setText("123");
        return;
    }

    if (className.toLower() == "decimal") {
        m_className = className.toLower();
        ui->pushButton->setText("12.3");
        return;
    }

    if (className.toLower() == "email") {
        m_className = className.toLower();
        ui->pushButton->setText("@");
        return;
    }
}

QString TextInput::text(){
    return ui->lineEdit->text();
}

void TextInput::setText(QString text){
    ui->lineEdit->setText(text);
}

void TextInput::paintEvent(QPaintEvent *event){
    Q_UNUSED(event)

    QStyleOption option;
    option.initFrom(this);

    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &option, &painter, this);
}
