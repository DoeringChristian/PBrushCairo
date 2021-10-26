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

int brush_pix_draw(struct brush *src, struct layer *dst, struct input_ctx *ctx){
    struct brush_pix *pb = (struct brush_pix *)src;
    // calculate

    lua_getglobal(src->lua, "f");

    lua_pushnumber(src->lua, ctx->data.pos.x);
    lua_pushnumber(src->lua, ctx->data.pos.y);
    lua_pushnumber(src->lua, ctx->data.press);
    lua_pushnumber(src->lua, ctx->data.dt);
    lua_pushnumber(src->lua, ctx->data.norm.x);
    lua_pushnumber(src->lua, ctx->data.norm.y);
    lua_pushnumber(src->lua, ctx->data.tan.x);
    lua_pushnumber(src->lua, ctx->data.tan.y);
    lua_pushnumber(src->lua, ctx->data.tilt.x);
    lua_pushnumber(src->lua, ctx->data.tilt.y);

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
#if 0
    g_print("opacity: %lf\n", param.opacity);
    g_print("pos: %lf, %lf\n", param.pos.x, param.pos.y);
    g_print("scale: %lf, %lf\n", param.scale.x, param.scale.y);
#endif

    cairo_t *cr = cairo_create(dst->surface);

    cairo_set_operator(cr, CAIRO_OPERATOR_OVER);

    cairo_scale(cr, param.scale.x / dst->scale / ctx->vp->scale, param.scale.y / dst->scale / ctx->vp->scale);
    cairo_rotate(cr, param.angle - ctx->vp->angle);
    
    struct vec2 pos = param.pos;
    pos = svec2_subtract(pos, svec2(dst->x, dst->y));
    pos = svec2_subtract(pos, svec2(ctx->vp->x, ctx->vp->y));
    pos = svec2_subtract(pos, svec2((double)cairo_image_surface_get_width(pb->surface)/2,
                (double)cairo_image_surface_get_height(pb->surface)/2));

    cairo_set_source_surface(cr, pb->surface, pos.x, pos.y);

    cairo_paint_with_alpha(cr, param.opacity);

    canvas_update_viewports(dst->canvas);

    cairo_destroy(cr);
    return 1;
}
