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
        struct json_object *Table_Schema;
        struct json_object *Column_Name;
        struct json_object *Column_Key;
        size_t n_Col_tab;
        size_t i;
        uint8_t t = 0;
        int j = 0;
        
        GtkWidget *window_Main;
        GtkWidget *mainPageLabel;
        GtkWidget *box_Main;
        GtkWidget *Box_For_Button_Deco;
        GtkWidget *Button_For_Deco;
        GtkWidget *grid;
    
        
        window_Main = gtk_application_window_new(ForCreateMainWindow->app);
        
        gtk_widget_destroy(ForCreateMainWindow->oldWindow);
        gtk_window_set_title (GTK_WINDOW (window_Main), "Window");
        gtk_window_set_default_size (GTK_WINDOW (window_Main), 1024, 768);
        grid = gtk_grid_new ();
        gtk_container_add (GTK_CONTAINER (window_Main), grid);
        //Box contenant les widgets
        box_Main = gtk_box_new(TRUE, 0);
        //gtk_container_add(GTK_CONTAINER(window_Main), box_Main);
        gtk_grid_attach (GTK_GRID (grid), box_Main, 0, 0, 1, 1);
        
        mainPageLabel = gtk_label_new("gestion de la BDD");//Nom de la fenetre
        
        
        n_Col_tab = json_object_array_length(arraysBase); //Renvoi un long (ce qui explique le size_t)
        //        printf("nombre de valeure dans le tableau : %lu.\n",n_Col_tab);
//        struct JSONReceiver JSONReceive{
//            table_Schema[n_Col_tab];
//            table_Name[n_Col_tab];
//            column_Name[n_Col_tab];
//            column_Type[n_Col_tab];
//            column_Key[n_Col_tab];
//        };

        //widget du retour JSON
        GtkWidget *table_Name[n_Col_tab];
        GtkWidget *column_Type[n_Col_tab];
        GtkWidget *table_Schema[n_Col_tab];
        GtkWidget *column_Name[n_Col_tab];
        GtkWidget *column_Key[n_Col_tab];
//        struct DataCopy Data[n_Col_tab];//Structure stockant toutes les données du JSON
        char check_Name_Base[50]={};
        char check_Name_Table[50]={};
        //afficher chaque élément du tableau
        for (i = 0; i<n_Col_tab; i++) {
            
            arrayBase_buffer = json_object_array_get_idx(arraysBase, i);
            //                    printf("%lu. %s\n", i+1,json_object_get_string(arrayBase_buffer));
            json_object_object_get_ex(arrayBase_buffer, "TABLE_NAME", &Table_name);
            json_object_object_get_ex(arrayBase_buffer, "COLUMN_TYPE", &Column_type);
            json_object_object_get_ex(arrayBase_buffer, "TABLE_SCHEMA", &Table_Schema);
            json_object_object_get_ex(arrayBase_buffer, "COLUMN_NAME", &Column_Name);
            json_object_object_get_ex(arrayBase_buffer, "COLUMN_KEY", &Column_Key);

            table_Schema[i] = gtk_label_new(json_object_get_string(Table_Schema));
            table_Name[i] = gtk_check_button_new_with_label(json_object_get_string(Table_name));
            column_Name[i] = gtk_check_button_new_with_label(json_object_get_string(Column_Name));
            column_Type[i] = gtk_label_new(json_object_get_string(Column_type));
            column_Key[i] = gtk_label_new(json_object_get_string(Column_Key));

            ++j;
            if(strcmp(check_Name_Base, json_object_get_string(Table_Schema))){
                strcpy(check_Name_Base, json_object_get_string(Table_Schema));
                gtk_grid_attach (GTK_GRID (grid), table_Schema[i], 0, j, 1, 2);
                ++j;
            }
            if(strcmp(check_Name_Table, json_object_get_string(Table_name))){
                strcpy(check_Name_Table, json_object_get_string(Table_name));
                gtk_grid_attach (GTK_GRID (grid), table_Name[i], 1, j, 2, 1);
            }


            gtk_grid_attach (GTK_GRID (grid), column_Name[i], 3, j, 2, 1);

            gtk_grid_attach (GTK_GRID (grid), column_Type[i], 5, j, 1, 1);

            gtk_grid_attach (GTK_GRID (grid), column_Key[i], 6, j, 1, 1);
            
            
//            strcpy(Data[i].tableName, json_object_get_string(Table_name));
//            strcpy(Data[i].tableType, json_object_get_string(Column_type));
            
            g_signal_connect (table_Name[i], "clicked", G_CALLBACK(save_selection) , table_Name[i]);
            
            //Fonction pour créer le JSON de requête à la BDD
            t = requestToBDD(json_object_get_string(Table_name),json_object_get_string(Column_type));
            printf("%d",t);
            
            
            
        };
        ++j;//Pour placer le bouton "Quitter" plus bas
         

        //Création du bouton de déconnexion et de son g_signal
        Button_For_Deco = gtk_button_new_with_label ("Quitter");
        g_signal_connect (Button_For_Deco, "clicked", G_CALLBACK (gtk_window_close),window_Main);
        
        //Création de la box du bouton déconnexion, ajout du bouton dans sa box et ajout dans la box main
        Box_For_Button_Deco = gtk_button_box_new (GTK_ORIENTATION_HORIZONTAL);
        gtk_box_pack_start(GTK_BOX(Box_For_Button_Deco),Button_For_Deco, TRUE, TRUE, 0);
//        gtk_container_add(GTK_CONTAINER(box_Main),Box_For_Button_Deco);
        gtk_grid_attach (GTK_GRID (grid), Box_For_Button_Deco, 0, j, 1, 1);
        gtk_widget_show_all (window_Main);
    }
    else
        gtk_label_set_text(GTK_LABEL(ForCreateMainWindow->LabelStatusConnection), "Connection refusée");
    
}
