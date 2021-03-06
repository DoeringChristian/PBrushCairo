#pragma once
#ifndef CANVAS_H
#define CANVAS_H

#include <gtk/gtk.h>
#include "darray.h"
#include "layer.h"
#include "brush.h"

struct canvas{
    darray(struct layer *) layers;
    darray(struct viewport *)viewports;

    struct brush *brush_cur;
    struct layer *layer_cur;
};

struct canvas *canvas_init(struct canvas *dst, int w, int h);
void canvas_free(struct canvas *dst);

int canvas_draw(struct canvas *dst, cairo_t *ctx);
int canvas_push_layer(struct canvas *dst, struct layer *src, size_t idx);
int canvas_set_cur_layer(struct canvas *dst, struct layer *src);
int canvas_set_brush(struct canvas *dst, struct brush *src);
int canvas_update_viewports(struct canvas *dst);


#endif //CANVAS_H
