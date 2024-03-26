#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "api.h"
#include "structure.h"

Noeud *racine = NULL;

void *getRootTree() {
    // On suppose que l'arbre est construit et qu'on a déjà appelé la fonctin parseur donc on renvoie juste la racine
    return racine;
}

_Token *searchTree(void *start,char *name) {
   // Dans cette fonction on va parcourir l'arbre à partir du noeud start et on va chercher tous les noeuds dont l'étiquette est égale à name
   // On va utiliser une pile pour parcourir l'arbre
    pile *p = malloc(sizeof(pile));
    _Token *liste = NULL;

    // On initialise la pile
    p->indice = 0;
    p->suivant = NULL;
    if (start == NULL) {
        p->noeud = racine;
    } else {
        p->noeud = (Noeud *) start;
    }

    // On parcourt la pile jusqu'à ce qu'elle soit vide
    while (p != NULL) {
        if (p->indice < p->noeud->nombreFils) {
            pile *nouveau = malloc(sizeof(pile));
            nouveau->noeud = &(p->noeud->fils[p->indice]);
            nouveau->indice = 0;
            nouveau->suivant = p;
            p = nouveau;
        } else {
            if (p->noeud->tag != NULL && strcmp(p->noeud->tag, name) == 0) {
                _Token *nouveau = malloc(sizeof(_Token));
                nouveau->node = p->noeud;
                nouveau->next = liste;
                liste = nouveau;
            }
            pile *tmp = p;
            p = p->suivant;
            free(tmp);
            if (p != NULL) {
                p->indice++;
            }
        }
    }

    return liste;
}

char *getElementTag(void *node,int *len) {
    Noeud *n = (Noeud *) node;
    if (len != NULL) {
        *len = strlen(n->tag);
    }
    return n->tag;
}

char *getElementValue(void *node,int *len) {
    Noeud *n = (Noeud *) node;
    if (len != NULL) {
        *len = n->longueur;
    }
    return n->valeur;
}

void purgeElement(_Token **r) {
    _Token *tmp = *r;
    while (tmp != NULL) {
        _Token *suivant = tmp->next;
        free(tmp);
        tmp = suivant;
    }
    *r = NULL;
}

void purgeTree(void *root) {
    // On parcourt l'arbre en profondeur et on libère chaque noeud
    pile *p = malloc(sizeof(pile));
    p->noeud = (Noeud *) root;
    p->indice = 0;
    p->suivant = NULL;

    while (p != NULL) {
        if (p->indice < p->noeud->nombreFils) {
            pile *nouveau = malloc(sizeof(pile));
            nouveau->noeud = &(p->noeud->fils[p->indice]);
            nouveau->indice = 0;
            nouveau->suivant = p;
            p = nouveau;
        } else {
            pile *tmp = p;
            p = p->suivant;
            free(tmp->noeud->tag);
            free(tmp->noeud->valeur);
            free(tmp);
            if (p != NULL) {
                p->indice++;
            }
        }
    }
}

 int parseur(requete[], int longueur){
    Noeud *racine=malloc(sizeof(Noeud));
    racine->valeur = requete;
    racine->longueur = strlen(requete);
    racine->tag = "requête";

    int i=0;
    int fin = checkCRLF(requete, longueur, i); //vaut indice où est le CRLF
    if (fin==0){
        return 0;
    }
    Noeud *start = malloc(sizeof(Noeud));
    if (!checkStartLine(requete, &i, (fin+2), start)){ //+2 car le CRLF est dans la start line
        return 0;
    }
    fin = checkCRLF(requete, longueur, i);
    int *tabHeader = malloc(sizeof(int));
    int nombreHeader = compteHeader(requete, i, longueur, tabHeader);
    racine->fils = malloc((nombreHeader+1) * sizeof(Noeud));
    racine->fils[0]=start; 
    racine->nombrefils=nombreHeader+1;
    
    for (int j=1; j<CompteHeader; j++){
        switch(header[j]){
            case 1 :
                checkConnectionHeader(requete, &i, longueur, &racine->fils[j]);
                break;
            
            case 2 :
                checkContentLengthHeader(requete, &i, longueur, &racine->fils[j]);
                break;

            case 3 : 
                checkContentTypeHeader(requete, &i, longueur,  &racine->fils[j]);
                break;
            
            case 4 :
                checkCookieHeader(requete, &i, longueur,  &racine->fils[j]);
                break;
        
            case 5 :
                checkTransferEncodingHeader(requete, &i, longueur,  &racine->fils[j]);
                break;

            case 6 :
                checkExpectHeader(requete, &i, longueur,  &racine->fils[j]);
                break;
            
            case 7 : 
                checkHostHeader(requete, &i, fin,  &racine->fils[j]);
                break;
            default :
                printf("erreur header non identifié à partir de l'indice %d", i);
                return -1;
        }
    }
    printArbre(racine, 0);
    free(tabHeader);

}