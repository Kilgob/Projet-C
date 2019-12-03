

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
void LectureWeb(char* AddURL,char *string_curl){
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
    result = curl_easy_perform(myHandle);  //voir la doc pour une gestion minimal des erreurs
    curl_easy_cleanup(myHandle);
 
    if(result!=0)
        LectureLC.size=1;
//    string_curl[LectureLC.size];
    strcpy(string_curl, LectureLC.buffer);
    strcat(string_curl,"\0");
    if(LectureLC.buffer) free(LectureLC.buffer);
 
//  return Chaine;
}
 
 
 
void get_Arrays_Base(json_object **array){
    char AddURL[200];
    char Chaine[50000];  //À changer selon vos besoin


    strcpy(AddURL,"http://54.37.153.32:7999/migrator/tables?schema=mainCBase");

    LectureWeb(AddURL,Chaine);

    
    struct json_object *parsed_json;
    parsed_json = json_tokener_parse(Chaine);
    //Récupérer le contenu du tableau JSON "data"
    json_object_object_get_ex(parsed_json, "data", array);
    
    
//    return data_wb;
}
