#include <cairo/cairo.h>
#include <X11/Xlib.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <gtk/gtk.h>
#include "mainwindow.h"
#include "brush_pix.h"

int main(int argc, char **argv){

    gtk_init(&argc, &argv);

    struct mainwindow mainwindow;
    mainwindow_init(&mainwindow);

    struct canvas c;
    canvas_init(&c, 100, 100);

    viewport_set_canvas(&mainwindow.vp, &c);

    struct layer l1, l2;
    //layer_init(&l1, CAIRO_OPERATOR_OVER, 0, 0, 100, 100);
    //layer_init(&l2, CAIRO_OPERATOR_OVER, 50, 50, 100, 100);
    layer_load_png(&l1, "res/img/test01.png", CAIRO_OPERATOR_OVER, 0, 0, 1.0);
    layer_load_png(&l2, "res/img/test02.png", CAIRO_OPERATOR_OVER, 10, 10, 1.0);

    canvas_push_layer(&c, &l1, 0);
    canvas_push_layer(&c, &l2, 1);

    struct brush_pix pb;
    brush_pix_loadfolder(&pb, "res/brushes/b1/");

    canvas_set_cur_layer(&c, &l2);
    canvas_set_brush(&c, &pb.interface);

    gtk_main();
    return 0;
}

