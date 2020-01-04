//
//  main.c
//  migrator
//
//  Created by Frédéric FLACELIERE on 27/12/2019.
//  Copyright © 2019 Frédéric FLACELIERE. All rights reserved.
//  Programme permettant l'import/export des bases de données stockées sur les serveurs

#include <stdio.h>
#include <string.h>
#include <libgen.h>
#include <time.h>
#include "base_export.h"
#include "test_connection.h"


void get_info_serv(struct Json_infoserv *, char *);
void get_info_export(struct Json_export **, char *, struct Json_infoserv *);
void repertoire_home(const char *prog);

void error_parmtr(char tmp_err[]/*, char file_StdErr[]*/);
void usage(char *);
int parameter(int, const char**, struct Json_infoserv *, struct Bdd_Test *);

int file_dir(const char *, struct Json_infoserv *);
int file_dir_ABS(const char *);
int file_dump_ABS(const char *);
int file_logs_ABS(const char *);
int check_parameter(const char *);
void error_ask(const char*);


//char *getenvstr( char *name ){
//    static char ptr[1001];
//
//    *ptr = 0;
//    if (getenv( name ))
//        strcpy(ptr,getenv( name ));
//
//        return ptr ? ptr : 0;
//}
//variable utilisée dans quasiment toutes les fonctions
char *replicator_Home;
char *replicator_Todo;
char *replicator_Dumps;
char *replicator_logs;

struct Json_export *Json_infoExport;

int main(int argc, const char * argv[]) {
    struct Json_infoserv *Json_infoServ = malloc(sizeof(struct Json_infoserv));
//    struct Json_export *Json_infoExport = NULL; //bug
    struct Bdd_Test *Bdd_Test1 = malloc(sizeof(struct Bdd_Test));
    
    if (argc <= 1) //si il n'y a aucun parametre
        error_parmtr("Erreur lors de la saisie des paramètres. \"-?\" pour l'usage");
    
    Bdd_Test1->status = 0;
    if(parameter(argc, argv, Json_infoServ, Bdd_Test1))
        return 1;

    if(Bdd_Test1->status)
        get_type_base_test(Bdd_Test1);//fonction de test de connexion bdd
//    printf("argv[0] = %s\n",argv[0]); //chemin de l'appli
    
    //ouverture du fichier de conf à son emplacement indiqué
    FILE *todo_File = NULL;
    if((todo_File = fopen(replicator_Todo, "r")) == NULL)
        error_parmtr("Aucun paramètre indiquant un fichier de conf détecté...\nFin du programme\n");
    
    
    char buffer_todo[30000];
    fread(buffer_todo, 30000, 1, todo_File);
    if(strlen(buffer_todo) ==30000)
        error_parmtr("Fichier trop volumineux... Veuillez en choisir un autre\n");
    
    get_info_serv(Json_infoServ, buffer_todo);
    
//    printf("user pass ptr : %p\n", Json_infoExport->array);
    get_info_export(&Json_infoExport, buffer_todo, Json_infoServ);
//    printf("\ntableau 1 -- : %s\n", json_object_get_string(Json_infoExport[0].array));
//    printf("tableau 2 -- : %s\n", json_object_get_string(Json_infoExport[1].array));
//    printf("tableau 1 ptr -- : %p\n", Json_infoExport[0].array);
//    printf("tableau 2 ptr -- : %p\n", Json_infoExport[1].array);
    
    //pour rediriger sur la bonne tache avec la bonne base
    if(!strcmp(json_object_get_string(Json_infoServ->task), "export")){
        if(!strcmp(json_object_get_string(Json_infoServ->bdd_type_source), "mysql")){
            printf("export de type mysql\n");
            if(mysql_dump(Json_infoServ, Json_infoExport, replicator_Dumps)){
                error_parmtr("erreur lors du dump...\n");
            }
        }
        else
            if(!strcmp(json_object_get_string(Json_infoServ->bdd_type_source), "vertica")){
                if(vertica_dump(Json_infoServ, Json_infoExport, replicator_Dumps)){
                    error_parmtr("erreur lors du dump...\n");
                }
            }
            else
                if(!strcmp(json_object_get_string(Json_infoServ->bdd_type_source), "oracle")){
                    if(oracle_dump())
                        error_parmtr("erreur lors du dump...\n");
                }
                else error_parmtr("Type de base non prise en comtpe\n");
    }
    else{
        if(!strcmp(json_object_get_string(Json_infoServ->bdd_type_target), "mysql")){
            if(!strcmp(json_object_get_string(Json_infoServ->bdd_type_source), json_object_get_string(Json_infoServ->bdd_type_target))){
                mysqlVmysql_import(Json_infoServ, Json_infoExport, replicator_Dumps);
            }
            else{
                if(mysql_import(Json_infoServ, Json_infoExport, replicator_Dumps))
                    error_parmtr("erreur lors de l'import...\n");
            }
        }else
            if(!strcmp(json_object_get_string(Json_infoServ->bdd_type_target), "vertica")){
                if(!strcmp(json_object_get_string(Json_infoServ->bdd_type_source), json_object_get_string(Json_infoServ->bdd_type_target))){
                    verticaVvertica_import(Json_infoServ, Json_infoExport, replicator_Dumps);
                }
                else //mysqlVvertica_import
                    if(!strcmp(json_object_get_string(Json_infoServ->bdd_type_source), "mysql")){
                        mysqlVvertica_import(Json_infoServ, Json_infoExport, replicator_Dumps);
                    }
            }else
                if(!strcmp(json_object_get_string(Json_infoServ->bdd_type_source), "oracle")){
                    if(oracle_import())
                        error_parmtr("erreur de l'import...\n");
                }
                else error_parmtr("Type de base non prise en charge\n");
    }
//    printf("json : %s\n",buffer_todo);
//    printf("info serv (user) : %s\n",json_object_get_string(Json_infoServ->user));
//    printf("tableaux 1 : %s\n",json_object_get_string(Json_infoExport[0].array));
//    printf("colonnes 1 : %s\n",json_object_get_string(Json_infoExport[0].column[0].column_name));
    

//    name_exec(argv[0]);
    
    return 0;
}



