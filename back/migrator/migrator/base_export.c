//
//  base_export.c
//  migrator
//
//  Created by Frédéric FLACELIERE on 30/12/2019.
//  Copyright © 2019 Frédéric FLACELIERE. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "base_export.h"
#include <mysql.h>




int mysql_dump(struct Json_infoserv *info_bdd, struct Json_export *info_export, char *replicator_Dumps){
    int i;
    char name_file[200];
    
    FILE *export_csv_file;
    
    MYSQL mysql;
    MYSQL_RES *result_1;
    MYSQL_FIELD *field_1;
    MYSQL_ROW row_1;
    char sql_select[200];
    char sql_select_end[70];
    int nb_attr_1;
    //fonction export des tables
//    printf("addr meme : %p\n", info_bdd);
//    printf("port : %s\n", json_object_get_string(info_bdd->port));
//    printf("user pass ptr : %p\n", info_bdd->pass_user);
    
    //si les 2 bases sont des bases mysql, pas besoin de faire la méthode 2
    if(!strcmp(json_object_get_string(info_bdd->bdd_type_source), json_object_get_string(info_bdd->bdd_type_target))){
        mysqlVmysql_export(info_bdd, info_export, replicator_Dumps);
    }
    else{
        mysql_init(&mysql);
        mysql_options(&mysql,MYSQL_READ_DEFAULT_GROUP,"option");
        mysql_options(&mysql, MYSQL_SET_CHARSET_NAME, "utf8");
        mysql_options(&mysql, MYSQL_INIT_COMMAND, "SET NAMES utf8");
        

        if(mysql_real_connect(&mysql, json_object_get_string(info_bdd->ip_source), json_object_get_string(info_bdd->user), json_object_get_string(info_bdd->pass_user), json_object_get_string(info_bdd->name_schema_source),atoi(json_object_get_string(info_bdd->port)),NULL,0)){
            for (i = 0; i < info_export->nbr_arrays; i++) {
                sprintf(name_file,"%s%s_data_%s.csv", replicator_Dumps, info_bdd->uuid, json_object_get_string(info_export[i].array));
                export_csv_file = fopen(name_file, "wb");
                
                sprintf(sql_select, "SELECT ");
                for(int g = 0; g < info_export->column->nbr_columns; g++){
                    if(g != 0)
                        strcat(sql_select, ",");
                    strcat(sql_select, json_object_get_string(info_export->column[g].column_name));
                }
                sprintf(sql_select_end, " from %s.%s",json_object_get_string(info_bdd->name_schema_source), json_object_get_string(info_export[i].array));
                strcat(sql_select, sql_select_end);

                mysql_query(&mysql, sql_select);
                result_1 = mysql_store_result(&mysql);

                nb_attr_1 = mysql_num_fields(result_1);

                for (i = 0; i < mysql_num_fields (result_1); i++){//nom des colonnes
                  field_1 = mysql_fetch_field (result_1);
                    if(i != 0)
                        fprintf(export_csv_file, "|");
                    fprintf(export_csv_file, "%s", field_1->name);
                }
                fprintf(export_csv_file, "\n");
                while ((row_1 = mysql_fetch_row(result_1)))
                {
                    mysql_field_seek (result_1, 0);
                    for (i = 0; i < mysql_num_fields (result_1); i++){
                      field_1 = mysql_fetch_field (result_1);
        //              printf ("name : %s --> %s\n", field_1->name, row_1[i]);//nom de la colonne --> contenu de la colonne
                        if(i != 0)
                            fprintf(export_csv_file, "|");
                        fprintf(export_csv_file, "%s",row_1[i]);
//                        printf("%s",row_1[i]);
                    }
                    fprintf(export_csv_file,"\n");
                }
                fclose(export_csv_file);
                mysql_free_result(result_1);
            }
             mysql_close(&mysql);
        }
    }
    
    return 0;
}
int mysqlVmysql_export(struct Json_infoserv *info_bdd, struct Json_export *info_export, char *replicator_Dumps){
    int i;
    char name_file[200];
//    FILE *replicator_Dumps_file;
    char requete_dump[500];
    sprintf(requete_dump, "mysqldump - h %s -P %s -u %s -p'%s' %s ", json_object_get_string(info_bdd->ip_source), json_object_get_string(info_bdd->port), json_object_get_string(info_bdd->user), json_object_get_string(info_bdd->pass_user), json_object_get_string(info_bdd->name_schema_source));
    for(i = 0; i < info_export->nbr_arrays; i++){
        if(i > 0)
            strcat(requete_dump, " ");
        strcat(requete_dump, json_object_get_string(info_export[i].array));
        
    }
    strcat(requete_dump, " > ");
    //crétion des noms de fichier
    sprintf(name_file,"%s%s_dump.sql", replicator_Dumps, info_bdd->uuid);
    strcat(requete_dump, name_file);
    system(requete_dump);
    printf("requete d'export mysql (vers mysql) : %s\n", requete_dump);
    return 0;
}

