#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "structure.h"
#include "affichage.h"

void printArbre(Noeud *noeud, int nombreTab) {
    for (int i = 0; i < nombreTab; i++) {
        printf("\t");
    }
    
    if (noeud->nombreFils == 0) {
        printf("%s : \"", noeud->tag);
        for (int i = 0; i < noeud->longueur; i++) {
            switch (noeud->valeur[i]) {
                case 13:
                    printf("CR");
                    break;
                case 10:
                    printf("LF");
                    break;
                default:
                    printf("%c", noeud->valeur[i]);
                    break;
            }
        }
        printf("\"\n");
    } else {
        printf("%s :\n", noeud->tag);
        for (int i = 0; i < noeud->nombreFils; i++) {
            printArbre(&noeud->fils[i], nombreTab + 1);
        }
    }
}