void get_info_serv(struct Json_infoserv *Serv_info,char *buffer_todo){
    struct json_object *json_parse_buffer = json_object_new_object();
    struct json_object *json_config = json_object_new_object();
    
    json_parse_buffer = json_tokener_parse(buffer_todo);
    json_config = json_object_array_get_idx(json_parse_buffer, 0);
    
    json_object_object_get_ex(json_config, "ip_source", &Serv_info->ip_source);
    json_object_object_get_ex(json_config, "ip_target", &Serv_info->ip_target);
    json_object_object_get_ex(json_config, "pass_root", &Serv_info->pass_root);
    json_object_object_get_ex(json_config, "id_user", &Serv_info->user);
    json_object_object_get_ex(json_config, "pass_user", &Serv_info->pass_user);
    json_object_object_get_ex(json_config, "port", &Serv_info->port);
    json_object_object_get_ex(json_config, "bdd_type_source", &Serv_info->bdd_type_source);
    json_object_object_get_ex(json_config, "bdd_type_target", &Serv_info->bdd_type_target);
    json_object_object_get_ex(json_config, "name_schema_source", &Serv_info->name_schema_source);
    json_object_object_get_ex(json_config, "name_schema_target", &Serv_info->name_schema_target);
    json_object_object_get_ex(json_config, "name_db_target", &Serv_info->name_db_target);
    json_object_object_get_ex(json_config, "name_db_source", &Serv_info->name_db_source);
    json_object_object_get_ex(json_config, "task", &Serv_info->task);
//    printf("user fonction get json : %s\n", json_object_get_string(Serv_info->user));
}

