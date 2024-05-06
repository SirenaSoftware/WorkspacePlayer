#ifndef DIALOG_H
#define DIALOG_H

#include <QWidget>

namespace Ui {class Dialog;}

class Dialog : public QWidget {
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();

    void setTitle(QString title);
    void setMainActionText(QString mainActionText);
    void hideMainAction();
    QWidget*container();

signals:
    void mainActionclicked();

private slots:
    void on_mainAction_clicked();

private:
    Ui::Dialog *ui;
    void paintEvent(QPaintEvent *event);
};

#endif // DIALOG_H
