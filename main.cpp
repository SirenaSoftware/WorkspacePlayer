#include "WorkspacePlayer.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    WorkspacePlayer w;
    w.showFullScreen();
    return a.exec();
}
