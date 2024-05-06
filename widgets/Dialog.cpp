#include "Dialog.h"
#include "ui_Dialog.h"
#include <QStyleOption>
#include <QPainter>
#include <QProcess>

Dialog::Dialog(QWidget *parent) : QWidget(parent), ui(new Ui::Dialog){
    ui->setupUi(this);
}

Dialog::~Dialog() {
    delete ui;
}

void Dialog::paintEvent(QPaintEvent *event){
    Q_UNUSED(event)

    QStyleOption option;
    option.initFrom(this);

    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &option, &painter, this);
}

void Dialog::setTitle(QString title){
    ui->title->setText(title);
}

void Dialog::setMainActionText(QString mainActionText){
    ui->mainAction->setText(mainActionText);
}

QWidget*Dialog::container(){
    return ui->widget;
}

void Dialog::on_mainAction_clicked()
{
    emit mainActionclicked();
}

void Dialog::hideMainAction(){
    ui->mainAction->hide();
}
