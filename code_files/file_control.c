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


//Fonction permettant la sélection de toutes les colonnes d'une table sélectionnée
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


//Fonction main d'export local des données
void data_export(struct WidgetBDD *Datas){
    if(gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(Datas->Export_Info->target_Type)) == NULL)
        printf("test\n");//remplacer par un label indiquant une erreur de sélection du type d'export
    
    FILE * fileSave;
    int status;
    
//    printf("chemin : %s\n",gtk_entry_get_text(GTK_ENTRY(Datas->Export_Info->target_Folder)));
    char Target[strlen(gtk_entry_get_text(GTK_ENTRY(Datas->Export_Info->target_Folder))+ 5)];
    strcpy(Target, gtk_entry_get_text(GTK_ENTRY(Datas->Export_Info->target_Folder)));
    strcat(Target, ".");
    if(gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(Datas->Export_Info->target_Type)) != NULL)
        strcat(Target,gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(Datas->Export_Info->target_Type)));
    else
        status = 1;
//    printf("%s\n",Target);
    if((fileSave = fopen(Target, "w")) == NULL)
        status = 1;
    
    switch (gtk_combo_box_get_active(GTK_COMBO_BOX(Datas->Export_Info->target_Type))) {
        case 0:
            status = export_MLD(Datas, fileSave);
            break;
        case 1:
            status = export_CSV(Datas, fileSave);
        break;
        case 2:
            status = export_JSON(Datas, fileSave);
        break;
        case 3:
            status = export_XML(Datas, fileSave);

        break;
            
        default:
            status = 1;
//            gtk_label_set_text(GTK_LABEL(Datas->Export_Info->label_Status), "Erreur lors du choix");
    }
    if(status == 0) //actuellement, les fonctions ne retournent que 0
        gtk_label_set_text(GTK_LABEL(Datas->Export_Info->label_Status), "Exportation réussi !");//export effectué
    else
        gtk_label_set_text(GTK_LABEL(Datas->Export_Info->label_Status), "Échec de l'exportation...\nVérifier votre choix d'export et les droits du dossiers"); //export reporté
    
    fclose(fileSave);
}




int export_MLD(struct WidgetBDD *Datas, FILE *fileSave){
    int i;
    char check_Name[50];
    char check_Pri[50];
    for(i = 0; i < Datas->nbr_Column; i++){
        if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(Datas->column_Name[i]))){
            if(strcmp(check_Name, gtk_button_get_label(GTK_BUTTON(Datas->array_Name[i])))){
                strcpy(check_Name, gtk_button_get_label(GTK_BUTTON(Datas->array_Name[i])));
                fprintf(fileSave, "DROP TABLE IF EXISTS '%s';\n",gtk_button_get_label(GTK_BUTTON(Datas->array_Name[i])));
            }
            fprintf(fileSave,"CREATE TABLE '%s'; \n",gtk_button_get_label(GTK_BUTTON(Datas->array_Name[i])));
            fprintf(fileSave,"ALTER TABLE '%s' ADD '%s' '%s';\n",gtk_button_get_label(GTK_BUTTON(Datas->array_Name[i])),gtk_button_get_label(GTK_BUTTON(Datas->column_Name[i])),gtk_label_get_text(GTK_LABEL(Datas->column_Type[i])));
            if(strcmp(check_Pri, gtk_label_get_text(GTK_LABEL(Datas->column_Key[i])))){
                strcpy(check_Pri, gtk_label_get_text(GTK_LABEL(Datas->column_Key[i])));
                fprintf(fileSave,"ALTER TABLE '%s' ADD PRIMARY KEY ('%s'); \n",gtk_button_get_label(GTK_BUTTON(Datas->array_Name[i])),gtk_label_get_text(GTK_LABEL(Datas->column_Key[i])));
            }
                
        }
    }
    return 0;
}
int export_CSV(struct WidgetBDD *Datas, FILE *fileSave){
    int i;
    for(i = 0; i < Datas->nbr_Column; i++){
        if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(Datas->column_Name[i]))){
            fprintf(fileSave,"%s,",gtk_button_get_label(GTK_BUTTON(Datas->array_Name[i])));
            fprintf(fileSave,"%s,",gtk_button_get_label(GTK_BUTTON(Datas->column_Name[i])));
            fprintf(fileSave,"%s,",gtk_label_get_text(GTK_LABEL(Datas->column_Type[i])));
            fprintf(fileSave,"%s\n",gtk_label_get_text(GTK_LABEL(Datas->column_Key[i])));
        }
    }
    return 0;
}
int export_JSON(struct WidgetBDD *Datas, FILE *fileSave){
    int i;
    struct json_object *JsonBuffer;
    struct json_object *JsonArray;
    struct json_object *JsonColumn;
    JsonBuffer = json_object_new_object();
   
    JsonColumn = json_object_new_object();;
    
    JsonBuffer = json_object_new_array();
    
    for(i = 0; i < Datas->nbr_Column; i++){
        JsonArray = json_object_new_object();
        if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(Datas->column_Name[i]))){
            json_object_object_add(JsonArray, "array name", json_object_new_string(gtk_button_get_label(GTK_BUTTON(Datas->array_Name[i])))) ;
            json_object_object_add(JsonArray, "column name", json_object_new_string(gtk_button_get_label(GTK_BUTTON(Datas->column_Name[i]))));
            json_object_object_add(JsonArray, "column type", json_object_new_string(gtk_label_get_text(GTK_LABEL(Datas->column_Type[i]))));
            json_object_object_add(JsonArray, "column key", json_object_new_string(gtk_label_get_text(GTK_LABEL(Datas->column_Key[i]))));
            json_object_array_add(JsonBuffer, JsonArray);
        }
    }
    fprintf(fileSave,"%s",json_object_get_string(JsonBuffer));
    return 0;
}
int export_XML(struct WidgetBDD *Datas, FILE *fileSave){
    //mdrr elle est vide
    return 0;
}

