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

GtkWidget *window_Main;

void unselect_array_names(struct WidgetBDD *Data){
//    GList *children, *children2 = NULL, *iter;
    //    printf("\n%p",Data);
//        children = gtk_container_get_children(GTK_CONTAINER(Data->Array_bdd));
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(Data->array_Name) , FALSE);
//    printf("\ndéselection du tableau\n");
//    gtk_widget_show(window_Main);
}

void desactivate_array(struct Recup_Widgets *Data){
//    GList *children;
//    children = gtk_container_get_children(GTK_CONTAINER(Data->array_Name));
//    printf("nom table : %s\n", gtk_button_get_label(GTK_BUTTON(Data->array_Name)));
    g_signal_handler_block(Data->array_Name, Data->idGsignalc);
//    printf("nom table verif : %s\n", gtk_button_get_label(GTK_BUTTON(Data->array_Name)));
//    g_list_free(children);
}
void reactivate_array(struct Recup_Widgets *Data){
    g_signal_handler_unblock(Data->array_Name, Data->idGsignalc);
}

void desactivate_col(struct Recup_Widgets *Data){
    GList *children, *children2 = NULL, *iter;
    int h = 0;
//    printf("\n%p",Data);
    children = gtk_container_get_children(GTK_CONTAINER(Data->Array_bdd));
    for(iter = children; iter != NULL; iter = g_list_next(iter)){
        children2 = gtk_container_get_children(GTK_CONTAINER(iter->data));
        if(GTK_IS_TOGGLE_BUTTON(children2->data)){
//            printf("il y a un toggle button : %s",gtk_button_get_label(children2->data));
            g_signal_handler_block(children2->data, Data->idGsignala[h++]);
        }
    }
    g_list_free(children);
    g_list_free(children2);
    g_list_free(iter);
}
void reactivate_col(struct Recup_Widgets *Data){
        GList *children, *children2 = NULL, *iter;
    //    printf("\n%p",Data);
    int h = 0;
        children = gtk_container_get_children(GTK_CONTAINER(Data->Array_bdd));
        for(iter = children; iter != NULL; iter = g_list_next(iter)){
            children2 = gtk_container_get_children(GTK_CONTAINER(iter->data));
            if(GTK_IS_TOGGLE_BUTTON(children2->data))
                g_signal_handler_unblock(children2->data, Data->idGsignala[h++]);
        }
        g_list_free(children);
        g_list_free(children2);
        g_list_free(iter);
        
}

