//
//  connectionBDD.c
//  Projet
//
//  Created by Frédéric FLACELIERE on 04/11/2019.
//  Copyright © 2019 Frédéric FLACELIERE. All rights reserved.
//

#include "link_main.h"
#include "main_window.h"
#include <mysql.h>

void connection_bdd(GtkWidget *widget, struct InputLogin *Data){
//    char *result = malloc(sizeof(char)*100);
//    printf("%s",result);
    MYSQL mysql;
    mysql_init(&mysql);
    

    mysql_options(&mysql,MYSQL_READ_DEFAULT_GROUP,"option");
    printf("%d\n",Data->returnStatusConnexion);

    if(mysql_real_connect(&mysql,gtk_entry_get_text(GTK_ENTRY(Data->InputTextIP)),gtk_entry_get_text(GTK_ENTRY(Data->InputTextID)),gtk_entry_get_text(GTK_ENTRY(Data->InputTextPass)),"mainCBase",3306,NULL,0))
    {
        printf("DONE ! :D\n");
//        result = "DONE ! :D\n";
        mysql_close(&mysql);
        Data->returnStatusConnexion = 1;
    }
    else
    {
        printf("%s",gtk_entry_get_text(GTK_ENTRY(Data->InputTextIP)));
        printf("%s",gtk_entry_get_text(GTK_ENTRY(Data->InputTextID)));
        printf("%s",gtk_entry_get_text(GTK_ENTRY(Data->InputTextPass)));
//        printf("\n%s",result);
        printf("Une erreur s'est produite lors de la connexion à la BDD\n");
//        result = "Une erreur s'est produite lors de la connexion à la BDD!";
        Data->returnStatusConnexion = 0;
        printf("%d\n",Data->returnStatusConnexion);
//        Create_main_window1.logSQLSatus = Login1.returnStatusConnexion;
    }

//    free(result);
}