void get_info_export(struct Json_export **Request_Info, char *buffer_todo, struct Json_infoserv *Json_infoServ){
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
    //impossible de boucler sur le nombre de tableau sans l'avoir stocké à l'avance
    Json_infoExport = malloc(sizeof(struct Json_export) *(int)json_object_array_length(json_array));//récupérer le nombre de tableaux
//    *Request_Info = malloc(sizeof(struct Json_export) *100);//récupérer le nombre de tableaux
//    printf("nombre de tableau : %lu\n", json_object_array_length(json_array));
    Json_infoExport[0].nbr_arrays = json_object_array_length(json_array);
    
    for(i =0; i < json_object_array_length(json_array); i++){
        json_array_idx = json_object_array_get_idx(json_array, i);
        
        json_object_object_get_ex(json_array_idx, "array", &Json_infoExport[i].array);
        json_object_object_get_ex(json_array_idx, "all_column", &Json_infoExport[i].all_column);
        
        json_object_object_get_ex(json_array_idx, "columns", &json_column);
        Json_infoExport[i].column = malloc(sizeof(struct Json_export_column) * json_object_array_length(json_column));
        Json_infoExport[i].column->nbr_columns = json_object_array_length(json_column);//récupérer le nombre de colonnes dans tableaux
        for(j = 0; j < json_object_array_length(json_column); j++){
            json_column_idx = json_object_array_get_idx(json_column, j);
            
            json_object_object_get_ex(json_column_idx, "column name", &Json_infoExport[i].column[j].column_name);
            json_object_object_get_ex(json_column_idx, "column type", &Json_infoExport[i].column[j].column_type);
            json_object_object_get_ex(json_column_idx, "column key", &Json_infoExport[i].column[j].column_key);
        }
//        printf("Tableau %d lors de la création : %s\n", i, json_object_get_string(Json_infoExport[i].array));
    }
//    printf("tableau fin fonction -- : %s\n", json_object_get_string(Json_infoExport[1].array));
//    printf("tableau 1 ptr fonction -- : %p\n", Json_infoExport[0].array);
//    printf("tableau 2 ptr fonction -- : %p\n", Json_infoExport[1].array);
}




void error_parmtr(char error[]){//affiche une erreur, l'ajoute dans un fichier de log puis exit
    if(replicator_logs != NULL){
        time_t actual_Time = time(NULL); /*contient maintenant la date et l'heure courante */
        char buffer_time[256];
        strftime(buffer_time, sizeof(buffer_time), "%a %c", localtime(&actual_Time));//Equivalent de sprintf mais pour l'heure

        FILE *file_StdErr;
        file_StdErr = fopen(replicator_logs, "ab");
        fprintf(file_StdErr,"[%s] %s\n", buffer_time, error);
    //    fflush(file_StdErr);
        fclose(file_StdErr);
    }
    printf(" %s\n",error);
//    fflush(stdout);

//    usage("NULL");
    exit(0);
}


