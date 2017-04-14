#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include "engine.h"

#define N 140      //number of cells n*n
#define PAUSE 60   //refresh rate (miliseconds)
#define CELL 4	   //size of cell (pixel)
#define BORDER 1  //size of cellborder (pixel)
#define COLOR 0   //if 1 then cells are coloured

#define WINDOW_SIZE (N*CELL+N*BORDER) //pixels needed by all cells with borders

#define GTK_WIDGET_STATE(wid) (GTK_WIDGET (wid)->state)

GdkGC* gdk_gc_new (GdkDrawable *);
void gdk_gc_set_foreground(GdkGC *, const GdkColor*);
void gdk_draw_drawable(GdkDrawable *, GdkGC*, GdkDrawable *, gint, gint, gint, gint, gint, gint);
void gdk_draw_rectangle(GdkDrawable *, GdkGC*, gboolean, gint, gint, gint, gint);
guint gtk_timeout_add(guint32, void*, gpointer);

static GdkPixmap *pixmap = NULL;
static msize matrix[N][N];
static msize oldmatrix[N][N];/*--------21.09.2008----------*/
static int _start;
static GtkWidget *area;
static GdkRectangle update_rect;

static GdkGC *mystyle;
static GdkGC *borderstyle;
static GdkGC *yellow;
static GdkGC *green;
static GdkGC *blue;
static GdkGC *red;
static GdkGC *grey;
static GdkGC *violet;
static GdkGC *orange;
static GdkGC *pink;
static GdkGC *black;


static void setColor(int i)
{
    /*----------19.10.2008------*/
    if(i == 0) mystyle = green;    
    if(i == 1) mystyle = blue;
    if(i == 2) mystyle = red;
    if(i == 3) mystyle = red;
    if(i == 4) mystyle = blue;    
    if(i == 5) mystyle = pink; 
    if(i == 6) mystyle = violet; 

}
static void configureColors(GtkWidget *widget)
{
    GdkColor colour;
    char *colour_yellow = "#ffff00";
    char *colour_green = "#00ff00";
    char *colour_blue = "#0000ff";
    char *colour_red = "#ff0000";
    char *colour_grey = "#9f9f9f";
    char *colour_violet = "#AA00FF";
    char *colour_orange = "#FF8000";
    char *colour_pink = "#FF0077";
    char *colour_black = "#000000";

    memset(&colour, 0, sizeof(colour));
    gdk_color_parse(colour_black, &colour);
    black = gdk_gc_new(widget->window);
    gdk_colormap_alloc_color(gtk_widget_get_colormap(widget), &colour, FALSE,FALSE);
    gdk_gc_set_foreground(black, &colour);


    memset(&colour, 0, sizeof(colour));
    gdk_color_parse(colour_blue, &colour);
    blue = gdk_gc_new(widget->window);
    gdk_colormap_alloc_color(gtk_widget_get_colormap(widget), &colour, FALSE,FALSE);
    gdk_gc_set_foreground(blue, &colour);

    memset(&colour, 0, sizeof(colour));
    gdk_color_parse(colour_red, &colour);
    red = gdk_gc_new(widget->window);
    gdk_colormap_alloc_color(gtk_widget_get_colormap(widget), &colour, FALSE,FALSE);
    gdk_gc_set_foreground(red, &colour);

    memset(&colour, 0, sizeof(colour));
    gdk_color_parse(colour_green, &colour);
    green = gdk_gc_new(widget->window);
    gdk_colormap_alloc_color(gtk_widget_get_colormap(widget), &colour, FALSE,FALSE);
    gdk_gc_set_foreground(green, &colour);

    memset(&colour, 0, sizeof(colour));
    gdk_color_parse(colour_yellow, &colour);
    yellow = gdk_gc_new(widget->window);
    gdk_colormap_alloc_color(gtk_widget_get_colormap(widget), &colour, FALSE,FALSE);
    gdk_gc_set_foreground(yellow, &colour);

    memset(&colour, 0, sizeof(colour));
    gdk_color_parse(colour_grey, &colour);
    grey = gdk_gc_new(widget->window);
    gdk_colormap_alloc_color(gtk_widget_get_colormap(widget), &colour, FALSE,FALSE);
    gdk_gc_set_foreground(grey, &colour);

    memset(&colour, 0, sizeof(colour));
    gdk_color_parse(colour_violet, &colour);
    violet = gdk_gc_new(widget->window);
    gdk_colormap_alloc_color(gtk_widget_get_colormap(widget), &colour, FALSE,FALSE);
    gdk_gc_set_foreground(violet, &colour);

    memset(&colour, 0, sizeof(colour));
    gdk_color_parse(colour_orange, &colour);
    orange = gdk_gc_new(widget->window);
    gdk_colormap_alloc_color(gtk_widget_get_colormap(widget), &colour, FALSE,FALSE);
    gdk_gc_set_foreground(orange, &colour);

    memset(&colour, 0, sizeof(colour));
    gdk_color_parse(colour_pink, &colour);
    pink = gdk_gc_new(widget->window);
    gdk_colormap_alloc_color(gtk_widget_get_colormap(widget), &colour, FALSE,FALSE);
    gdk_gc_set_foreground(pink, &colour);
    
    //Set default color 
    mystyle = blue;
}

