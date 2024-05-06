#ifndef LABEL_H
#define LABEL_H
#include <QWidget>
#include <QObject>
#include <QLabel>

class Label : public QLabel
{
    Q_OBJECT
    Q_PROPERTY(int heading__ READ heading WRITE setHeading)
    Q_PROPERTY(QString text__ READ text WRITE setText)
public:
    explicit Label(QWidget *parent = nullptr);
    int heading();
    void setHeading(int heading);

    QString text();
    void setText(QString text);

signals:

};

#endif // LABEL_H
