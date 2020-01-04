#include "link_main.h"
#include "file_control.h"
//#include "main_window.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
//#include <libxml/xlink.h>
//#include <cjson/json.h>
//#include <math.h>
#include <gtk/gtk.h>
//#include <iostream>

struct create_main_window Create_main_window1;
GtkWidget *window;
GtkWidget *boxJSON_Conf; //Box regroupant les 3 boxs du fichier de conf
GtkWidget *widgConfs; //Box pour les serveurs
GtkWidget *widgConfb; //Box pour les bdd
GtkWidget *BoxServer;
GtkWidget *BoxBDD;


void button_Connection(struct Button_Connection *Button_Connection){ //rendre le bouton sélectable
    gtk_button_set_label(GTK_BUTTON(Button_Connection->connection_button), "Se connecter");
    Button_Connection->status = 1;
}

void set_json_save(GtkWidget *name_Bdd_Widg, struct json_conf *jsonconf){
    int i = 0;
    while(strcmp(gtk_button_get_label(GTK_BUTTON(name_Bdd_Widg)), json_object_get_string(jsonconf->bdd[i].name_BDD))){
        i++;
    }
    jsonconf->bdd_select = i;
    
    Create_main_window1.Json_conf = jsonconf;
    printf("nom du shéma sélectionné %d: %s\n", i, json_object_get_string(Create_main_window1.Json_conf->bdd[Create_main_window1.Json_conf->bdd_select].name_BDD));

}
//void set_json_save2(struct json_object **test){
////    Create_main_window1.Json_conf = *jsonconf;
//    printf("%p\n",*test);
//    printf("%s\n",json_object_get_string(*test));
//}

void button_set_color(GtkWidget **Conf_Name_bdd){ //retirer la couleur une fois une nouvelle sélection effectuée
    
    GdkRGBA SetColor={0,0,250,0.50};
//    GdkRGBA SetColor_Compare={0,0,250,0.50};
    
    gtk_widget_override_color(*Conf_Name_bdd, GTK_STATE_FLAG_NORMAL, &SetColor);
//    gtk_style_context_get_color(<#GtkStyleContext *context#>, GTK_STATE_FLAG_NORMAL, &SetColor_Compare);
    
    
    gtk_widget_show_now(*Conf_Name_bdd);
}

void delete(){
    GList *children, *iter;

    children = gtk_container_get_children(GTK_CONTAINER(widgConfb));
    for(iter = children; iter != NULL; iter = g_list_next(iter))
    {
        g_object_ref(G_OBJECT(iter->data)); //Pour ajouter une ref sinon lors du remove 
        gtk_container_remove(GTK_CONTAINER(widgConfb), GTK_WIDGET(iter->data));
        
    }
    g_list_free(children);
}


void set_bdd(struct Struct_Conf_Name_Server Struct_Conf_Name_Server[]){
    int i;
    delete();
    for(i = 0; i < Struct_Conf_Name_Server->nbr_bdd ; i++){
        gtk_box_pack_start(GTK_BOX(widgConfb), Struct_Conf_Name_Server->Conf_Name_bdd[i], FALSE, FALSE, 2);
    }
    gtk_widget_show_all (window);
}