/* Create a new backing pixmap of the appropriate size */
static gint configure_event( GtkWidget *widget, GdkEventConfigure *event )
{

    configureColors( widget );
    borderstyle = grey;

    if (pixmap)
        g_object_unref(pixmap);

    pixmap = gdk_pixmap_new(widget->window, WINDOW_SIZE, WINDOW_SIZE, -1);
    gdk_draw_rectangle (pixmap,
                        //widget->style->black_gc, 
                        borderstyle,
                        TRUE, 0, 0,
                        WINDOW_SIZE, WINDOW_SIZE
                       );

    for(int row = 0; row<N; row++){
        for(int col = 0; col<N; col++){

            update_rect.x = row * CELL + row*BORDER;
            update_rect.y = col * CELL + col*BORDER;
            update_rect.width = CELL;
            update_rect.height = CELL;

            gdk_draw_rectangle (pixmap,
                                widget->style->white_gc,
                                TRUE,
                                update_rect.x, update_rect.y,
                                update_rect.width, update_rect.height);
        }
    }


    return TRUE;
}

static gint expose_event( GtkWidget *widget, GdkEventExpose *event )
{
    gdk_draw_drawable(widget->window,
                      widget->style->fg_gc[GTK_WIDGET_STATE (widget)],

                      pixmap,
                      event->area.x, event->area.y,
                      event->area.x, event->area.y,
                      event->area.width, event->area.height
                     );
    return TRUE;
}

static int draw( GtkWidget *widget )
{
    for(int row = 0; row<N; row++)
    {
        for(int col = 0; col<N; col++)
        {
            /*--------21.09.2008----------*/			
            if( oldmatrix[row][col] == matrix[row][col])
                continue;
            /*--------21.09.2008----------*/

            update_rect.x = row * CELL + row*BORDER;
            update_rect.y = col * CELL + col*BORDER;
            update_rect.width = CELL;
            update_rect.height = CELL;

            if(matrix[row][col]==1)
            {
                if(COLOR) setColor(neigh(N, matrix, row, col));
                gdk_draw_rectangle (pixmap,
                                    //widget->style->black_gc,
                                    mystyle,
                                    TRUE,
                                    update_rect.x, update_rect.y,
                                    update_rect.width, update_rect.height);
            }else
            {
                gdk_draw_rectangle (pixmap,
                                    widget->style->white_gc,
                                    TRUE,
                                    update_rect.x, update_rect.y,
                                    update_rect.width, update_rect.height);
            }
        }
    }
    gtk_widget_queue_draw_area (widget, 0, 0, WINDOW_SIZE, WINDOW_SIZE);

    return 1;
}

static int run (GtkWidget *widget )
{
    if(_start==0)
        return 1;

    copyMatrix(N, matrix, oldmatrix);
    engine(N, matrix);
    draw(widget);

    return 1;
}

static void byebye() 
{
    exit(0);
}

static void randomize(GtkWidget *widget, gpointer *data)
{
    copyMatrix(N, matrix, oldmatrix);/*--------21.09.2008----------*/
    random1(N, matrix);
    draw(area);
}

static void clearIt(GtkWidget *widget, gpointer *data)
{
    copyMatrix(N, matrix, oldmatrix);/*--------21.09.2008----------*/
    clear(N, matrix); 
    draw(area);
}

void presetIt(GtkWidget *widget, gpointer *data) 
{
    copyMatrix(N, matrix, oldmatrix);/*--------21.09.2008----------*/
    preset(N, matrix);
    draw(area);
}

static void step(GtkWidget *widget, gpointer *data) 
{
    if(_start == 0)
    {
        _start=1;
        run(area);
        _start = 0;
    }
}

static void start(GtkWidget *widget, GtkLabel *data) 
{
    if(_start == 0)
    {
        _start=1;
        gtk_label_set_text(data, "Stop");
    }else
    {
        _start=0;
        gtk_label_set_text(data, "Start");
    }
}

static gint button_press_event( GtkWidget *widget, GdkEventButton *event )
{
    int x, y;
    x = event->x;
    y = event->y;

    if (event->button == 1 && pixmap != NULL)
    {
        x = ((x - x%(CELL+BORDER))/(CELL+BORDER));
        y = ((y - y%(CELL+BORDER))/(CELL+BORDER));
        copyMatrix(N, matrix, oldmatrix);/*--------21.09.2008----------*/ 
        matrix[x][y] = 1;
        draw(area);
    }
    return TRUE;
}

static gint motion_notify_event( GtkWidget *widget, GdkEventMotion *event )
{
    int x, y;
    GdkModifierType state;

    if (event->is_hint)
        gdk_window_get_pointer (event->window, &x, &y, &state);
    else
    {
        x = event->x;
        y = event->y;
        state = event->state;
    }

    if( x >= WINDOW_SIZE )
        x = WINDOW_SIZE-1;

    if( x <= 0 )
        x = 0;

    if( y >= WINDOW_SIZE )
        y = WINDOW_SIZE-1;

    if( y <= 0 )
        y = 0;

    g_print("(%d; %d)\n", x, y);
    if (state & GDK_BUTTON1_MASK && pixmap != NULL )
    { 
        x = ((x - x%(CELL+BORDER))/(CELL+BORDER));
        y = ((y - y%(CELL+BORDER))/(CELL+BORDER));
        copyMatrix(N, matrix, oldmatrix);/*--------21.09.2008----------*/
        matrix[x][y] = 1;
        draw(area);
    }
    return TRUE;
}

