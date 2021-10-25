#pragma once
#ifndef BRUSH_H
#define BRUSH_H

#include <gtk/gtk.h>
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include "mathc.h"

struct brush_state{
    double dt;
    double pressure;
    struct vec2 pos;
    struct vec2 speed;
    struct vec2 tilt;
    int rand;
};

struct brush_ops;

struct brush{
    struct brush_ops *ops;

    struct brush_state state;

    lua_State *lua;
};

struct brush_ops{
    void (*free)(struct brush *dst);
};

static inline void brush_free(struct brush *dst){
    if(dst->ops != NULL && dst->ops->free != NULL)
        dst->ops->free(dst);
}


#endif //BRUSH_H
