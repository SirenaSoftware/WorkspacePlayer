#include "LuaBinder.h"
#include <QVariant>
#include <QLayout>
#include "widgets/TextInput.h"
#include "widgets/Label.h"
#include "widgets/Button.h"

int l__index(lua_State *L) {
    const int key = 2;
    const int widget = 3;

    lua_pushvalue(L, lua_upvalueindex(1));
    lua_insert(L, widget);

    QWidget *wdg = (QWidget*)lua_touserdata(L, widget);;

    QVariant value = wdg->property((QString(lua_tostring(L,key))+"__").toLocal8Bit());
    int type = value.metaType().id();

    if (value.metaType().id() == QMetaType::Int) {
        lua_pushinteger(L,value.toInt());
        return 1;
    }

    if (type == QMetaType::Double || type == QMetaType::Float) {
        lua_pushnumber(L,value.toDouble());
        return 1;
    }

    if (type == QMetaType::QString) {
        lua_pushstring(L,value.toString().toUtf8());
        return 1;
    }

    if (type == QMetaType::Bool) {
        lua_pushboolean(L,value.toBool()?1:0);
        return 1;
    }

    lua_pushnil(L);
    return 1;
}

int l__newindex(lua_State *L) {
    const int key = 2;
    const int widget = 3;
    const int value = 4;

    lua_pushvalue(L, lua_upvalueindex(1));
    lua_insert(L, widget);

    QWidget *wdg = (QWidget*)lua_touserdata(L, widget);

    if (lua_isstring(L,value)) {
        wdg->setProperty((QString(lua_tostring(L,key))+"__").toLocal8Bit(),QVariant(QString(lua_tostring(L,value))));
        return 0;
    }

    lua_pushnil(L);
    return 1;
}

void registerWidget(lua_State*L, QWidget*wdg){
    lua_pushlightuserdata(L,wdg);
    lua_newtable(L);
    lua_rotate(L, -lua_gettop(L), 1);
    lua_pushcclosure(L,l_ui_create_widget,0);
    lua_rotate(L, -lua_gettop(L), 1);

    lua_call(L,3,LUA_MULTRET);
}


int l_ui_create_widget(lua_State *L) {
    //const int self = 1;
    const int childrens = 2; // We don`t need self
    const int parent = 3;

    lua_pushvalue(L, lua_upvalueindex(1));

    QWidget*parentWidget = (QWidget *)lua_touserdata(L, parent);

    // In pratice, does nothing, but is required to avoid CLang warnings
    if (parentWidget == nullptr) {
        lua_pushlightuserdata(L,new QWidget);
        return 1;
    }

    QString id = "";

    // Iterate over the 'childrens' table
    lua_pushnil(L);
    while (lua_next(L, childrens) != 0) {
        const int key = -2;
        const int value = -1;


        if (!lua_isnumber(L, key)) {
            QString property = (QString(lua_tostring(L,key))+"__");

            if (property == "id__") {
                id = QString(lua_tostring(L,value));
                parentWidget->setObjectName(id);
            } else if (property == "margin") {
                if (parentWidget->layout()) {
                    int margin = lua_tointeger(L,value);
                    parentWidget->layout()->setContentsMargins(margin,margin,margin,margin);
                }
            } else {
                switch (lua_type(L,value)) {
                    case LUA_TBOOLEAN:
                      parentWidget->setProperty(property.toLocal8Bit(),lua_toboolean(L,value)==1);
                      break;
                    case LUA_TSTRING:
                      parentWidget->setProperty(property.toLocal8Bit(),QString(lua_tostring(L,value)));
                      break;
                    case LUA_TNUMBER:
                      parentWidget->setProperty(property.toLocal8Bit(),lua_isinteger(L,value)==1?lua_tointeger(L,value):lua_tonumber(L,value));
                      break;
                    default:
                      // TODO: Invalid type, handle it
                    ;

                }
            }
        } else if (lua_islightuserdata(L,value)) {
            QWidget*children = (QWidget *)lua_touserdata(L, value);
            if (children&&children!=parentWidget) {
                children->setParent(parentWidget);
                if (parentWidget->layout()) {
                    parentWidget->layout()->addWidget(children);
                }
            } else if (children==parentWidget) {
                // TODO: atempt of ui {ui}
            }
        } else {
            // TODO: Is a malicious developer atempt to crash WorkspacePlayer?
        }

        lua_pop(L, 1);
    }

    if (!id.isEmpty()) {
        const int ui = lua_gettop(L)+1;
        const int component_table = lua_gettop(L)+2;
        const int component_metatable = lua_gettop(L)+3;

        lua_getglobal(L, "ui");
        lua_newtable(L);
        lua_newtable(L);

        lua_pushlightuserdata(L,parentWidget);
        lua_pushcclosure(L,l__index,1);
        lua_setfield (L, component_metatable,"__index");

        lua_pushlightuserdata(L,parentWidget);
        lua_pushcclosure(L,l__newindex,1);
        lua_setfield (L, component_metatable,"__newindex");

        lua_pushstring(L,parentWidget->metaObject()->className());
        lua_setfield (L, component_metatable,"__name");

        lua_setmetatable(L,component_table);

        lua_pushstring(L, id.toUtf8());
        lua_pushvalue(L,component_table);

        lua_rawset(L,ui);

        lua_pop(L, 2);
    }

    lua_pushvalue(L,parent);
    return 1;
}

// Containers

int l_HBox(lua_State*L){
    QWidget*wdg = new QWidget;
    wdg->setLayout(new QHBoxLayout);
    wdg->layout()->setSpacing(9);
    wdg->layout()->setContentsMargins(0,0,0,0);
    wdg->layout()->setAlignment(Qt::AlignLeft);

    registerWidget(L, wdg);
    return lua_gettop(L);
}

int l_VBox(lua_State*L){
    QWidget*wdg = new QWidget;
    wdg->setLayout(new QVBoxLayout);
    wdg->layout()->setSpacing(9);
    wdg->layout()->setContentsMargins(0,0,0,0);
    wdg->layout()->setAlignment(Qt::AlignTop);

    registerWidget(L, wdg);
    return lua_gettop(L);
}

// Widgets

int l_Label(lua_State*L){
    registerWidget(L, new Label);
    return lua_gettop(L);
}

int l_TextInput(lua_State*L){
    registerWidget(L, new TextInput);
    return lua_gettop(L);
}
