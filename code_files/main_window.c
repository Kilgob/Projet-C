//
//  main_window.c
//  Projet
//
//  Created by Frédéric FLACELIERE on 25/11/2019.
//  Copyright © 2019 Frédéric FLACELIERE. All rights reserved.
//

#include "link_main.h"
#include "file_control.h"
#include "prgm_fork.h"
#include <stdint.h>

GtkWidget *window_Main;
pid_t pid = -1;

void unselect_array_names(struct WidgetBDD *Data){
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(Data->array_Name) , FALSE);
}

void desactivate_array(struct Recup_Widgets *Data){
    g_signal_handler_block(Data->array_Name, Data->idGsignalc);
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
//Fonction pour afficher ou non les colonnes d'une table
void Set_visibility_columns(struct Recup_Widgets *Data){
    GList *children, *children2 = NULL, *iter;
    children = gtk_container_get_children(GTK_CONTAINER(Data->Array_bdd));
    for(iter = children; iter != NULL; iter = g_list_next(iter)){
        gtk_widget_set_visible(iter->data, !gtk_widget_get_visible(iter->data));
    }
    g_list_free(children);
    g_list_free(children2);
    g_list_free(iter);
    
}


void main_windows_create(GtkWidget *widget, struct create_main_window *ForCreateMainWindow){
//    printf("nom du shéma sélectionné : %s\n", json_object_get_string(ForCreateMainWindow->Json_conf->bdd->name_BDD));
//    printf("Log SQLSTATUS : %d\n",ForCreateMainWindow->Login->returnStatusConnexion); //Debug du statut de connexion à la BDD
    if(ForCreateMainWindow->status_connection->status){
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
        GtkWidget *Information_Scroll_Box;
//        GtkWidget *box_Main;
        GtkWidget *Box_For_Button_Deco;
        GtkWidget *Button_For_Deco;
        GtkWidget *gridLeft;
        GtkWidget *gridRight;
        GtkWidget *gridMain;
        GtkWidget *button_Back = gtk_button_new_with_label("Retour"); //bouton permettant de revenir à la page de choix des schémas
        GtkWidget *button_Export;
        GtkWidget *button_Migration;
        GtkWidget *groupExport = gtk_box_new(FALSE, 1);
//        gtk_box_set_homogeneous(GTK_BOX(groupExport),FALSE);
        GtkWidget **screen_Select;
        
        GtkWidget *right_Block = gtk_box_new(FALSE, 10);
        GtkWidget *right_Block_Current_Task = gtk_box_new(FALSE, 10);
        
        //Blocks de widgets contenant les statuts des réplications en cours
        GtkWidget *Statut_Transfert_Block = gtk_box_new(TRUE, 10);
        GtkWidget *statut_Task_Block_Frame = gtk_frame_new(NULL);
        
        GtkWidget *frame_Left = gtk_frame_new(NULL);
        GtkWidget *frame_Right = gtk_frame_new(NULL);
        GtkWidget *frame_Right_Current_Task = gtk_frame_new(NULL);
        gtk_frame_set_shadow_type(GTK_FRAME(frame_Left), GTK_SHADOW_OUT);
        gtk_frame_set_shadow_type(GTK_FRAME(frame_Right), GTK_SHADOW_OUT);
        gtk_frame_set_shadow_type(GTK_FRAME(frame_Right_Current_Task), GTK_SHADOW_OUT);

        GtkWidget * Picture;
        Picture = gtk_image_new_from_file("/Users/fred/OneDrive/ESGI/Annee2/ProjetC/Oral/ferme-de-serveur.jpg");
        
        
        window_Main = gtk_application_window_new(ForCreateMainWindow->app);
        Information_Scroll_Box = gtk_scrolled_window_new(NULL, NULL);
        ForCreateMainWindow->window_Main = window_Main;
        
        gtk_widget_destroy(ForCreateMainWindow->oldWindow);
        gtk_window_set_title (GTK_WINDOW (window_Main), "gestion de la BDD");//Nom de la fenetre
        gtk_window_set_default_size (GTK_WINDOW (window_Main), 1000, 600);
        gtk_window_set_resizable(GTK_WINDOW(window_Main), FALSE);
        
        gridLeft = gtk_grid_new ();
        gridRight = gtk_grid_new ();
        gridMain = gtk_grid_new ();

        
        gtk_container_add (GTK_CONTAINER (frame_Left), Information_Scroll_Box);
        gtk_container_add (GTK_CONTAINER (Information_Scroll_Box), gridLeft);
        gtk_widget_set_size_request(Information_Scroll_Box, 750, 550);
        gtk_grid_attach(GTK_GRID(gridMain), frame_Left, 1, 100, 700, 500);
//        gtk_container_add (GTK_CONTAINER (left_Block), gridMain);
//        gtk_container_add (GTK_CONTAINER (frame_Left), left_Block);
        gtk_container_add (GTK_CONTAINER (window_Main), gridMain);//frame_Left
        
        mainPageLabel = gtk_label_new("Liste des tables et des colonnes de la base");
//        gtk_label_set_width_chars(GTK_LABEL(mainPageLabel), 50); //aucun impacte
        gtk_grid_attach(GTK_GRID(gridMain), mainPageLabel, 1, 1, 1, 100);

        GtkWidget *groupDatasJSON[n_Col_tab];
        
        struct Migration *Migration_Datas = malloc(sizeof(struct Migration));
        Migration_Datas->Json_conf_foa = ForCreateMainWindow->Json_conf_foa;
        
        //widget du retour JSON
        static struct WidgetBDD Widgets;
        
        Widgets.column_Key = malloc(sizeof(GtkWidget *) *n_Col_tab);
        Widgets.column_Name = malloc(sizeof(GtkWidget *) *n_Col_tab);
        Widgets.column_Type = malloc(sizeof(GtkWidget *) *n_Col_tab);
        Widgets.array_Name = malloc(sizeof(GtkWidget *) *n_Col_tab);
        Widgets.array_Schema = malloc(sizeof(GtkWidget *) *n_Col_tab);
        

        
        struct ExportData *Export;
        Export = malloc(sizeof(struct ExportData));

        //        GtkWidget *Array_bdd[n_Col_tab/5]; //pas optimale
        Widgets.nbr_Array = -1;
        
        struct Recup_Widgets *Recup_Widgets;
        Recup_Widgets = malloc(sizeof(struct Recup_Widgets)*n_Col_tab);
        Recup_Widgets->number_array = 0;
        int h = 0;
        
//        struct desactivate *Desactivate[2]; //struct permettant de bloquer le signal
//        Desactivate[0] = malloc(sizeof(struct desactivate)*n_Col_tab); //colonne
//        Desactivate[1] = malloc(sizeof(struct desactivate)*n_Col_tab); //table (peu etre inverse)
        
        screen_Select = malloc(sizeof(GtkWidget) * n_Col_tab);//widgets pour afficher les colonnes d'une tables
        //afficher chaque élément du tableau
        for (i = 0; i<n_Col_tab; i++) {
            
            groupDatasJSON[i] = gtk_box_new(FALSE, 80);
            char check_Name_Base[50];
            char check_Name_Table[50];
            arrayBase_buffer = json_object_array_get_idx(arraysBase, i);
//            printf("%lu. %s\n", i+1,json_object_get_string(arrayBase_buffer));
            json_object_object_get_ex(arrayBase_buffer, "TABLE_SCHEMA", &Table_Schema);
            if (json_object_get_string(Table_Schema) == NULL)
                json_object_object_get_ex(arrayBase_buffer, "table_schema", &Table_Schema);
            
            json_object_object_get_ex(arrayBase_buffer, "TABLE_NAME", &Table_Name);
            if (json_object_get_string(Table_Name) == NULL)
                json_object_object_get_ex(arrayBase_buffer, "table_name", &Table_Name);
            
            json_object_object_get_ex(arrayBase_buffer, "COLUMN_TYPE", &Column_Type);
            if (json_object_get_string(Column_Type) == NULL)
                json_object_object_get_ex(arrayBase_buffer, "data_type", &Column_Type);
            
            json_object_object_get_ex(arrayBase_buffer, "COLUMN_NAME", &Column_Name);
            if (json_object_get_string(Column_Name) == NULL)
                json_object_object_get_ex(arrayBase_buffer, "column_name", &Column_Name);
            
            json_object_object_get_ex(arrayBase_buffer, "COLUMN_KEY", &Column_Key);
            if (json_object_get_string(Column_Key) == NULL)
                json_object_object_get_ex(arrayBase_buffer, "is_identity", &Column_Key);

            
            //Les widgets d'affichage
            Widgets.array_Schema[i] = gtk_label_new(json_object_get_string(Table_Schema));
            Widgets.array_Name[i] = gtk_check_button_new_with_label(json_object_get_string(Table_Name));
            Widgets.column_Name[i] = gtk_check_button_new_with_label(json_object_get_string(Column_Name));
            Widgets.column_Type[i] = gtk_label_new(json_object_get_string(Column_Type));
            gtk_label_set_xalign(GTK_LABEL(Widgets.column_Type[i]), 0.0);
            Widgets.column_Key[i] = gtk_label_new(json_object_get_string(Column_Key));
            
            
            ++j; //Pour ne pas démarrer à 0
            if(strcmp(check_Name_Base, json_object_get_string(Table_Schema))){
                strcpy(check_Name_Base, json_object_get_string(Table_Schema));
                gtk_grid_attach (GTK_GRID (gridLeft), Widgets.array_Schema[i], 0, j++, 1, 2);
//                g_signal_connect (array_Schema[i], "clicked", G_CALLBACK(save_schema_selection) , &Data[i]);
            }
            
       
            if(strcmp(check_Name_Table, json_object_get_string(Table_Name))){
                strcpy(check_Name_Table, json_object_get_string(Table_Name));
                gtk_grid_attach (GTK_GRID (gridLeft), Widgets.array_Name[i], 1, j, 2, 1);
                Widgets.nbr_Array++;
                
                Recup_Widgets[Widgets.nbr_Array].idGsignala = malloc(sizeof(gulong) *n_Col_tab);
//                Recup_Widgets[Widgets.nbr_Array].Array_bdd = malloc(sizeof(GtkWidget *)*n_Col_tab);
                Recup_Widgets[Widgets.nbr_Array].Array_bdd = gtk_box_new(TRUE, 0);
                gtk_grid_attach(GTK_GRID (gridLeft), Recup_Widgets[Widgets.nbr_Array].Array_bdd, 2, ++j, 12, 1);
            
                
                screen_Select[i] = gtk_link_button_new_with_label("", "afficher");//bouton pour afficher ou non les colonnes
                gtk_grid_attach(GTK_GRID (gridLeft), screen_Select[i], 2, j -1, 12, 1); // j-1 pour revenir au même niveau que le nom de la table
                //signal permettant d'afficher ou non les colonnes
                g_signal_connect_swapped (screen_Select[i], "activate-link", G_CALLBACK(Set_visibility_columns) , &Recup_Widgets[Widgets.nbr_Array]);
                
                Recup_Widgets[Widgets.nbr_Array].array_Name = Widgets.array_Name[i];
//                Recup_Widgets[Widgets.nbr_Array].datas = &Array_bdd[Widgets.nbr_Array];
                Recup_Widgets->number_array++;//pour pouvoir boucler sur tous les éléments lors de l'export/migration
                
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
        g_signal_connect_swapped (Button_For_Deco, "clicked", G_CALLBACK (gtk_window_close), window_Main);
        
        //Création de la box du bouton déconnexion, ajout du bouton dans sa box et ajout dans la box main
        Box_For_Button_Deco = gtk_button_box_new (GTK_ORIENTATION_HORIZONTAL);
        gtk_box_pack_start(GTK_BOX(Box_For_Button_Deco),Button_For_Deco, TRUE, TRUE, 0);
        gtk_grid_attach (GTK_GRID (gridRight), Box_For_Button_Deco, 5, 60, 1, 1);

        
        Migration_Datas->Widgets = &Widgets;
        Migration_Datas->Json_conf = ForCreateMainWindow->Json_conf;
        //Migration
        button_Migration = gtk_button_new_with_label("Migrer");
        g_signal_connect_swapped (button_Migration, "clicked", G_CALLBACK (migration), Migration_Datas);
        
        Migration_Datas->label_Migration_Status = gtk_label_new("");
        Migration_Datas->Target_Schema = gtk_combo_box_text_new();
        
        
        Widgets.nbr_Column = i;
        
        
        j = 0;//pour le switch suivant (gtk_combo_box)
        char temp[2];//peut devenir une fonction
        for(i = 0; i < ForCreateMainWindow->Json_conf->nbr_server; i++){
            for(h = 0; h < ForCreateMainWindow->Json_conf[i].nbr_bdd; h++){
                sprintf(temp, "%d",j++); //convetir int en char pour la fonciton suivante
                if(strcmp(json_object_get_string(ForCreateMainWindow->Json_conf->bdd[ForCreateMainWindow->Json_conf->bdd_select].name_BDD), json_object_get_string(ForCreateMainWindow->Json_conf[i].bdd[h].name_BDD)))
                    gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(Migration_Datas->Target_Schema), temp, json_object_get_string(ForCreateMainWindow->Json_conf[i].bdd[h].name_BDD));
            }
        }

        gtk_grid_attach (GTK_GRID (gridRight), button_Migration, 2, 1, 1, 1);
        gtk_grid_attach (GTK_GRID (gridRight), Migration_Datas->Target_Schema, 3, 1, 1, 1);
        gtk_grid_attach (GTK_GRID (gridRight), Migration_Datas->label_Migration_Status, 10, 1, 1, 1);
        

        GtkWidget *statut_Task_Block_Scrollbar = gtk_scrolled_window_new(NULL, NULL);
        
        struct Block_Status *Block_Status_Replic = malloc(sizeof(Block_Status_Replic));
        Block_Status_Replic->statut_Task_Block = gtk_box_new(TRUE, 0);
        Block_Status_Replic->progress_Bar = gtk_progress_bar_new();
        Block_Status_Replic->label_Status_bar = gtk_label_new("Statut des réplications et liste :");
        
        GtkWidget *Box_Statut_Bar = gtk_box_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(Box_Statut_Bar), Block_Status_Replic->label_Status_bar, FALSE, FALSE, 0);
        gtk_box_pack_start(GTK_BOX(Box_Statut_Bar), Block_Status_Replic->progress_Bar, FALSE, FALSE, 0);
        
        gtk_box_pack_start(GTK_BOX(Statut_Transfert_Block), Box_Statut_Bar, FALSE, FALSE, 0);
        gtk_widget_set_size_request(Box_Statut_Bar, 100, 20);
        gtk_widget_set_size_request(Block_Status_Replic->progress_Bar, 1, 5);
        
        gtk_container_add(GTK_CONTAINER(statut_Task_Block_Scrollbar), Block_Status_Replic->statut_Task_Block);
        gtk_widget_set_size_request(statut_Task_Block_Scrollbar, 50, 150);
        gtk_container_add(GTK_CONTAINER(Statut_Transfert_Block), statut_Task_Block_Scrollbar);
//        gtk_widget_set_size_request(Block_Status_Replic->statut_Task_Block, 50, 200);
        gtk_container_add(GTK_CONTAINER(statut_Task_Block_Frame), Statut_Transfert_Block);
        gtk_widget_set_size_request(statut_Task_Block_Frame, 50, 150);
        gtk_grid_attach(GTK_GRID (gridMain), statut_Task_Block_Frame, 905, 450, 1, 1);//905, 300, 1, 1


        
        pid = fork(); //Process qui boucle pour envoyer les fichier d'import de manière asynchrone
        if(pid == 0){
            fork_import(json_object_get_string(ForCreateMainWindow->Json_conf->PJSON), json_object_get_string(ForCreateMainWindow->Json_conf->IP));
        }
        
        g_timeout_add_seconds(2, G_SOURCE_FUNC(block_status_get), Block_Status_Replic);//Pour la partie d'affichage des imports en cours
        
        
        
        
        
        //information pour l'export
        //bouton export
        Export->target_Type = gtk_combo_box_text_new();
        button_Export = gtk_button_new_with_label("Exporter la sélection");
        Export->label_Status = gtk_label_new("");

        g_signal_connect_swapped (button_Export, "clicked", G_CALLBACK (data_export), &Widgets);
        gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(Export->target_Type), "0", "sql");//0/1/2 car le switch attend du int
        gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(Export->target_Type), "1", "csv");
        gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(Export->target_Type), "2", "json");
        gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(Export->target_Type), "3", "xml");

        //Chemin d'export
        Export->target_Folder = gtk_entry_new();
        gtk_entry_set_placeholder_text(GTK_ENTRY(Export->target_Folder), "Chemin absolu du répertoire avec le nom du fichier sans extension");
        gtk_entry_set_text(GTK_ENTRY(Export->target_Folder), "/Users/fred/OneDrive/ESGI/Annee2/ProjetC/Files/export");
        
        //box_pack des widgets pour l'export
        gtk_box_pack_start(GTK_BOX(groupExport), button_Export, TRUE, TRUE, 0);
        gtk_box_pack_start(GTK_BOX(groupExport), Export->target_Folder, TRUE, TRUE, 0);
        gtk_box_pack_start(GTK_BOX(groupExport), Export->target_Type, TRUE, TRUE, 0);
        gtk_box_pack_start(GTK_BOX(groupExport), Export->label_Status, TRUE, TRUE, 5);
        gtk_widget_set_size_request(Export->target_Folder, 400, 5);
        
    
