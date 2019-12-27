

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
 

static curlioerr my_ioctl(CURL *handle, curliocmd cmd, void *userp)
{
  int *fdp = (int *)userp;
  int fd = *fdp;
 
  (void)handle; /* not used in here */
 
  switch(cmd) {
  case CURLIOCMD_RESTARTREAD:
    /* mr libcurl kindly asks as to rewind the read data stream to start */
    if(-1 == lseek(fd, 0, SEEK_SET))
      /* couldn't rewind */
      return CURLIOE_FAILRESTART;
 
    break;
 
  default: /* ignore unknown commands */
    return CURLIOE_UNKNOWNCMD;
  }
  return CURLIOE_OK; /* success! */
}
 
/* read callback function, fread() look alike */
static size_t read_callback(void *ptr, size_t size, size_t nmemb, void *stream)
{
  ssize_t retcode;
  curl_off_t nread;
 
  int *fdp = (int *)stream;
  int fd = *fdp;
 
  retcode = read(fd, ptr, (READ_3RD_ARG)(size * nmemb));
 
  nread = (curl_off_t)retcode;
 
  fprintf(stderr, "*** We read %" CURL_FORMAT_CURL_OFF_T
          " bytes from file\n", nread);
 
  return retcode;
}
 
//Lecture de la page web
void LectureWeb(char* AddURL,char *string_curl, const char *user, const char *pass, int task, FILE *file,char *name_file, const char *fileT){
//    strcat(file_Name, ".json");
    
    curl_global_init(CURL_GLOBAL_ALL);

    CURL *myHandle;
    CURLcode result;
    struct stat file_info;
    curl_off_t speed_upload, total_time;
    struct BufferStruct LectureLC;
    LectureLC.buffer = NULL;
    LectureLC.size = 0;
    CURLcode res;
    int hd;
//    struct stat file_info;
    curl_mime *form = NULL;
    curl_mimepart *field = NULL;
    
    
    myHandle = curl_easy_init();
    curl_easy_setopt(myHandle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    curl_easy_setopt(myHandle, CURLOPT_WRITEDATA, (void*)&LectureLC);
    curl_easy_setopt(myHandle, CURLOPT_URL, AddURL);
    curl_easy_setopt(myHandle, CURLOPT_USERNAME,user);
    curl_easy_setopt(myHandle, CURLOPT_PASSWORD,pass);
    if(task ==1){
        struct curl_slist *headers = NULL;
        curl_slist_append(headers, "Accept: application/json");
        curl_slist_append(headers, "Content-Type: multipart/form-data");
        //         curl_slist_append(headers, "charsets: utf-8");

        //         curl_easy_setopt(curl, CURLOPT_URL, url);
        form = curl_mime_init(myHandle);

        field = curl_mime_addpart(form);
        curl_mime_name(field, "fichier_json");//sendfile
        curl_mime_filedata(field, name_file);
        /* Fill in the filename field */
        field = curl_mime_addpart(form);
        curl_mime_name(field, "file_name");//fichier_json
        curl_mime_data(field, name_file, CURL_ZERO_TERMINATED);
        field = curl_mime_addpart(form);
        curl_mime_name(field, "submit");
        curl_mime_data(field, "send", CURL_ZERO_TERMINATED);
        //         curl_easy_setopt(curl, CURLOPT_MIMEPOST, form);
        curl_easy_setopt(myHandle, CURLOPT_HTTPPOST, form);
        curl_easy_setopt(myHandle, CURLOPT_CUSTOMREQUEST, "PUT");
        curl_easy_setopt(myHandle, CURLOPT_HTTPHEADER, headers);
        /* we want to use our own read function */
        curl_easy_setopt(myHandle, CURLOPT_READFUNCTION, read_callback);
        /* which file to upload */
        curl_easy_setopt(myHandle, CURLOPT_READDATA, (void *)&hd);
        /* set the ioctl function */
        curl_easy_setopt(myHandle, CURLOPT_IOCTLFUNCTION, my_ioctl);
        /* pass the file descriptor to the ioctl callback as well */
        curl_easy_setopt(myHandle, CURLOPT_IOCTLDATA, (void *)&hd);
        curl_easy_setopt(myHandle, CURLOPT_UPLOAD, 1L);
//        curl_easy_setopt(myHandle, CURLOPT_URL, url);
        curl_easy_setopt(myHandle, CURLOPT_INFILESIZE_LARGE, (curl_off_t)file_info.st_size);
        curl_easy_setopt(myHandle, CURLOPT_HTTPAUTH, (long)CURLAUTH_BASIC);

        /* set user name and password for the authentication */
        curl_easy_setopt(myHandle, CURLOPT_USERPWD, "user:pass");

        curl_easy_setopt(myHandle, CURLOPT_VERBOSE, 1L);
    }
    result = curl_easy_perform(myHandle);  //voir la doc pour une gestion minimal des erreurs
//    curl_easy_cleanup(myHandle);
    if(result != CURLE_OK)
        printf("erreur lors de la requete\n");
//    string_curl[LectureLC.size];
    if(task == 1){
        curl_easy_getinfo(myHandle, CURLINFO_SPEED_UPLOAD_T, &speed_upload);
        curl_easy_getinfo(myHandle, CURLINFO_TOTAL_TIME_T, &total_time);
        printf("Speed: %" CURL_FORMAT_CURL_OFF_T " bytes/sec during %"CURL_FORMAT_CURL_OFF_T ".%06ld seconds\n",
                speed_upload,
               (total_time / 1000000),
                (long)(total_time % 1000000));
    }
    strcpy(string_curl, LectureLC.buffer);
    strcat(string_curl,"\0");
    if(LectureLC.buffer)
        free(LectureLC.buffer);
    curl_easy_cleanup(myHandle);
 
//  return Str;
}
 
 
 
void get_Arrays_Base(json_object **array, struct json_conf *Json_conf){
    char AddURL[200];
    char Str_result[30000];  //À changer selon vos besoin
//    printf("%s",json_object_get_string(Json_conf->IP));
    char url[100] = "http://";
    strcat(url, json_object_get_string(Json_conf->IP));
    strcat(url, ":");
    strcat(url, json_object_get_string(Json_conf->bdd->PAPI));
    strcat(url, json_object_get_string(Json_conf->bdd->api_route));
    strcat(url, "?get_count=true&db=");
    strcat(url, json_object_get_string(Json_conf->bdd->name_BDD));
    //    strcpy(AddURL,"http://54.37.153.32:5001/mysql1/mysql1?get_count=true&db=db");//V3
//    strcpy(AddURL,"http://54.37.153.32:5010/mysql1?get_count=true&db=db");//V2
//    strcpy(AddURL,"http://54.37.153.32:7999/migrator/tables?schema=mainCBase");//V1
    strcpy(AddURL,url);
       
    LectureWeb(AddURL,Str_result, json_object_get_string(Json_conf->user_webService), json_object_get_string(Json_conf->pass_webService), 0, NULL, NULL, NULL);

    
    struct json_object *parsed_json;
    parsed_json = json_tokener_parse(Str_result);
    //Récupérer le contenu du tableau JSON "data"
    json_object_object_get_ex(parsed_json, "data", array);
    
//    return data_wb;
}



int requestToBDD(char *Table_Name, char *Column_Type){
    
    json_object *sendData = json_object_new_object();
    json_object *sendData2 = json_object_new_object();
     
    //Ajout des chaines JSON dans le tableau data;
    json_object_object_add(sendData, "TABLE_NAME", json_object_new_string(Table_Name));
    json_object_object_add(sendData, "COLUMN_TYPE", json_object_new_string(Column_Type));

    
    //Ajout du tableau data dans la requete;
    json_object_object_add(sendData2, "Data", sendData);
    printf("%s",json_object_to_json_string(sendData2));
    
    //(À modifier) Si l'envoi est passé, on return 0
    return 0;
    
}



void migration(struct Migration *Datas){
    struct json_object *file = json_object_new_object();
    file = json_object_new_array();
    
    char name_file[UUID4_LEN +5];
    uuid4_init();
    uuid4_generate(name_file);
    printf("uuid file : %s\n",name_file);
    char AddURL[100];
    char Str_result[500];  //À changer selon vos besoin
    char url[200] = "http://";
    
    
    FILE *file_temp;
    char fileDirectory[200];
    strcpy(fileDirectory, g_get_current_dir());
    strcat(fileDirectory, "/");
    strcat(name_file, ".json");
    strcat(fileDirectory, name_file);//nom du fichier avec l'uuid
//    strcat(fileDirectory, ".json");
    printf("file and directory : %s\n",fileDirectory);
    printf("name file : %s\n",name_file);
    file_temp = fopen(fileDirectory, "wb+");
    
    
    strcat(url, json_object_get_string(Datas->Json_conf->IP)); //attention à la bdd ?
    strcat(url, ":");
    strcat(url, json_object_get_string(Datas->Json_conf->PJSON));
    strcat(url, json_object_get_string(Datas->Json_conf->PJSon_Pass_API));

    strcpy(AddURL,url);
           
    if(migration_serv(Datas->Json_conf, file, "export", name_file)){
        gtk_label_set_text(GTK_LABEL(Datas->label_Migration_Status), "erreur lors de la création du JSON. Il manque une information");
        return;
    }
    
    //envoi du JSON contenant les paramétrages d'export
    migration_datas(Datas->Widgets, file);
    gtk_label_set_text(GTK_LABEL(Datas->label_Migration_Status), "envoi du JSON d'export..");
//    fseek(file_temp, 0, SEEK_SET);
    fprintf(file_temp,"%s",json_object_get_string(file));
    fclose(file_temp);
//    fseek(file_temp, 0, SEEK_SET);
    LectureWeb(AddURL,Str_result, json_object_get_string(Datas->Json_conf->user_webService), json_object_get_string(Datas->Json_conf->pass_webService), 1, file_temp, name_file, json_object_get_string(file));
    
    printf("%s\n",Str_result);
    printf("url : %s\n",url);
    
    

    //Traitement du JSON côté serveur pour le dump de la base
    file_temp = fopen(fileDirectory, "wb+");
    migration_serv(Datas->Json_conf, file, "import", name_file);
        gtk_label_set_text(GTK_LABEL(Datas->label_Migration_Status), "Début du dump");
    fprintf(file_temp,"%s",json_object_get_string(file));
    fclose(file_temp);
    LectureWeb(AddURL,Str_result, json_object_get_string(Datas->Json_conf->user_webService), json_object_get_string(Datas->Json_conf->pass_webService), 1, file_temp, name_file, json_object_get_string(file));//Ajouter l'uudi pour retrouver le fichier
    remove(fileDirectory);//suppression du fichier tampon stocké sur le disque
}


int migration_serv(struct json_conf *Json_conf, json_object *file, char *task, char *name_file){
    struct json_object *conf = json_object_new_object();
    json_object_object_add(conf, "task",json_object_new_string(task));
    json_object_object_add(conf, "pass_root", Json_conf->bdd->Pass_Root);
    json_object_object_add(conf, "id_user", Json_conf->bdd->user);
    json_object_object_add(conf, "pass_user", Json_conf->bdd->pass_User);
    json_object_object_add(conf, "p_mysql", Json_conf->bdd->PMYSQ);
    json_object_object_add(conf, "bdd_type", Json_conf->bdd->BDD_Type);
    json_object_object_add(conf, "name_schema", Json_conf->bdd->name_BDD);
    json_object_object_add(conf, "uuid_file", json_object_new_string(name_file));
    
    
    json_object_array_put_idx(file, 0, conf);
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
