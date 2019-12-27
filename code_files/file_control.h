//
//  file_control.h
//  Projet
//
//  Created by Frédéric FLACELIERE on 09/12/2019.
//  Copyright © 2019 Frédéric FLACELIERE. All rights reserved.
//



#ifndef file_control_h
#define file_control_h

#include <gtk/gtk.h>
#include <json-c/json.h>
#include <stdint.h>

//struct JSONReceiver{ //encore (à) utilisée ?
//    char table_Schema[50];
//    char table_Name[50];
//    char column_Name[50];
//    char column_Type[50];
//    char column_Key[50];
//    uint16_t i;
//    uint16_t j;
//    uint16_t cnb;
//    uint16_t cnt;
//};

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
    char *array_NameBis[50];//encore utilisée ?
    int nbr_Array; //encore utilisée ?
    int nbr_Column;
    struct ExportData *Export_Info;
    struct MigrationData *Migration_Info;
};

struct desactivate{
    gulong idGsignal;
    GtkWidget **name;
};

int check_file_conf(void);
void save_table_selection(struct Recup_Widgets *Data);
void data_export(struct WidgetBDD *Datas);

int export_SQL(struct WidgetBDD *Data, FILE *fileSave);
int export_CSV(struct WidgetBDD *Data, FILE *fileSave);
int export_JSON(struct WidgetBDD *Data, FILE *fileSave, int task, struct json_object *file_Migration);
int export_XML(struct WidgetBDD *Data, FILE *fileSave);
//void datas_Migration_base(struct ExportData *Datas){
#endif /* file_control_h */
