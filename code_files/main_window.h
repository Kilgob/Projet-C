//
//  main_window.h
//  Projet
//
//  Created by Frédéric FLACELIERE on 25/11/2019.
//  Copyright © 2019 Frédéric FLACELIERE. All rights reserved.
//
#include <stdio.h>
#include <gtk/gtk.h>

struct create_main_window{
    GtkWidget *oldWindow;
    GtkApplication *app;
    int *logSQLStatus;
};

#ifndef main_window_h
#define main_window_h

void main_windows_create(GtkWidget *widget, struct create_main_window *Data);
void destroy_Connection_Window(GtkWidget *widget,gpointer data);

#endif /* main_window_h */