int export_table_mysqlsql(struct Json_infoserv *info_bdd, struct Json_export *info_export, char *replicator_Dumps){
    int i;
    char name_file[200];
//    FILE *replicator_Dumps_file;
    char requete_dump[500];
    for(i = 0; i < info_export->nbr_arrays; i++){
        strcpy(requete_dump, "mysqldump");
        strcat(requete_dump, " -h ");
        strcat(requete_dump, json_object_get_string(info_bdd->ip_source));
        strcat(requete_dump, " -P ");
        strcat(requete_dump, json_object_get_string(info_bdd->port));
        strcat(requete_dump, " -u ");
        strcat(requete_dump, json_object_get_string(info_bdd->user));
        strcat(requete_dump, " -p ");
        strcat(requete_dump, json_object_get_string(info_bdd->pass_user));
        strcat(requete_dump, " --no-data ");
        strcat(requete_dump, json_object_get_string(info_bdd->name_schema_source));
        strcat(requete_dump, " ");
        strcat(requete_dump, json_object_get_string(info_export[i].array));
        strcat(requete_dump, " > ");
        //crétion des noms de fichier
        strcpy(name_file, replicator_Dumps);
        strcat(name_file, info_bdd->uuid);
        strcat(name_file, "_create_");
        strcat(name_file, json_object_get_string(info_export[i].array));
        strcat(name_file, ".sql");
        strcat(requete_dump, name_file);
        printf("requete d'export mysql : %s\n", requete_dump);
//        system(replicator_Dumps);
    }
    return 0;
}
int export_table_mysqlcsv(struct Json_infoserv *info_bdd, struct Json_export *info_export, char *replicator_Dumps){
    int i;
    char name_file[200];
//    FILE *replicator_Dumps_file;
    char requete_dump[1000];
    printf("port : %s\n", json_object_get_string(info_bdd->port));
    for(i = 0; i < info_export->nbr_arrays; i++){
//        strcpy(requete_dump, "mysqldump");
//        strcat(requete_dump, " -h ");
//        strcat(requete_dump, json_object_get_string(info_bdd->ip_source));
//        strcat(requete_dump, " -P ");
//        strcat(requete_dump, json_object_get_string(info_bdd->port));
//        strcat(requete_dump, " -u ");
//        strcat(requete_dump, json_object_get_string(info_bdd->user));
//        strcat(requete_dump, " -p ");
//        strcat(requete_dump, json_object_get_string(info_bdd->pass_user));
//        strcat(requete_dump, " --no-data ");
//        strcat(requete_dump, json_object_get_string(info_bdd->name_schema_source));
//        strcat(requete_dump, " ");
//        strcat(requete_dump, json_object_get_string(info_export[i].array));
//        strcat(requete_dump, " > ");
//        //crétion des noms de fichier
//        strcpy(name_file, replicator_Dumps);
//        strcat(name_file, info_bdd->uuid);
//        strcat(name_file, "_create_");
//        strcat(name_file, json_object_get_string(info_export[i].array));
//        strcat(name_file, ".sql");
//        strcat(requete_dump, name_file);
//        printf("requete d'export mysql : %s\n", requete_dump);
////        system(replicator_Dumps);
    }
    return 0;
}
int export_table_mysqlcmd(struct Json_infoserv *Json_infoserv, struct Json_export *Json_export, char *replicator_Dumps){
    return 1;
}


int oracle_dump(void){
    return 0;
}




int vertica_dump(struct Json_infoserv *info_bdd, struct Json_export *info_export, char *replicator_Dumps){

    
    //si les 2 bases sont des bases mysql, pas besoin de faire la méthode 2
    if(!strcmp(json_object_get_string(info_bdd->bdd_type_source), json_object_get_string(info_bdd->bdd_type_target))){
        verticaVvertica_export(info_bdd, info_export, replicator_Dumps);
    }
    else{
    //    int i;
        //fonction export des tables
        printf("addr meme : %p\n", info_bdd);
        printf("port : %s\n", json_object_get_string(info_bdd->port));
        printf("user pass ptr : %p\n", info_bdd->pass_user);
        int requete_dumpsql = export_table_verticasql(info_bdd, info_export, replicator_Dumps);
        int requete_dumpcsv = export_table_verticacsv(info_bdd, info_export, replicator_Dumps);
    //    int requete_dumpcmd = export_table_verticacmd(info_bdd, info_export, replicator_Dumps);
        
        if(requete_dumpsql)
            printf("erreur lors de lgénération d'une table (sql)\n");
        if(requete_dumpcsv)
            printf("erreur lors de lgénération d'une table (csv)\n");
    //    if(requete_dumpcmd)
    //    printf("erreur lors de lgénération d'une table (csv)\n");
        
    //    system(requete_dump);
        printf("url de dump csv: %d\n", requete_dumpsql);
    }
    return 0;

}

