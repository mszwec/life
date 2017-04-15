#ifndef GUI_H
#define GUI_H

#include <gtk/gtk.h>
#include <gdk/gdk.h>

#define N 140      //number of cells n*n
#define PAUSE 60   //refresh rate (miliseconds)
#define CELL 4	   //size of cell (pixel)
#define BORDER 1  //size of cellborder (pixel)
#define COLOR 0   //if 1 then cells are coloured

#define WINDOW_SIZE (N*CELL+N*BORDER) //pixels needed by all cells with borders

#define GTK_WIDGET_STATE(wid) (GTK_WIDGET (wid)->state)


GdkPixmap *pixmap;
GtkWidget *area;
GdkRectangle update_rect;
GdkGC *mystyle;

GdkGC* gdk_gc_new (GdkDrawable *);
void gdk_gc_set_foreground(GdkGC *, const GdkColor*);
void gdk_draw_drawable(GdkDrawable *, GdkGC*, GdkDrawable *, gint, gint, gint, gint, gint, gint);
void gdk_draw_rectangle(GdkDrawable *, GdkGC*, gboolean, gint, gint, gint, gint);
guint gtk_timeout_add(guint32, void*, gpointer);

void setColor(int);
void initialize_gui(int argc, char *argv[]);
void set_run_fun(int run ( GtkWidget *widget ));
int draw (GtkWidget *widget);

#endif
