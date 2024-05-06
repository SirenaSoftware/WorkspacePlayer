#ifndef BLOCKVIEWER_H
#define BLOCKVIEWER_H

#include <QWidget>
#include <lua.hpp>

class BlockViewer : public QWidget {
    Q_OBJECT

public:
    explicit BlockViewer(QWidget *parent = nullptr);
    ~BlockViewer();
    void initialize(QString script);

private:
    void paintEvent(QPaintEvent *event);

    lua_State*L;
};

#endif // BLOCKVIEWER_H
