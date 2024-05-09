#include "BlockViewer.h"
#include <QVBoxLayout>
#include <QStyleOption>
#include <QPainter>
#include <QLineEdit>
#include "LuaBinder.h"

BlockViewer::BlockViewer(QWidget *parent) : QWidget(parent){
    setLayout(new QVBoxLayout);
    layout()->setContentsMargins(18,18,18,18);
    layout()->setAlignment(Qt::AlignTop);
}

BlockViewer::~BlockViewer(){
    lua_close(L);
}

void BlockViewer::initialize(QString script){
    L = luaL_newstate();
    luaL_openlibs(L);

    const int ui = 1;
    const int meta = 2;

    lua_newtable(L);
    lua_newtable(L);

    lua_pushlightuserdata(L,this);
    lua_pushcclosure(L,l_ui_create_widget,1);
    lua_setfield (L, meta,"__call");

    lua_setmetatable(L,ui);
    lua_setglobal(L,"ui");

    lua_pushcclosure(L,l_TextInput,0);
    lua_setglobal(L,"TextInput");

    lua_pushcclosure(L,l_HBox,0);
    lua_setglobal(L,"HBox");

    lua_pushcclosure(L,l_VBox,0);
    lua_setglobal(L,"VBox");


    luaL_dofile(L,script.toUtf8());
    luaL_dostring(L,"start()");
}

void BlockViewer::paintEvent(QPaintEvent *event){
    Q_UNUSED(event)

    QStyleOption option;
    option.initFrom(this);

    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &option, &painter, this);
}
