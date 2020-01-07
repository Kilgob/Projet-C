////
////  prgm_fork.c
////  Projet
////
////  Created by Frédéric FLACELIERE on 05/01/2020.
////  Copyright © 2020 Frédéric FLACELIERE. All rights reserved.
////
//
#include "prgm_fork.h"
#include "link_main.h"
#include "file_control.h"
#include "uuid/uuid4.h"
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <json-c/json.h>


void fork_import(const char *port, const char *ip){
    struct json_object *result;
    struct json_object *result_buffer;
    struct json_object *result_ID_Task;
    struct json_object *result_Type_Task;

    char Str_result[20000];  //À changer selon vos besoin
    char url[400];
    char url_temp[200];

    FILE *file_temp;
    char file_Name[100];
    char file_Name_Pass[400];
    char fileDirectory[400];
    char json_File_Buffer[8000];

    struct dirent * ent;
    DIR * rep;
    
    while (1) {
        sprintf(fileDirectory, "%s/conf/",getenv("REPLICATOR_HOME"));
        rep = opendir(fileDirectory);
        if (rep != NULL){
            while ((ent = readdir(rep)) != NULL)
            {
//                printf("répertoire temporaire (fork): %s\n", ent->d_name);
                if(strcmp(ent->d_name, ".") && strcmp(ent->d_name, "..") && strcmp(ent->d_name, ".DS_Store")){
                    strcpy(file_Name, ent->d_name);
                    //ouverture du fichier puis on regarde le statut de la tache
                    sprintf(fileDirectory, "%s/conf/",getenv("REPLICATOR_HOME"));
                    sprintf(file_Name_Pass, "%s%s", fileDirectory, ent->d_name);
                    file_temp = fopen(file_Name_Pass, "r");
                    fread(json_File_Buffer, 8000, 1, file_temp);
                    fclose(file_temp);

                    //Récupération des données
                    result_ID_Task = json_object_new_object();
                    result_buffer = json_object_new_object();
                    result_Type_Task = json_object_new_object();
                    result_buffer = json_tokener_parse(json_File_Buffer);
                    result = json_object_array_get_idx(result_buffer, 0);
                    json_object_object_get_ex(result, "ID_Exec", &result_ID_Task);
                    json_object_object_get_ex(result, "task", &result_Type_Task);

                    //Si la tache est de type import, on exécute
                    if(strcmp(json_object_get_string(result_Type_Task),"export")){

//                            printf(" 1 avant l'envoi du JSON \n");
//                            printf(" ID de la tache : %s\n", json_object_get_string(result_ID_Task));
//                            printf(" Type de tache : %s\n", json_object_get_string(result_Type_Task));
//                        printf(" PORT de la tache : %s\n", json_object_get_string(json_conf->PJSON));
                        //Création de l'url
                        sprintf(url, "%s:%s/tom/cmd/etat_tache?task_id=%s&serveur=%s", ip, port, json_object_get_string(result_ID_Task), ip);
                        ApiWs(url, Str_result, 0, NULL, NULL); //boucle tant que le status n'est pas SUCCES
                        //Récupération du statut de l'export
                        result_buffer = json_object_new_object();
                        result_buffer = json_tokener_parse(Str_result);
                        json_object_object_get_ex(result_buffer, "state", &result);

                        if(!strcmp("SUCCESS", json_object_get_string(result))){

                            //Envoi du fichier de commande
                            sprintf(url, "%s:%s/tom/json_upload", ip, port);
                            strcat(fileDirectory, file_Name);
                            ApiWs(url,Str_result, 1, file_Name, fileDirectory);

                            //Requête d'exécution de l'import
                            sprintf(url, "%s:%s/tom/cmd/exec?cmd=replicator_c",ip, port);
                            strcat(url, "%20-f%20");
                            sprintf(url_temp, "%s&serveur=%s", file_Name, ip);
                            strcat(url, url_temp);
                            ApiWs(url, Str_result, 2, NULL, NULL); //boucle tant que le status n'est pas SUCCES

                            //Récupération du statut de l'export
                            result_buffer = json_object_new_object();
                            result_buffer = json_tokener_parse(Str_result);
                            json_object_object_get_ex(result_buffer, "task-id", &result);

                            sprintf(url, "%s:%s/tom/cmd/etat_tache?task_id=%s&serveur=%s", ip,  port, json_object_get_string(result), ip);
                            ApiWs(url, Str_result, 0, NULL, NULL); //boucle tant que le status n'est pas SUCCES
                            //Attente du statut de la tâche d'import
                            while (strcmp(json_object_get_string(result), "SUCCESS")) {
//                                    sprintf(url, "%s:%s/tom/cmd/etat_tache?task_id=%s&serveur=%s", ip,  port, json_object_get_string(result), ip);
                                ApiWs(url, Str_result, 0, NULL, NULL); //boucle tant que le status n'est pas SUCCES
                                printf("Import complete pour la tache %s !\n", json_object_get_string(result));
                                //Récupération du statut de l'export
                                result_buffer = json_object_new_object();
                                result_buffer = json_tokener_parse(Str_result);
                                json_object_object_get_ex(result_buffer, "state", &result);
                                sleep(2);
                            }
//                                sleep(5);
                                remove(file_Name_Pass);
                        }
                        sleep(2);
                    }
                }
            }
            closedir(rep);
            sleep(2);
        }
    }
}
