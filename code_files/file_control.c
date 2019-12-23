//
//  file_control.c
//  Projet
//
//  Created by Frédéric FLACELIERE on 09/12/2019.
//  Copyright © 2019 Frédéric FLACELIERE. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "file_control.h"
#include "xml_parser.h"
#include "link_main.h"


int check_file_conf(){
    FILE *file_conf;
    if((file_conf = fopen("/Users/fred/OneDrive/ESGI/Annee2/ProjetC/conf_bdd.json", "r")) != NULL){
        fclose(file_conf);
        return 0;
    }
    printf("/!\\Il manque le fichier de conf ! /!\\");
    printf("chemin d'accès standard au fichier : /Users/fred/OneDrive/ESGI/Annee2/ProjetC/conf_bdd.json");
   return 1;
}



void save_table_selection(struct Recup_Widgets *Data){

    GList *children, *children2 = NULL, *iter;
//    printf("\n%p",Data);
    children = gtk_container_get_children(GTK_CONTAINER(Data->Array_bdd));
    if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(Data->array_Name))){
//        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(Data->array_Name) , FALSE);
        for(iter = children; iter != NULL; iter = g_list_next(iter)){
            children2 = gtk_container_get_children(GTK_CONTAINER(iter->data));
            if(GTK_IS_TOGGLE_BUTTON(children2->data))
                gtk_toggle_button_set_active(children2->data , TRUE);
        }
    }
    else{
        for(iter = children; iter != NULL; iter = g_list_next(iter)){
            children2 = gtk_container_get_children(GTK_CONTAINER(iter->data));
            if(GTK_IS_TOGGLE_BUTTON(children2->data))
                gtk_toggle_button_set_active(children2->data , FALSE);
        }
    }
    g_list_free(children);
    g_list_free(children2);
    
}


void data_export(struct WidgetBDD *Datas){
//    GList *children = NULL, *children2 = NULL, *iter, *iter2;
    int i;

    for(i = 0; i < Datas->nbr_Column; i++){
        if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(Datas->column_Name[i]))){
            printf("%s ",gtk_button_get_label(GTK_BUTTON(Datas->array_Name[i])));
            printf("%s ",gtk_button_get_label(GTK_BUTTON(Datas->column_Name[i])));
            printf("%s ",gtk_label_get_text(GTK_LABEL(Datas->column_Type[i])));
            printf("%s ",gtk_label_get_text(GTK_LABEL(Datas->column_Key[i])));
        }
        printf("\n");
    }
//    g_list_free(children);
//    g_list_free(children2);
}


//void datas_Migration_base(struct ExportData *Datas){
//    const char *valueSelection = gtk_combo_box_get_active_id(GTK_COMBO_BOX(Datas->cb_Export));
//    int t;
//    switch (atoi(valueSelection)) {
//        case 0:
//            //Fonction export JSON
//            t = requestToBDD(Datas->JSONDatas->column_Name, Datas->JSONDatas->column_Type);
////            printf("%d",t);
//            break;
//
//        case 1:
//            //Fonction export XML
//            xml_to_text();
//            text_to_xml();
//            break;
//        case 2:
//            pause();
//            break;
//    }
//}

