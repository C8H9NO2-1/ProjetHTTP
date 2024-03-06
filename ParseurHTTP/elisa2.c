#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "structure.h"
#include "elisa2.h"
#include "affichage.h"

int main(int argc, char *argv[]) {
    
    char transferEncoding[] = "Transfer-Encoding:";

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
    (*i)++;

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
    if (!checkOWS(transferEncoding, i, longueur, filsOWS2)){
        free(noeud);
        *i = indice;
        return false;
    }

    noeud->fils = malloc(nombreFils * sizeof(Noeud));
    noeud->indice = indice;
    noeud->longueur = *i - indice;
    noeud->nombreFils = nombreFils;
    noeud->tag = "Transfer-Encoding-header";

    *i = indice; // On réinitialise i
    
    noeud->fils[0] = *filsTransferEncodingMot;
    (*i) += noeud->fils[0].longueur;
    
    createFilsSimple("case-insensitive-string", *i, 1, &noeud->fils[1]);
    (*i)++;
    
    noeud->fils[2] = *filsOWS1;
    (*i) += noeud->fils[2].longueur;

    noeud->fils[3] = *filsTransferEncoding;
    (*i) += noeud->fils[3].longueur;

    noeud->fils[4] = *filsOWS2;
    (*i) += noeud->fils[4].longueur;

    return true;

}

void createFilsSimple(char nom[], int i, int longueur, Noeud *noeud) {
    noeud->tag = nom;
    noeud->fils = NULL;
    noeud->indice = i;
    noeud->longueur = longueur;
    noeud->nombreFils = 0;
}

bool checkTransferEncodingMot(const char transferEncoding[], Noeud *noeud){
    noeud->indice = 0;
    noeud->longueur = 17;
    noeud->tag = "Transfer-Encoding-Mot";
    noeud->fils = NULL;
    noeud->nombreFils = 0;

    if (transferEncoding[0] != 'T' || transferEncoding[0] != 't') {
        return false;
    }
    if (transferEncoding[1] != 'r' || transferEncoding[1] != 'R') {
        return false;
    }
    if (transferEncoding[2] != 'a' || transferEncoding[2] != 'A') {
        return false;
    }
    if (transferEncoding[3] != 'n' || transferEncoding[3] != 'N') {
        return false;
    }
    if (transferEncoding[4] != 's' || transferEncoding[4] != 'S') {
        return false;
    }
    if (transferEncoding[5] != 'f' || transferEncoding[5] != 'F') {
        return false;
    }  
    if (transferEncoding[6] != 'e' || transferEncoding[6] != 'E') {
        return false;
    }
    if (transferEncoding[7] != 'r' || transferEncoding[7] != 'R') {
        return false;
    }
    if (transferEncoding[8] != '-') {
        return false;
    }
    if (transferEncoding[9] != 'E' || transferEncoding[9] != 'e') {
        return false;
    }  
    if (transferEncoding[10] != 'n' || transferEncoding[10] != 'N') {
        return false;
    }
    if (transferEncoding[11] != 'c' || transferEncoding[11] != 'C') {
        return false;
    }
    if (transferEncoding[12] != 'o' || transferEncoding[12] != 'O') {
        return false;
    }  
    if (transferEncoding[13] != 'd'|| transferEncoding[13] != 'D') {
        return false;
    }
    if (transferEncoding[14] != 'i' || transferEncoding[14] != 'i') {
        return false;
    }
    if (transferEncoding[15] != 'n' || transferEncoding[15] != 'N') {
        return false;
    }  
    if (transferEncoding[16] != 'g' || transferEncoding[16] != 'G') {
        return false;
    }
    return true;
}