int verticaVvertica_export(struct Json_infoserv *info_bdd, struct Json_export *info_export, char *replicator_Dumps){
// creation de la table
//     vsql -h debian10-2 -p 5433 -U dbadmin -d P1ODSGL1 -w admin -c "select export_tables('', 'R1ODSGL1.tabvilles');" -A -P footer=off -t -o uuid_create_tabvilles.sql_tmp
    int i;
    char name_file[200];
    char name_file_dump_temp[200];
//    FILE *replicator_Dumps_file;
    char requete_dump[500];
//    char requete_dump_temp[50];
    sprintf(requete_dump, "vsql -h %s -p %s -U %s -d %s -w %s -c \"SELECT export_tables('',", json_object_get_string(info_bdd->ip_source), json_object_get_string(info_bdd->port), json_object_get_string(info_bdd->user), /*-d*/json_object_get_string(info_bdd->name_db_source), json_object_get_string(info_bdd->pass_user));
    for(i = 0; i < info_export->nbr_arrays; i++){
        if(i > 0)
            strcat(requete_dump, ", ");
        char temp[300];
        sprintf(temp,"'%s.%s'", json_object_get_string(info_bdd->name_schema_source), json_object_get_string(info_export[i].array));
        strcat(requete_dump, temp);
    }
    strcat(requete_dump, ")\" -A -P footer=off -t -o ");
    
    //crétion des noms de fichier
    sprintf(name_file,"%s%s_dump.sql", replicator_Dumps, info_bdd->uuid);
    sprintf(name_file_dump_temp,"%s%s_dump_tmp.sql", replicator_Dumps, info_bdd->uuid);
    strcat(requete_dump, name_file_dump_temp);
    system(requete_dump);
    printf("requete d'export vertica (vers vertica) : %s\n", requete_dump);
    
// modifier le schema R1ODSGL1 par DBFRED
//     sed  's/R1ODSGL1/DBFRED/' uuid_dump_tmp.sql >uuid_dump.sql
    sprintf(requete_dump, "sed 's/%s/%s/' %s > %s",json_object_get_string(info_bdd->name_schema_source), json_object_get_string(info_bdd->name_schema_target), name_file_dump_temp, name_file);
    system(requete_dump);
//     rm -f uuid_create_tabvilles.sql_tmp
    remove(name_file_dump_temp);
    
//    vsql -h debian10-2 -p 5433 -U dbadmin -d P1odsgl1 -w admin -c "SELECT * FROM r1odsgl1.tabvilles" -o uuid_data_tabvilles.csv -A -P footer=off  // dump des datas

    for(i = 0; i < info_export->nbr_arrays ; i++){
            sprintf(name_file,"%s%s_data_%s.csv", replicator_Dumps, info_bdd->uuid, json_object_get_string(info_export[i].array));
            sprintf(requete_dump, "vsql -h %s -p %s -U %s -d %s -w %s -c \"SELECT * FROM %s.%s\" -o %s -A -P footer=off " , json_object_get_string(info_bdd->ip_source), json_object_get_string(info_bdd->port), json_object_get_string(info_bdd->user), json_object_get_string(info_bdd->name_db_source), json_object_get_string(info_bdd->pass_user), json_object_get_string(info_bdd->name_schema_source), json_object_get_string(info_export[i].array), name_file);
           system(requete_dump);
       }
    
    return 0;
}


