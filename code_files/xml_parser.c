//
//  xml_parser.c
//  Projet
//
//  Created by Frédéric FLACELIERE on 07/12/2019.
//  Copyright © 2019 Frédéric FLACELIERE. All rights reserved.
//

#include "xml_parser.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>



typedef void (*fct_parcours_t)(xmlNodePtr);

void parcours_postfixe(xmlNodePtr noeud, fct_parcours_t f) {
    xmlNodePtr n;

    for (n = noeud; n != NULL; n = n->next) {
        if ((XML_ELEMENT_NODE == n->type) && (NULL != n->children)) {
            parcours_postfixe(n->children, f);
        }
        f(n);
    }
}

void afficher_noeud(xmlNodePtr noeud) {
    if (XML_ELEMENT_NODE == noeud->type) {
        xmlChar *chemin = xmlGetNodePath(noeud);
        if (NULL != noeud->children && XML_TEXT_NODE == noeud->children->type) {
            xmlChar *contenu = xmlNodeGetContent(noeud);
            printf("%s -> %s\n", chemin, contenu);
            xmlFree(contenu);
        } else {
            printf("%s\n", chemin);
        }
        xmlFree(chemin);
    }
}




void xml_to_text(){
    xmlDocPtr doc;
    xmlNodePtr racine;

    // Ouverture du fichier XML
    doc = xmlParseFile("/Users/fred/OneDrive/ESGI/Annee2/ProjetC/test.xml");
    if (doc == NULL) {
        fprintf(stderr, "Document XML invalide\n");
        printf("erreur");
    }
    // Récupération de la racine
    racine = xmlDocGetRootElement(doc);
    if (racine == NULL) {
        fprintf(stderr, "Document XML vierge\n");
        xmlFreeDoc(doc);
        printf("erreur");
    }
    printf("La racine du document est : %s\n", racine->name);
    
    // Parcours
    printf("Parcours postfixé :\n");
    parcours_postfixe(racine, afficher_noeud);
    // Libération de la mémoire
    xmlFreeDoc(doc);
    

    printf("done");
}




void text_to_xml(){
    static struct content Content;
    
    int result = -1;
    xmlDocPtr doc = NULL;       /* document pointer */
    xmlNodePtr root_node = NULL;/* node pointer */
    //char buff[256];
    xmlNodePtr node;

    
    //Creates a new document, a node and set it as a root node
    doc = xmlNewDoc(BAD_CAST "1.0");
    root_node = xmlNewNode(NULL, BAD_CAST "data");
    xmlDocSetRootElement(doc, root_node);

    
    //dat 1
    node = xmlNewChild(root_node, NULL, BAD_CAST "dat", NULL);
    //ligne 1
    strcpy(Content.name[0],"COLUMN_TYPE");//modifier l'envoi dans la structure avec des variables pour la création du xml
    strcpy(Content.content[0],"int(11)");//modifier l'envoi dans la structure avec des variables pour la création du xml
    strcpy(Content.nameAttribute[0],"test_attribut");//modifier l'envoi dans la structure avec des variables pour la création du xml
    strcpy(Content.attribute[0],"attribut");//modifier l'envoi dans la structure avec des variables pour la création du xml
    
    //ligne 2
    strcpy(Content.name[1],"COLUMN_NAME");//modifier l'envoi dans la structure avec des variables pour la création du xml
    strcpy(Content.content[1],"fred1");//modifier l'envoi dans la structure avec des variables pour la création du xml
    strcpy(Content.nameAttribute[1],"");//modifier l'envoi dans la structure avec des variables pour la création du xml
    strcpy(Content.attribute[1],"");//modifier l'envoi dans la structure avec des variables pour la création du xml
    
    result = ajouter_data(&Content, node, 2); //3e paramètre correspond au nombre de ligne
    
    
    //dat 2
    node = xmlNewChild(root_node, NULL, BAD_CAST "dat", NULL);
    //ligne 1
    strcpy(Content.name[0],"COLUMN_TYPE");//modifier l'envoi dans la structure avec des variables pour la création du xml
    strcpy(Content.content[0],"int(11)");//modifier l'envoi dans la structure avec des variables pour la création du xml
    strcpy(Content.nameAttribute[0],"test_attribut");//modifier l'envoi dans la structure avec des variables pour la création du xml
    strcpy(Content.attribute[0],"attribut");//modifier l'envoi dans la structure avec des variables pour la création du xml
    
    //ligne 2
    strcpy(Content.name[1],"COLUMN_NAME");//modifier l'envoi dans la structure avec des variables pour la création du xml
    strcpy(Content.content[1],"fred1");//modifier l'envoi dans la structure avec des variables pour la création du xml
    strcpy(Content.nameAttribute[1],"");//modifier l'envoi dans la structure avec des variables pour la création du xml
    strcpy(Content.attribute[1],"");//modifier l'envoi dans la structure avec des variables pour la création du xml
    
    result = ajouter_data(&Content, node, 2); //3e paramètre correspond au nombre de ligne

    // Affichage de l'arbre DOM tel qu'il est en mémoire
    xmlDocFormatDump(stdout, doc, 1);
    // Libération de la mémoire
    xmlFreeDoc(doc);
    
}




int ajouter_data(struct content *Content , xmlNodePtr root_node ,int size) {
    xmlNodePtr node;
    int i = 0;
    
    for(i =0; i< size; i++){

        // Création du noeud "produit"
        node = xmlNewChild(root_node, NULL, BAD_CAST Content->name[i], BAD_CAST Content->content[i]);

        //ajout de son attribut si différent de blanc
        if(strchr(Content->nameAttribute[i], ' ')){
            xmlNewProp(node, BAD_CAST Content->nameAttribute[i], BAD_CAST Content->attribute[i]);
        }
    }
    //xmladdchild();
    return 0;
}
