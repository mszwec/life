#ifndef LIFE_H
#define LIFE_H

#include "gui.h"

typedef unsigned char msize;

msize matrix[N][N];
msize oldmatrix[N][N];/*--------21.09.2008----------*/
int __start;


void byebye();
void randomize(GtkWidget *widget, gpointer *data);
void clearIt(GtkWidget *widget, gpointer *data);
void presetIt(GtkWidget *widget, gpointer *data); 
void step(GtkWidget *widget, gpointer *data) ;
void start(GtkWidget *widget, GtkLabel *data);
void putLife(int x, int y);


#endif