int parameter(int nr, const char **parameters, struct Json_infoserv *Json_infoserv, struct Bdd_Test *Bdd_Test){//verif pour ne pas depasser la taille des inputs (envisagé des mallocs), verif des paramètres
    int i, g, status = 0;
    for(i = 1; i < nr ; i++){
        status = 0;
        switch (parameters[i][1]) {
            case '?':
                usage("?");
            break;
            case 'f':
                if(i+1 >= nr || check_parameter(parameters[i+1])){
                    error_ask(parameters[i]);
//                    status = 1;
                    break;
                }
                if(file_dir(parameters[i+1], Json_infoserv)){
                    printf("problème d'allocation\n");
                    return 1;
                }
                status = 1;
            break;
            case 'F':
                if(i+1 >= nr || check_parameter(parameters[i+1])){
                    error_ask(parameters[i]);
//                    status = 1;
                    break;
                }
                if(file_dir_ABS(parameters[i+1])){
                    printf("problème d'allocation\n");
                    return 1;
                }
                status = 1;
            break;
            case 'D':
                if(i+1 >= nr || check_parameter(parameters[i+1])){
                    error_ask(parameters[i]);
//                    status = 1;
                    break;
                }
                if(file_dump_ABS(parameters[i+1])){
                    printf("problème d'allocation\n");
                    return 1;
                }
                status = 1;
            break;
            case 'L':
                if(i+1 >= nr || check_parameter(parameters[i+1])){
                    error_ask(parameters[i]);
//                    status = 1;
                    break;
                }
                if(file_logs_ABS(parameters[i+1])){
                    printf("problème d'allocation\n");
                    return 1;
                }
                status = 1;
            break;
            case 'c'://parametre nécessitant pas d'argument
//                if(i+1 >= nr || check_parameter(parameters[i+1])){
//                    error_ask(parameters[i]);
//                    status = 1;
//                    break;
//                }
                usage("c");
            break;
            case 'd'://parametre nécessitant pas d'argument
//                if(i+1 >= nr || check_parameter(parameters[i+1])){
//                    error_ask(parameters[i]);
//                    status = 1;
//                    break;
//                }
                usage("d");
            break;
            
            //partie test de connexion à une bdd
            case 'n':
                if(i+1 >= nr || check_parameter(parameters[i+1])){
                    error_ask(parameters[i]);
//                    status = 1;
                    break;
                }
                strcpy(Bdd_Test->name_Schema, parameters[i+1]);
                Bdd_Test->status = 1;
                status = 1;
            break;
            case 'p'://parametre nécessitant pas d'argument
                if(i+1 >= nr || check_parameter(parameters[i+1])){
                    error_ask(parameters[i]);
//                    status = 1;
                    break;
                }
                Bdd_Test->port = atoi(parameters[i+1]);
                Bdd_Test->status = 1;
                status = 1;
            break;
            case 't'://parametre nécessitant pas d'argument
                if(i+1 >= nr || check_parameter(parameters[i+1])){
                    error_ask(parameters[i]);
//                    status = 1;
                    break;
                }
                strcpy(Bdd_Test->bdd_type, parameters[i+1]);
                Bdd_Test->status = 1;
                status = 1;
            break;
            case 'b'://parametre nécessitant pas d'argument
                if(i+1 >= nr || check_parameter(parameters[i+1])){
                    error_ask(parameters[i]);
//                    status = 1;
                    break;
                }
                strcpy(Bdd_Test->name_Base, parameters[i+1]);
                Bdd_Test->status = 1;
                status = 1;
            break;
            case 'U'://parametre nécessitant pas d'argument
                if(i+1 >= nr || check_parameter(parameters[i+1])){
                    error_ask(parameters[i]);
//                    status = 1;
                    break;
                }
                strcpy(Bdd_Test->user, parameters[i+1]);
                Bdd_Test->status = 1;
                status = 1;
            break;
            case 'P'://parametre nécessitant pas d'argument
                if(i+1 >= nr || check_parameter(parameters[i+1])){
                    error_ask(parameters[i]);
//                    status = 1;
                    break;
                }
                strcpy(Bdd_Test->pass, parameters[i+1]);
                Bdd_Test->status = 1;
                status = 1;
            break;
            case 'I'://parametre nécessitant pas d'argument
                if(i+1 >= nr || check_parameter(parameters[i+1])){
                    error_ask(parameters[i]);
                //                    status = 1;
                    break;
                }
                strcpy(Bdd_Test->ip, parameters[i+1]);
                Bdd_Test->status = 1;
                status = 1;
            break;
            case 'C'://parametre nécessitant pas d'argument
                if(i+1 >= nr || check_parameter(parameters[i+1])){
                    error_ask(parameters[i]);
                //                    status = 1;
                    break;
                }
//                printf("|%s|\n",parameters[i+1]);
                strcpy(Bdd_Test->request, parameters[i+1]);
                Bdd_Test->status = 1;
                status = 1;
            break;
                
            default:
                printf("Paramètre %d non interprêté (%s)\n", i, parameters[i]);
        }
        if(status)
            i++;//après le -'' se trouve un parametre
    }
    return 0;
}

