#include <stdio.h>
#include <stdlib.h>
#include <winsock.h>
#include <MYSQL/mysql.h>


int main(int argc, char **argv)
{
    MYSQL mysql;
    mysql_init(&mysql);

    mysql_options(&mysql,MYSQL_READ_DEFAULT_GROUP,"option");

    if(mysql_real_connect(&mysql,"54.37.153.32","cproject","Fr3dCl3m","mainCBase",3306,NULL,0))
    {
        printf("DONE ! :D");
        mysql_close(&mysql);
    }
    else
    {
        printf("Une erreur s'est produite lors de la connexion à la BDD!");
    }

    return 0;
}