void main_windows_create(GtkWidget *widget, struct create_main_window *ForCreateMainWindow){
    
//    printf("Log SQLSTATUS : %d\n",ForCreateMainWindow->Login->returnStatusConnexion); //Debug du statut de connexion à la BDD
    if(ForCreateMainWindow->returnStatusConnexion){
//        printf("%p",ForCreateMainWindow->Json_conf);
//        printf("%s",json_object_get_string(ForCreateMainWindow->Json_conf->IP));
//        struct json_object *arraysBase = get_Arrays_Base();
        static struct json_object *arraysBase;
        get_Arrays_Base(&arraysBase, ForCreateMainWindow->Json_conf);
        
        
        struct json_object *arrayBase_buffer;
        struct json_object *Table_Name;
        struct json_object *Column_Type;
        struct json_object *Table_Schema;
        struct json_object *Column_Name;
        struct json_object *Column_Key;
        const size_t n_Col_tab = json_object_array_length(arraysBase);
        int i;
//        uint8_t t = 0;
        int j = 1;
        
//        GtkWidget *window_Main;
        GtkWidget *mainPageLabel;
//        GtkWidget *box_Main;
        GtkWidget *Box_For_Button_Deco;
        GtkWidget *Button_For_Deco;
        GtkWidget *grid;
        GtkWidget *button_Export;
        GtkWidget *button_Migration;
        GtkWidget *groupExport = gtk_box_new(FALSE, 1);
        
        window_Main = gtk_application_window_new(ForCreateMainWindow->app);
        
        gtk_widget_destroy(ForCreateMainWindow->oldWindow);
        gtk_window_set_title (GTK_WINDOW (window_Main), "Window");
        gtk_window_set_default_size (GTK_WINDOW (window_Main), 1024, 768);
        grid = gtk_grid_new ();
        gtk_container_add (GTK_CONTAINER (window_Main), grid);
        //Box contenant les widgets
//        box_Main = gtk_box_new(TRUE, 1);
        //gtk_container_add(GTK_CONTAINER(window_Main), box_Main);
//        gtk_grid_attach (GTK_GRID (grid), box_Main, 0, 0, 1, 1);
        
        mainPageLabel = gtk_label_new("gestion de la BDD");//Nom de la fenetre
        
        
//        n_Col_tab = json_object_array_length(arraysBase); //Renvoi un long (ce qui explique le size_t)
        //        printf("nombre de valeure dans le tableau : %lu.\n",n_Col_tab);

        GtkWidget *groupDatasJSON[n_Col_tab];
        //widget du retour JSON
        static struct WidgetBDD Widgets;
        
        Widgets.column_Key = malloc(sizeof(GtkWidget *) *n_Col_tab);
        Widgets.column_Name = malloc(sizeof(GtkWidget *) *n_Col_tab);
        Widgets.column_Type = malloc(sizeof(GtkWidget *) *n_Col_tab);
        Widgets.array_Name = malloc(sizeof(GtkWidget *) *n_Col_tab);
        Widgets.array_Schema = malloc(sizeof(GtkWidget *) *n_Col_tab);
        *Widgets.array_NameBis = malloc(sizeof(char) *n_Col_tab);
        
        struct JSONReceiver *Data/*[n_Col_tab]*/;//Structure stockant toutes les données du JSON
        Data = malloc(sizeof(struct JSONReceiver)*n_Col_tab);
        
        static struct ExportData Export;
        Export.JSONDatas = Data;
        
//        GtkWidget *Array_bdd[n_Col_tab/5]; //pas optimale
        Widgets.nbr_Array = -1;
//        Widgets.nbr_Column = -1;
        
        struct Recup_Widgets *Recup_Widgets;
        Recup_Widgets = malloc(sizeof(struct Recup_Widgets)*n_Col_tab);
        Recup_Widgets->number_array = 0;
        int h = 0;
        
        struct desactivate *Desactivate[2]; //struct permettant de bloquer le signal
        Desactivate[0] = malloc(sizeof(struct desactivate)*n_Col_tab);
        Desactivate[1] = malloc(sizeof(struct desactivate)*n_Col_tab);
        //afficher chaque élément du tableau
        for (i = 0; i<n_Col_tab; i++) {
            
            groupDatasJSON[i] = gtk_box_new(FALSE, 80);
            char check_Name_Base[50];
            char check_Name_Table[50];
            uint16_t cnbs = 0;
            uint16_t cnts = 0;
            arrayBase_buffer = json_object_array_get_idx(arraysBase, i);
//            printf("%lu. %s\n", i+1,json_object_get_string(arrayBase_buffer));
            json_object_object_get_ex(arrayBase_buffer, "TABLE_NAME", &Table_Name);
            json_object_object_get_ex(arrayBase_buffer, "COLUMN_TYPE", &Column_Type);
            json_object_object_get_ex(arrayBase_buffer, "TABLE_SCHEMA", &Table_Schema);
            json_object_object_get_ex(arrayBase_buffer, "COLUMN_NAME", &Column_Name);
            json_object_object_get_ex(arrayBase_buffer, "COLUMN_KEY", &Column_Key);

            
            //Les widgets d'affichage
            Widgets.array_Schema[i] = gtk_label_new(json_object_get_string(Table_Schema));
            Widgets.array_Name[i] = gtk_check_button_new_with_label(json_object_get_string(Table_Name));
            Widgets.column_Name[i] = gtk_check_button_new_with_label(json_object_get_string(Column_Name));
            Widgets.column_Type[i] = gtk_label_new(json_object_get_string(Column_Type));
            gtk_label_set_xalign(GTK_LABEL(Widgets.column_Type[i]), 0.0);
            Widgets.column_Key[i] = gtk_label_new(json_object_get_string(Column_Key));

            //Le stockage des datas par rapport aux widgets
            strcpy(Data[i].table_Schema, json_object_get_string(Table_Schema));
            strcpy(Data[i].table_Name, json_object_get_string(Table_Name));
            strcpy(Data[i].column_Name, json_object_get_string(Column_Name));
            strcpy(Data[i].column_Type, json_object_get_string(Column_Type));
            strcpy(Data[i].column_Key, json_object_get_string(Column_Key));
            
            
            
//            printf("%lu",idGsignal);
            
            ++j; //Pour ne pas démarrer à 0
            if(strcmp(check_Name_Base, json_object_get_string(Table_Schema))){
                strcpy(check_Name_Base, json_object_get_string(Table_Schema));
                gtk_grid_attach (GTK_GRID (grid), Widgets.array_Schema[i], 0, j++, 1, 2);
                cnbs = i;
                Data[i].i = (int)i;
                Data[i].cnb = 0;
//                g_signal_connect (array_Schema[i], "clicked", G_CALLBACK(save_schema_selection) , &Data[i]);
            }
            Data[cnbs].cnb++;// Permet de savoir le nombre de fois que l'on doit boucler pour la copie
            

            

            
            
            if(strcmp(check_Name_Table, json_object_get_string(Table_Name)) != 0){
                strcpy(check_Name_Table, json_object_get_string(Table_Name));
                gtk_grid_attach (GTK_GRID (grid), Widgets.array_Name[i], 1, j, 2, 1);
                cnts = i;
                Data[i].i = (int)i;
                Data[i].cnt = 0;
                Widgets.nbr_Array++;
                
                
                Recup_Widgets[Widgets.nbr_Array].idGsignala = malloc(sizeof(gulong) *n_Col_tab);
//                Recup_Widgets[Widgets.nbr_Array].Array_bdd = malloc(sizeof(GtkWidget *)*n_Col_tab);
                Recup_Widgets[Widgets.nbr_Array].Array_bdd = gtk_box_new(TRUE, 0);
                gtk_grid_attach(GTK_GRID (grid), Recup_Widgets[Widgets.nbr_Array].Array_bdd, 2, ++j, 12, 1);
            
//                strcpy(Widgets.array_NameBis[i], json_object_get_string(Table_Name));
                
                Recup_Widgets[Widgets.nbr_Array].array_Name = Widgets.array_Name[i];
//                Recup_Widgets[Widgets.nbr_Array].datas = &Array_bdd[Widgets.nbr_Array];
                Recup_Widgets->number_array++;//pour pouvoir boucler sur tous les éléments lors de l'export
                
//                Desactivate[1][Widgets.nbr_Array].name = &Widgets.array_Name[i];
                g_signal_connect_swapped (Widgets.array_Name[i], "clicked", G_CALLBACK(desactivate_col) , &Recup_Widgets[Widgets.nbr_Array]);
                
                Recup_Widgets[Widgets.nbr_Array].idGsignalc = g_signal_connect_swapped (Widgets.array_Name[i], "clicked", G_CALLBACK(save_table_selection) , &Recup_Widgets[Widgets.nbr_Array]);
                g_signal_connect_swapped (Widgets.array_Name[i], "clicked", G_CALLBACK(reactivate_col) , &Recup_Widgets[Widgets.nbr_Array]);
                
                h = 0;
            }
            
            g_signal_connect_swapped (Widgets.column_Name[i], "clicked", G_CALLBACK(desactivate_array) , &Recup_Widgets[Widgets.nbr_Array]);
            
            Recup_Widgets[Widgets.nbr_Array].idGsignala[h++] = g_signal_connect_swapped (Widgets.column_Name[i], "clicked", G_CALLBACK(unselect_array_names), &Recup_Widgets[Widgets.nbr_Array]);
//            Recup_Widgets[Widgets.nbr_Array].idGsignala = temp;
            g_signal_connect_swapped (Widgets.column_Name[i], "clicked", G_CALLBACK(reactivate_array) , &Recup_Widgets[Widgets.nbr_Array]);
            
            
            
            
            
            Data[cnts].cnt++;// Permet de savoir le nombre de fois que l'on doit boucler pour la copie
            
            gtk_box_pack_start(GTK_BOX(groupDatasJSON[i]), Widgets.column_Name[i], FALSE, TRUE, 0);
            gtk_box_pack_start(GTK_BOX(groupDatasJSON[i]), Widgets.column_Type[i], FALSE, TRUE, 0);
            gtk_box_pack_start(GTK_BOX(groupDatasJSON[i]), Widgets.column_Key[i], FALSE, TRUE, 0);
            gtk_box_set_homogeneous(GTK_BOX(groupDatasJSON[i]), TRUE);
            gtk_container_add(GTK_CONTAINER(Recup_Widgets[Widgets.nbr_Array].Array_bdd), groupDatasJSON[i]);

            
            
            

        };       
//        printf("%p",&Data[0]);
        ++j;//Pour placer le bouton "Quitter" sur la ligne suivante
         

        //Création du bouton de déconnexion et de son g_signal
        Button_For_Deco = gtk_button_new_with_label ("Quitter");
        g_signal_connect (Button_For_Deco, "clicked", G_CALLBACK (gtk_window_close),window_Main);
        
        //Création de la box du bouton déconnexion, ajout du bouton dans sa box et ajout dans la box main
        Box_For_Button_Deco = gtk_button_box_new (GTK_ORIENTATION_HORIZONTAL);
        gtk_box_pack_start(GTK_BOX(Box_For_Button_Deco),Button_For_Deco, TRUE, TRUE, 0);
//        gtk_container_add(GTK_CONTAINER(box_Main),Box_For_Button_Deco);
        gtk_grid_attach (GTK_GRID (grid), Box_For_Button_Deco, 0, j, 1, 1);
        
        //bouton export
        Export.cb_Export = gtk_combo_box_text_new();
        button_Export = gtk_button_new_with_label("Exporter la sélection");
//        printf("\n%d",Recup_Widgets->number_array);
        Widgets.nbr_Column = i;
        g_signal_connect_swapped (button_Export, "clicked", G_CALLBACK (data_export), &Widgets); //ne passe pas la structure...
        gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(Export.cb_Export), "0", "JSON");//0/1/2 car le switch attend du int
        gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(Export.cb_Export), "1", "XML");
        gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(Export.cb_Export), "2", "MLD");
        
        gtk_box_pack_start(GTK_BOX(groupExport), button_Export, TRUE, TRUE, 0);
        gtk_box_pack_start(GTK_BOX(groupExport), Export.cb_Export, TRUE, TRUE, 0);
        gtk_grid_attach(GTK_GRID (grid), groupExport, 0, 300, 50, 50);
        
        //bouton migration
//        button_Migration = gtk_button_new_with_label("Exporter la sélection");
//        g_signal_connect (button_Migration, "clicked", G_CALLBACK (datas_Migration_base), &Widgets);
//        gtk_box_pack_start(GTK_BOX(groupExport), button_Migration, TRUE, TRUE, 0);
        
        gtk_widget_show_all (window_Main);
    }
    else
        gtk_label_set_text(GTK_LABEL(ForCreateMainWindow->LabelStatusConnection), "Connection refusée");
    
}