bool checkOWS(const char transferEncoding[], int *i, int longueur, Noeud *noeud) { //OWS = *( SP / HTAB )
    int compteur = 0;
    const int indice = *i;
    while ((transferEncoding[*i]==32 || transferEncoding[*i]==9)&&(*i<longueur)){
        (*i)++;
        compteur++;
    }
    if (noeud!=NULL){
        noeud->indice = indice;
        noeud->longueur = *i - indice;
        noeud->tag = "OWS";
        if (compteur > 0) {
            noeud->fils = malloc(compteur * sizeof(Noeud));
        }
        noeud->nombreFils = compteur;

        // On réinitialise l'indice i pour la suite de la fonction
        *i = indice;
        // On remplit le tableau des fils du noeud OWS
        for (int j = 0; j < compteur; j++) {
            if (transferEncoding[*i]==32){
                createFilsSimple("SP", *i, 1, &noeud->fils[j]);
            }
            else {
                createFilsSimple("HTAB", *i, 1, &noeud->fils[j]);
            }
            (*i)++;
        }
    }
    return true;
}

bool checkTransferEncoding(const char transferEncoding[], int *i, int longueur, Noeud *noeud){
    //Transfer-Encoding = *( "," OWS ) transfer-coding *( OWS "," [ OWS transfer-coding ] )
    int compteur=0;
    int const indice=*i;
    while (transferEncoding[*i]==44){
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
    if (!checkTransferCoding(transferEncoding, i, longueur, NULL)){
            free(noeud);
            *i = indice;
            return false;
    }
    compteur++;
    (*i)++;
    while (checkOWS(transferEncoding, i, longueur, NULL)){
        (*i)++;
        compteur++;
        if (transferEncoding[*i]==44){
            (*i)++;
            compteur++;
            int iTemp=(*i);
            checkOWS(transferEncoding, i, longueur, NULL);
            compteur++;
            if(checkTransferCoding(transferEncoding, i, longueur, NULL)){
                (*i)++;
                compteur++;
            }
            else{
                (*i)=iTemp;
                compteur-=1;
            }
        }
        else {
            (*i)-=1;
            compteur-=1;
            break;
        }
    }
    noeud->indice = indice;
    noeud->longueur = *i - indice;
    noeud->tag = "transfer-encoding";
    noeud->fils = malloc(compteur * sizeof(Noeud));
    noeud->nombreFils =compteur;

    // On réinitialise l'indice i pour la suite de la fonction
    *i = indice;

    // On remplit le tableau des fils du noeud method
    for (int j = 0; j < compteur; j++) {
        int iTemp=*i;
        if(transferEncoding[*i]==44){
            createFilsSimple(",", *i, 1, &noeud->fils[j]);
        }
        if (checkTransferCoding(transferEncoding, i, longueur, NULL)){
            *i=iTemp;
            checkTransferCoding(transferEncoding, i, longueur, &noeud->fils[j]);
        }
        else {
            checkOWS(transferEncoding, i, longueur, &noeud->fils[j]);
        }
        (*i)++;
    }
    return true;  
}

bool checkTransferCoding(const char transferEncoding[], int *i, int longueur, Noeud *noeud){
    //transfer-coding = "chunked" / "compress" / "deflate" / "gzip" / transfer-extension
    const int indice=*i;
    if (noeud!=NULL){
        noeud->indice = indice;
        noeud->tag = "transfer-coding";
        noeud->fils = malloc(sizeof(Noeud));
        noeud->nombreFils = 1;
    }
    if (checktransferextension(transferEncoding, i, longueur, NULL)){
        Noeud *filsTransferExtension=malloc(sizeof(Noeud));
        checktransferextension(transferEncoding, i, longueur, filsTransferExtension);
        if (noeud!=NULL){
            noeud->longueur = (*i)-indice;
            noeud->fils[0]=*filsTransferExtension;
        }
        return true;
    }

    (*i)=indice;
    if (transferEncoding[*i]=='c'){
    (*i)++;
        if (transferEncoding[*i]=='h'){
            (*i)++;
            if ((transferEncoding[(*i)]=='u') && (transferEncoding[(*i)+1]=='n') && (transferEncoding[(*i)+2]=='k') && (transferEncoding[(*i)+3]=='e') && (transferEncoding[(*i)+4]=='d')){
                (*i)+=4;
                if (noeud!=NULL){
                    noeud->longueur = 7;
                    (*i)=indice;
                    createFilsSimple("chunked", *i, 7, &noeud->fils[0]);
                    (*i)+=7;
                }
                return true;
            }
            else{
                free(noeud);
                (*i)=indice;
                return false;
            }
        }
        else if (transferEncoding[*i]=='o'){
            (*i)++;
            if ((transferEncoding[(*i)]=='m') && (transferEncoding[(*i)+1]=='p') && (transferEncoding[(*i)+2]=='r') && (transferEncoding[(*i)+3]=='e') && (transferEncoding[(*i)+4]=='s') && (transferEncoding[(*i)+5]=='s')){
                (*i)+=5;
                if (noeud!=NULL){
                    (*i)=indice;
                    noeud->longueur = 8;
                    createFilsSimple("compress", *i, 8, &noeud->fils[0]);
                    (*i)+=8;
                }
                return true;
            }
            else{
                free(noeud);
                (*i)=indice;
                return false;
            }    
        }
    }
    else if ((transferEncoding[(*i)]=='d')&&(transferEncoding[(*i)+1]=='e')&&(transferEncoding[(*i)+2]=='f')&&(transferEncoding[(*i)+3]=='l')&&(transferEncoding[(*i)+4]=='a')&&(transferEncoding[(*i)+5]=='t')&&(transferEncoding[(*i)+6]=='e')){
        (*i)+=6;
        if (noeud!=NULL){
            (*i)=indice;
            noeud->longueur = 7;
            createFilsSimple("compress", *i, 7, &noeud->fils[0]);
            (*i)+=7;
        }
        return true;
    }
    else if ((transferEncoding[(*i)]=='g')&&(transferEncoding[(*i)+1]=='z')&&(transferEncoding[(*i)+2]=='i')&&(transferEncoding[(*i)+3]=='p')){
        (*i)+=3;
        if (noeud!=NULL){
            (*i)=indice;
            noeud->longueur = 4;
            createFilsSimple("gzip", *i, 4, &noeud->fils[0]);
            (*i)+=4;
        }
        return true;
    }
    else{
        free(noeud);
        (*i)=indice;
        return false;
    }
    return true;
}

bool checktransferextension(const char transferEncoding[], int *i, int longueur, Noeud *noeud){
    //transfer-extension = token *( OWS ";" OWS transfer-parameter )
    int compteur=0;
    int const indice=*i;
    while ((checkTChar(transferEncoding, *i, NULL))&&((*i)<longueur)){
        compteur++;
        (*i)++; 
    }
    if (compteur==0){
        free(noeud);
        (*i)=indice;
        return false;
    }
    while (checkOWS(transferEncoding, i, longueur, NULL)){
        if (transferEncoding[*i]==59){
            (*i)++;
            if (checkOWS(transferEncoding, i, longueur, NULL)){
                if(checkTransferParameter(transferEncoding, i, longueur, NULL)){
                    compteur+=4;
                }
                else{
                    free(noeud);
                    (*i)=indice;
                    return false;
                }
            }
        }
        else{
            free(noeud);
            (*i)=indice;
            return false;
        }
    }
    if (noeud!=NULL){
        noeud->indice = indice;
        noeud->longueur = *i - indice;
        noeud->tag = "Transfer-Extension";
        noeud->fils = malloc(compteur * sizeof(Noeud));
        noeud->nombreFils =compteur;

        // On réinitialise l'indice i pour la suite de la fonction
        *i = indice;
        for (int j=0; j<compteur; j++){
            int iTemp=*i;
            if (checkTChar(transferEncoding, *i, NULL)){
                checkTChar(transferEncoding, *i, &noeud->fils[j]);
            }
            else if(transferEncoding[*i]==59){
                createFilsSimple("case-insensitive-string", *i, 1, &noeud->fils[j]);
            }
            else if(checkOWS(transferEncoding, i, longueur, NULL)){
                *i=iTemp;
                checkOWS(transferEncoding, i, longueur, &noeud->fils[j]);
            }
            else {
                checkTransferParameter(transferEncoding, i, longueur, &noeud->fils[j]);
            }
        }
    }
    return true;
}

bool checkTransferParameter(const char transferEncoding[], int *i, int longueur, Noeud *noeud){
    //transfer-parameter = token BWS "=" BWS ( token / quoted-string )
    //BWS=OWS
    const int indice=*i;
    int compteur=0;
    while (checkTChar(transferEncoding, *i, NULL)){
        (*i)++;
        compteur++;
    }
    if (compteur<0){
        free(noeud);
        *i=indice;
        return false;
    }
    checkOWS(transferEncoding, i, longueur, NULL);
    compteur++;
    if (transferEncoding[*i]!=61){
        free(noeud);
        *i=indice;
        return false;
    }
    (*i)++;
    compteur++;
    checkOWS(transferEncoding, i, longueur, NULL);
    compteur++;
    if (checkQuotedString(transferEncoding, i, longueur, NULL)){
        compteur++;
    }
    else {
        while ((*i<longueur) && (checkTChar(transferEncoding, *i, NULL))){
            (*i)++;
            compteur++;
        }
        if (compteur==0){
            free(noeud);
            *i=indice;
            return false;
        }
    }

    noeud->indice = indice;
    noeud->longueur = *i - indice;
    noeud->tag = "transfer-parameter";
    noeud->fils = malloc(compteur * sizeof(Noeud));
    noeud->nombreFils =compteur;

    for (int j=0;j<compteur;j++){
        int iTemp=*i;
        if (checkTChar(transferEncoding, *i, NULL)){
            checkTChar(transferEncoding, *i, &noeud->fils[j]);
        }
        else if (transferEncoding[*i]==61){
            createFilsSimple("case-insensitive-string", *i, 1, &noeud->fils[j]);
        }
        else if (checkQuotedString(transferEncoding, i, longueur, NULL)){
            *i=iTemp;
            checkQuotedString(transferEncoding, i, longueur, &noeud->fils[j]);
        }
        else {
            (*i)=iTemp;
            checkOWS(transferEncoding, i, longueur, &noeud->fils[j]);
        }
        (*i)++;
    }
    return true;
}

bool checkQuotedString(const char transferEncoding[], int *i, int longueur, Noeud *noeud){
    //quoted-string = DQUOTE *( qdtext / quoted-pair ) DQUOTE
    int compteur=0;
    int const indice =(*i);
    if (transferEncoding[*i]!=34){
        (*i)=indice;
        free(noeud);
        return false;
    }
    while(checkQdtext(transferEncoding, *i, NULL)||checkQuotedPair(transferEncoding, i, longueur, NULL)){
        compteur++;
        (*i)++;
    }
    if (transferEncoding[*i]!=34){
        (*i)=indice;
        free(noeud);
        return false;
    }

    noeud->indice = indice;
    noeud->longueur = *i - indice;
    noeud->tag = "quoted-string";
    noeud->fils = malloc(compteur * sizeof(Noeud));
    noeud->nombreFils =compteur;

    for (int j=0;j<compteur;j++){
        if (transferEncoding[*i]==34){
            createFilsSimple("dquote", *i, 1, &noeud->fils[j]);
        }
        else if (checkQdtext(transferEncoding, *i, NULL)){
            checkQdtext(transferEncoding, *i, &noeud->fils[j]);
        }
        else{
            checkQuotedPair(transferEncoding, i, longueur, &noeud->fils[j]);
        }
        (*i)++;
    }
    return true;
}

bool checkQdtext(const char transferEncoding[], int i, Noeud *noeud){
    // qdtext = HTAB / SP / "!" / %x23-5B ; '#'-'['
    //          / %x5D-7E ; ']'-'~'
    //         / obs-text
    // obs-text = %x80-FF
    if (noeud!=NULL){
        noeud->indice = i;
        noeud->longueur = 1;
        noeud->tag = "qdtext";
        noeud->fils = malloc(sizeof(Noeud));
        noeud->nombreFils =1;
    }

    if (transferEncoding[i]==9){
        if (noeud!=NULL){
            createFilsSimple("HTAB", i, 1, &noeud->fils[0]);
        }
    }
    else if (transferEncoding[i]==32){
        if (noeud!=NULL){
            createFilsSimple("SP", i, 1, &noeud->fils[0]);
        }
    }
    else if (transferEncoding[i]==33){
        if (noeud!=NULL){
            createFilsSimple("case-insensitive-string", i, 1, &noeud->fils[0]);
        }
    }
    else if (transferEncoding[i]>=35 && transferEncoding[i]<=91){
        if (noeud!=NULL){
            createFilsSimple("23-5B", i, 1, &noeud->fils[0]);
        }
    }
    else if (transferEncoding[i]>=93 && transferEncoding[i]<=126){
        if (noeud!=NULL){
            createFilsSimple("5D-7E", i, 1, &noeud->fils[0]);
        }
    }
    else if (transferEncoding[i]>=128 && transferEncoding[i]<=255){
        if (noeud!=NULL){
            createFilsSimple("obs-text", i, 1, &noeud->fils[0]);
        }
    }
    else {
        free(noeud);
        return false;
    }
    return true;
}

bool checkQuotedPair(const char transferEncoding[], int *i, int longueur, Noeud *noeud){
    //quoted-pair = "\" ( HTAB / SP / VCHAR / obs-text )
    int indice= *i;
    if (noeud!=NULL){
        noeud->indice = *i;
        noeud->longueur = 2;
        noeud->tag = "quoted-pair";
        noeud->fils = malloc(2*sizeof(Noeud));
        noeud->nombreFils =2;
    }
    if (transferEncoding[*i]!=92){
        free(noeud);
        return false;
    }
    (*i)++;
    if (noeud!=NULL){
        createFilsSimple("case-insensitive-string",*i, 1, &noeud->fils[0]);
    }
    if (transferEncoding[*i]==9){
        if (noeud!=NULL){
            createFilsSimple("HTAB", *i, 1, &noeud->fils[1]);
        }
    }
    else if (transferEncoding[*i]==32){
        if (noeud!=NULL){
            createFilsSimple("SP", *i, 1, &noeud->fils[1]);
        }
    }
    else if (transferEncoding[*i]>=33 && transferEncoding[*i]<= 126){
        if (noeud!=NULL){
            createFilsSimple("VCHAR", *i, 1, &noeud->fils[1]);
        }
    }
    else if (transferEncoding[*i]>=128 && transferEncoding[*i]<=255){
        if (noeud!=NULL){
            createFilsSimple("obs-text", *i, 1, &noeud->fils[1]);
        }
    }
    else {
        free(noeud);
        (*i)=indice;
        return false;
    }
    return true;
}
bool checkTChar(const char requete[], int i, Noeud *noeud) {
    //! Cette fonction a deux éxécution différentes:
    //* - Une qui ne fait que vérifier syntaxiquement un caractère
    //* - Une qui vérifie syntaxiquement un caractère mais stocke aussi ce caractère

    // On stocke les données nécessaires pour le noeud courant
    if (noeud != NULL) {
        noeud->fils = NULL;
        noeud->indice = i;
        noeud->longueur = 1;
        noeud->nombreFils = 0;
        noeud->tag = "tchar";
    }

    // On vérifie que la syntaxe est correcte
    switch (requete[i]) {
        case '!':
        case '#':
        case '$':
        case '%':
        case '&':
        case 39 : //? ' = 39
        case '*':
        case '+':
        case '-':
        case '.':
        case '^':
        case '_':
        case '`':
        case '|':
        case '~':
            return true;
            break;
        
        default:
            break;
    }

    if (checkAlpha(requete, i) || checkDigit(requete, i)) {
        return true;
    }

    // Si la syntaxe n'est pas correcte, on libère la mémoire et on renvoie false
    if (noeud != NULL) {
        free(noeud);
    }

    return false;
}

bool checkAlpha(const char requete[], int i) {
    return ((requete[i] >= 'a' && requete[i] <= 'z') || (requete[i] >= 'A' && requete[i] <= 'Z'));
}

bool checkDigit(const char requete[], int i) {
    return (requete[i] >= '0' && requete[i] <= '9');
}