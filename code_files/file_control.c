//
//  file_control.c
//  Projet
//
//  Created by Frédéric FLACELIERE on 09/12/2019.
//  Copyright © 2019 Frédéric FLACELIERE. All rights reserved.
//
#include <stdio.h>
#include <string.h>

#include "file_control.h"

 //ajouter le if pour vérifier l'exécution (label erreur)
int save_selection(struct JSONReceiver *Datas){
    json_object *savedData = json_object_new_object();
    json_object *parsed_json_file = json_object_new_object();
    FILE *fileSave;
    
    json_object_object_add(parsed_json_file, "TABLE_NAME", json_object_new_string(Datas->table_Name));
    json_object_object_add(parsed_json_file, "COLUMN_NAME", json_object_new_string(Datas->column_Name));
    
    if((fileSave = fopen("/Users/fred/OneDrive/ESGI/Annee2/ProjetC/Files/save_select.json", "rb+")) != NULL){
        fseek(fileSave, 1, SEEK_END);
        fprintf(fileSave, " %s\n",json_object_get_string(parsed_json_file));
    }
    else{
        fileSave = fopen("chemin","ab+");

        //Ajout du tableau data dans la requete;
        json_object_object_add(savedData, "Data", parsed_json_file);
        fprintf(fileSave, " %s\n",json_object_get_string(savedData));
    }
    
    fclose(fileSave);//On recupère le fichier dans le mémoire, puis on efface tout avec le nouvel ajout
    return 0;
}


 //ajouter le if pour vérifier l'exécution (label erreur)
int save_schema_selection(struct JSONReceiver *Datas[]){
    json_object *savedData = json_object_new_object();
    json_object *parsed_json_file = json_object_new_object();
    FILE *fileSave;
    int i;
    
    

    if((fileSave = fopen("/Users/fred/OneDrive/ESGI/Annee2/ProjetC/Files/save_select.json", "rb+")) != NULL){
        fseek(fileSave, 1, SEEK_END);
        for ( i = 0 ; i > Datas[0]->cnb; i++) {
            json_object_object_add(parsed_json_file, "TABLE_NAME", json_object_new_string(Datas[i]->table_Name));
            json_object_object_add(parsed_json_file, "COLUMN_NAME", json_object_new_string(Datas[i]->column_Name));
        }
        fprintf(fileSave, " %s\n",json_object_get_string(parsed_json_file));
    }
    else{
        fileSave = fopen("chemin","ab+");
        for ( i = 0 ; i > Datas[0]->cnb; i++) {
            json_object_object_add(parsed_json_file, "TABLE_NAME", json_object_new_string(Datas[i]->table_Name));
            json_object_object_add(parsed_json_file, "COLUMN_NAME", json_object_new_string(Datas[i]->column_Name));
            json_object_object_add(savedData, "Data", parsed_json_file);
        }
        //Ajout du tableau data dans la requete;
        fprintf(fileSave, " %s\n",json_object_get_string(savedData));
    }

    fclose(fileSave);//On recupère le fichier dans le mémoire, puis on efface tout avec le nouvel ajout
    
    return 0;
}

int save_table_selection(struct JSONReceiver *Datas[]){
    json_object *savedData = json_object_new_object();
    json_object *parsed_json_file = json_object_new_object();
    FILE *fileSave;
    int i;
    

    
    if((fileSave = fopen("/Users/fred/OneDrive/ESGI/Annee2/ProjetC/Files/save_select.json", "rb+")) != NULL){
        fseek(fileSave, 1, SEEK_END);
        for ( i = 0 ; i > Datas[0]->cnt; i++) {
            json_object_object_add(parsed_json_file, "TABLE_NAME", json_object_new_string(Datas[i]->table_Name));
            json_object_object_add(parsed_json_file, "COLUMN_NAME", json_object_new_string(Datas[i]->column_Name));
        }
        fprintf(fileSave, " %s\n",json_object_get_string(parsed_json_file));
    }
    else{
        for ( i = 0 ; i > Datas[0]->cnt; i++) {
            json_object_object_add(parsed_json_file, "TABLE_NAME", json_object_new_string(Datas[i]->table_Name));
            json_object_object_add(parsed_json_file, "COLUMN_NAME", json_object_new_string(Datas[i]->column_Name));
            json_object_object_add(savedData, "Data", parsed_json_file);
        }
        fileSave = fopen("chemin","ab+");
        //Ajout du tableau data dans la requete;
        fprintf(fileSave, " %s\n",json_object_get_string(savedData));
    }
    
    fclose(fileSave);//On recupère le fichier dans le mémoire, puis on efface tout avec le nouvel ajout
    
    return 0;
}





 //ajouter le if pour vérifier l'exécution (label erreur)
int delete_selection(struct JSONReceiver *Datas){
    FILE *fileSave;
    
    fileSave = fopen("/Users/fred/OneDrive/ESGI/Annee2/ProjetC/Files/save_select.json", "r+");
    
    if(fileSave == NULL)
        return 1;
    
    /*code pour supprimer le texte existant*/
    
    fclose(fileSave);
    return 0;
}


int save_all_columns(struct JSONReceiver *Datas){
    return 0;
}
