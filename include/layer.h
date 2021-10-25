#pragma once
#ifndef LAYER_H
#define LAYER_H

#include <gtk/gtk.h>

struct layer{
    struct canvas *canvas;

    cairo_surface_t *surface;
    cairo_operator_t op;

    //uint8_t *img;

    int x, y, w, h;
    double scale;
};

struct layer *layer_init(struct layer *dst, cairo_operator_t op, int x, int y, int w, int h, double scale);
struct layer *layer_load_png(struct layer *dst, const char *path, cairo_operator_t op, int x, int y, double scale);
void layer_free(struct layer *dst);

int layer_paint_to_ctx(struct layer *src, cairo_t *ctx);

#endif //LAYER_H
