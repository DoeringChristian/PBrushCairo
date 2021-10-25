#include "brush_pix.h"

struct brush_pix *brush_pix_init(struct brush_pix *dst){
    dst->interface.ops = &brush_pix_ops;

    dst->interface.lua = luaL_newstate();
    luaL_openlibs(dst->interface.lua);
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
