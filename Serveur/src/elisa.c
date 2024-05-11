/* Vérification de la "sémantique" du cookie header et du transfer-encoding header*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "header/api.h"
#include "header/elisa.h"


bool semanticCookie(void *root){
    _Token *r;
    r = searchTree(root, "Cookie-header");

    /* Il ne doit y avoir qu'un seul cookie header */
    if (r->next != NULL){
        purgeElement(&r);
        return false;
    }
    else {
        purgeElement(&r);
        return true;
    }
}

/* Transfer-Encoding header est une nouveauté de la version 1*/
bool semanticTransferCodings(void *root, listeEncodage *liste, int version) {
    _Token *r, *tok;
    r = searchTree(root, "transfer-coding");
    tok = r;
    listeEncodage *element = liste;
    /* A sender MUST NOT apply chunked more than once to a message body */
    /* If any transfer coding other than chunked is applied to a request payload body, the sender MUST apply chunked as the final transfer coding */
    /* --> Ainsi on a forcément un unique chunked en dernière position */

    bool chunkedLast = false;

    char minuscule[9];

    while (tok != NULL) {
        int l;
        char *s;
        s = getElementValue(tok->node, &l);

        sousChaineMinuscule(s, minuscule, l, 7);
        if (strcmp("chunked", minuscule)==0){
            if (tok -> next !=NULL){
                purgeListeEncodage(&liste);
                printf("chunked n'est pas en dernière position \n");
                return false;
            }
            else {
                element->value = CHUNKED;
                chunkedLast = true;
            }
        } 

        sousChaineMinuscule(s, minuscule, l, 4);
        if (strcmp("gzip", minuscule)==0){
            element->value = GZIP;
        } 

        sousChaineMinuscule(s, minuscule, l, 8);
        if (strcmp("compress", minuscule) == 0) {
            element->value = COMPRESS;
        } 

        sousChaineMinuscule(s, minuscule, l, 7);
        if (strcmp("deflate", minuscule) == 0){
            element->value = DEFLATE;
        }

        else {
            printf("Option de connexion inconnue, il faut renvoyer 501\n");
        }

        listeEncodage *suite = malloc(sizeof(listeEncodage));
        element->next = suite ; 
        element = suite ; 
        tok = tok->next;
    }
    free(element);
    purgeElement(&r);

    // On liste tous les cas possibles
    if (!chunkedLast) {
        purgeListeEncodage(&liste);
        printf(" Un message doit être chunked \n");
        return false;
    }

    return true;
}


/* J'ai volé ta fonction de function.c en l'adaptant pour remplacer const char chaine1[] par char *chaine1 */
void sousChaineMinuscule(const char *chaine1, char chaine2[], int n, int j) {
    int diff = 'a' - 'A';
    for (int k = 0; k < j; k++) {
        if (k < n) {
            if (chaine1[k] >= 'A' && chaine1[k] <= 'Z') {
                chaine2[k] = chaine1[k] + diff;
            } else {
                chaine2[k] = chaine1[k];
            }
        }
    }
    chaine2[j] = '\0';
}

void purgeListeEncodage(listeEncodage **r) {
    listeEncodage *tmp = *r;
    while (tmp != NULL) {
        listeEncodage *suivant = tmp->next;
        free(tmp);
        tmp = suivant;
    }
    *r = NULL;
}