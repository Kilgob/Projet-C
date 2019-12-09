//
//  file_control.c
//  Projet
//
//  Created by Frédéric FLACELIERE on 09/12/2019.
//  Copyright © 2019 Frédéric FLACELIERE. All rights reserved.
//
#include <stdio.h>
#include <string.h>
#include <gtk/gtk.h>

#include "file_control.h"

 //ajouter le if pour vérifier l'exécution (label erreur)
int save_selection(GtkWidget *Data){
    FILE *fileSave;
    
    //ajouter le if pour créer le fichier si il n'existe pas
    fileSave = fopen("/Users/fred/OneDrive/ESGI/Annee2/ProjetC/Files/save_select", "a");
    
    if(fileSave == NULL)
        return 1;
    fprintf(fileSave, " %s",gtk_button_get_label(GTK_BUTTON(Data)));
    
    fclose(fileSave);
    return 0;
};

 //ajouter le if pour vérifier l'exécution (label erreur)
int modified_selection(GtkWidget *Data){
    FILE *fileSave;
    
    fileSave = fopen("/Users/fred/OneDrive/ESGI/Annee2/ProjetC/Files/save_select", "r+");
    
    if(fileSave == NULL)
        return 1;
    
    /*code pour supprimer le texte existant*/
    
    fclose(fileSave);
    return 0;
}
