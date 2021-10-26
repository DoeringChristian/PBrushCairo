#include "canvas.h"

struct canvas *canvas_init(struct canvas *dst, int w, int h){
    dst->brush_cur = NULL;
    darray_init(&dst->layers, 16);
    darray_init(&dst->viewports, 16);
    return dst;
}
void canvas_free(struct canvas *dst){
    darray_free(&dst->layers);
    darray_free(&dst->viewports);
}

int canvas_draw(struct canvas *dst, cairo_t *ctx){
    for(size_t i = 0;i < darray_len(&dst->layers);i++){
        layer_paint_to_ctx(dst->layers[i], ctx);
    }

    return 1;
}
int canvas_push_layer(struct canvas *dst, struct layer *src, size_t idx){
    src->canvas = dst;
    return darray_push(&dst->layers, src, idx);
}
int canvas_set_brush(struct canvas *dst, struct brush *src){
    dst->brush_cur = src;
    return 1;
}
int canvas_set_cur_layer(struct canvas *dst, struct layer *src){
    dst->layer_cur = src;
    return 1;
}
int canvas_update_viewports(struct canvas *dst){
    for(size_t i = 0;i < darray_len(&dst->viewports);i++){
        gtk_widget_queue_draw(dst->viewports[i]->da);
    }
}
