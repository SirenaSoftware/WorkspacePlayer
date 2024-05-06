#include "Flow.h"
#include <QHBoxLayout>

Flow::Flow(QWidget *parent, QStringList list) : QScrollArea{parent} {
    QHBoxLayout*flowContainer = new QHBoxLayout;
    flowContainer->setContentsMargins(32,32,32,32);
    flowContainer->setSpacing(32);
    flowContainer->setAlignment(Qt::AlignCenter);

    setWidget(new QWidget(this));
    widget()->setLayout(flowContainer);
    setWidgetResizable(true);

    classes = list;
}

void Flow::addWidget(QWidget*wdg){
    widget()->layout()->addWidget(wdg);
}

void Flow::addBlock(QString cname,QString script){
    Q_UNUSED(script);

    BlockViewer*wdg = new BlockViewer(this);
    wdg->setMaximumSize(640,480);
    wdg->setMinimumSize(640,480);
    wdg->show();

    widget()->layout()->addWidget(wdg);

    classes.append(cname);
}

void Flow::removeLastBlock(){
    QLayout* layout = widget()->layout();
    QLayoutItem* lastItem = layout->takeAt(layout->count()-1);
    if (lastItem) {
        lastItem->widget()->deleteLater();
        delete lastItem;
    }
}

QListWidget*Flow::addImportModuleDialog(){
    Dialog*wdg = new Dialog(this);
    wdg->setMaximumSize(640,480);
    wdg->setMinimumSize(640,480);
    wdg->show();
    wdg->setTitle("Adicionar um bloco");
    wdg->hideMainAction();

    QListWidget*list = new QListWidget(wdg);

    wdg->container()->layout()->addWidget(list);

    widget()->layout()->addWidget(wdg);

    return list;
}

QString Flow::getClasses(){
    return classes.join("\n");
}