int file_dir(const char *parameters, struct Json_infoserv *Json_infoserv){
//    if(strlen(parameters) >= 50) //uniquement la taille tu fichier
//        return 1;
    replicator_Home = malloc(sizeof(char) * 45 + strlen(getenv("REPLICATOR_HOME")));
    if(replicator_Home == NULL)
        return 1;
    strncpy(Json_infoserv->uuid, parameters, strlen(parameters)-5);
    Json_infoserv->uuid[strlen(parameters)-5] = '\0';
//    printf("uuid : %s\n", Json_infoserv->uuid);
//    printf("get_env : %s",getenv("REPLICATOR_HOME"));
    strcpy(replicator_Home, getenv("REPLICATOR_HOME"));
//    printf("\npath du home : %s\n", replicator_Home);
    
    replicator_Todo = malloc(sizeof(char) * (50 + strlen(replicator_Home)));
    if(replicator_Home == NULL)
        return 1;
    strcpy(replicator_Todo, replicator_Home);
    strcat(replicator_Todo, "/todo/");
    strcat(replicator_Todo, parameters);
    
    replicator_Dumps = malloc(sizeof(replicator_Home) * (50 + strlen(replicator_Home)));
    if(replicator_Home == NULL)
        return 1;
    strcpy(replicator_Dumps, replicator_Home);
    strcat(replicator_Dumps, "/dumps/");
//    strcat(replicator_Dumps, parameters);

    replicator_logs = malloc(sizeof(replicator_Home) * (50 + strlen(replicator_Home)));
    if(replicator_Home == NULL)
        return 1;
    strcpy(replicator_logs, replicator_Home);
    strcat(replicator_logs, "/logs/");
    strcat(replicator_logs, "error_replicator.log");
    
    return 0;
}

int file_dir_ABS(const char *parameters){
    if(strlen(parameters) >= 200)//taille total (chemin + nom du fichier)
        return 1;
    replicator_Todo = malloc(sizeof(char) * (strlen(parameters) + 1));
    strcpy(replicator_Todo, parameters);
    return 0;
}
int file_dump_ABS(const char *parameters){
    if(strlen(parameters) >= 200)//taille total (chemin + nom du fichier)
        return 1;
    replicator_Dumps = malloc(sizeof(char) * (strlen(parameters) + 1));
    if(replicator_Dumps == NULL)
        return 1;
    strcpy(replicator_Dumps, parameters);
    return 0;
}
int file_logs_ABS(const char *parameters){
    if(strlen(parameters) >= 200)//taille total (chemin + nom du fichier)
        return 1;
    replicator_logs = malloc(sizeof(char) * (strlen(parameters) + 1));
    if(replicator_logs == NULL)
        return 1;
    strcpy(replicator_logs, parameters);
    return 0;
}

int check_parameter(const char * check){
    if(check[0] == '-')
        return 1;
    return 0;
}
void error_ask(const char*ask){
    printf("erreur lors de la saisie d'un paramètre %s... (paramètre ignoré)\n",ask);
}


void usage(char *usage){
    switch (usage[0]) {
        case 'c':
            printf("dossier standard des fichiers de conf : %s/todo\n", getenv("REPLICATOR_HOME"));
        break;
        case 'd':
            printf("dossier standard des dumps : %s/dump\n", getenv("REPLICATOR_HOME"));
        break;
            
        default:
            printf( "Usage\n");
            printf( "   -? help \n");
            printf( "   -f nom du fichier\n");
            printf( "   [-F] donner un chemin absolue vers le dossier \"todo\"\n");
            printf( "   [-D] donner un chemin absolue pour la sortie du dump\n");
            printf( "   [-L] donner un chemin absolue pour la sortie des logs\n");
            printf( "   [-c] indique le dossier standard dans lequel doit se trouver les fichier de conf (%s/todo)\n", getenv("REPLICATOR_HOME"));
            printf( "   [-d] indique le dossier standard dans lequel doit se trouver les fichiers de dump (%s/dump)\n", getenv("REPLICATOR_HOME"));
            
            printf("\ntester la connection à une bdd (vertica, mysql, oracle)\n");
            printf( "   [-I] IP de la bdd \n");
            printf( "   [-n] nom du schéma \n");
            printf( "   [-p] port de la bdd \n");
            printf( "   [-t] type de bdd \n");
            printf( "   [-b] nom de la base \n");
            printf( "   [-U] user \n");
            printf( "   [-P] pass \n");
            printf( "   [-C] le select à effectuer\n");
    }
}
