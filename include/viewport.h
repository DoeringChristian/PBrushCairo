#pragma once
#ifndef VIEWPORT_H
#define VIEWPORT_H

#include <gtk/gtk.h>
#include "canvas.h"
#include "input.h"
#include "input_s.h"

struct viewport{
    struct input_ctx in_ctx;
    struct canvas *canvas;
    GtkWidget *da;

    double scale;
    double x, y;
    double angle;
};

struct viewport *viewport_init(struct viewport *dst, double x, double y, double scale, double angle);
void viewport_free(struct viewport *dst);

void viewport_draw(GtkWidget *area, cairo_t *cr, gpointer data);

int viewport_set_canvas(struct viewport *dst, struct canvas *src);

#endif //VIEWPORT_H
