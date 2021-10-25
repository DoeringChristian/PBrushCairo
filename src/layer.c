#include "layer.h"

struct layer *layer_init(struct layer *dst, cairo_operator_t op, int x, int y, int w, int h, double scale){
    dst->x = x;
    dst->y = y;
    dst->w = w;
    dst->h = h;
    dst->op = op;
    dst->scale = scale;

    dst->surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, w, h);

    cairo_surface_flush(dst->surface);

    for(size_t i = 0;i < w;i++){
        for(size_t j = 0;j < h;j++){
            uint8_t *pix_p = &cairo_image_surface_get_data(dst->surface)[j * w * 4 + i * 4];
            pix_p[0] = 255;
            pix_p[1] = 255;
            pix_p[2] = 0;
            pix_p[3] = 255;
        }
    }

    cairo_surface_mark_dirty(dst->surface);

    return dst;
}

struct layer *layer_load_png(struct layer *dst, const char *path, cairo_operator_t op, int x, int y, double scale){
    dst->surface = cairo_image_surface_create_from_png(path);

    dst->x = x;
    dst->y = y;
    dst->w = cairo_image_surface_get_width(dst->surface);
    dst->h = cairo_image_surface_get_height(dst->surface);
    dst->op = op;
    dst->scale = scale;

    return dst;
}

void layer_free(struct layer *dst){
    cairo_surface_destroy(dst->surface);
}

int layer_paint_to_ctx(struct layer *src, cairo_t *ctx){
    cairo_set_operator(ctx, src->op);
    cairo_set_source_surface(ctx, src->surface, src->x, src->y);
    cairo_scale(ctx, src->scale, src->scale);
    cairo_paint(ctx);
    return 1;
}
