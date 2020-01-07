//
//  file_control.h
//  Projet
//
//  Created by Frédéric FLACELIERE on 09/12/2019.
//  Copyright © 2019 Frédéric FLACELIERE. All rights reserved.
//



#ifndef file_control_h
#define file_control_h

struct content{
    char name[50];
    char content[50];
    char nameAttribute[50];
    char attribute[50];
};

#include <gtk/gtk.h>
#include <json-c/json.h>
#include <stdint.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

struct ExportData{ //encore (à) utilisée ?
    GtkWidget *target_Folder;
    GtkWidget *target_Type;
    GtkWidget *label_Status;
};

struct Recup_Widgets{
    GtkWidget *array_Name;
    GtkWidget *Array_bdd;
    int number_array;
    gulong idGsignalc;
    gulong *idGsignala;
};

struct WidgetBDD{
    GtkWidget **array_Name;
    GtkWidget **column_Type;
    GtkWidget **array_Schema;
    GtkWidget **column_Name;
    GtkWidget **column_Key;
    int nbr_Array;
    int nbr_Column;
    struct ExportData *Export_Info;
    struct MigrationData *Migration_Info;
};

struct desactivate{
    gulong idGsignal;
    GtkWidget **name;
};

struct Block_Status{
    GtkWidget *progress_Bar;
    GtkWidget *label_Status_bar;
    GtkWidget **list_Element;
    GtkWidget *statut_Task_Block;
    GtkWidget *window;
};
int block_status_get(struct Block_Status *Data);

int check_file_conf(void);
void save_table_selection(struct Recup_Widgets *Data);
void data_export(struct WidgetBDD *Datas);

int export_SQL(struct WidgetBDD *Data, FILE *fileSave);
int export_CSV(struct WidgetBDD *Data, FILE *fileSave);
int export_JSON(struct WidgetBDD *Data, FILE *fileSave, int task, struct json_object *file_Migration);
int export_XML(struct WidgetBDD *Data, FILE *fileSave);
void text_to_xml(void);

int ajouter_data(struct content *content , xmlNodePtr xmlNodePtr, int size);
void delete(GtkWidget *);

#endif /* file_control_h */
