//
//  main.c
//  migrator
//
//  Created by Frédéric FLACELIERE on 27/12/2019.
//  Copyright © 2019 Frédéric FLACELIERE. All rights reserved.
//  Programme permettant l'import/export des bases de données stockées sur les serveurs

#include <stdio.h>
#include <string.h>
#include <json-c/json.h>
#include <libgen.h>

struct Json_infoserv{
    struct json_object *pass_root;
    struct json_object *user;
    struct json_object *pass_user;
    struct json_object *pmysql;
    struct json_object *bdd_type;
    struct json_object *name_schema;
    struct json_object *task;
};

struct Json_export{
    struct json_object *array;
    struct json_object *all_column;
    struct Json_export_column *column;
};
struct Json_export_column{
    struct json_object *column_name;
    struct json_object *column_type;
    struct json_object *column_key;
};


void get_info_serv(struct Json_infoserv *, char *);
void get_info_export(struct Json_export **, char *);


int main(int argc, const char * argv[]) {
    struct Json_infoserv *Json_infoServ = malloc(sizeof(struct Json_infoserv));
    struct Json_export *Json_infoExport = NULL;

//    if(argc == 0)
//        return 1;
//    const char *uuid = argv[1];
//    printf("%s\n",uuid);
    
    FILE *todo_File = NULL;
    todo_File = fopen("/Users/fred/Library/Developer/Xcode/DerivedData/Projet-euvctrrwdscuwbhiazntkgfduxwu/Build/Products/Debug/297912f4-3d6a-42b2-971c-3bec43e0581f.json", "r");
    char buffer_todo[10000];
    fread(buffer_todo, 30000, 1, todo_File);
    get_info_serv(Json_infoServ, buffer_todo);
    get_info_export(&Json_infoExport, buffer_todo);
    printf("json : %s\n",buffer_todo);
    printf("info serv (user) : %s\n",json_object_get_string(Json_infoServ->user));
    printf("tableaux 1 : %s\n",json_object_get_string(Json_infoExport[0].array));
    printf("colonnes 1 : %s\n",json_object_get_string(Json_infoExport[0].column[0].column_name));
    
        char chemin[50];
    char *dirc = strdup (chemin);
    char *basec = strdup (chemin);
    char *dname = dirname (dirc);
    char *bname = basename (basec);
    printf ("dirname =%s, basename =%s \n", dname, bname);
    
    
    return 0;
}



void get_info_serv(struct Json_infoserv *Serv_info,char *buffer_todo){
    struct json_object *json_parse_buffer = json_object_new_object();
    struct json_object *json_config = json_object_new_object();
    
    json_parse_buffer = json_tokener_parse(buffer_todo);
    json_config = json_object_array_get_idx(json_parse_buffer, 0);
    
    json_object_object_get_ex(json_config, "pass_root", &Serv_info->pass_root);
    json_object_object_get_ex(json_config, "id_user", &Serv_info->user);
    json_object_object_get_ex(json_config, "pass_user", &Serv_info->pass_user);
    json_object_object_get_ex(json_config, "p_mysql", &Serv_info->pmysql);
    json_object_object_get_ex(json_config, "bdd_type", &Serv_info->bdd_type);
    json_object_object_get_ex(json_config, "name_schema", &Serv_info->name_schema);
    json_object_object_get_ex(json_config, "task", &Serv_info->task);
}

void get_info_export(struct Json_export **Request_Info, char *buffer_todo){
    int i,j;
    
    struct json_object *json_parse_buffer = json_object_new_object();
    struct json_object *json_export = json_object_new_object();
    struct json_object *json_array = json_object_new_object();
    struct json_object *json_array_idx = json_object_new_object();
    struct json_object *json_column = json_object_new_object();
    struct json_object *json_column_idx = json_object_new_object();
    
    json_parse_buffer = json_tokener_parse(buffer_todo);
    json_export = json_object_array_get_idx(json_parse_buffer, 1);
    json_object_object_get_ex(json_export, "request_export", &json_array);
    *Request_Info = malloc(sizeof(struct Json_export) *(int)json_object_array_length(json_array));
    
    for(i =0; i < json_object_array_length(json_array); i++){
        json_array_idx = json_object_array_get_idx(json_array, i);
        
        json_object_object_get_ex(json_array_idx, "array", &Request_Info[i]->array);
        json_object_object_get_ex(json_array_idx, "all_column", &Request_Info[i]->all_column);
        
        json_object_object_get_ex(json_array_idx, "columns", &json_column);
        Request_Info[i]->column = malloc(sizeof(struct Json_export_column) * json_object_array_length(json_column));
        for(j = 0; j < json_object_array_length(json_column); j++){
            json_column_idx = json_object_array_get_idx(json_column, i);
            
            json_object_object_get_ex(json_column_idx, "column name", &Request_Info[i]->column[j].column_name);
            json_object_object_get_ex(json_column_idx, "column type", &Request_Info[i]->column[j].column_type);
            json_object_object_get_ex(json_column_idx, "column key", &Request_Info[i]->column[j].column_key);
        }
    }
    
    
}
