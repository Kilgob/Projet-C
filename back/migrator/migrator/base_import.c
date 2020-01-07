//
//  base_import.c
//  migrator
//
//  Created by Frédéric FLACELIERE on 30/12/2019.
//  Copyright © 2019 Frédéric FLACELIERE. All rights reserved.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include "base_export.h"


int mysqlVmysql_import(struct Json_infoserv *Json_infoserv, struct Json_export *Json_export,  char *dir){
    DIR * rep = opendir(dir);
    char check_name_file[200];
    char cmd[200];
    int i = 0;
    //boucle pour créer les tables
    if (rep != NULL)
    {
        sprintf(check_name_file, "%s_dump.sql", Json_infoserv->uuid);
        printf("nom du fichier : %s\n",check_name_file);
        struct dirent * ent;
        while ((ent = readdir(rep)) != NULL){
            if(!strcmp(ent->d_name, check_name_file)){
                sprintf(cmd, "mysql -h %s -P %s -u %s -p'%s' %s < %s%s", json_object_get_string(Json_infoserv->ip_target), json_object_get_string(Json_infoserv->port), json_object_get_string(Json_infoserv->user), json_object_get_string(Json_infoserv->pass_user), json_object_get_string(Json_infoserv->name_schema_target), dir, ent->d_name);
                printf("fichier repéré, requête créée: %s\n", cmd);
                system(cmd);
            }
//                printf("fichier dans le dossier %s\n", ent->d_name);
            i++;
        }
        closedir(rep);
    }
    else
        return 1;
    
    return 0;
}

int mysqlVvertica_import(struct Json_infoserv *Json_infoserv, struct Json_export *Json_export,  char *dir){
    DIR * rep;
    char check_name_file[200];
    char cmd[200];
    int i = 0;
    
    sprintf(check_name_file, "%s_dump.sql", Json_infoserv->uuid);
    struct dirent * ent;

//      vsql -h debian10-2 -U dbadmin -d P1ODSGL1 -w admin -c "drop table if exists DBFRED.tabvilles" ; // drop des tables
    for(i = 0; i < Json_export->nbr_arrays ; i++){
        char temp[100];
        sprintf(cmd, "vsql -h %s -p %s -U %s -d %s -w %s -c \"truncate table " , json_object_get_string(Json_infoserv->ip_target), json_object_get_string(Json_infoserv->port), json_object_get_string(Json_infoserv->user), json_object_get_string(Json_infoserv->name_db_target), json_object_get_string(Json_infoserv->pass_user));
        sprintf(temp, "%s.%s\";",json_object_get_string(Json_infoserv->name_schema_target),json_object_get_string(Json_export[i].array));
        strcat(cmd, temp);
        system(cmd);
    }

    
//    vsql -h debian10-2 -U dbadmin -d P1ODSGL1 -w admin -c "COPY DBFRED.tabvilles from LOCAL 'uuid_data_tabvilles.csv' parser fdelimitedparser(header=true)" ; //import des données
//            printf("import vers vertica %ld\n", Json_export->nbr_arrays);
    for(i = 0; i <Json_export->nbr_arrays ; i++){
        rep = opendir(dir);
        if (rep == NULL)
            return 1;
        else{
            sprintf(check_name_file, "%s_data_%s.csv", Json_infoserv->uuid, json_object_get_string(Json_export[i].array));
    //                printf("import vers vertica (boucle) %s %s\n", dir, check_name_file);
            while ((ent = readdir(rep)) != NULL){
                if(!strcmp(ent->d_name, check_name_file)){
                    sprintf(cmd, "vsql -h %s -p %s -U %s -d %s -w %s -c \"COPY %s.%s from LOCAL '%s%s' parser fdelimitedparser(header=true)\" ;" , json_object_get_string(Json_infoserv->ip_target), json_object_get_string(Json_infoserv->port), json_object_get_string(Json_infoserv->user), json_object_get_string(Json_infoserv->name_db_target), json_object_get_string(Json_infoserv->pass_user), json_object_get_string(Json_infoserv->name_schema_target), json_object_get_string(Json_export[i].array), dir, check_name_file);
    //                        printf("fichier repéré, remonté du fichié csv %d (cmd) : %s\n", i, cmd);
                    system(cmd);
                    break;
                }
                    printf("fichier dans le dossier %s\n", ent->d_name);
    //                i++;
            }
            closedir(rep);
        }

    }
    return 0;
}


