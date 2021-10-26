#include "viewport.h"

struct viewport *viewport_init(struct viewport *dst, double x, double y, double scale, double angle){
    dst->da = gtk_drawing_area_new();
    dst->canvas = NULL;
    dst->scale = scale;
    dst->x = x;
    dst->y = y;
    dst->angle = angle;

    input_ctx_init(&dst->in_ctx, dst);

    g_signal_connect(G_OBJECT(dst->da), "draw", G_CALLBACK(viewport_draw), dst);
    
    gtk_widget_show_all(dst->da);
    return dst;
}
void viewport_free(struct viewport *dst){
    gtk_widget_destroy(dst->da);
}

void viewport_draw(GtkWidget *area, cairo_t *cr, gpointer data){
    struct viewport *dst = data;


    GdkRGBA color;
    GtkStyleContext *context;
    int width, height;
    GtkWidget *widget = GTK_WIDGET(area);
    context = gtk_widget_get_style_context (widget);
    width = gtk_widget_get_allocated_width (widget);
    height = gtk_widget_get_allocated_height (widget);
    gtk_render_background(context, cr, 0, 0, width, height);

    if(dst->canvas == NULL)
        return;

    cairo_translate(cr, dst->x, dst->y);
    cairo_scale(cr, dst->scale, dst->scale);
    cairo_rotate(cr, dst->angle);

    canvas_draw(dst->canvas, cr);
}

int viewport_set_canvas(struct viewport *dst, struct canvas *src){
    if(dst->canvas == NULL){
        dst->canvas = src;
        darray_push_back(&dst->canvas->viewports, dst);
        return 1;
    }
    return 0;
}
