#include "WorkspacePlayer.h"
#include "ui_WorkspacePlayer.h"
#include <future>
#include <QPropertyAnimation>
#include <QGraphicsDropShadowEffect>
#include <QEasingCurve>
#include <QLineEdit>
#include <QListWidget>
#include <QDir>
#include <QUuid>
#include <QTimer>
#include <QDateTime>
#include <QScrollBar>

WorkspacePlayer::WorkspacePlayer(QWidget *parent): QMainWindow(parent), ui(new Ui::WorkspacePlayer){
    ui->setupUi(this);

    QFile layout_file("workspace/flows.yaml");
    if (layout_file.open(QFile::ReadOnly)){
        QTextStream file_data(&layout_file);
        while (!file_data.atEnd()) {
            QString line = file_data.readLine();

            QString uuid = line.left(line.indexOf(":")).trimmed();
            QString title = line.mid(line.indexOf(":")+1).trimmed();

            QListWidgetItem *item = new QListWidgetItem;
            item->setData(42,uuid);

            if (title.left(1) == "\"" && title.right(1) == "\"") {
                title = title.mid(1,title.length()-2).toLocal8Bit();
            }

            item->setText(title);

            ui->flowList->addItem(item);
        }
        layout_file.close();
    }


    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &WorkspacePlayer::updateClock);
    timer->start(1000);
}

WorkspacePlayer::~WorkspacePlayer(){
    delete ui;
}

void WorkspacePlayer::closeDialog(){
    QPropertyAnimation*ani = new QPropertyAnimation;
    ani->setTargetObject(m_widget);
    ani->setDuration(500);
    ani->setEasingCurve(QEasingCurve::OutQuad);
    ani->setPropertyName("geometry");
    ani->setEndValue(QRect(width()/2,height()/2,0,0));
    ani->setStartValue(QRect((width()/2)-320,(height()/2)-240,640,480));
    ani->start(QPropertyAnimation::DeleteWhenStopped);


    delete QObject::sender();
}

void WorkspacePlayer::openDialog(){

    if (m_widget) m_widget->deleteLater();

    outside = new QPushButton(this);
    outside->setStyleSheet("border: 0; background-color: transparent;");
    outside->setGeometry(geometry());
    outside->show();

    connect(outside,SIGNAL(clicked()),this,SLOT(closeDialog()));

    Dialog*dlg = new Dialog(this);
    dlg->show();

    connect(dlg,SIGNAL(mainActionclicked()),this,SLOT(createEmptyFlow()));

    QGraphicsDropShadowEffect*shadow = new QGraphicsDropShadowEffect;
    shadow->setBlurRadius(100);
    shadow->setOffset(QPoint(0,0));

    dlg->setGraphicsEffect(shadow);
    dlg->setFocus();

    QPropertyAnimation*ani = new QPropertyAnimation;
    ani->setTargetObject(dlg);
    ani->setDuration(500);
    ani->setEasingCurve(QEasingCurve::OutQuad);
    ani->setPropertyName("geometry");
    ani->setStartValue(QRect(width()/2,height()/2,0,0));
    ani->setEndValue(QRect((width()/2)-320,(height()/2)-240,640,480));
    ani->start(QPropertyAnimation::DeleteWhenStopped);

    m_widget = dlg;
}

void WorkspacePlayer::on_shutdown_clicked(){
    QFile flows_file("workspace/flows.yaml");
    if (flows_file.open(QFile::WriteOnly|QFile::Truncate)) {
        for(int i = 0; i < ui->flowList->count(); ++i) {
            QListWidgetItem* item = ui->flowList->item(i);
            QString title = item->text();
            QString uuid = item->data(42).toString();
            flows_file.write(QString(uuid+": \""+title+"\"\n").toUtf8());
        }
        flows_file.close();
    }

    for (const QString &uuid : flowsMap.keys()) {
        Flow*flow = flowsMap[uuid];

        QFile flowsClasses("workspace/flows/"+uuid+"/classes");
        if (flowsClasses.open(QFile::WriteOnly|QFile::Truncate)) {
            flowsClasses.write(flow->getClasses().toUtf8());
            flowsClasses.close();
        }
    }
    close();
}

void fillResourceList(QListWidget*list, QString base) {
    QDir dir(base);
    dir.setNameFilters(QStringList());
    dir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);

    QStringList fileList = dir.entryList();

    for (int i=0; i<fileList.count(); i++) {
        if (!list) break;
        QListWidgetItem*item = new QListWidgetItem;
        QFile title(base+"/"+fileList[i]+"/title");
        title.open(QFile::ReadOnly);

        item->setSizeHint(QSize(48,48));
        item->setText(QString(title.readAll()).remove("\n"));
        item->setData(42,fileList[i]);
        list->addItem(item);

        title.close();
    }
}

