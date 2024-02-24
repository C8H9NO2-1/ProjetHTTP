#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "structure.h"
#include "affichage.h"

void printArbre(const char requete[], Noeud *noeud, int nombreTab) {
    for (int i = 0; i < nombreTab; i++) {
        printf("\t");
    }
    
    if (noeud->nombreFils == 0) {
        printf("%s : \"", noeud->tag);
        for (int i = 0; i < noeud->longueur; i++) {
            switch (requete[noeud->indice + i]) {
                case 13:
                    printf("CR");
                    break;
                case 10:
                    printf("LF");
                    break;
                default:
                    printf("%c", requete[noeud->indice + i]);
                    break;
            }
        }
        printf("\"\n");
    } else {
        printf("%s :\n", noeud->tag);
        for (int i = 0; i < noeud->nombreFils; i++) {
            printArbre(requete, &noeud->fils[i], nombreTab + 1);
        }
    }
}