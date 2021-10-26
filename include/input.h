#pragma once
#ifndef INPUT_H
#define INPUT_H

#include "mathc.h"
#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <sys/time.h>
#include "input_s.h"
#include "viewport.h"

#if 0
struct input_data{
    struct vec2 pos; 
    struct vec2 tilt;
    struct vec2 tan;
    struct vec2 norm;
    struct vec2 scroll;
    double press; 
    double wheel;
    double dist;
    double rot;
    double slide;
    double dt;
    struct timeval time;
};

struct input_ctx{
    struct input_data data;
    struct viewport *vp;
};
#endif

struct input_data *input_data_init(struct input_data *dst);
int input_data_update(struct input_data *dst, GdkEvent *event);

struct input_ctx *input_ctx_init(struct input_ctx *dst, struct viewport *vp);
gboolean input_ctx_event_cb(GtkWidget *widget, GdkEvent *event, gpointer data);


#endif //INPUT_H