int export_table_verticasql(struct Json_infoserv *info_bdd, struct Json_export *info_export, char *replicator_Dumps){
    int i;
    char name_file[200];
//    FILE *replicator_Dumps_file;
    char requete_dump[500];
    for(i = 0; i < info_export->nbr_arrays; i++){
        strcpy(requete_dump, "vsql");
        strcat(requete_dump, " -h ");
        strcat(requete_dump, json_object_get_string(info_bdd->ip_source));
        strcat(requete_dump, " -p ");
        strcat(requete_dump, json_object_get_string(info_bdd->port));
        strcat(requete_dump, " -U ");//login
        strcat(requete_dump, json_object_get_string(info_bdd->user));
        strcat(requete_dump, " -d ");//nom de la base
        strcat(requete_dump, json_object_get_string(info_bdd->name_db_source));
        strcat(requete_dump, " -w ");//mdp
        strcat(requete_dump, json_object_get_string(info_bdd->pass_user));
        strcat(requete_dump, " -c \"select export_table('', '");
        strcat(requete_dump, json_object_get_string(info_bdd->name_schema_source));
        strcat(requete_dump, ".");
        strcat(requete_dump, json_object_get_string(info_export[i].array));
        strcat(requete_dump, "');\" -A -P footer=off -t -o ");
        
        //crétion des noms de fichier
        strcpy(name_file, replicator_Dumps);
        strcat(name_file, info_bdd->uuid);
        strcat(name_file, "_create_");
        strcat(name_file, json_object_get_string(info_export[i].array));
        strcat(name_file, ".sql");
        strcat(requete_dump, name_file);
    
        printf("requete d'export vertica (sql) : %s\n", requete_dump);
//        system(replicator_Dumps);
    }
    return 0;
}
int export_table_verticacsv(struct Json_infoserv *info_bdd, struct Json_export *info_export, char *replicator_Dumps){
    int i,j;
    char name_file[200];
    char name_fileCmd[200];
    //    FILE *replicator_Dumps_file;
    char requete_dump[500];
    for(i = 0; i < info_export->nbr_arrays; i++){
        strcpy(requete_dump, "vsql");
        strcat(requete_dump, " -h ");
        strcat(requete_dump, json_object_get_string(info_bdd->ip_source));
        strcat(requete_dump, " -p ");
        strcat(requete_dump, json_object_get_string(info_bdd->port));
        strcat(requete_dump, " -U ");//login
        strcat(requete_dump, json_object_get_string(info_bdd->user));
        strcat(requete_dump, " -d ");//nom de la base
        strcat(requete_dump, json_object_get_string(info_bdd->name_db_source));
        strcat(requete_dump, " -w ");//mdp
        strcat(requete_dump, json_object_get_string(info_bdd->pass_user));
        strcat(requete_dump, " -c \"SELECT ");
        for(j = 0; j < info_export->column->nbr_columns; i++){
            strcat(requete_dump, json_object_get_string(info_export->column[i].column_name));
            strcat(requete_dump, ",");
        }
        strcat(requete_dump, "FROM");
        strcat(requete_dump, json_object_get_string(info_bdd->name_schema_source));
        strcat(requete_dump, ".");
        strcat(requete_dump, json_object_get_string(info_export[i].array));
        strcat(requete_dump, "\" -o ");
        
        //crétion des noms de fichier
        strcpy(name_file, replicator_Dumps);
        strcat(name_file, info_bdd->uuid);
        strcat(name_file, "_");
        strcpy(name_fileCmd, name_file);//ficher pour remonter la base 1/2
        strcat(name_fileCmd, "_cmd.sql");//ficher pour remonter la base 2/2
        strcat(name_file, json_object_get_string(info_export[i].array));
        strcat(name_file, ".csv");
        strcat(requete_dump, name_file);
        
        //fichier de conf pour remonter la base
        if(export_table_verticacmd(json_object_get_string(info_bdd->bdd_type_target), &info_export[i], name_file))
            printf("erreur lors de la création du fichier de conf\n");
        printf("requete d'export vertica (csv) : %s\n", requete_dump);
//        system(replicator_Dumps);
    }
    return 0;
}
int export_table_verticacmd(const char *name_bdd, struct Json_export *info_export, char *name_file){
    int j;
//    FILE *replicator_Dumps_file;
//    replicator_Dumps_file = fopen(name_file, "ab");
    char requete_dump[500];
    //truncate
    strcpy(requete_dump, "truncate table ");
    strcpy(requete_dump, json_object_get_string(info_export->array));
    strcpy(requete_dump, ";\n");
    
    //copy
    strcpy(requete_dump, "COPY ");
    strcat(requete_dump, name_bdd);
    strcpy(requete_dump, ".");
    strcpy(requete_dump, json_object_get_string(info_export->array));
    strcpy(requete_dump, "(");
 
    for(j = 0; j < info_export->column->nbr_columns; j++){
        strcat(requete_dump, json_object_get_string(info_export->column[j].column_name));
        strcat(requete_dump, ",");
    }
    strcat(requete_dump, ") from LOCAL '");//possiblité de remplacer LOCAL par une ip
    strcpy(requete_dump, name_file);
    strcat(requete_dump, "parser fdelimitedparser(header=true);\n");

    printf("requete d'export vertica (cmd) : %s\n", requete_dump);
//    fprintf(replicator_Dumps_file, "%s", requete_dump);
//    fclose(replicator_Dumps_file);
        
    return 0;
}



