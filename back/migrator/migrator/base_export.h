//
//  base_export.h
//  migrator
//
//  Created by Frédéric FLACELIERE on 30/12/2019.
//  Copyright © 2019 Frédéric FLACELIERE. All rights reserved.
//

#include <json-c/json.h>

struct Json_infoserv{
    struct json_object *ip_source;
    struct json_object *ip_target;
    struct json_object *pass_root;
    struct json_object *user;
    struct json_object *pass_user;
    struct json_object *port;
    struct json_object *bdd_type_source;
    struct json_object *bdd_type_target;
    struct json_object *name_schema_source;
    struct json_object *name_schema_target;
    struct json_object *name_db_source;
    struct json_object *name_db_target;
    struct json_object *task;
    char uuid[37];//taille standard d'un uuid (voir le front)
};

struct Json_export{
    struct json_object *array;
    struct json_object *all_column;
    struct Json_export_column *column;
    size_t nbr_arrays;
};
struct Json_export_column{
    struct json_object *column_name;
    struct json_object *column_type;
    struct json_object *column_key;
    size_t nbr_columns;
};

#ifndef base_export_h
#define base_export_h

int mysql_dump(struct Json_infoserv *, struct Json_export *, char *);
int mysqlVmysql_export(struct Json_infoserv *, struct Json_export *, char *);
int export_table_mysqlsql(struct Json_infoserv *, struct Json_export *, char *);
int export_table_mysqlcsv(struct Json_infoserv *, struct Json_export *, char *);
int export_table_mysqlcmd(struct Json_infoserv *, struct Json_export *, char *);

int oracle_dump(void);

int vertica_dump(struct Json_infoserv *, struct Json_export *, char *);
int verticaVvertica_export(struct Json_infoserv *, struct Json_export *, char *);
int export_table_verticasql(struct Json_infoserv *, struct Json_export *, char *);
int export_table_verticacsv(struct Json_infoserv *, struct Json_export *, char *);
int export_table_verticacmd(const char *, struct Json_export *, char *);


#endif /* base_export_h */

#ifndef base_import_h
#define base_import_h

int mysql_import(struct Json_infoserv *, struct Json_export *, char *);
int mysqlVmysql_import(struct Json_infoserv *, struct Json_export *, char *);
int mysqlVvertica_import(struct Json_infoserv *, struct Json_export *,  char *);

int oracle_import(void);

int vertica_import(void);
int verticaVvertica_import(struct Json_infoserv *, struct Json_export *, char *);

#endif /* base_import_h */
