//
//  file_control.h
//  Projet
//
//  Created by Frédéric FLACELIERE on 09/12/2019.
//  Copyright © 2019 Frédéric FLACELIERE. All rights reserved.
//

struct DataCopy{
    char tableName[50];
    char tableType[50];
};

#ifndef file_control_h
#define file_control_h

int save_selection(GtkWidget *Data);
int modified_selection(GtkWidget *Data);
#endif /* file_control_h */