int main(int argc,char *argv[])
{

    gtk_init (&argc, &argv);

    GtkWidget *label;
    label = gtk_label_new( "Start" );
    gtk_widget_show(label);
    //Window:
    GtkWidget *window;
    window = gtk_window_new( GTK_WINDOW_TOPLEVEL );
    gtk_window_set_title( GTK_WINDOW(window), "Life" );
    gtk_window_set_default_size( GTK_WINDOW(window), WINDOW_SIZE + 110, WINDOW_SIZE);
    gtk_container_set_border_width (GTK_CONTAINER (window), 10);
    //end Window

    //hBox
    GtkWidget *hbox;
    hbox = gtk_hbox_new (FALSE, 0);
    gtk_container_add (GTK_CONTAINER (window), hbox);
    gtk_widget_show (hbox);
    //end Box

    //Drawing area
    area = gtk_drawing_area_new();
    gtk_widget_set_size_request (GTK_WIDGET (area), WINDOW_SIZE, WINDOW_SIZE);
    gtk_box_pack_start (GTK_BOX (hbox), area, TRUE, TRUE, 0);
    gtk_widget_show (area);
    //end

    //vBox
    GtkWidget *vbox;
    vbox = gtk_vbox_new (FALSE, 0);
    gtk_box_pack_start (GTK_BOX (hbox), vbox, TRUE, TRUE, 0);
    gtk_widget_show (vbox);
    //end Box

    //Buttons

    GtkWidget *button0;
    button0 = gtk_button_new();
    gtk_container_add(GTK_CONTAINER(button0), label);
    gtk_box_pack_start (GTK_BOX (vbox), button0, FALSE, FALSE, 0);
    gtk_widget_show(button0);

    GtkWidget *button1;
    button1 = gtk_button_new_with_label("Clear");
    gtk_box_pack_start (GTK_BOX (vbox), button1, FALSE, FALSE, 0);
    gtk_widget_show(button1);

    GtkWidget *button2;
    button2 = gtk_button_new_with_label("Random");
    gtk_box_pack_start (GTK_BOX (vbox), button2, FALSE, FALSE, 0);
    gtk_widget_show(button2);

    GtkWidget *button3;
    button3 = gtk_button_new_with_label("Step");
    gtk_box_pack_start (GTK_BOX (vbox), button3, FALSE, FALSE, 0);
    gtk_widget_show(button3);

    GtkWidget *button5;
    button5 = gtk_button_new_with_label("Preset");
    gtk_box_pack_start (GTK_BOX (vbox), button5, FALSE, FALSE, 0);
    gtk_widget_show(button5);

    GtkWidget *button4;
    button4 = gtk_button_new_with_label("Quit");
    gtk_box_pack_start (GTK_BOX (vbox), button4, FALSE, FALSE, 10);
    gtk_widget_show(button4);

    //end Buttons

    //Signals
    g_signal_connect (GTK_OBJECT (area),"configure_event", G_CALLBACK (configure_event), NULL);
    g_signal_connect (GTK_OBJECT (area), "expose_event", G_CALLBACK (expose_event), NULL);
    g_signal_connect (GTK_OBJECT (window), "destroy", G_CALLBACK (byebye), NULL);
    g_signal_connect (GTK_OBJECT (button2), "clicked", G_CALLBACK (randomize), NULL);
    g_signal_connect (GTK_OBJECT (button1), "clicked", G_CALLBACK (clearIt), NULL);
    g_signal_connect (GTK_OBJECT (button4), "clicked", G_CALLBACK (byebye), NULL);
    g_signal_connect (GTK_OBJECT (button0), "clicked", G_CALLBACK (start),GTK_LABEL(label));
    g_signal_connect (GTK_OBJECT (button3), "clicked", G_CALLBACK (step),GTK_LABEL(label));
    g_signal_connect (GTK_OBJECT (button5), "clicked", G_CALLBACK (presetIt),GTK_LABEL(label));
    g_signal_connect (GTK_OBJECT (area), "motion_notify_event", G_CALLBACK (motion_notify_event), NULL);
    g_signal_connect (GTK_OBJECT (area), "button_press_event", G_CALLBACK (button_press_event), NULL);

    gtk_widget_set_events (area, GDK_EXPOSURE_MASK
                           | GDK_LEAVE_NOTIFY_MASK
                           | GDK_BUTTON_PRESS_MASK
                           | GDK_POINTER_MOTION_MASK
                           | GDK_POINTER_MOTION_HINT_MASK);

    //Set run() function
    gtk_timeout_add(PAUSE, run, area);
    //end Signals

    gtk_widget_show (window);
    gtk_main();

    return 1;
}
