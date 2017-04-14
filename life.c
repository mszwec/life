#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include "engine.h"

#define n 140      //liczba komorek n*n
#define N 140      //liczba komorek n*n

#define pause 75   //predkosc odswierzania (ms)
#define cell 4	   //rozmiar rysowanej komorki (pixel)
#define border 1  //rozmiar siatki (pixel)

#define GTK_WIDGET_STATE(wid) (GTK_WIDGET (wid)->state)

GdkGC* gdk_gc_new (GdkDrawable *);
void gdk_gc_set_foreground(GdkGC *, const GdkColor*);
void gdk_draw_drawable(GdkDrawable *, GdkGC*, GdkDrawable *, gint, gint, gint, gint, gint, gint);
void gdk_draw_rectangle(GdkDrawable *, GdkGC*, gboolean, gint, gint, gint, gint);
guint gtk_timeout_add(guint32, void*, gpointer);

static GdkPixmap *pixmap = NULL;
static int matrix[n][n];
static int gradient[n][n];
static int oldgradient[n][n];
static int oldmatrix[n][n];/*--------21.09.2008----------*/
static int check = 1;
static int _start = 0;
static GtkWidget *area;
static GdkRectangle update_rect;
static int row;
static int col;

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


static void setColor(int i){
 /*----------19.10.2008------*/
         if(i == 0) mystyle = green;    
         if(i == 1) mystyle = blue;
         if(i == 2) mystyle = yellow;
         if(i == 3) mystyle = red;
	 if(i == 4) mystyle = orange;    
	 if(i == 5) mystyle = pink; 
	 if(i == 6) mystyle = violet; 

	 mystyle = blue;
}
static void configureColors(GtkWidget *widget){
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
}

/* Create a new backing pixmap of the appropriate size */
static gint configure_event( GtkWidget *widget, GdkEventConfigure *event ){

 	configureColors( widget );
     borderstyle = grey;

	if (pixmap)
		g_object_unref(pixmap);
		pixmap = gdk_pixmap_new(widget->window, n*cell + n*border, n*cell + n*border, -1);
		gdk_draw_rectangle (pixmap,
					//widget->style->black_gc, 
                    borderstyle,
                    TRUE, 0, 0,
					n*cell + n*border, n*cell + n*border
					);

	for(row = 0; row<n; row++){
		for(col = 0; col<n; col++){

			update_rect.x = row * cell + row*border;
      			update_rect.y = col * cell + col*border;
      			update_rect.width = cell;
      			update_rect.height = cell;

			  gdk_draw_rectangle (pixmap,
                          widget->style->white_gc,
                          TRUE,
                          update_rect.x, update_rect.y,
                          update_rect.width, update_rect.height);
    		}
	}


		return TRUE;
	}

static gint expose_event( GtkWidget *widget, GdkEventExpose *event ){
	gdk_draw_drawable(widget->window,
			//widget->style->fg_gc[(int)widget],
			widget->style->fg_gc[GTK_WIDGET_STATE (widget)],
		
			pixmap,
			event->area.x, event->area.y,
			event->area.x, event->area.y,
			event->area.width, event->area.height
			);
      return TRUE;
    }

static int draw( GtkWidget *widget ){

	if(check==1){
		check=0;
	}
	if(_start==0){
		return 1;
	}

	copyMatrix(N, matrix, oldmatrix);
	engine(N, matrix);

	for(row = 0; row<(n); row++){
		for(col = 0; col<n; col++){

/*--------21.09.2008----------*/			
			if( oldmatrix[row][col] == matrix[row][col] ){//&& oldgradient[row][col] == gradient[row][col]){
				continue;
			}
/*--------21.09.2008----------*/

			update_rect.x = row * cell + row*border;
      			update_rect.y = col * cell + col*border;
      			update_rect.width = cell;
      			update_rect.height = cell;

         
           
			if(matrix[row][col]==1){

            setColor(gradient[row][col]);
			gdk_draw_rectangle (pixmap,
                          //widget->style->black_gc,
                          mystyle,
                          TRUE,
                          update_rect.x, update_rect.y,
                          update_rect.width, update_rect.height);
			}else{
			  gdk_draw_rectangle (pixmap,
                          widget->style->white_gc,
                          TRUE,
                          update_rect.x, update_rect.y,
                          update_rect.width, update_rect.height);
			}
 		}
	}
	
    gtk_widget_queue_draw_area (widget, 0, 0, n*cell + n*border, n*cell + n*border);
	return 1;
}