void WorkspacePlayer::copyDirectory(QString source,QString target){
    QDir("").mkpath(target);
    QStringList blocks = QDir(source).entryList(QDir::NoDotAndDotDot | QDir::Dirs);
    foreach (QString filename, blocks) {
        QFile::copy(filename, target);
    }
}

void WorkspacePlayer::on_addFlow_clicked(){
    openDialog();

    m_widget->setTitle("Adicionar fluxo...");
    m_widget->setMainActionText("Adicionar !");

    QLineEdit*input = new QLineEdit;
    QListWidget*list = new QListWidget;

    m_widget->container()->layout()->addWidget(input);
    m_widget->container()->layout()->addWidget(list);

    Q_UNUSED(std::async(std::launch::async, fillResourceList, list,"flows"));

    connect(list,SIGNAL(itemPressed(QListWidgetItem*)),this,SLOT(createFlowByTemplate(QListWidgetItem*)));

    input->setFocus();
}

void WorkspacePlayer::createEmptyFlow(){
    QString title = m_widget->container()->layout()->itemAt(0)->widget()->property("text").toString();
    QString uuid = QUuid::createUuid().toString();

    QDir flow("");
    flow.mkpath("workspace/flows/"+uuid);

    QFile file("workspace/flows/"+uuid+"/title");
    file.open(QFile::WriteOnly);
    file.write(title.toLocal8Bit());
    file.close();

    QListWidgetItem*flow_item = new QListWidgetItem(QIcon(),title);
    flow_item->setData(42,uuid);

    ui->flowList->addItem(flow_item);

    outside->click();
}

void WorkspacePlayer::createFlowByTemplate(QListWidgetItem *item){
    QString title = item->text();
    QString uuid = QUuid::createUuid().toString();

    copyDirectory("flows/"+item->data(42).toString()+"/","workspace/flows/"+uuid+"/");

    QFile file("workspace/flows/"+uuid+"/title");
    file.open(QFile::WriteOnly);
    file.write(title.toLocal8Bit());
    file.close();

    QListWidgetItem*flow_item = new QListWidgetItem(QIcon(),title);
    flow_item->setData(42,uuid);

    ui->flowList->addItem(flow_item);

    outside->click();
}

void WorkspacePlayer::swapFlow(Flow *new_flow){
    if (active_flow) {
        QPropertyAnimation*ani = new QPropertyAnimation;
        ani->setTargetObject(active_flow);
        ani->setDuration(500);
        ani->setEasingCurve(QEasingCurve::OutQuad);
        ani->setPropertyName("pos");
        ani->setStartValue(active_flow->pos());
        ani->setEndValue(QPoint(48,-height()));
        ani->start(QPropertyAnimation::DeleteWhenStopped);
    }

    QPropertyAnimation*ani = new QPropertyAnimation;
    ani->setTargetObject(new_flow);
    ani->setDuration(500);
    ani->setEasingCurve(QEasingCurve::OutQuad);
    ani->setPropertyName("pos");
    ani->setStartValue(QPoint(48,height()));
    ani->setEndValue(QPoint(48,0));
    ani->start(QPropertyAnimation::DeleteWhenStopped);

    active_flow = new_flow;
}

void WorkspacePlayer::addNewBlockButton(){
    QFont font;
    font.setBold(true);
    font.setPointSize(18);

    QToolButton*btn = new QToolButton;
    btn->setMinimumSize(48,48);
    btn->setText("+");
    btn->setFont(font);
    btn->setCursor(QCursor(Qt::BlankCursor));

    QWidget*wdg = new QWidget(this);
    wdg->setLayout(new QVBoxLayout);
    wdg->layout()->setContentsMargins(0,0,0,0);
    wdg->layout()->addItem(new QSpacerItem(0, 0, QSizePolicy::Fixed, QSizePolicy::Expanding));
    wdg->layout()->addWidget(btn);
    wdg->layout()->addItem(new QSpacerItem(0, 0, QSizePolicy::Fixed, QSizePolicy::Expanding));
    wdg->setMaximumSize(48,48);

    btn->setParent(wdg);

    connect(btn,SIGNAL(clicked()),this,SLOT(addNewBlockClick()));

    active_flow->widget()->layout()->addWidget(wdg);
}

void loadModule(BlockViewer*viewer,QString filename){
    viewer->initialize(filename);
}

