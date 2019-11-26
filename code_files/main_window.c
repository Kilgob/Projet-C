//
//  main_window.c
//  Projet
//
//  Created by Frédéric FLACELIERE on 25/11/2019.
//  Copyright © 2019 Frédéric FLACELIERE. All rights reserved.
//

#include "main_window.h"
#include "link_main.h"


//void destroy_Connection_Window(GtkWidget *widget,gpointer data){
//    (void)(G_CALLBACK(gtk_widget_destroy)), data;
//}

void main_windows_create(GtkWidget *widget, struct create_main_window *ForCreateMainWindow){
    GtkWidget *window_Main;
    GtkWidget *mainPageLabel;
    GtkWidget *box_Main;
    GtkWidget *Box_For_Button_Deco;
    GtkWidget *Button_For_Deco;
    

    printf("Log SQLSTATUS : %d\n",ForCreateMainWindow->Login->returnStatusConnexion);
    if(ForCreateMainWindow->Login->returnStatusConnexion){
        window_Main = gtk_application_window_new (ForCreateMainWindow->app);
        
        gtk_widget_destroy(ForCreateMainWindow->oldWindow);
        gtk_window_set_title (GTK_WINDOW (window_Main), "Window");
        gtk_window_set_default_size (GTK_WINDOW (window_Main), 1024, 768);

        //Box contenant les widgets
        box_Main = gtk_box_new(TRUE, 0);
        gtk_container_add(GTK_CONTAINER(window_Main), box_Main);

        mainPageLabel = gtk_label_new("gestion de la BDD");//Nom de la fenetre
        
        
        //Création du bouton et de son g_signal
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