int mysql_import(struct Json_infoserv *Json_infoserv, struct Json_export *Json_export,  char *dir){
    DIR * rep = opendir(dir);
    char check_name_file[200];
    char cmd[200];
    int i = 0;
    //boucle pour créer les tables
    if (rep != NULL)
    {
        struct dirent * ent;
        while ((ent = readdir(rep)) != NULL)
        {
            strcpy(check_name_file, Json_infoserv->uuid);
            strcat(check_name_file, "_create_");
            strcat(check_name_file, json_object_get_string(Json_export[i].array));
            strcat(check_name_file, ".sql");
            if(!strcmp(ent->d_name, check_name_file)){
                strcpy(cmd, "mysql -h ");
                strcat(cmd, json_object_get_string(Json_infoserv->ip_target));
                strcpy(cmd, "mysql -P ");
                strcat(cmd, json_object_get_string(Json_infoserv->port));
                strcat(cmd, " -u ");
                strcat(cmd, json_object_get_string(Json_infoserv->user));
                strcat(cmd, " -p ");
                strcat(cmd, json_object_get_string(Json_infoserv->pass_user));
                strcat(cmd, " < ");
                strcat(cmd, ent->d_name);
                printf("fichier repéré, requête créée: %s\n", cmd);
//                system(cmd);
            }
            i++;
        }
         
        closedir(rep);
    }
    
    //boucle pour remplir les tables
    rep = opendir(dir);
    if (rep != NULL)
    {
        struct dirent * ent;
         
        while ((ent = readdir(rep)) != NULL)
        {
            strcpy(check_name_file, Json_infoserv->uuid);
            strcat(check_name_file, "_cmd.sql");
            if(!strcmp(ent->d_name, check_name_file)){
                strcpy(cmd, "mysql -h ");
                strcat(cmd, json_object_get_string(Json_infoserv->ip_target));
                strcpy(cmd, "mysql -P ");
                strcat(cmd, json_object_get_string(Json_infoserv->ip_target));
                strcat(cmd, " -u ");
                strcat(cmd, json_object_get_string(Json_infoserv->user));
                strcat(cmd, " -p ");
                strcat(cmd, json_object_get_string(Json_infoserv->pass_user));
                strcat(cmd, " < ");
                strcat(cmd, ent->d_name);
                printf("fichier repéré, requête créée: %s\n", cmd);
//                system(cmd);
            }
            i++;
        }
         
        closedir(rep);
    }
    
    return 0;
}


int oracle_import(){
    return 0;
}


int vertica_import(){
    return 0;
}
int verticaVvertica_import(struct Json_infoserv *Json_infoserv, struct Json_export *Json_export,  char *dir){
    DIR * rep = opendir(dir);
    char check_name_file[200];
    char cmd[200];
    int i = 0;
    
    if (rep != NULL)
    {
        struct dirent * ent;

//      vsql -h debian10-2 -U dbadmin -d P1ODSGL1 -w admin -c "drop table if exists DBFRED.tabvilles" ; // drop des tables
        for(i = 0; i < Json_export->nbr_arrays ; i++){
            sprintf(check_name_file, "%s_dump_%s.sql", Json_infoserv->uuid, json_object_get_string(Json_export[i].array));
//            char temp[100];
            sprintf(cmd, "vsql -h %s -p %s -U %s -d %s -w %s -c \"drop table if exists %s.%s\";" , json_object_get_string(Json_infoserv->ip_target), json_object_get_string(Json_infoserv->port), json_object_get_string(Json_infoserv->user), json_object_get_string(Json_infoserv->name_db_target), json_object_get_string(Json_infoserv->pass_user), json_object_get_string(Json_infoserv->name_schema_target), json_object_get_string(Json_export[i].array));
//            sprintf(temp, "%s.%s\";",json_object_get_string(Json_infoserv->name_schema_target), json_object_get_string(Json_export[i].array));
//            strcat(cmd, temp);
            system(cmd);
        
//        rep = opendir(dir);
//      vsql -h debian10-2 -U dbadmin -d P1ODSGL1 -w admin -f uuid_create_tabvilles.sql ; // creation des tables
        while ((ent = readdir(rep)) != NULL)
        {
            if(!strcmp(ent->d_name, check_name_file)){
                sprintf(cmd, "vsql -h %s -p %s -U %s -d %s -w %s -f %s%s" , json_object_get_string(Json_infoserv->ip_target), json_object_get_string(Json_infoserv->port), json_object_get_string(Json_infoserv->user), json_object_get_string(Json_infoserv->name_db_target), json_object_get_string(Json_infoserv->pass_user), dir, check_name_file);
                printf("fichier repéré, requête de création de table : %s\n", cmd);
                system(cmd);
            }
//                printf("fichier dans le dossier %s\n", ent->d_name);
        }
         
        closedir(rep);
        
//    vsql -h debian10-2 -U dbadmin -d P1ODSGL1 -w admin -c "COPY DBFRED.tabvilles from LOCAL 'uuid_data_tabvilles.csv' parser fdelimitedparser(header=true)" ; //import des données
//        for(i = 0; i <Json_export->nbr_arrays ; i++){
            rep = opendir(dir);
            sprintf(check_name_file, "%s_data_%s.csv", Json_infoserv->uuid, json_object_get_string(Json_export[i].array));
            while ((ent = readdir(rep)) != NULL){
                if(!strcmp(ent->d_name, check_name_file)){
                    sprintf(cmd, "vsql -h %s -p %s -U %s -d %s -w %s -c \"COPY %s.%s from LOCAL '%s%s' parser fdelimitedparser(header=true)\" ;" , json_object_get_string(Json_infoserv->ip_target), json_object_get_string(Json_infoserv->port), json_object_get_string(Json_infoserv->user), json_object_get_string(Json_infoserv->name_db_target), json_object_get_string(Json_infoserv->pass_user), json_object_get_string(Json_infoserv->name_schema_target), json_object_get_string(Json_export[i].array), dir, check_name_file);
                    printf("fichier repéré, remonté du fichié csv %d (cmd) : %s\n", i, cmd);
                    system(cmd);
                    break;
                }
    //                printf("fichier dans le dossier %s\n", ent->d_name);
//                i++;
            }
            closedir(rep);
        }
    }
    else
        return 1;
        
    return 0;
}

