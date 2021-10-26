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

    //if(luaL_loadfile(dst->interface.lua, path_lua) != LUA_OK){
    if(luaL_dofile(dst->interface.lua, path_lua) != LUA_OK){
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

int brush_pix_draw(struct brush *src, struct layer *dst, struct input_data *data){
    struct brush_pix *pb = (struct brush_pix *)src;
    // calculate

#if 0
    lua_pushnumber(src->lua, data->pos.x);
    lua_setglobal(src->lua, "src_pos_x");
    lua_pushnumber(src->lua, data->pos.y);
    lua_setglobal(src->lua, "src_pos_y");
    lua_pushnumber(src->lua, data->press);
    lua_setglobal(src->lua, "src_pressure");
    lua_pushnumber(src->lua, data->dt);
    lua_setglobal(src->lua, "src_dt");
    lua_pushnumber(src->lua, data->norm.x);
    lua_setglobal(src->lua, "src_norm_x");
    lua_pushnumber(src->lua, data->norm.y);
    lua_setglobal(src->lua, "src_norm_y");
    lua_pushnumber(src->lua, data->tan.x);
    lua_setglobal(src->lua, "src_tan_x");
    lua_pushnumber(src->lua, data->tan.y);
    lua_setglobal(src->lua, "src_tan_y");
    lua_pushnumber(src->lua, data->tilt.x);
    lua_setglobal(src->lua, "src_tilt_x");
    lua_pushnumber(src->lua, data->tilt.y);
    lua_setglobal(src->lua, "src_tilt_y");
#endif

    lua_getglobal(src->lua, "f");

    lua_pushnumber(src->lua, data->pos.x);
    lua_pushnumber(src->lua, data->pos.y);
    lua_pushnumber(src->lua, data->press);
    lua_pushnumber(src->lua, data->dt);
    lua_pushnumber(src->lua, data->norm.x);
    lua_pushnumber(src->lua, data->norm.y);
    lua_pushnumber(src->lua, data->tan.x);
    lua_pushnumber(src->lua, data->tan.y);
    lua_pushnumber(src->lua, data->tilt.x);
    lua_pushnumber(src->lua, data->tilt.y);

    //g_print("%s\n", lua_tostring(src->lua, -1));

    if(lua_pcall(src->lua, 10, 6, 0) != LUA_OK){
        // err
        g_print("ERR lua: %s\n", lua_tostring(src->lua, -1));
        return 0;
    }

    struct brush_draw_param param;

    param.opacity = lua_tonumber(src->lua, -1);
    lua_pop(src->lua, 1);
    param.scale.y = lua_tonumber(src->lua, -1);
    lua_pop(src->lua, 1);
    param.scale.x = lua_tonumber(src->lua, -1);
    lua_pop(src->lua, 1);
    param.angle = lua_tonumber(src->lua, -1);
    lua_pop(src->lua, 1);
    param.pos.y = lua_tonumber(src->lua, -1);
    lua_pop(src->lua, 1);
    param.pos.x = lua_tonumber(src->lua, -1);
    lua_pop(src->lua, 1);

    lua_pop(src->lua, lua_gettop(src->lua));

    
    // draw

    g_print("opacity: %lf\n", param.opacity);
    g_print("pos: %lf, %lf\n", param.pos.x, param.pos.y);
    g_print("scale: %lf, %lf\n", param.scale.x, param.scale.y);
    param.pos = data->pos;

    cairo_t *ctx = cairo_create(dst->surface);

    cairo_set_operator(ctx, CAIRO_OPERATOR_OVER);

    cairo_scale(ctx, param.scale.x / dst->scale, param.scale.y / dst->scale);
    cairo_rotate(ctx, param.angle);
    cairo_translate(ctx, param.pos.x - dst->x, param.pos.y - dst->x);

    cairo_set_source_surface(ctx, pb->surface, param.pos.y, param.pos.y);

    cairo_paint_with_alpha(ctx, param.opacity);

    canvas_update_viewports(dst->canvas);

    cairo_destroy(ctx);
    return 1;
}