void WorkspacePlayer::on_flowList_itemPressed(QListWidgetItem *item){
    ui->toggleListbox->click();

    QString uuid = item->data(42).toString();

    // Avoid flow reload
    if (!flowsMap[uuid]) {
        bool has_module = false;

        QFile classes("workspace/flows/"+uuid+"/classes");
        classes.open(QFile::ReadOnly);
        QStringList classList = QString(classes.readAll()).split("\n");

        Flow*flow = new Flow(ui->home,classList);
        flow->resize(ui->home->size());
        flow->move(48,height());
        flow->setMaximumWidth(ui->home->size().width()-48);
        flow->setProperty("uuid",uuid);
        flow->show();

        QStringList blocks = QDir("workspace/flows/"+uuid+"/").entryList(QDir::NoDotAndDotDot | QDir::Files);
        foreach (QString filename, blocks) {
            if (!filename.endsWith(".iasy")) continue;

            BlockViewer*wdg = new BlockViewer(this);
            wdg->setMaximumSize(640,480);
            wdg->setMinimumSize(640,480);
            wdg->initialize("workspace/flows/"+uuid+"/"+filename);
            wdg->show();

            flow->addWidget(wdg);

            has_module = true;
        }

        flowsMap[uuid] = flow;

        if (active_flow!=flowsMap[uuid]) swapFlow(flowsMap[uuid]);

        if (!has_module) {
            // Let's add welcome block if flow is empty
            openImportModuleDialog();
            return;
        }

        addNewBlockButton();
    }

    if (active_flow!=flowsMap[uuid]) swapFlow(flowsMap[uuid]);
}

void WorkspacePlayer::scrollFlow(){
    // Wait active_flow->horizontalScrollBar() receive next maximum value
    QTime dieTime= QTime::currentTime().addMSecs(5);
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);

    QScrollBar*bar = active_flow->horizontalScrollBar();

    QPropertyAnimation*ani = new QPropertyAnimation;
    ani->setTargetObject(bar);
    ani->setDuration(500);
    ani->setEasingCurve(QEasingCurve::OutQuad);
    ani->setPropertyName("value");
    ani->setStartValue(bar->value());
    ani->setEndValue(bar->maximum());
    ani->start(QPropertyAnimation::DeleteWhenStopped);
}

void WorkspacePlayer::importModule(QListWidgetItem *item) {
    QString block_class = item->data(42).toString();
    QString block_name = QDateTime::currentDateTime().toString("yyyyMMddhhsszzz");
    QString flow = active_flow->property("uuid").toString();

    QDir().mkpath("workspace/flows/"+flow+"/");

    QFile::copy("blocks/"+block_class+"/block.iasy","workspace/flows/"+flow+"/"+block_name+".iasy");
    QFile::copy("blocks/"+block_class+"/block.yaml","workspace/flows/"+flow+"/"+block_name+".yaml");

    active_flow->removeLastBlock();
    active_flow->addBlock("blocks/"+block_class,"workspace/flows/"+flow+"/"+block_name+".iasy");
    addNewBlockButton();

    scrollFlow();
}

void WorkspacePlayer::on_toggleListbox_clicked(bool checked){
    QPropertyAnimation*ani = new QPropertyAnimation;
    ani->setTargetObject(ui->flowListBox);
    ani->setDuration(500);
    ani->setEasingCurve(QEasingCurve::OutQuad);
    ani->setPropertyName("geometry");
    ani->setStartValue(QRect(checked?-226:20,20,274,242));
    ani->setEndValue(QRect(checked?20:-226,20,274,242));
    ani->start(QPropertyAnimation::DeleteWhenStopped);

    ui->toggleListbox->setGeometry(checked?geometry():QRect(0,0,48,height()));

    if (checked) {
        ui->toggleListbox->raise();
        ui->flowListBox->raise();
        ui->flowListBox->setFocus();
        return;
    }

    ui->flowListBox->raise();
    ui->toggleListbox->raise();
}

void WorkspacePlayer::updateClock(){
    ui->clock->setText(QDateTime::currentDateTime().toString("hh:mm"));
}

void WorkspacePlayer::openImportModuleDialog(){
    QListWidget*list = active_flow->addImportModuleDialog();

    Q_UNUSED(std::async(std::launch::async, fillResourceList, list,"blocks"));
    connect(list,SIGNAL(itemPressed(QListWidgetItem*)),this,SLOT(importModule(QListWidgetItem*)));

    scrollFlow();
}

void WorkspacePlayer::addNewBlockClick(){
    active_flow->removeLastBlock();
    openImportModuleDialog();
}

