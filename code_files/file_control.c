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

#include "file_control.h"
#include "xml_parser.h"
#include "link_main.h"


int check_file_conf(){
    FILE *file_conf;
    if((file_conf = fopen("/Users/fred/OneDrive/ESGI/Annee2/ProjetC/conf_bdd.json", "r")) != NULL){
        fclose(file_conf);
        printf("/!\\Il manque le fichier de conf ! /!\\");
        printf("chemin d'accès standard au fichier : /Users/fred/OneDrive/ESGI/Annee2/ProjetC/conf_bdd.json");
        return 0;
    }
       return 1;
}



 //ajouter le if pour vérifier l'exécution (label erreur)
void save_selection(struct JSONReceiver *Datas){
    json_object *savedData = json_object_new_object();
    json_object *parsed_json_file = json_object_new_object();
    FILE *fileSave;
    
    json_object_object_add(parsed_json_file, "TABLE_NAME", json_object_new_string(Datas->table_Name));
    json_object_object_add(parsed_json_file, "COLUMN_NAME", json_object_new_string(Datas->column_Name));
    
    if((fileSave = fopen("/Users/fred/OneDrive/ESGI/Annee2/ProjetC/Files/save_select.json", "rb+")) != NULL){

        if(Datas->i)//Si il y a une suppression, fin
            exit(0);
        
        fseek(fileSave, 1, SEEK_END);
        fprintf(fileSave, " %s\n",json_object_get_string(parsed_json_file));
    }
    else{
        fileSave = fopen("chemin","ab");

        //Ajout du tableau data dans la requete;
        json_object_object_add(savedData, "Data", parsed_json_file);
        fprintf(fileSave, " %s\n",json_object_get_string(savedData));
    }
    
    fclose(fileSave);//On recupère le fichier dans le mémoire, puis on efface tout avec le nouvel ajout
//    return 0;
}


//ajouter le if pour vérifier l'exécution (label erreur)
void save_schema_selection(struct JSONReceiver *Datas[]){
    json_object *savedData = json_object_new_object();
    json_object *parsed_json_file[Datas[0]->cnb];
    FILE *fileSave;
    int i;

        if((fileSave = fopen("/Users/fred/OneDrive/ESGI/Annee2/ProjetC/Files/save_select.json", "rb+")) != NULL){
            
            if(Datas[0]->i)//Si il y a une suppression, fin
                return;
            
            fseek(fileSave, 1, SEEK_END);
            for ( i = 0 ; i < Datas[0]->cnb; i++) {
                json_object_object_add(parsed_json_file[i], "TABLE_NAME", json_object_new_string(Datas[i]->table_Name));
                json_object_object_add(parsed_json_file[i], "COLUMN_NAME", json_object_new_string(Datas[i]->column_Name));
//                json_object_object_add(parsed_json_file[i], "COLUMN_NAME", json_object_new_string(Datas[i]->column_Name));
            }
            fprintf(fileSave, " %s\n",json_object_get_string(parsed_json_file[i]));
        }
        else{
            fileSave = fopen("chemin","ab");
            for ( i = 0 ; i < Datas[0]->cnb; i++) {
                json_object_object_add(parsed_json_file[i], "TABLE_NAME", json_object_new_string(Datas[i]->table_Name));
                json_object_object_add(parsed_json_file[i], "COLUMN_NAME", json_object_new_string(Datas[i]->column_Name));
                json_object_object_add(savedData, "Data", parsed_json_file[i]);
            }
            //Ajout du tableau data dans la requete;
            fprintf(fileSave, " %s\n",json_object_get_string(savedData));
        }
    fclose(fileSave);//On recupère le fichier dans le mémoire, puis on efface tout avec le nouvel ajout
    
//    return 0;
}

