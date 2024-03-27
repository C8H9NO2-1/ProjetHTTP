#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "structure.h"
#include "pm.h"
#include "affichage.h"

int checkCRLF(char requete[], int longueur, int i){
    while ((i+1)<longueur && (requete[i]!=13 || requete[i+1]!=10)){
        i++;
    }
    return i;
}

int compteHeader(char requete[], int i, int longueur, int tabHeader[]) {
    int indice = i;
    int j = i;
    int k = 0;

    while (j<longueur) {
        Noeud *test = malloc(sizeof(Noeud));
        if (checkConnectionHeader(requete, &j, longueur, test)) {
            free(test);
            tabHeader[k]=1;
            k++;
        }
        else if(checkContentLengthHeader(requete, &j, longueur, test)) {
            free(test);
            tabHeader[k]=2;
            k++;
        }
        else if(checkContentTypeHeader(requete, &j, longueur,  test)) {
            free(test);
            tabHeader[k]=3;
            k++;
        }
        else if(checkCookieHeader(requete, &j, longueur, test)) {
            free(test);
            tabHeader[k]=4;
            k++;
        }
        else if(checkTransferEncodingHeader(requete, &j, longueur, test)) {
            free(test);
            tabHeader[k]=5;
            k++;
        }
        else if(checkExpectHeader(requete, &j, longueur,  test)) {
            free(test);
            tabHeader[k]=6;
            k++;
        }
        else if(checkHostHeader(requete, &j, longueur,  test)) {
            free(test);
            tabHeader[k]=7;
            k++;
        }
    }
    return k;
}