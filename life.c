#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "engine.h"
#include "gui.h"
#include "life.h"



int run (GtkWidget *widget )
{
    if(__start==0)
        return 1;

    copyMatrix(N, matrix, oldmatrix);
    engine(N, matrix);
    draw(widget);

    return 1;
}

void byebye() 
{
    exit(0);
}

void randomize(GtkWidget *widget, gpointer *data)
{
    copyMatrix(N, matrix, oldmatrix);/*--------21.09.2008----------*/
    random1(N, matrix);
    draw(area);
}

void clearIt(GtkWidget *widget, gpointer *data)
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

void putLife(int x, int y)
{
    copyMatrix(N, matrix, oldmatrix);/*--------21.09.2008----------*/
    matrix[x][y] = 1;
    draw(area);
}

void step(GtkWidget *widget, gpointer *data) 
{
g_print("step\n");
    if(__start)
        return;

    __start=1;
    run(area);
    __start = 0;
}

void start(GtkWidget *widget, GtkLabel *data) 
{
    if(__start == 0)
    {
        __start=1;
        gtk_label_set_text(data, "Stop");
    }else
    {
        __start=0;
        gtk_label_set_text(data, "Start");
    }
}

int main(int argc,char *argv[])
{

    initialize_gui(argc, argv);

    set_run_fun(run);

    gtk_main();

    return 1;
}
