

#include "link_main.h"
#include "uuid/uuid4.h"
#include <sys/stat.h>
//#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
 
#ifdef WIN32
#include <io.h>
#define READ_3RD_ARG unsigned int
#else
#include <unistd.h>
#define READ_3RD_ARG size_t
#endif
 
//Met le contenu de la page web dans la struct
static size_t WriteMemoryCallback(void* ptr, size_t size, size_t nmemb, void* data){
  size_t realsize = size * nmemb;
  struct BufferStruct* mem = (struct BufferStruct*) data;
  mem->buffer = (char*)realloc(mem->buffer, mem->size + realsize + 1);
 
  if ( mem->buffer ){
    memcpy(&(mem->buffer[mem->size]), ptr, realsize );
    mem->size += realsize;
    mem->buffer[ mem->size ] = 0;
  }
 
  return realsize;
}
 
//Lecture de la page web
int ApiWs(char* AddURL,char *string_curl, int task, char *name_file, char *fileTgrt){
    
    curl_global_init(CURL_GLOBAL_ALL);

//    char error[3000];
    struct curl_httppost *formpost = NULL;
    struct curl_httppost *lastptr = NULL;
//    struct curl_slist *headerlist = NULL;
    
    CURL *myHandle;
    CURLcode result;
    curl_off_t speed_upload, total_time;
    struct BufferStruct LectureLC;
    LectureLC.buffer = NULL;
    LectureLC.size = 0;
    
    
    
    if ((myHandle = curl_easy_init()) == NULL)
        return -1;
//    curl_easy_setopt(myHandle, CURLOPT_ERRORBUFFER, error);//buffer for error
    curl_easy_setopt(myHandle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    curl_easy_setopt(myHandle, CURLOPT_WRITEDATA, (void*)&LectureLC);
    curl_easy_setopt(myHandle, CURLOPT_URL, AddURL);
//    curl_easy_setopt(myHandle, CURLOPT_USERNAME,user);
//    curl_easy_setopt(myHandle, CURLOPT_PASSWORD,pass);
    curl_easy_setopt(myHandle, CURLOPT_HTTPAUTH, (long)CURLAUTH_BASIC);
    
    /* set user name and password for the authentication */
    curl_easy_setopt(myHandle, CURLOPT_USERPWD, "user:pass");//passer les variables
    if(task == 1){//quand je veux passer un fichier
        //Pour passer un json dans la requete
        curl_formadd(&formpost,
               &lastptr,
               CURLFORM_COPYNAME, "jsondata",
               CURLFORM_COPYCONTENTS,"text json",
               CURLFORM_END);

           curl_formadd(&formpost,
               &lastptr,
               CURLFORM_COPYNAME, "fichier_json",
               CURLFORM_FILE, fileTgrt,//nom du fichier
               CURLFORM_END);
//            headerlist = curl_slist_append(headerlist, buf);
        
        
        curl_easy_setopt(myHandle, CURLOPT_SSL_VERIFYHOST, 0);
        curl_easy_setopt(myHandle, CURLOPT_SSL_VERIFYPEER, 0);
//        curl_easy_setopt(myHandle, CURLOPT_VERBOSE, 1);
        curl_easy_setopt(myHandle, CURLOPT_HTTPPOST, formpost);
    }
//    curl_easy_setopt(myHandle, CURLOPT_VERBOSE, 1);
    if(task == 2){
        curl_formadd(&formpost,
            &lastptr,
            CURLFORM_COPYNAME, "jsondata",
            CURLFORM_COPYCONTENTS,"text json",
                     CURLFORM_END);
        curl_easy_setopt(myHandle, CURLOPT_HTTPPOST, formpost);
    }
    result = curl_easy_perform(myHandle);  //voir la doc pour une gestion minimal des erreurs

    if(result != CURLE_OK)
        printf("erreur lors de la requete curl\n");
//    string_curl[LectureLC.size];
    if(task == 1){
        curl_easy_getinfo(myHandle, CURLINFO_SPEED_UPLOAD_T, &speed_upload);
        curl_easy_getinfo(myHandle, CURLINFO_TOTAL_TIME_T, &total_time);
        printf("Speed: %" CURL_FORMAT_CURL_OFF_T " bytes/sec during %"CURL_FORMAT_CURL_OFF_T ".%02ld seconds\n",
                speed_upload,
               (total_time / 1000000),
                (long)(total_time % 1000000));
    }
//    printf("longueur = %ld\n |%s|\n",strlen(LectureLC.buffer), LectureLC.buffer);
    strcpy(string_curl, LectureLC.buffer);
    strcat(string_curl,"\0");
    if(LectureLC.buffer)
        free(LectureLC.buffer);
    curl_easy_cleanup(myHandle);
    return 1;
//  return Str;
}
 
 
 
void get_Arrays_Base(json_object **array, struct json_conf *Json_conf){
//    int status = 0;
    char AddURL[200];
    char Str_result[20000];  //À changer selon vos besoin
//    printf("%s",json_object_get_string(Json_conf->IP));
    char url[100] = "http://";
    strcat(url, json_object_get_string(Json_conf->IP));
    strcat(url, ":");
    strcat(url, json_object_get_string(Json_conf->bdd[Json_conf->bdd_select].PAPI));
    strcat(url, json_object_get_string(Json_conf->bdd[Json_conf->bdd_select].api_route));
    strcat(url, "?get_count=true&db=");
    strcat(url, json_object_get_string(Json_conf->bdd[Json_conf->bdd_select].name_BDD));
    strcpy(AddURL,url);
       
    ApiWs(AddURL,Str_result, 0, NULL, NULL);
//    printf("status : %d\n",status);
    
    struct json_object *parsed_json;
    parsed_json = json_tokener_parse(Str_result);
    //Récupérer le contenu du tableau JSON "data"
    json_object_object_get_ex(parsed_json, "data", array);
    
//    return data_wb;
}


void migration(struct Migration *Datas){
//    gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(Datas->Target_Serv));
    
    struct json_object *file = json_object_new_array();
    struct json_object *result = json_object_new_object();
    struct json_object *result_buffer/* = json_object_new_object()*/;
    
//    int i = 1;
    char name_file[UUID4_LEN +6];
    uuid4_init();
    uuid4_generate(name_file);
    printf("uuid file : %s\n",name_file);
    char Str_result[5500];  //À changer selon vos besoin
    char url[200] = "http://";
    char ID_Exec[50];
    
    FILE *file_temp;
    char fileDirectory[300];
    strcpy(fileDirectory, getenv("REPLICATOR_HOME"));
    strcat(fileDirectory, "/conf/");
    strcat(name_file, ".json");
    strcat(fileDirectory, name_file);//nom du fichier avec l'uuid
//    printf("file and directory : %s\n",fileDirectory);
//    printf("name file : %s\n",name_file);
    
    if(gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(Datas->Target_Schema)) == NULL){
        gtk_label_set_text(GTK_LABEL(Datas->label_Migration_Status), "Sélectionnez une base vers laquel migrer...");
        return;
    }
               
    
    //----------------------------------------------------------------------------------- Requete d'export puis boucle jusqu'au le statut == SUCCES
    strcat(url, json_object_get_string(Datas->Json_conf->IP)); //attention à la bdd ?
    strcat(url, ":");
    strcat(url, json_object_get_string(Datas->Json_conf->PJSON));
    strcat(url, json_object_get_string(Datas->Json_conf->PJSon_Pass_API));

    //JSON TODO : demande d'export
   file_temp = fopen(fileDirectory, "wb");
    if(migration_serv(Datas, file, "export", name_file, "")){
        gtk_label_set_text(GTK_LABEL(Datas->label_Migration_Status), "erreur lors de la création du JSON. Il manque une information");
        return;
    }
    //envoi du JSON contenant les paramétrages d'export
    migration_datas(Datas->Widgets, file);
    gtk_label_set_text(GTK_LABEL(Datas->label_Migration_Status), "envoi du JSON d'export..");
    fprintf(file_temp,"%s",json_object_get_string(file));
    fclose(file_temp);
    ApiWs(url,Str_result, 1, name_file, fileDirectory);//Upload du fichier todo
    
    
//    strcpy(url, "http://");
    strcpy(url, json_object_get_string(Datas->Json_conf->IP)); //attention à la bdd ?
    strcat(url, ":");
    strcat(url, json_object_get_string(Datas->Json_conf->PJSON));
    strcat(url, "/tom/cmd/exec?cmd=");
    strcat(url, "replicator_c");//peu etre mettre le nom de l'app dans le fichier de conf ?
    strcat(url, "%20-f%20"); //passage en parametre du nom du fichier 1/2
    strcat(url, name_file); //passage en parametre du nom du fichier 2/2
    strcat(url, "&serveur=");
    strcat(url, json_object_get_string(Datas->Json_conf->IP));
    ApiWs(url, Str_result, 2, name_file, NULL); //Correspond au passage de l'uuid en parametre
    result_buffer = json_tokener_parse(Str_result);
    json_object_object_get_ex(result_buffer, "task-id", &result);
    strcpy(ID_Exec,json_object_get_string(result));
    printf("\nRetour de l'ID de la tache : %s\n",json_object_get_string(result));
    
    
    
//    file_temp = fopen(fileDirectory, "wb");
    file = json_object_new_array();
    file_temp = fopen(fileDirectory, "wb");
    migration_serv(Datas, file, "import", name_file, ID_Exec);
    migration_datas(Datas->Widgets, file);
    gtk_label_set_text(GTK_LABEL(Datas->label_Migration_Status), "import..");
    fprintf(file_temp,"%s",json_object_get_string(file));
//    printf("\ncontenu du fichier : %s\n\n",json_object_get_string(file));
    fclose(file_temp);
    
//    sleep(1);//pour temporiser "la création" du fichier d'import sur le disque
}




