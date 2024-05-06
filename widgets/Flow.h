#ifndef FLOW_H
#define FLOW_H

#include <QScrollArea>
#include <QListWidget>
#include "widgets/Dialog.h"
#include "widgets/BlockViewer.h"

class Flow : public QScrollArea {
    Q_OBJECT
public:
    explicit Flow(QWidget *parent = nullptr,QStringList list = QStringList());
    void addWidget(QWidget*wdg);
    void addBlock(QString cname,QString script);
    void removeLastBlock();
    QListWidget*addImportModuleDialog();
    QString getClasses();

signals:

private:
    QStringList classes;
};

#endif // FLOW_H
