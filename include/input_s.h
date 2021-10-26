#ifndef INPUT_S_H
#define INPUT_S_H
#include "mathc.h"
#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <sys/time.h>

struct viewport;

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

#endif //INPUT_S_H
