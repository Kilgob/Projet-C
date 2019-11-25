//
//  connectionBDD.h
//  Projet
//
//  Created by Frédéric FLACELIERE on 04/11/2019.
//  Copyright © 2019 Frédéric FLACELIERE. All rights reserved.
//
#include <stdio.h>
#include <gtk/gtk.h>

struct InputLogin{
    GtkWidget *InputTextIP;
    GtkWidget *InputTextID;
    GtkWidget *InputTextPass;
    GtkWidget *InputValidate;
    GtkWidget *button_box_login;
    int returnStatusConnexion;
};

#ifndef connectionBDD_h
#define connectionBDD_h

void connection_bdd(GtkWidget *widget, struct InputLogin *Data);

#endif /* connectionBDD_h */



#ifndef json_parser_h
#define json_parser_h

int json_parser(void);
#endif /* connectionBDD_h */
