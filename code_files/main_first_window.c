#include "link_main.h"
#include "main_window.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <libxml/xlink.h>
//#include <cjson/json.h>
//#include <math.h>
#include <gtk/gtk.h>
//#include <iostream>



static void print_hello(GtkWidget *widget,gpointer data){
    g_print ("Hello World\n");
    g_print("%s",gtk_entry_get_text(data));
}



//static void test_connection(GtkWidget *widget, struct InputLogin *Data){
////    struct InputLogin *Data = datas;
////    struct InputLogin *Data = user_data;
//    printf("%s\n",gtk_entry_get_text(GTK_ENTRY(Data->InputTextIP)));
//    printf("%s\n",gtk_entry_get_text(GTK_ENTRY(Data->InputTextID)));
//    printf("%s\n",gtk_entry_get_text(GTK_ENTRY(Data->InputTextPass)));
//    }



static void activate (GtkApplication *app,gpointer user_data){
    static struct InputLogin Login1;
    static struct create_main_window Create_main_window1;
    GtkWidget *window;
    GtkWidget *button;
    GtkWidget *button_box;
    GtkWidget *boxLogin;
    GtkWidget *mainPageText;
    GtkWidget *boxConnection;

    
//    Create_main_window1.app = app;

    
    window = gtk_application_window_new (app);
    gtk_window_set_title (GTK_WINDOW (window), "Se connecter");
    gtk_window_set_default_size (GTK_WINDOW (window), 600, 400);

    //Box contenant les widgets
    boxConnection = gtk_box_new(TRUE, 0);
    gtk_container_add(GTK_CONTAINER(window), boxConnection);

    
    //Ajout des inputs pour la connection
    mainPageText = gtk_label_new("Se connecter à une BDD");
    
    boxLogin =gtk_box_new(TRUE, 0);
    gtk_container_add(GTK_CONTAINER(boxConnection),boxLogin);
    Login1.returnStatusConnexion = -1;
    Login1.InputTextIP = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(Login1.InputTextIP), "Saisissez l'IP");
    Login1.InputTextID = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(Login1.InputTextID), "Saisissez l' identifiant");
    Login1.InputTextPass = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(Login1.InputTextPass), "Saisissez le mot de passe");
    gtk_entry_set_visibility(GTK_ENTRY(Login1.InputTextPass), FALSE);//Pour cacher les caractères saisies
//    gtk_entry_get_text ()
    Login1.InputValidate = gtk_button_new_with_label("Se connecter");
    Login1.button_box_login = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_container_add (GTK_CONTAINER (Login1.button_box_login), Login1.InputValidate);
    
    gtk_box_pack_start(GTK_BOX(boxLogin), Login1.InputTextIP, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(boxLogin), Login1.InputTextID, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(boxLogin), Login1.InputTextPass, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(boxLogin), Login1.button_box_login, TRUE, TRUE, 0);
    //gtk_box_pack_start(GTK_BOX(boxLogin), returnStatusConnexion, TRUE, TRUE, 0);
    
    //Stockage des adresses mémoires des inputs pour l'envois des datas
//    gpointer *dataLogIn = g_new(gpointer,3);
//    dataLogIn[0] = InputTextIP;
//    dataLogIn[1] = InputTextID;
//    dataLogIn[2] = InputTextPass;
    Create_main_window1.app = app;
    Create_main_window1.oldWindow = GTK_WIDGET(window);
    Create_main_window1.logSQLStatus = &Login1.returnStatusConnexion;
    g_signal_connect (Login1.InputValidate, "clicked", G_CALLBACK(connection_bdd), &Login1); //Envois de la structure de login pour la connecxion à la BDD
//    printf("%d",Login1.returnStatusConnexion);

    g_signal_connect (Login1.InputValidate, "clicked", G_CALLBACK(main_windows_create) , &Create_main_window1);

        
    //ajout du label dans la box
    gtk_box_pack_start(GTK_BOX(boxConnection), mainPageText, TRUE, TRUE, 0);


    //Création de la box du bouton déconnexion et ajout dans la box main
    button_box = gtk_button_box_new (GTK_ORIENTATION_HORIZONTAL);
    gtk_box_pack_start(GTK_BOX(boxConnection),button_box, TRUE, TRUE, 0);

    
    //Bouton déconnexion pour fermer le programme
    button = gtk_button_new_with_label ("Quitter");
    g_signal_connect (button, "clicked", G_CALLBACK (print_hello),Login1.InputTextID);
    g_signal_connect_swapped (button, "clicked", G_CALLBACK (gtk_widget_destroy), window);

    //Ajout du bouton dans le bouton box
    gtk_container_add (GTK_CONTAINER (button_box), button);
    gtk_widget_show_all (window);
}

int main(int argc, char **argv){
    static GtkApplication *app;
    int status;
    
    app = gtk_application_new ("org.gtk.example", G_APPLICATION_FLAGS_NONE);
    g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
    status = g_application_run (G_APPLICATION (app), argc, argv);
    g_object_unref (app);

    return status;
}

    