static int draw2( GtkWidget *widget ){

	for(row = 0; row<n; row++){
		for(col = 0; col<n; col++){
/*--------21.09.2008----------*/			
			if( oldmatrix[row][col] == matrix[row][col]){// && oldgradient[row][col] == gradient[row][col]){
				continue;
			}
/*--------21.09.2008----------*/
			update_rect.x = row * cell + row*border;
      			update_rect.y = col * cell + col*border;
      			update_rect.width = cell;
      			update_rect.height = cell;

			if(matrix[row][col]==1){
			setColor(gradient[row][col]);
			gdk_draw_rectangle (pixmap,
                          //widget->style->black_gc,
                          mystyle,
                          TRUE,
                          update_rect.x, update_rect.y,
                          update_rect.width, update_rect.height);
			}else{
			  gdk_draw_rectangle (pixmap,
                          widget->style->white_gc,
                          TRUE,
                          update_rect.x, update_rect.y,
                          update_rect.width, update_rect.height);
			}
        }gtk_widget_queue_draw_area (widget, 0, 0,
		      n*cell + n*border, n*cell + n*border);
	}
    
	return 1;
}



static void byebye() {
	exit(0);
}

static void randomize(GtkWidget *widget, gpointer *data) {
	copyMatrix(N, matrix, oldmatrix);/*--------21.09.2008----------*/
	random1(n, matrix);
	if(_start == 0){
		_start=1;
		draw2(area);
		_start = 0;
	}else draw2(area);/*--------21.09.2008----------*/
}

static void clearIt(GtkWidget *widget, gpointer *data) {
	copyMatrix(N, matrix, oldmatrix);/*--------21.09.2008----------*/
	clear(n, matrix); 
	if(_start == 0){
		_start=1;
		draw2(area);
		_start = 0;
	}else draw2(area);/*--------21.09.2008----------*/
}

void presetIt(GtkWidget *widget, gpointer *data) {
	copyMatrix(N, matrix, oldmatrix);/*--------21.09.2008----------*/
	preset( n, matrix);
	draw2(area);/*--------21.09.2008----------*/
}

static void step(GtkWidget *widget, gpointer *data) {
	if(_start == 0){
		_start=1;
		draw(area);
		_start = 0;
	}
}

static void start(GtkWidget *widget, GtkLabel *data) {
	if(_start == 0){
		_start=1;
		gtk_label_set_text(data, "Stop");
	}else{
		_start=0;
		gtk_label_set_text(data, "Start");
	}
}

static gint button_press_event( GtkWidget *widget, GdkEventButton *event ){
	int x, y;
	x = event->x;
	y = event->y;

	if (event->button == 1 && pixmap != NULL){
		x = ((x - x%(cell+border))/(cell+border));
		y = ((y - y%(cell+border))/(cell+border));
		copyMatrix(N, matrix, oldmatrix);/*--------21.09.2008----------*/ 
		copyMatrix(N, gradient, oldgradient);
		matrix[x][y] = 1;
		draw2(area);
	}
	return TRUE;
}

static gint motion_notify_event( GtkWidget *widget, GdkEventMotion *event ){
	int x, y;
	GdkModifierType state;
	
	if (event->is_hint)
		gdk_window_get_pointer (event->window, &x, &y, &state);
	else{
		x = event->x;
		y = event->y;
		state = event->state;
	}
	
	if( x >= n*cell+n*border ){
		x = n*cell + n*border-1;
	}
	if( x <= 0 ){
		x = 0;
	}
	if( y >= n*cell+n*border ){
		y = n*cell + n*border-1;
	}
	if( y <= 0 ){
		y = 0;
	}        

g_print("(%d; %d)\n", x, y);
	if (state & GDK_BUTTON1_MASK && pixmap != NULL ){ //x < n*cell+n*border && y < n*cell+n*border && x >= 0 && y >= 0
		x = ((x - x%(cell+border))/(cell+border));
		y = ((y - y%(cell+border))/(cell+border));
		copyMatrix(N, matrix, oldmatrix);/*--------21.09.2008----------*/
		copyMatrix(N, gradient, oldgradient);
		matrix[x][y] = 1;
		draw2(area);
	}
	return TRUE;
}

int main(int argc,char *argv[]){

	gtk_init (&argc, &argv);

	GtkWidget *label;
	label = gtk_label_new( "Start" );
	gtk_widget_show(label);
//Window:
	GtkWidget *window;
	window = gtk_window_new( GTK_WINDOW_TOPLEVEL );
	gtk_window_set_title( GTK_WINDOW(window), "Life" );
	gtk_window_set_default_size( GTK_WINDOW(window), n*cell+n*border+110, n*cell+n*border);
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
	//gtk_drawing_area_size(GTK_DRAWING_AREA(area), n*cell+n*border, n*cell+n*border);
	gtk_widget_set_size_request (GTK_WIDGET (area), n*cell+n*border, n*cell+n*border);
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

/*  g_signal_connect_swapped (G_OBJECT (button), "clicked",
			    G_CALLBACK (gtk_widget_destroy),
			    G_OBJECT (window));
*/
      gtk_widget_set_events (area, GDK_EXPOSURE_MASK
                             | GDK_LEAVE_NOTIFY_MASK
                             | GDK_BUTTON_PRESS_MASK
                             | GDK_POINTER_MOTION_MASK
                             | GDK_POINTER_MOTION_HINT_MASK);

gtk_timeout_add(pause, draw, area);
//end Signals

	gtk_widget_show (window);
	gtk_main();
return 1;
}
