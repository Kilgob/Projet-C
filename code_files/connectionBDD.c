//
//  connectionBDD.c
//  Projet
//
//  Created by Frédéric FLACELIERE on 04/11/2019.
//  Copyright © 2019 Frédéric FLACELIERE. All rights reserved.
//

#include "link.h"
#include <mysql.h>

char* connection_bdd(){
    char *result = malloc(sizeof(char)*100);
//    printf("%s",result);
    MYSQL mysql;
    mysql_init(&mysql);

    // insert code here...
    printf("Hello, World!\n");
    printf("Hello, World a seconde time!\n");
    


    mysql_options(&mysql,MYSQL_READ_DEFAULT_GROUP,"option");

    if(mysql_real_connect(&mysql,"54.37.153.32","cproject","Fr3dCl3m","mainCBase",3306,NULL,0))
    {
        printf("DONE ! :D\n");
        result = "DONE ! :D\n";
        mysql_close(&mysql);
    }
    else
    {
        printf("Une erreur s'est produite lors de la connexion à la BDD!");
        result = "Une erreur s'est produite lors de la connexion à la BDD!";
    }
    return result;
//    free(result);
}
