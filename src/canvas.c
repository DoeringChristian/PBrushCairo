#include "canvas.h"

struct canvas *canvas_init(struct canvas *dst, int w, int h){

    darray_init(&dst->layers, 16);
    return dst;
}
void canvas_free(struct canvas *dst){
    darray_free(&dst->layers);
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
