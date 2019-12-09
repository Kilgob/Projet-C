//
//  main_window.c
//  Projet
//
//  Created by Frédéric FLACELIERE on 25/11/2019.
//  Copyright © 2019 Frédéric FLACELIERE. All rights reserved.
//

#include "link_main.h"
#include "file_control.h"
#include <stdint.h>

//void destroy_Connection_Window(GtkWidget *widget,gpointer data){
//    (void)(G_CALLBACK(gtk_widget_destroy)), data;
//}


void main_windows_create(GtkWidget *widget, struct create_main_window *ForCreateMainWindow){
    
//    printf("Log SQLSTATUS : %d\n",ForCreateMainWindow->Login->returnStatusConnexion); //Debug du statut de connexion à la BDD
    if(ForCreateMainWindow->Login->returnStatusConnexion){
        
//        struct json_object *arraysBase = get_Arrays_Base();
        static struct json_object *arraysBase;
        get_Arrays_Base(&arraysBase);
        
        
        struct json_object *arrayBase_buffer;
        struct json_object *Table_name;
        struct json_object *Column_type;
        size_t n_Col_tab;
        size_t i;
        uint8_t t = 0;
        
        GtkWidget *window_Main;
        GtkWidget *mainPageLabel;
        GtkWidget *box_Main;
        GtkWidget *Box_For_Button_Deco;
        GtkWidget *Button_For_Deco;
    
        
        window_Main = gtk_application_window_new (ForCreateMainWindow->app);
        
        gtk_widget_destroy(ForCreateMainWindow->oldWindow);
        gtk_window_set_title (GTK_WINDOW (window_Main), "Window");
        gtk_window_set_default_size (GTK_WINDOW (window_Main), 1024, 768);

        //Box contenant les widgets
        box_Main = gtk_box_new(TRUE, 0);
        gtk_container_add(GTK_CONTAINER(window_Main), box_Main);

        mainPageLabel = gtk_label_new("gestion de la BDD");//Nom de la fenetre
        
        
        n_Col_tab = json_object_array_length(arraysBase); //Renvoi un long (ce qui explique le size_t)
        //        printf("nombre de valeure dans le tableau : %lu.\n",n_Col_tab);
        //widget du retour JSON
        GtkWidget *table_Name[n_Col_tab];
        GtkWidget *column_Type[n_Col_tab];
//        struct DataCopy Data[n_Col_tab];//Structure stockant toutes les données du JSON
        
        //afficher chaque élément du tableau
        for (i = 0; i<n_Col_tab; i++) {

            arrayBase_buffer = json_object_array_get_idx(arraysBase, i);
//                    printf("%lu. %s\n", i+1,json_object_get_string(arrayBase_buffer));
            json_object_object_get_ex(arrayBase_buffer, "TABLE_NAME", &Table_name);
            json_object_object_get_ex(arrayBase_buffer, "COLUMN_TYPE", &Column_type);
            
            table_Name[i] = gtk_check_button_new_with_label(json_object_get_string(Table_name));
            column_Type[i] = gtk_check_button_new_with_label(json_object_get_string(Column_type));
            gtk_box_pack_start(GTK_BOX(box_Main), table_Name[i], TRUE, TRUE, 0);
            gtk_box_pack_start(GTK_BOX(box_Main), column_Type[i], TRUE, TRUE, 0);
            
//            strcpy(Data[i].tableName, json_object_get_string(Table_name));
//            strcpy(Data[i].tableType, json_object_get_string(Column_type));
            
            g_signal_connect (table_Name[i], "clicked", G_CALLBACK(save_selection) , table_Name[i]);
            
            //Fonction pour créer le JSON de requête à la BDD
            t = requestToBDD(json_object_get_string(Table_name),json_object_get_string(Column_type));
            printf("%d",t);
            
            
            
        };
         
        

        //Création du bouton de déconnexion et de son g_signal
        Button_For_Deco = gtk_button_new_with_label ("Quitter");
        g_signal_connect (Button_For_Deco, "clicked", G_CALLBACK (gtk_window_close),window_Main);
        
        //Création de la box du bouton déconnexion, ajout du bouton dans sa box et ajout dans la box main
        Box_For_Button_Deco = gtk_button_box_new (GTK_ORIENTATION_HORIZONTAL);
        gtk_box_pack_start(GTK_BOX(Box_For_Button_Deco),Button_For_Deco, TRUE, TRUE, 0);
        gtk_container_add(GTK_CONTAINER(box_Main),Box_For_Button_Deco);
        
        gtk_widget_show_all (window_Main);
    }
    else
        gtk_label_set_text(GTK_LABEL(ForCreateMainWindow->LabelStatusConnection), "Connection refusée");
    
}