void save_table_selection(struct JSONReceiver *Datas[]){
    json_object *savedData = json_object_new_object();
    json_object *parsed_json_file[Datas[0]->cnt]/* = json_object_new_object()*/;
    FILE *fileSave;
    int i;
    

    
    if((fileSave = fopen("/Users/fred/OneDrive/ESGI/Annee2/ProjetC/Files/save_select.json", "rb+")) != NULL){
        
//        if(check_delete(1,fileSave,&Datas[0]))//Si il y a une suppression, fin
//        return 0;
        
        fseek(fileSave, 1, SEEK_END);
        for ( i = 0 ; i < Datas[0]->cnt; i++) {
            json_object_object_add(parsed_json_file[i], "TABLE_NAME", json_object_new_string(Datas[i]->table_Name));
            json_object_object_add(parsed_json_file[i], "COLUMN_NAME", json_object_new_string(Datas[i]->column_Name));
            fprintf(fileSave, " %s\n",json_object_get_string(parsed_json_file[i]));
        }
        
    }
    else{
        for ( i = 0 ; i < Datas[0]->cnt; i++) {
            json_object_object_add(parsed_json_file[i], "TABLE_NAME", json_object_new_string(Datas[i]->table_Name));
            json_object_object_add(parsed_json_file[i], "COLUMN_NAME", json_object_new_string(Datas[i]->column_Name));
            json_object_object_add(savedData, "Data", parsed_json_file[i]);
        }
        fileSave = fopen("chemin","ab+");
        //Ajout du tableau data dans la requete;
        fprintf(fileSave, " %s\n",json_object_get_string(savedData));
    }
    
    fclose(fileSave);//On recupère le fichier dans le mémoire, puis on efface tout avec le nouvel ajout
    
//    Datas 0;
}




void check_delete(struct JSONReceiver *Datas[]){ //check = 0/1/2 en fonction du niveau de suppression (shema/table/colonne)
    //Section pour la suppression
    int i;
    struct json_object *parsed_json = json_object_new_object();
    struct json_object *parsed_json_buffer = json_object_new_object();
    struct json_object *parsed_json_buffer2 = json_object_new_object();
    struct json_object *Table_Schema = json_object_new_object();;
    struct json_object *Table_Name = json_object_new_object();;
    struct json_object *Column_Name = json_object_new_object();;
    char buffer[1000];
    int search = 0;
    FILE *pf;
    pf = fopen("/Users/fred/OneDrive/ESGI/Annee2/ProjetC/Files/save_select.json", "r");
    fread(&buffer, 1000, 1, pf);
    
    parsed_json = json_tokener_parse(buffer);
//    json_object_object_get_ex(parsed_json_buffer, "", &parsed_json);
    printf("%s",json_object_get_string(parsed_json));
    int check = 2;
    
    size_t n_Col_tab = json_object_array_length(parsed_json);
    for ( i = 0 ; i < n_Col_tab; i++) {
        parsed_json_buffer2 = json_object_array_get_idx(parsed_json, (int)i);
//        json_object_object_get_ex(parsed_json_buffer2, "TABLE_SCHEMA", &Table_Schema);
        json_object_object_get_ex(parsed_json_buffer2, "TABLE_NAME", &Table_Name);
        json_object_object_get_ex(parsed_json_buffer2, "COLUMN_NAME", &Column_Name);
        printf("\n\n test : %s",json_object_get_string(Column_Name));
        
        switch (check) {
            case 0:
                if(strcmp(json_object_get_string(Table_Schema), Datas[0]->table_Schema) == 0){
                    search = 1;
                    json_object_array_del_idx(parsed_json_buffer, 0, i);
                }
                break;
            case 1:
                if(strcmp(json_object_get_string(Table_Schema), Datas[0]->table_Schema) == 0
                   &&  strcmp(json_object_get_string(Table_Name), Datas[0]->table_Name)){
                    search = 1;
                    json_object_array_del_idx(parsed_json_buffer, 0, i);
                }
            break;
            case 2:
                printf("\ntable schema : %s",Datas[0]->table_Schema);
                if( /*strcmp(json_object_get_string(Table_Schema), Datas[0]->table_Schema) == 0 &&*/
                    strcmp(json_object_get_string(Table_Name), Datas[0]->table_Name) &&
                    strcmp(json_object_get_string(Column_Name), Datas[0]->column_Name) ){
                    search = 1;
                    json_object_array_del_idx(parsed_json, 0, i);
                }
                break;
                
            default:
                break;
        }
        
        //le réouvrir en w pour mettre le nouveau contenu
        
        Datas[0]->j = 0;
    }
}


void data_export(struct ExportData *Datas){
    const char *valueSelection = gtk_combo_box_get_active_id(GTK_COMBO_BOX(Datas->cb_Export));
    int t;
    switch (atoi(valueSelection)) {
        case 0:
            //Fonction export JSON
            t = requestToBDD(Datas->JSONDatas->column_Name, Datas->JSONDatas->column_Type);
//            printf("%d",t);
            break;

        case 1:
            //Fonction export XML
            xml_to_text();
            text_to_xml();
            break;
        case 2:
            pause();
            break;
    }
}
