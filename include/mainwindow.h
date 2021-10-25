#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <gtk/gtk.h>
#include "viewport.h"

struct mainwindow{
    GtkWidget *win;
    struct viewport vp;
};

struct mainwindow *mainwindow_init(struct mainwindow *dst);
void mainwindow_free(struct mainwindow *dst);

void mainwindow_destroy_cb(GtkWidget *widget, gpointer data);
void mainwindow_draw_cb(GtkWidget *are, cairo_t *cr, int width, int height, gpointer data);

#endif //MAINWINDOW_H
