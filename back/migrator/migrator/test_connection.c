//
//  test_connection.c
//  migrator
//
//  Created by Frédéric FLACELIERE on 03/01/2020.
//  Copyright © 2020 Frédéric FLACELIERE. All rights reserved.
//

#include "test_connection.h"
//#include <stdlib.h>
#include <mysql.h>
#include <string.h>

void get_type_base_test(struct Bdd_Test *Data){
    if(!strcmp(Data->bdd_type, "mysql")){
        if(strcmp(Data->bdd_type, "") && strcmp(Data->ip, "") /*&& strcmp(Data->name_Base, NULL)*/ && strcmp(Data->name_Schema, "") && strcmp(Data->pass, "") && /*Data->port == 0 &&*/ strcmp(Data->user, "")){
            connection_bdd(Data);
        }
        else
            printf("erreur de connexion\n");
    }
    else
        printf("erreur : Il manque des paramètres pour tester la connexion à la bdd\n");
}


int connection_bdd(struct Bdd_Test *Data){
    MYSQL mysql;
    mysql_init(&mysql);
    
    int i;
    mysql_options(&mysql,MYSQL_READ_DEFAULT_GROUP,"option");
    mysql_options(&mysql, MYSQL_SET_CHARSET_NAME, "utf8");
    mysql_options(&mysql, MYSQL_INIT_COMMAND, "SET NAMES utf8");
//    printf("%d\n",Data->returnStatusConnexion);

    printf("saisie des infos :\n ip : '%s' \nport : '%d'\n user : '%s'\n pass :'%s'\n name schema : '%s'\n",Data->ip, Data->port, Data->user, Data->pass, Data->name_Schema);
    
    if(mysql_real_connect(&mysql, Data->ip, Data->user, Data->pass,Data->name_Schema,Data->port,NULL,0))
    {
        printf("Une bdd a bien été trouvé et la connexion s'est bien effectué\n");
        
        //--------------
        MYSQL_RES *result_1;
        MYSQL_FIELD *field_1;
        MYSQL_ROW row_1;
        char sql_select[100];
        int nb_attr_1;

        if(!strcmp(Data->request, "")){
            printf("Il manque la requete..\n");
            return 1;
        }
        strcpy(sql_select, Data->request);//"SELECT * from dbhuissier.tabvilles\0"

        mysql_query(&mysql, sql_select);
        result_1 = mysql_store_result(&mysql);

        nb_attr_1 = mysql_num_fields(result_1);

        for (i = 0; i < mysql_num_fields (result_1); i++){//nom des colonnes
          field_1 = mysql_fetch_field (result_1);
            if(i != 0)
                printf("|");
            printf( "%s", field_1->name);
        }
        printf("\n");
        while ((row_1 = mysql_fetch_row(result_1)))
        {
            mysql_field_seek (result_1, 0);
            for (i = 0; i < mysql_num_fields (result_1); i++){
              field_1 = mysql_fetch_field (result_1);
//              printf ("name : %s --> %s\n", field_1->name, row_1[i]);//nom de la colonne --> contenu de la colonne
                printf("%s|",row_1[i]);
            }
            printf("\n");
        }

        mysql_free_result(result_1);
        
        
        mysql_close(&mysql);
        //Data->returnStatusConnexion = 1;
    }
    else
    {
        printf("erreur : %s\n", mysql_error(&mysql));
        printf("BDD mysql non trouvé ou bien il y a un problème de connexion\n");
        return 1;
    }

    return 0;
}
