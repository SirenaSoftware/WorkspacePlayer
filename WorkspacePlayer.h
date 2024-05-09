#ifndef WORKSPACEPLAYER_H
#define WORKSPACEPLAYER_H

#include <QMainWindow>
#include <QPushButton>
#include <QListWidget>
#include <QScrollArea>
#include <QListWidgetItem>
#include <QMap>
#include <QLayoutItem>
#include "widgets/Dialog.h"
#include "widgets/BlockViewer.h"
#include "widgets/Flow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class WorkspacePlayer; }
QT_END_NAMESPACE

class WorkspacePlayer : public QMainWindow {
    Q_OBJECT

public:
    WorkspacePlayer(QWidget *parent = nullptr);
    ~WorkspacePlayer();

private slots:
    void on_shutdown_clicked();
    void on_addFlow_clicked();
    void on_flowList_itemPressed(QListWidgetItem *item);
    void on_toggleListbox_clicked(bool checked);

    void closeDialog();
    void openDialog();
    void createEmptyFlow();
    void createFlowByTemplate(QListWidgetItem *item);
    void importModule(QListWidgetItem *item);

    void openImportModuleDialog();
    void addNewBlockClick();

private:
    Ui::WorkspacePlayer *ui;
    Dialog*m_widget = nullptr;

    void swapFlow(Flow*new_flow);
    void updateClock();
    void scrollFlow();

    void addNewBlockButton();

    QPushButton*outside = nullptr;
    Flow*active_flow = nullptr;

    void copyDirectory(QString source,QString target);

    QMap<QString,Flow*> flowsMap;
    QListWidget*newBlockList;
};
#endif // WORKSPACEPLAYER_H
