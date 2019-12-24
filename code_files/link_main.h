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

struct Button_Connection{
    GtkWidget *connection_button;
    GtkWidget *connection_button_box;
};


struct Struct_Conf_Name_Server{
    GtkWidget *Conf_Name_Server; //widget du fichier de conf
//    struct Struct_Conf_Name_bdd *Conf_Name_bdd;
    size_t nbr_bdd;
    size_t nbr_server;
    GtkWidget *widgConfb;
    GtkWidget **Conf_Name_bdd;
    GtkWidget *Boxbdd;
    GtkWidget *boxJSON_Conf;
};


struct Json_Conf_BDD{
    struct json_object *name_BDD;
    struct json_object *PMYSQL;
    struct json_object *PLMYSQL;
    struct json_object *Pass_Root;
    struct json_object *BDD_Type;
};

struct json_conf{
    struct json_object *IP;
    struct json_object *name_server;
    struct json_object *user_webService;
    struct json_object *pass_webService;
    struct Json_Conf_BDD *bdd;

};

struct InputLogin{
    GtkWidget *InputTextIP;
    GtkWidget *InputTextID;
    GtkWidget *InputTextPass;
};

struct create_main_window{
    GtkWidget *oldWindow;
    GtkApplication *app;
    GtkWidget *LabelStatusConnection;
    struct InputLogin *Login;
    int returnStatusConnexion;
    struct json_conf *Json_conf;
    struct Struct_Conf_Name_Server *Servers_and_bdds;
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

void get_Arrays_Base(json_object **array, struct json_conf *Json_conf);
int requestToBDD(char *, char*);

#endif /* connectionBDD_h */



#ifndef main_window_h
#define main_window_h

//struct JSONReceiver{
//    char table_Schema[50];
//    char table_Name[50];
//    char column_Name[50];
//    char column_Type[50];
//    char column_Key[50];
//};

void main_windows_create(GtkWidget *widget, struct create_main_window *Data);
void destroy_Connection_Window(GtkWidget *widget,gpointer data);

#endif /* main_window_h */
