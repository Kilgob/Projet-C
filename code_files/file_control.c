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
    json_object *parsed_json_file;
    FILE *fileSave;
    char buffer[50000];
    
    //ajouter le if pour créer le fichier si il n'existe pas
    fileSave = fopen("/Users/fred/OneDrive/ESGI/Annee2/ProjetC/Files/save_select.json", "a+");
    
    if(fileSave == NULL)
        return 1;
    
    fread(buffer, 50000, 1, fileSave);
    parsed_json_file = json_tokener_parse(buffer);
    json_object_object_get_ex(parsed_json_file, "data", &parsed_json_file); //peu etre remplacer par une autre variable de sortie
    
    json_object_object_add(parsed_json_file, "TABLE_NAME", json_object_new_string(Datas->table_Name));
    json_object_object_add(parsed_json_file, "COLUMN_NAME", json_object_new_string(Datas->column_Name));

    
    //Ajout du tableau data dans la requete;
    json_object_object_add(savedData, "Data", parsed_json_file);
    fseek(fileSave, 0, SEEK_SET);
    fprintf(fileSave, " %s",json_object_get_string(savedData));
    fclose(fileSave);//On recupère le fichier dans le mémoire, puis on efface tout avec le nouvel ajout
    
    return 0;
}


 //ajouter le if pour vérifier l'exécution (label erreur)
int save_all_columns_selection(struct JSONReceiver *Datas[]){
    json_object *savedData = json_object_new_object();
    json_object *parsed_json_file = json_object_new_object();
    FILE *fileSave;
    char buffer[50000];
    int i;
    
    //ajouter le if pour créer le fichier si il n'existe pas
    fileSave = fopen("/Users/fred/OneDrive/ESGI/Annee2/ProjetC/Files/save_select.json", "a+");
    
    if(fileSave == NULL)
        return 1;
    
    fread(buffer, 50000, 1, fileSave);
    parsed_json_file = json_tokener_parse(buffer);
    json_object_object_get_ex(parsed_json_file, "data", &parsed_json_file); //peu etre remplacer par une autre variable de sortie
    
    for ( i = 0 ; i > Datas[0]->g; i++) {
        json_object_object_add(parsed_json_file, "TABLE_NAME", json_object_new_string(Datas[i]->table_Name));
        json_object_object_add(parsed_json_file, "COLUMN_NAME", json_object_new_string(Datas[i]->column_Name));
    }
    
    

    
    //Ajout du tableau data dans la requete;
    json_object_object_add(savedData, "Data", parsed_json_file);
    fseek(fileSave, 0, SEEK_SET);
    fprintf(fileSave, " %s",json_object_get_string(savedData));
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
