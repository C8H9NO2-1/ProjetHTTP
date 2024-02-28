#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "structure.h"
#include "elisa2.h"
#include "affichage.h"

int main(int argc, char *argv[]) {
    
    char transferEncoding[] = "Transfer-Encoding: chunked";

    Noeud *test = malloc(sizeof(Noeud));

    int i = 0;

    if (!checkTransferEncodingHeader(transferEncoding, &i, strlen(transferEncoding), test)) {
        printf("Hello world\n");
        test = NULL;
    }

    if (test != NULL) {
        printArbre(transferEncoding, test, 0);
    }
    
    return 0;
}

bool checkTransferEncodingHeader(const char transferEncoding[], int *i, int longueur, Noeud *noeud){//Transfer-Encoding-header = "Transfer-Encoding" ":" OWS Transfer-Encoding OWS
    int nombreFils = 5;
    const int indice = *i;

    Noeud *filsTransferEncodingMot = malloc(sizeof(Noeud));
    if (!checkTransferEncodingMot(transferEncoding, filsTransferEncodingMot)) {
        free(noeud);
        *i = indice;
        return false;
    }
    *i=17;
    if (transferEncoding[*i] != 58) { //? ":" = 58
        free(noeud);
        *i = indice;
        return false;
    }

    Noeud *filsOWS1 = malloc(sizeof(Noeud));
    if (!checkOWS(transferEncoding, i, longueur, filsOWS1)){
        free(noeud);
        *i = indice;
        return false;
    }

    Noeud *filsTransferEncoding = malloc(sizeof(Noeud));
    if (!checkTransferEncoding(transferEncoding, i, longueur, filsTransferEncoding)){
        free(noeud);
        *i = indice;
        return false;
    }

    Noeud *filsOWS2 = malloc(sizeof(Noeud));
    if (!checkOWS(TransferEncoding, i, longueur, filsOWS2)){
        free(noeud);
        *i = indice;
        return false;
    }

}

bool checkTransferEncodingMot(const char transferEncoding[], Noeud *noeud){
    noeud->indice = 0;
    noeud->longueur = 17;
    noeud->tag = "Transfer-Encoding-Mot";
    noeud->tab = NULL;
    noeud->nombreFils = 0;

    if (transferEncoding[0] != 'T') {
        return false;
    }
    if (transferEncoding[1] != 'r') {
        return false;
    }
    if (transferEncoding[2] != 'a') {
        return false;
    }
    if (transferEncoding[3] != 'n') {
        return false;
    }
    if (transferEncoding[4] != 's') {
        return false;
    }
    if (transferEncoding[5] != 'f') {
        return false;
    }  
    if (transferEncoding[3] != 'e') {
        return false;
    }
    if (transferEncoding[4] != 'r') {
        return false;
    }
    if (transferEncoding[4] != '-') {
        return false;
    }
    if (transferEncoding[5] != 'E') {
        return false;
    }  
    if (transferEncoding[3] != 'n') {
        return false;
    }
    if (transferEncoding[4] != 'c') {
        return false;
    }
    if (transferEncoding[5] != 'o') {
        return false;
    }  
    if (transferEncoding[3] != 'd') {
        return false;
    }
    if (transferEncoding[4] != 'i') {
        return false;
    }
    if (transferEncoding[5] != 'n') {
        return false;
    }  
    if (transferEncoding[3] != 'g') {
        return false;
    }
    return true;
}

void checkOWS(const char TransferEncoding[], int *i, int longueur, Noeud *noeud) { //OWS = *( SP / HTAB )
    int compteur = 0;
    const int indice = *i;
    while ((TransferEncoding[*i]==32 || TransferEncoding[*i]==9)&& *i<longueur){
        (*i)++;
        compteur++;
    }

    noeud->indice = indice;
    noeud->longueur = *i - indice;
    noeud->tag = "OWS";
    noeud->fils = malloc(compteur * sizeof(Noeud));
    noeud->nombreFils = compteur;

    // On réinitialise l'indice i pour la suite de la fonction
    *i = indice;

    // On remplit le tableau des fils du noeud OWS
    for (int j = 0; j < compteur; j++) {
        Noeud *noeud = &noeud->fils[j];
        noeud->fils = NULL;
        noeud->indice = *i;
        noeud->longueur = 1;
        noeud->nombreFils = 0;
        if (TransferEncoding[*i]==32){
        noeud->tag = "SP";
        }
        else {
        noeud->tag = "HTAB"; 
        }
        (*i)++;
    }
    return true;
}

bool checkTransferEncoding(const char TransferEncoding[], int *i, int longueur, Noeud *noeud){
    //Transfer-Encoding = *( "," OWS ) transfer-coding *( OWS "," [ OWS transfer-coding ] )
    int compteur=0;
    int const indice=*i;
    while (TransferEncoding[*i]==44){
        (*i)++;
        compteur++;
        if (!checkOWS(transferEncoding, i, longueur, NULL)){
            free(noeud);
            *i = indice;
            return false;
        }
        else{
            (*i)++;
            compteur++;
        }
    }
    Noeud *filstransferCoding = malloc(sizeof(Noeud));
    compteur++;
    if (!checkTransferCoding(transferEncoding, i, longueur, transferCoding)){
            free(noeud);
            *i = indice;
            return false;
    }
    while (checkOWS(transferEncoding, i, longueur, NULL)){
        (*i)++;
        compteur++;
        if (!TransferEncoding[*i]==44){
            free(noeud);
            *i = indice;
            return false;
        }
        else{
            (*i)++;
            compteur++;
            int *j=i;
            (*j)++;
            if((checkOWS(transferEncoding, i, longueur, NULL)))&&(checkTransfercoding(transferEncoding, j, longueur, NULL)){
                *i=*j;
                compteur+=2;
            }
        }
    }
    noeud->indice = indice;
    noeud->longueur = *i - indice;
    noeud->tag = "Transfer-Encoding";
    noeud->fils = malloc(compteur * sizeof(Noeud));
    noeud->nombreFils =compteur;

    // On réinitialise l'indice i pour la suite de la fonction
    *i = indice;

    // On remplit le tableau des fils du noeud method
    for (int k = 0; k < compteur; k++) {
        if(TransferEncoding[*i]==44){
            createFilsSimple(",", *i, 1, &noeud->fils[j])
        }
        else {
        checkOWS(transferEncoding, i, longueur, &noeud->fils[j]);
        checkTransferCoding(transferEncoding, i, longueur, &noeud->fils[j])
        }
        (*i)++;
    }
    
}

bool checkTransferCoding(const char TransferEncoding[], int *i, int longueur, Noeud *noeud){
    //transfer-coding = "chunked" / "compress" / "deflate" / "gzip" / transfer-extension
}
