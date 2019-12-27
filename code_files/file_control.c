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
    
    FILE * fileSave = NULL;
    int status = 0;
    
//    printf("chemin : %s\n",gtk_entry_get_text(GTK_ENTRY(Datas->Export_Info->target_Folder)));
    char Target[strlen(gtk_entry_get_text(GTK_ENTRY(Datas->Export_Info->target_Folder))+ 5)];
    strcpy(Target, gtk_entry_get_text(GTK_ENTRY(Datas->Export_Info->target_Folder)));
    strcat(Target, ".");
    if(gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(Datas->Export_Info->target_Type)) != NULL){
        strcat(Target,gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(Datas->Export_Info->target_Type)));
        if((fileSave = fopen(Target, "w")) == NULL)
            status = 1;
        else
            switch (gtk_combo_box_get_active(GTK_COMBO_BOX(Datas->Export_Info->target_Type))) {
                case 0:
                    status = export_SQL(Datas, fileSave);
                    break;
                case 1:
                    status = export_CSV(Datas, fileSave);
                break;
                case 2:
                    status = export_JSON(Datas, fileSave, 0, NULL);
                break;
                case 3:
                    status = export_XML(Datas, fileSave);

                break;
                    
                default:
                    status = 1;
            }
    }
    else
        status = 1;
//    printf("%s\n",Target);

    if(status == 0){ //actuellement, les fonctions ne retournent que 0
        gtk_label_set_text(GTK_LABEL(Datas->Export_Info->label_Status), "Exportation réussi !");//export effectué
        fclose(fileSave);
    }
    else
        gtk_label_set_text(GTK_LABEL(Datas->Export_Info->label_Status), "Échec de l'exportation...\nVérifier votre choix d'export et les droits du dossiers"); //export reporté
    
    
}




int export_SQL(struct WidgetBDD *Datas, FILE *fileSave){
    int i = 0,j;
    int check_complete_array = 1;
    char check_array[50];
//    char check_Pri[50];
        for(i = 0; i < Datas->nbr_Column; i++){
            strcpy(check_array, gtk_button_get_label(GTK_BUTTON(Datas->array_Name[i])));
            if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(Datas->column_Name[i]))){
                fprintf(fileSave, "DROP TABLE IF EXISTS '%s';\n",gtk_button_get_label(GTK_BUTTON(Datas->array_Name[i])));
                fprintf(fileSave,"CREATE TABLE '%s'(\n",gtk_button_get_label(GTK_BUTTON(Datas->array_Name[i])));
                for(j = i ; j < Datas->nbr_Column && !strcmp(check_array, gtk_button_get_label(GTK_BUTTON(Datas->array_Name[j]))) ;j++){
                    if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(Datas->column_Name[j]))){
                        fprintf(fileSave,"'%s' '%s' ",gtk_button_get_label(GTK_BUTTON(Datas->column_Name[j])),gtk_label_get_text(GTK_LABEL(Datas->column_Type[j])));
                        if(/*i != Datas->nbr_Column-1 && */!strcmp(check_array, gtk_button_get_label(GTK_BUTTON(Datas->array_Name[i]))))
                            fprintf(fileSave,",\n");
                    }
//                    i++;
                }
                i = j-1;//car j dépasse de 1 le nombre de colonnes sinon
                fprintf(fileSave, ");\n");
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
int export_JSON(struct WidgetBDD *Datas, FILE *fileSave, int task, struct json_object *file_Migration){ //task = 0/1 export ou migration
        int i = 0,j,h =0;
        int check_complete_array = 1;
        char check_array[50];
        
         struct json_object *Json;
         struct json_object *JsonArray;
         struct json_object *JsonColumn;
        struct json_object *JsonColumnIN;
         Json = json_object_new_array();
        
        for(i = 0; i < Datas->nbr_Column; i++){
            check_complete_array = 1;
            JsonArray = json_object_new_object();
            JsonColumn = json_object_new_array();

            h = 0;
            if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(Datas->column_Name[i]))){
                json_object_object_add(JsonArray, "array", json_object_new_string(gtk_button_get_label(GTK_BUTTON(Datas->array_Name[i]))));
                strcpy(check_array, gtk_button_get_label(GTK_BUTTON(Datas->array_Name[i])));
                for(j = i ; j < Datas->nbr_Column && !strcmp(check_array, gtk_button_get_label(GTK_BUTTON(Datas->array_Name[j]))) ;j++){
                    if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(Datas->column_Name[j]))){
                        JsonColumnIN = json_object_new_object();
                        json_object_object_add(JsonColumnIN, "column name", json_object_new_string(gtk_button_get_label(GTK_BUTTON(Datas->column_Name[j]))));
                        json_object_object_add(JsonColumnIN, "column type", json_object_new_string(gtk_label_get_text(GTK_LABEL(Datas->column_Type[j]))));
                        json_object_object_add(JsonColumnIN, "column key", json_object_new_string(gtk_label_get_text(GTK_LABEL(Datas->column_Key[j]))));

                        json_object_array_add(JsonColumn, JsonColumnIN);
                        h++;
                    }
                    else
                        check_complete_array = 0;
                    
                }
                json_object_object_add(JsonArray, "all_column", json_object_new_int(check_complete_array));
                json_object_object_add(JsonArray, "columns", JsonColumn);
                i = j-1;//car j dépasse de 1 le nombre de colonnes sinon
                json_object_array_add(Json, JsonArray);
            }
            
        }
    if(task == 0)
        fprintf(fileSave,"%s",json_object_get_string(Json));
    else
        json_object_object_add(file_Migration, "request_export", Json);
        
    return 0;
}
int export_XML(struct WidgetBDD *Datas, FILE *fileSave){
    return 1;
}

