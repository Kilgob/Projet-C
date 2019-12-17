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

struct JSONReceiver{
    char table_Schema[50];
    char table_Name[50];
    char column_Name[50];
    char column_Type[50];
    char column_Key[50];
    uint16_t i;
    uint16_t j;
    uint16_t cnb;
    uint16_t cnt;
};

struct ExportData{
    struct JSONReceiver *JSONDatas;
    GtkWidget *cb_Export;
    
};

int check_file_conf(void);


void save_schema_selection(struct JSONReceiver *Datas[]);
void save_table_selection(struct JSONReceiver *Datas[]);
void save_selection(struct JSONReceiver *Datas);
void check_delete(struct JSONReceiver *Datas[]);
void data_export(struct ExportData *Datas);
#endif /* file_control_h */
