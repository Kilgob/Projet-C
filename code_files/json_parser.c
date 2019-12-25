

#include "link_main.h"
 
 
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
void LectureWeb(char* AddURL,char *string_curl, const char *user, const char *pass){
    curl_global_init(CURL_GLOBAL_ALL);

    CURL *myHandle;
    CURLcode result;
    struct BufferStruct LectureLC;
    LectureLC.buffer = NULL;
    LectureLC.size = 0;

    myHandle = curl_easy_init();
    curl_easy_setopt(myHandle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    curl_easy_setopt(myHandle, CURLOPT_WRITEDATA, (void*)&LectureLC);
    curl_easy_setopt(myHandle, CURLOPT_URL, AddURL);
    curl_easy_setopt(myHandle, CURLOPT_USERNAME,user);
    curl_easy_setopt(myHandle, CURLOPT_PASSWORD,pass);
    result = curl_easy_perform(myHandle);  //voir la doc pour une gestion minimal des erreurs
    curl_easy_cleanup(myHandle);
 
    if(result!=0)
        LectureLC.size=1;
//    string_curl[LectureLC.size];
    strcpy(string_curl, LectureLC.buffer);
    strcat(string_curl,"\0");
    if(LectureLC.buffer) free(LectureLC.buffer);
 
//  return Str;
}
 
 
 
void get_Arrays_Base(json_object **array, struct json_conf *Json_conf){
    char AddURL[200];
    char Str[60000];  //À changer selon vos besoin
//    printf("%s",json_object_get_string(Json_conf->IP));
    char url[100] = "http://";
    strcat(url, json_object_get_string(Json_conf->IP));
    strcat(url, ":");
    strcat(url, json_object_get_string(Json_conf->bdd->PMYSQL));
    strcat(url, json_object_get_string(Json_conf->bdd->api_route));
    strcat(url, "?get_count=true&db=");
    strcat(url, json_object_get_string(Json_conf->bdd->name_BDD));
    //    strcpy(AddURL,"http://54.37.153.32:5001/mysql1/mysql1?get_count=true&db=db");//V3
//    strcpy(AddURL,"http://54.37.153.32:5010/mysql1?get_count=true&db=db");//V2
//    strcpy(AddURL,"http://54.37.153.32:7999/migrator/tables?schema=mainCBase");//V1
    strcpy(AddURL,url);
       
    LectureWeb(AddURL,Str, json_object_get_string(Json_conf->user_webService), json_object_get_string(Json_conf->pass_webService) );

    
    struct json_object *parsed_json;
    parsed_json = json_tokener_parse(Str);
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
