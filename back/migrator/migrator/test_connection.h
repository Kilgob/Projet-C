//
//  test_connection.h
//  migrator
//
//  Created by Frédéric FLACELIERE on 03/01/2020.
//  Copyright © 2020 Frédéric FLACELIERE. All rights reserved.
//

#ifndef test_connection_h
#define test_connection_h

#include <stdio.h>

struct Bdd_Test{
    int status;
    char name_Schema[50];
    int port;
    char bdd_type[50];
    char name_Base[50];//Obligatoire pour vertica
    char user[50];
    char pass[50];
    char ip[50];
    char request[100];
};


void get_type_base_test(struct Bdd_Test *);
int connection_bdd(struct Bdd_Test *);

#endif /* test_connection_h */


//printf( "   [-n] nom du schéma \n");
//printf( "   [-p] port de la bdd \n");
//printf( "   [-t] type de bdd \n");
//printf( "   [-b] nom de la base \n");
//printf( "   [-U] user \n");
//printf( "   [-P] pass \n");
