#pragma once
#ifndef BRUSH_PIX_H
#define BRUSH_PIX_H

#include "brush.h"

enum brush_pix_fname{
    BRUSH_PIX_FNAME_LUA = 0,
    BRUSH_PIX_FNAME_IMG = 1,
};

struct brush_pix{
    struct brush interface;

    cairo_surface_t *surface;

    const char *folder_path;
};

struct brush_pix *brush_pix_init(struct brush_pix *dst);
struct brush_pix *brush_pix_loadfolder(struct brush_pix *dst, const char *path);

void brush_pix_free(struct brush *dst);
int brush_pix_draw(struct brush *src, struct layer *dst, struct input_ctx *ctx);

static struct brush_ops brush_pix_ops = {
    .free = brush_pix_free,
    .draw = brush_pix_draw,
};

static const char **brush_pix_fnames = (const char *[]){
    "brush.lua",
    "brush.png",
};

#endif //BRUSH_PIX_H