//        gtk_grid_attach(GTK_GRID (gridRight), groupExport, 1, 1, 20, 1);
        gtk_grid_attach(GTK_GRID (gridRight), groupExport, 2, 40, 20, 1);
        gtk_box_pack_start(GTK_BOX(right_Block), gridRight, TRUE, TRUE, 10);
        gtk_container_add(GTK_CONTAINER(frame_Right), right_Block);
        
        //bouton de retour sur la fenetre 1;
        gtk_grid_attach(GTK_GRID (gridRight), button_Back, 6, 60, 1, 1);
        g_signal_connect(button_Back, "clicked", G_CALLBACK(activate), ForCreateMainWindow);
        
        //Block contenant les tâches en cours (fork)
        gtk_container_add(GTK_CONTAINER(frame_Right_Current_Task), right_Block_Current_Task);
        gtk_grid_attach(GTK_GRID (gridMain), frame_Right_Current_Task, 950, 300, 1, 1);
        
        gtk_grid_attach(GTK_GRID (gridMain), frame_Right, 905, 400, 1, 1);
        
        Widgets.Export_Info = Export;
        
        
        gtk_widget_show_all (window_Main);
    }
    else
        gtk_label_set_text(GTK_LABEL(ForCreateMainWindow->LabelStatusConnection), "Choisissez un schéma dans une base de données");
    
}
