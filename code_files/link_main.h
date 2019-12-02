//
//  connectionBDD.h
//  Projet
//
//  Created by Frédéric FLACELIERE on 04/11/2019.
//  Copyright © 2019 Frédéric FLACELIERE. All rights reserved.
//
#include <stdio.h>
#include <gtk/gtk.h>
#include <json-c/json.h>
#include <mysql.h>
#include <string.h>
#include <curl/curl.h>

struct InputLogin{
    GtkWidget *InputTextIP;
    GtkWidget *InputTextID;
    GtkWidget *InputTextPass;
    GtkWidget *InputValidate;
    GtkWidget *button_box_login;
    int returnStatusConnexion;
};

struct create_main_window{
    GtkWidget *oldWindow;
    GtkApplication *app;
    GtkWidget *LabelStatusConnection;
    struct InputLogin *Login;
};

#ifndef connectionBDD_h
#define connectionBDD_h

void connection_bdd(GtkWidget *widget, struct InputLogin *Data);

#endif /* connectionBDD_h */



#ifndef json_parser_h
#define json_parser_h
//Structure recevant la sortie de LibCurl
struct BufferStruct
{
  char* buffer;
  size_t size;
};

void get_Arrays_Base(json_object **array);

#endif /* connectionBDD_h */



#ifndef main_window_h
#define main_window_h

void main_windows_create(GtkWidget *widget, struct create_main_window *Data);
void destroy_Connection_Window(GtkWidget *widget,gpointer data);

#endif /* main_window_h */