static void activate (GtkApplication *app,gpointer user_data){
    int j,k;
    
    boxJSON_Conf = gtk_box_new(FALSE, 0);
    widgConfs = gtk_box_new(TRUE, 1);
    widgConfb = gtk_box_new(TRUE, 1);
    BoxServer = gtk_scrolled_window_new(NULL,NULL);
    BoxBDD = gtk_scrolled_window_new(NULL,NULL);
    
    static struct Button_Connection Button_Connection;


    GtkWidget *button;
    GtkWidget *button_box;
    GtkWidget *boxLogin; //box du bouton se connecter et avec le lab en cas d'erreur
    GtkWidget *mainPageText;
    GtkWidget *boxConnection; //box regroupant tout
    
    GtkWidget *grid = gtk_grid_new();


    FILE *PF_json_Conf;
    char Json_Conf [50000];
    if((PF_json_Conf = fopen("/Users/fred/OneDrive/ESGI/Annee2/ProjetC/conf_bdd.json", "r")) == NULL){
        printf("ouverture du fichier de conf impossible");
        exit(1);
    }
    
    struct json_object *parsed_Json_Conf;
    struct json_object *parsed_Json_Conf_BDD_Buffer;
    struct json_object *parsed_Json_Conf_BDD_Buffer_idx;
    struct json_object *parsed_Json_Conf_Server_Buffer;
    fread(Json_Conf, 50000, 1, PF_json_Conf);
    parsed_Json_Conf = json_tokener_parse(Json_Conf);
    
    
    size_t n_Col_tab2 = json_object_array_length(parsed_Json_Conf);
    size_t n_Col_tab3;

    
    window = gtk_application_window_new (app);
    gtk_window_set_title (GTK_WINDOW (window), "Se connecter");
    gtk_window_set_default_size (GTK_WINDOW (window), 600, 400);
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
    gtk_container_add (GTK_CONTAINER (window), grid);
    //Box contenant les widgets
    boxConnection = gtk_box_new(TRUE, 0);
    gtk_grid_attach(GTK_GRID(grid), boxConnection, 0, 1, 50, 50);

    
    //Ajout des inputs pour la connection
    //ajout du label dans la box
    mainPageText = gtk_label_new("Se connecter à une BDD");
    gtk_box_pack_start(GTK_BOX(boxConnection), mainPageText, TRUE, TRUE, 0);
    
    boxLogin =gtk_box_new(TRUE, 0);
    gtk_box_pack_start(GTK_BOX(boxConnection),boxLogin, TRUE, TRUE, 0);
    
    
    Create_main_window1.app = app;
    Create_main_window1.oldWindow = GTK_WIDGET(window);


    
    gtk_box_pack_start(GTK_BOX(boxConnection), boxJSON_Conf, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(boxJSON_Conf), BoxServer, FALSE, FALSE, 0);
    gtk_container_add(GTK_CONTAINER(BoxServer),widgConfs);
    
    gtk_container_add(GTK_CONTAINER(boxJSON_Conf),BoxBDD);
    gtk_container_add(GTK_CONTAINER(BoxBDD),widgConfb);
    gtk_widget_set_size_request(BoxBDD, 300, 10);
    gtk_widget_set_size_request(boxJSON_Conf, 500, 300);

    gtk_widget_set_size_request(BoxServer, 220, 10);
    

    
    //Bouton de validation de connection
    Button_Connection.connection_button = gtk_button_new_with_label("Se connecter");
    Button_Connection.connection_button_box = gtk_button_box_new (GTK_ORIENTATION_HORIZONTAL);
    gtk_box_pack_start(GTK_BOX(boxConnection), Button_Connection.connection_button, FALSE, FALSE, 0);
    
    struct json_conf *json_conf = malloc(sizeof(struct json_conf)*n_Col_tab2); /*[n_Col_tab2]*/;

    struct Struct_Conf_Name_Server *Struct_Conf_Name_Server;
    Struct_Conf_Name_Server = malloc(sizeof(struct Struct_Conf_Name_Server) *2)/*[n_Col_tab2]*/;
    json_conf->nbr_server = n_Col_tab2;
    Struct_Conf_Name_Server->nbr_server = n_Col_tab2;
    
    Create_main_window1.Json_conf_foa = json_conf;//stockage du 1er élément de la structure json_conf (1er élément du tableau)
    
    int g = 1;
    
    for(k = 0; k < n_Col_tab2; k++){
        
        parsed_Json_Conf_Server_Buffer = json_object_array_get_idx(parsed_Json_Conf, k);
        json_object_object_get_ex(parsed_Json_Conf_Server_Buffer, "IP", &json_conf[k].IP);
        json_object_object_get_ex(parsed_Json_Conf_Server_Buffer, "name_server", &json_conf[k].name_server);
        json_object_object_get_ex(parsed_Json_Conf_Server_Buffer, "user_webService", &json_conf[k].user_webService);
        json_object_object_get_ex(parsed_Json_Conf_Server_Buffer, "pass_webService", &json_conf[k].pass_webService);
        json_object_object_get_ex(parsed_Json_Conf_Server_Buffer, "pjson", &json_conf[k].PJSON);
        json_object_object_get_ex(parsed_Json_Conf_Server_Buffer, "pjson_pass_api", &json_conf[k].PJSon_Pass_API);
        json_object_object_get_ex(parsed_Json_Conf_Server_Buffer, "BDD", &parsed_Json_Conf_BDD_Buffer);
        n_Col_tab3 = json_object_array_length(parsed_Json_Conf_BDD_Buffer);
        
        Struct_Conf_Name_Server[k].Conf_Name_Server = gtk_button_new_with_label(json_object_get_string(json_conf[k].name_server));

           Struct_Conf_Name_Server[k].Conf_Name_bdd = malloc(sizeof(GtkWidget *) * n_Col_tab3);
        gtk_box_pack_start(GTK_BOX(widgConfs), Struct_Conf_Name_Server[k].Conf_Name_Server, FALSE, FALSE, 10);
        g_signal_connect_swapped (Struct_Conf_Name_Server[k].Conf_Name_Server, "clicked", G_CALLBACK(set_bdd) , &Struct_Conf_Name_Server[k]);
//            gtk_widget_set_size_request(Struct_Conf_Name_Server[k].Conf_Name_Server, 10, 0);
        
        json_conf[k].nbr_bdd = n_Col_tab3;
        Struct_Conf_Name_Server[k].nbr_bdd = n_Col_tab3;
        Struct_Conf_Name_Server[k].widgConfb = widgConfb;
        Struct_Conf_Name_Server[k].Boxbdd = BoxBDD;
        Struct_Conf_Name_Server[k].boxJSON_Conf = boxJSON_Conf;
        if(g == 0)
            g++;
        for(j = 0; j < n_Col_tab3; j++){
            if(g){
//                struct Json_Conf_BDD Json_Conf_BDD[n_Col_tab3];
                json_conf[k].bdd = malloc(sizeof(struct Json_Conf_BDD)*n_Col_tab3);
//                json_conf[k].bdd[j]= &Json_Conf_BDD[j];
                g--;
            }
            
            parsed_Json_Conf_BDD_Buffer_idx = json_object_array_get_idx(parsed_Json_Conf_BDD_Buffer, j);
            json_object_object_get_ex(parsed_Json_Conf_BDD_Buffer_idx, "api_route", &json_conf[k].bdd[j].api_route);
            json_object_object_get_ex(parsed_Json_Conf_BDD_Buffer_idx, "schema", &json_conf[k].bdd[j].name_BDD);
            json_object_object_get_ex(parsed_Json_Conf_BDD_Buffer_idx, "port_api", &json_conf[k].bdd[j].PAPI);
            json_object_object_get_ex(parsed_Json_Conf_BDD_Buffer_idx, "port_mysql", &json_conf[k].bdd[j].PMYSQ);
            json_object_object_get_ex(parsed_Json_Conf_BDD_Buffer_idx, "pass_root", &json_conf[k].bdd[j].Pass_Root);
            json_object_object_get_ex(parsed_Json_Conf_BDD_Buffer_idx, "bdd_type", &json_conf[k].bdd[j].BDD_Type);
            json_object_object_get_ex(parsed_Json_Conf_BDD_Buffer_idx, "user_mysql", &json_conf[k].bdd[j].user);
            json_object_object_get_ex(parsed_Json_Conf_BDD_Buffer_idx, "pass_mysql", &json_conf[k].bdd[j].pass_User);
            json_object_object_get_ex(parsed_Json_Conf_BDD_Buffer_idx, "name_db", &json_conf[k].bdd[j].name_db);
            
            Struct_Conf_Name_Server[k].Conf_Name_bdd[j] = gtk_button_new_with_label(json_object_get_string(json_conf[k].bdd[j].name_BDD));
            g_signal_connect_swapped (Struct_Conf_Name_Server[k].Conf_Name_bdd[j], "clicked", G_CALLBACK(button_set_color) , &Struct_Conf_Name_Server[k].Conf_Name_bdd[j]);
//            g_signal_connect_swapped (Struct_Conf_Name_Server[k].Conf_Name_bdd[j], "clicked", G_CALLBACK(set_json_save) , &json_conf[k].IP);
            g_signal_connect (Struct_Conf_Name_Server[k].Conf_Name_bdd[j], "clicked", G_CALLBACK(set_json_save) , &json_conf[k]);
            Button_Connection.status = 0;
            g_signal_connect_swapped (Struct_Conf_Name_Server[k].Conf_Name_bdd[j], "clicked", G_CALLBACK(button_Connection) , &Button_Connection);
            
        }
    }
    
    //label du status de la connection
    Create_main_window1.LabelStatusConnection = gtk_label_new("");
    gtk_box_pack_start(GTK_BOX(boxLogin), Create_main_window1.LabelStatusConnection, TRUE, TRUE, 0);
    
//    Create_main_window1.Json_conf = json_conf;
    Create_main_window1.status_connection = &Button_Connection;
    gtk_box_pack_start(GTK_BOX(boxConnection),Button_Connection.connection_button_box, TRUE, TRUE, 0);
    g_signal_connect (Button_Connection.connection_button, "clicked", G_CALLBACK (main_windows_create), &Create_main_window1);

    
    
    //Création de la box du bouton déconnexion et ajout dans la box main
    button_box = gtk_button_box_new (GTK_ORIENTATION_HORIZONTAL);
    gtk_box_pack_start(GTK_BOX(boxConnection),button_box, TRUE, TRUE, 0);
    //Bouton déconnexion pour fermer le programme
    button = gtk_button_new_with_label ("Quitter");
    g_signal_connect_swapped (button, "clicked", G_CALLBACK (gtk_widget_destroy), window);
    gtk_container_add (GTK_CONTAINER (button_box), button);

    
    gtk_widget_show_all (window);
}

int main(int argc, char **argv){
    static GtkApplication *app;
    int status;
    
    if(check_file_conf())
        return 1;
    
    app = gtk_application_new ("org.gtk.example", G_APPLICATION_FLAGS_NONE);
    g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
    status = g_application_run (G_APPLICATION (app), argc, argv);
    g_object_unref (app);

    return status;
}

    

