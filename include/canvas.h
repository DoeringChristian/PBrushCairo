#pragma once
#ifndef CANVAS_H
#define CANVAS_H

#include <gtk/gtk.h>
#include "darray.h"
#include "layer.h"

struct canvas{
    darray(struct layer *) layers;
};

struct canvas *canvas_init(struct canvas *dst, int w, int h);
void canvas_free(struct canvas *dst);

int canvas_draw(struct canvas *dst, cairo_t *ctx);
int canvas_push_layer(struct canvas *dst, struct layer *src, size_t idx);


#endif //CANVAS_H
