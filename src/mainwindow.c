#include "mainwindow.h"

struct mainwindow *mainwindow_init(struct mainwindow *dst){
    dst->win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    viewport_init(&dst->vp, 100, 100, 0.1, 1);

    gtk_container_add(GTK_CONTAINER(dst->win), dst->vp.da);

    g_signal_connect(G_OBJECT(dst->win), "destroy", G_CALLBACK(mainwindow_destroy_cb), dst);

    gtk_widget_show_all(dst->win);
    return dst;
}
void mainwindow_free(struct mainwindow *dst){
    gtk_main_quit();
}

void mainwindow_destroy_cb(GtkWidget *widget, gpointer data){
    mainwindow_free(data);
}

