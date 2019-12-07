//
//  xml_parser.h
//  Projet
//
//  Created by Frédéric FLACELIERE on 07/12/2019.
//  Copyright © 2019 Frédéric FLACELIERE. All rights reserved.
//

struct content{
    char name[2][50];
    char content[2][50];
    char nameAttribute[2][50];
    char attribute[2][50];
};

#ifndef xml_parser_h
#define xml_parser_h


#include <libxml/parser.h>
#include <libxml/tree.h>

void xml_to_text(void);
void text_to_xml(void);
int ajouter_data(struct content *content , xmlNodePtr xmlNodePtr, int size);
#endif /* xml_parser_h */
