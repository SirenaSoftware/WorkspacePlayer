#ifndef LUABINDER_H
#define LUABINDER_H

#include <lua.hpp>
#include <QWidget>

struct wrapper{
  QWidget*wdg;
  wrapper(QWidget*w){
      wdg = w;
  };
};

// Containers
int l_HBox(lua_State*L);
int l_VBox(lua_State*L);

// Widgets
int l_Label(lua_State*L);
int l_TextInput(lua_State*L);

int l_ui_create_widget(lua_State*L);

#endif // LUABINDER_H
