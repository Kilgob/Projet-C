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
    uint16_t g;
};

int save_all_columns_selection(struct JSONReceiver *Datas[]);
int save_selection(struct JSONReceiver *Datas);
int delete_selection(struct JSONReceiver *Datas);
#endif /* file_control_h */
