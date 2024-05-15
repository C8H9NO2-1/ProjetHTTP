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
    if (r == NULL) {
        return true;
    }
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

    bool first = true;
    bool boucle = false;

    char minuscule[9];

    while (tok != NULL) {
        int l;
        char *s;
        s = getElementValue(tok->node, &l);

        sousChaineMinuscule2(s, minuscule, l, 7);
        if (first && strcmp("chunked", minuscule)==0){
            boucle = true;
            first = false;
            element->value = CHUNKED;
        }
        else if (first || strcmp("chunked", minuscule)==0){
            element->next = NULL;
            purgeListeEncodage(&liste);
            printf("chunked n'est pas une unique fois en dernière position \n");
            return false;
        }

        sousChaineMinuscule2(s, minuscule, l, 4);
        if (strcmp("gzip", minuscule)==0){
            boucle = true;
            element->value = GZIP;
        } 

        sousChaineMinuscule2(s, minuscule, l, 8);
        if (strcmp("compress", minuscule) == 0) {
            boucle = true;
            element->value = COMPRESS;
        } 

        sousChaineMinuscule2(s, minuscule, l, 7);
        if (strcmp("deflate", minuscule) == 0){
            boucle = true;
            element->value = DEFLATE;
        }

        if (boucle == false){
            printf("Option de encoding inconnu, il faut renvoyer 501\n");
            return false;
        }

        boucle = false; // permet de faire les vérifications dans la boucle  
        listeEncodage *suite = malloc(sizeof(listeEncodage));
        suite->next = NULL;
        element->next = suite ; 
        element = suite ; 
        tok = tok->next;
    }
    //free(element);
    purgeElement(&r);
    return true;
}


/* J'ai volé ta fonction de function.c en l'adaptant pour remplacer const char chaine1[] par char *chaine1 */
void sousChaineMinuscule2(const char *chaine1, char chaine2[], int n, int j) {
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
    listeEncodage *suivant ;
    while (tmp != NULL) {
        suivant = tmp->next;
        free(tmp);
        tmp = suivant;
    }
    *r = NULL;
}