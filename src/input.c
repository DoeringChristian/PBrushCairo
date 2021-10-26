#include "input.h"

struct input_data *input_data_init(struct input_data *dst){
    *dst = (struct input_data){0};

    return dst;
}

int input_data_update(struct input_data *dst, GdkEvent *event){

    struct vec2 pos_tmp = dst->pos;
    struct vec2 tilt_tmp = dst->tilt;
    struct timeval time_tmp = dst->time;

    double pos_x, pos_y, tilt_x, tilt_y;

    gdk_event_get_axis(event, GDK_AXIS_X, &pos_x);
    gdk_event_get_axis(event, GDK_AXIS_Y, &pos_y);

    gdk_event_get_axis(event, GDK_AXIS_XTILT, &tilt_x);
    gdk_event_get_axis(event, GDK_AXIS_YTILT, &tilt_y);

    gdk_event_get_axis(event, GDK_AXIS_PRESSURE, &dst->press);

    gettimeofday(&dst->time, NULL);

    dst->pos.x = pos_x;
    dst->pos.y = pos_y;
    dst->tilt.x = tilt_x;
    dst->tilt.y = tilt_y;

    dst->tan = svec2_subtract(dst->pos, pos_tmp);
    dst->norm = svec2_rotate(dst->tan, G_PI/4);
    dst->dt = ((double)dst->time.tv_sec - (double)time_tmp.tv_sec) + ((double)dst->time.tv_usec - (double)time_tmp.tv_usec) / 1000000;

#if 1
    printf("pos:  %lf, %lf\n", dst->pos.x,  dst->pos.y);
    printf("tan:  %lf, %lf\n", dst->tan.x,  dst->tan.y);
    printf("tilt: %lf, %lf\n", dst->tilt.x, dst->tilt.y);
#endif
    return 0;
}
struct input_ctx *input_ctx_init(struct input_ctx *dst, struct viewport *vp){
    input_data_init(&dst->data);
    dst->vp = vp;

    gtk_widget_set_support_multidevice(dst->vp->da, TRUE);
    int mask =
        // Key handling
        GDK_KEY_PRESS_MASK |

        // Allow scrolling
        GDK_SCROLL_MASK |

        // Touch / Pen / Mouse
        GDK_TOUCH_MASK | GDK_POINTER_MOTION_MASK | GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK |
        GDK_SMOOTH_SCROLL_MASK | GDK_ENTER_NOTIFY_MASK | GDK_LEAVE_NOTIFY_MASK | GDK_PROXIMITY_IN_MASK |
        GDK_PROXIMITY_OUT_MASK;

    gtk_widget_add_events(dst->vp->da, mask);

    g_signal_connect(G_OBJECT(dst->vp->da), "event", G_CALLBACK(input_ctx_event_cb), dst);

    return dst;
}
gboolean input_ctx_event_cb(GtkWidget *widget, GdkEvent *event, gpointer data){
    struct input_ctx *dst = data;

    GdkDevice *dev = gdk_event_get_source_device(event);
    GdkInputSource src = gdk_device_get_source(dev);
    

    if(src != GDK_SOURCE_KEYBOARD && gdk_device_get_device_type(dev) != GDK_DEVICE_TYPE_MASTER){
        input_data_update(&dst->data, event);
        if(dst->vp->canvas != NULL && dst->vp->canvas->brush_cur != NULL && dst->vp->canvas->layer_cur != NULL)
            brush_draw(dst->vp->canvas->brush_cur, dst->vp->canvas->layer_cur, &dst->data);
    }
    return FALSE;
}
