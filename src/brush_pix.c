#include "brush_pix.h"

struct brush_pix *brush_pix_init(struct brush_pix *dst){
    dst->interface.ops = &brush_pix_ops;

    dst->interface.lua = luaL_newstate();
    luaL_openlibs(dst->interface.lua);

    return dst;
}

struct brush_pix *brush_pix_loadfolder(struct brush_pix *dst, const char *path){
    dst->interface.ops = &brush_pix_ops;
    dst->interface.lua = luaL_newstate();
    luaL_openlibs(dst->interface.lua);

    size_t path_size = strlen(path);

    char *path_lua = malloc(path_size + strlen(brush_pix_fnames[BRUSH_PIX_FNAME_LUA]) + 1);
    char *path_img = malloc(path_size + strlen(brush_pix_fnames[BRUSH_PIX_FNAME_IMG]) + 1);

    memcpy(path_lua, path, path_size+1);
    memcpy(path_img, path, path_size+1);

    strcat(path_lua, brush_pix_fnames[BRUSH_PIX_FNAME_LUA]);
    strcat(path_img, brush_pix_fnames[BRUSH_PIX_FNAME_IMG]);

    if(luaL_loadfile(dst->interface.lua, path_lua) != LUA_OK){
        free(path_lua);
        free(path_img);
        lua_close(dst->interface.lua);
        return NULL;
    }
    //lua_pcall(dst->interface.lua, 0, LUA_MULTRET, 0);

    dst->surface = cairo_image_surface_create_from_png(path_img);

    free(path_lua);
    free(path_img);
    return dst;
}

void brush_pix_free(struct brush *dst){
    struct brush_pix *pb = (struct brush_pix *)dst;
    lua_close(dst->lua);
    cairo_surface_destroy(pb->surface);
}

int brush_pix_draw(struct brush *src, struct layer *dst){
    struct brush_pix *pb = (struct brush_pix *)src;
    // calculate

    lua_pushnumber(src->lua, src->state.pos.x);
    lua_setglobal(src->lua, "src_pos_x");
    lua_pushnumber(src->lua, src->state.pos.y);
    lua_setglobal(src->lua, "src_pos_y");
    lua_pushnumber(src->lua, src->state.pressure);
    lua_setglobal(src->lua, "src_pressure");
    lua_pushnumber(src->lua, src->state.dt);
    lua_setglobal(src->lua, "src_dt");
    lua_pushnumber(src->lua, src->state.speed.x);
    lua_setglobal(src->lua, "src_speed_x");
    lua_pushnumber(src->lua, src->state.speed.y);
    lua_setglobal(src->lua, "src_speed_y");
    lua_pushnumber(src->lua, src->state.tilt.x);
    lua_setglobal(src->lua, "src_tilt_x");
    lua_pushnumber(src->lua, src->state.tilt.y);
    lua_setglobal(src->lua, "src_tilt_y");

    //lua_getglobal(src->lua, "f");

    if(lua_pcall(src->lua, 0, 0, 0) != LUA_OK){
        // err
        g_print("ERR lua: %s\n", lua_tostring(src->lua, -1));
        return 0;
    }

    struct brush_draw_param param;

    lua_getglobal(src->lua, "dst_pos_x");
    param.pos.x = lua_tonumber(src->lua, -1);
    lua_getglobal(src->lua, "dst_pos_y");
    param.pos.y = lua_tonumber(src->lua, -1);
    lua_getglobal(src->lua, "dst_angle");
    param.angle = lua_tonumber(src->lua, -1);
    lua_getglobal(src->lua, "dst_scale_x");
    param.scale.x = lua_tonumber(src->lua, -1);
    lua_getglobal(src->lua, "dst_scale_y");
    param.scale.y = lua_tonumber(src->lua, -1);
    lua_getglobal(src->lua, "dst_opacity");
    param.opacity = lua_tonumber(src->lua, -1);

#if 0
    struct brush_draw_param param = {
        .pos = svec2(10, 10),
        .scale = svec2(1.0, 1.0),
        .angle = 0,
        .opacity = 1,
    };
#endif
    
    // draw

    g_print("%lf\n", param.pos.x);
    g_print("%lf\n", param.opacity);

    cairo_t *ctx = cairo_create(dst->surface);

    cairo_set_operator(ctx, CAIRO_OPERATOR_OVER);

    cairo_scale(ctx, param.scale.x, param.scale.y);
    cairo_rotate(ctx, param.angle);
    cairo_translate(ctx, param.pos.x, param.pos.y);

    cairo_set_source_surface(ctx, pb->surface, param.pos.y, param.pos.y);

    cairo_paint_with_alpha(ctx, param.opacity);

    cairo_destroy(ctx);
    return 1;
}