int migration_serv(struct Migration *Serv_info, json_object *file, char *task, char *name_file, char *ID_Exec){
    int i = 0;
    int j = 0;
//    char *target_Schema;
    
    struct json_object *conf = json_object_new_object();
    if (strcmp(task, "import")){
        json_object_object_add(conf, "task",json_object_new_string(task));
        json_object_object_add(conf, "ip_source", Serv_info->Json_conf->IP);
        json_object_object_add(conf, "pass_root", Serv_info->Json_conf->bdd[Serv_info->Json_conf->bdd_select].Pass_Root);
        json_object_object_add(conf, "id_user", Serv_info->Json_conf->bdd[Serv_info->Json_conf->bdd_select].user);
        json_object_object_add(conf, "pass_user", Serv_info->Json_conf->bdd[Serv_info->Json_conf->bdd_select].pass_User);
        json_object_object_add(conf, "port",Serv_info->Json_conf->bdd[Serv_info->Json_conf->bdd_select].PMYSQ);
        json_object_object_add(conf, "name_db_source",Serv_info-> Json_conf->bdd[Serv_info->Json_conf->bdd_select].name_db);
        json_object_object_add(conf, "name_schema_source", Serv_info->Json_conf->bdd[Serv_info->Json_conf->bdd_select].name_BDD);
        
        json_object_object_add(conf, "bdd_type_source", Serv_info->Json_conf->bdd[Serv_info->Json_conf->bdd_select].BDD_Type);//bdd source
        
        for(j = 0; j < Serv_info->Json_conf_foa->nbr_server ; j++)
            for(i = 0; i < Serv_info->Json_conf_foa[j].nbr_bdd; i++)
                if(!strcmp(gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(Serv_info->Target_Schema)), json_object_get_string(Serv_info->Json_conf_foa[j].bdd[i].name_BDD))){
                    json_object_object_add(conf, "bdd_type_target", Serv_info->Json_conf_foa[j].bdd[i].BDD_Type);//bdd cible
                    json_object_object_add(conf, "name_schema_target", Serv_info->Json_conf_foa[j].bdd[i].name_BDD);
//                    printf("ip target : %s\n", json_object_get_string(Serv_info->Json_conf_foa[j].IP));
                    json_object_object_add(conf, "ip_target", Serv_info->Json_conf_foa[j].IP);
                    json_object_object_add(conf, "name_db_target",Serv_info-> Json_conf_foa[j].bdd[i].name_db);
                    break;
                }
    }
    else{
        for(j = 0; j < Serv_info->Json_conf_foa->nbr_server ; j++)
            for(i = 0; i < Serv_info->Json_conf_foa[j].nbr_bdd; i++){
//                printf("nom de la bdd : %s\n",json_object_get_string(Serv_info->Json_conf_foa[j].bdd[i].name_BDD));
                if(!strcmp(gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(Serv_info->Target_Schema)), json_object_get_string(Serv_info->Json_conf_foa[j].bdd[i].name_BDD))){
                    json_object_object_add(conf, "ID_Exec",json_object_new_string(ID_Exec));
                    json_object_object_add(conf, "task",json_object_new_string(task));
                    json_object_object_add(conf, "pass_root", Serv_info->Json_conf_foa[j].bdd[i].Pass_Root);
                    json_object_object_add(conf, "id_user", Serv_info->Json_conf_foa[j].bdd[i].user);
                    json_object_object_add(conf, "pass_user", Serv_info->Json_conf_foa[j].bdd[i].pass_User);
                    json_object_object_add(conf, "port",Serv_info-> Json_conf_foa[j].bdd[i].PMYSQ);
                    json_object_object_add(conf, "bdd_type_source", Serv_info->Json_conf->bdd[Serv_info->Json_conf->bdd_select].BDD_Type);//bdd cible
                    json_object_object_add(conf, "bdd_type_target", Serv_info->Json_conf_foa[j].bdd[i].BDD_Type);//bdd cible
                    json_object_object_add(conf, "name_schema_target", Serv_info->Json_conf_foa[j].bdd[i].name_BDD);
                    json_object_object_add(conf, "ip_source", Serv_info->Json_conf->IP);
                    json_object_object_add(conf, "ip_target", Serv_info->Json_conf_foa[j].IP);
                    json_object_object_add(conf, "name_db_source", Serv_info->Json_conf->bdd[Serv_info->Json_conf->bdd_select].name_db);
                    json_object_object_add(conf, "name_db_target", Serv_info->Json_conf_foa[j].bdd[i].name_db);
                    
//                    printf("\nMigration vers une base de type %s\n\n",json_object_get_string(Serv_info->Json_conf_foa[j].bdd[i].BDD_Type));
                    break;
                }
            }//ajouter des break pour sortir directement une fois la bdd trouvée
        json_object_object_add(conf, "name_schema",json_object_new_string(gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(Serv_info->Target_Schema))) );
    }
    
    json_object_object_add(conf, "uuid_file", json_object_new_string(name_file));
    json_object_array_add(file, conf); //json object put idx : placer à l'endroit voulu (index 0 ne fonctionne pas)
    return 0;
}

int migration_datas(struct WidgetBDD *Widgets, json_object *file){
//    struct json_object *Contain_data = json_object_new_object();
    struct json_object *data = json_object_new_object();
    
    export_JSON(Widgets, NULL, 1, data);
//    json_object_object_add(Contain_data, "request_export", file);
    
    json_object_array_add(file, data);
    return 0;
}
