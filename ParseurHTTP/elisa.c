#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "structure.h"
#include "elisa.h"
#include "affichage.h"

int main(int argc, char *argv[]) {
    
    char cookie[] = "Cookie: theme=light";

    Noeud *test = malloc(sizeof(Noeud));

    int i = 0;

    if (!checkCookieHeader(cookie, &i, strlen(cookie), test)) {
        printf("Hello world\n");
        test = NULL;
    }

    if (test != NULL) {
        printArbre(cookie, test, 0);
    }
    
    return 0;
}

bool checkCookieHeader(const char cookie[], int *i, int longueur, Noeud *noeud) { //Cookie-header = "Cookie" ":" OWS cookie-string OWS
    int nombreFils = 5;
    const int indice = *i;

    Noeud *filsCookie = malloc(sizeof(Noeud));
    if (!checkCookie(cookie, filsCookie)) {
        free(noeud);
        *i = indice;
        return false;
    }
    *i=6;

    if (cookie[*i] != 58) { //? ":" = 58
        free(noeud);
        *i = indice;
        return false;
    }

    Noeud *filsOWS1 = malloc(sizeof(Noeud));
    if (!chekOWS(cookie, i, longueur, filsOWS1)){
        free(noeud);
        *i = indice;
        return false;
    }

    Noeud *filsCookieString = malloc(sizeof(Noeud));
    if (!checkCookieString(cookie, i, longueur, filsCookieString)){
        free(noeud);
        *i = indice;
        return false;
    }

    Noeud *filsOWS2 = malloc(sizeof(Noeud));
    if (!chekOWS(cookie, i, longueur, filsOWS2)){
        free(noeud);
        *i = indice;
        return false;
    }
   
    noeud->fils = malloc(nombreFils * sizeof(Noeud));
    noeud->indice = indice;
    noeud->longueur = *i - indice;
    noeud->nombreFils = nombreFils;
    noeud->tag = "Cookie-header";

    *i = indice; // On réinitialise i
    
    noeud->fils[0] = *filsCookie;
    (*i) += noeud->fils[0].longueur;
    
    createFilsSimple(":", *i, 1, &noeud->fils[1]);
    (*i)++;
    
    noeud->fils[2] = *filsOWS1;
    (*i) += noeud->fils[2].longueur;

    noeud->fils[3] = *filsCookieString;
    (*i) += noeud->fils[3].longueur;

    noeud->fils[4] = *filsOWS2;
    (*i) += noeud->fils[4].longueur;

    return true;
}


bool checkCookie(const char cookie[], Noeud *noeud){
    noeud->indice = 0;
    noeud->longueur = 6;
    noeud->tag = "Cookie";
    noeud->tab = NULL;
    noeud->nombreFils = 0;

    if (cookie[0] != 'C') {
        return false;
    }
    if (cookie[1] != 'o') {
        return false;
    }
    if (cookie[2] != 'o') {
        return false;
    }
    if (cookie[3] != 'k') {
        return false;
    }
    if (cookie[4] != 'i') {
        return false;
    }
    if (cookie[5] != 'e') {
        return false;
    }

    return true;
}

void checkOWS(const char cookie[], int *i, int longueur, Noeud *noeud) { //OWS = *( SP / HTAB )
    int compteur = 0;
    const int indice = *i;
    while ((cookie[*i]==32 || cookie[*i]==9)&& *i<longueur){
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
        if (cookie[*i]==32){
        noeud->tag = "SP";
        }
        else {
        noeud->tag = "HTAB"; 
        }
        (*i)++;
    }
    return true;
}


bool checkCookieString(const char cookie[], int *i, int longueur, Noeud *noeud){ //cookie-pair *( ";" SP cookie-pair )
    const int indice=*i;
    int compteur=0;

    Noeud *filsCookiePair1 = malloc(sizeof(Noeud));
    if (!checkCookiePair(cookie, i, longueur, filsCookiePair1)){
        free(noeud);
        *i = indice;
        return false;
    }
    else {
        compteur++;
    }
    while (cookie[*i]==59){ //";"=59
        (*i)+=2;
        if(cookie[(*i)-1]!=32 || !checkCookiePair(cookie, i, longueur, NULL)){
            free(noeud);
            *i = indice;
            return false;
        }
        compteur+=3;
    }
    if (compteur>1){ // Il y a au moins un "( ";" SP cookie-pair )"
        noeud->indice = indice;
        noeud->longueur = *i - indice;
        noeud->tag = "cookie-string";
        noeud->fils = malloc(compteur * sizeof(Noeud));
        noeud->nombreFils = compteur;

        // On réinitialise l'indice i pour la suite de la fonction
        *i = indice;

        // On remplit le tableau des fils du noeud cookie-string
        noeud->fils[0]=filsCookiePair1;
        for (int j = 1; j < compteur; j++) {
            checkCookiePair(cookie, *i, &noeud->fils[j]);
        }
    }
    else {
        noeud->indice = indice;
        noeud->longueur = *i - indice;
        noeud->tag = "cookie-string";
        noeud->fils = malloc(sizeof(Noeud));
        noeud->nombreFils = 1;
        noeud->fils[0]=filsCookiePair1;
    }

}

bool checkCookiePair(const char cookie[], int *i, int longueur, Noeud *noeud){ //cookie-pair = cookie-name "=" cookie-value
    const int indice=*i;
    Noeud *filsCookieName = malloc(sizeof(Noeud));
    if (!checkCookieName(cookie, i, longueur, filsCookieName)){
        free(noeud);
        *i = indice;
        return false;
    }
    if (cookie[*i]!=61){//"="=61
        free(noeud);
        *i = indice;
        return false;
    }
    Noeud *filsCookieValue = malloc(sizeof(Noeud));
    if (!checkCookieValue(cookie, i, longueur, filsCookieValue)){
        free(noeud);
        *i = indice;
        return false;
    }

    if (noeud!=NULL){
        noeud->fils = malloc(3 * sizeof(Noeud));
        noeud->indice = indice;
        noeud->longueur = *i - indice;
        noeud->nombreFils = 3;
        noeud->tag = "cookie-pair";
    
        *i = indice; // On réinitialise i
        
        noeud->fils[0] = *filsCookieName;
        (*i) += noeud->fils[0].longueur;
        
        createFilsSimple("=", *i, 1, &noeud->fils[1]);
        (*i)++;

        noeud->fils[2] = *filsCookieValue;
        (*i) += noeud->fils[2].longueur;
    }
}

bool checkCookieName(const char cookie[], int *i, int longueur, Noeud *noeud){ //cookie-name = 1*tchar
    int compteur = 0;
    const int indice = *i;

    // On compte le nombre de caractères dans le champ courant
    while (*i < longueur && checkTChar(requete, *i, NULL)) {
        (*i)++;
        compteur++;
    }

    // Si on a 0 tchar, on renvoie false
    if (compteur < 1) {
        free(noeud);
        return false;
    }

    // On stocke les données nécessaires pour le noeud courant
    noeud->indice = indice;
    noeud->longueur = *i - indice;
    noeud->tag = "cookie-name";
    noeud->fils = malloc(compteur * sizeof(Noeud));
    noeud->nombreFils = compteur;

    // On réinitialise l'indice i pour la suite de la fonction
    *i = indice;

    // On remplit le tableau des fils du noeud cookie-name
    for (int j = 0; j < compteur; j++) {
        checkTChar(cookie, *i, &noeud->fils[j]); 
        (*i)++;
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

bool cookieValue(const char cookie[], int *i, int longueur, Noeud *noeud){ //cookie-value = ( DQUOTE *cookie-octet DQUOTE ) / *cookie-octet 
    const int indice=*i;
    int compteur=0;
    if (cookie[*i]==34){ 
        compteur++;
        (*i)++;
        while (checkCookieOctet(cookie, i) && *i<longueur){
            compteur++;
            (*i)++;
        }
        if (cookie[*i]==34){
            compteur++;
            (*i)++;
        }
        else {
            free(noeud);
            *i = indice;
            return false;

        }
    }
    else {
        while (checkCookieOctet(cookie, i) && *i<longueur){
            compteur++;
            (*i)++;
        }
    }
    // On stocke les données nécessaires pour le noeud courant
    noeud->indice = indice;
    noeud->longueur = *i - indice;
    noeud->tag = "cookie-value";
    noeud->fils = malloc(compteur * sizeof(Noeud));
    noeud->nombreFils = compteur;

    // On réinitialise l'indice i pour la suite de la fonction
    *i = indice;

    // On remplit le tableau des fils du noeud cookie-value
    for (int j = 0; j < compteur; j++) {
        Noeud *noeud = &noeud->fils[j];
        noeud->fils = NULL;
        noeud->indice = i;
        noeud->longueur = 1;
        noeud->nombreFils = 0;
        if (cookie[*i]==34){
            noeud->tag="DQUOTE";
        }
        else {
            noeud->tag="cookie-octet";
        }
        (*i)++;
    }  
    return true;
}

bool checkCookieOctet(const char cookie[], int *i){ //cookie-octet = %x21 / %x23-2B / %x2D-3A / %x3C-5B / %x5D-7E
    if (cookie[*i]==33 || (cookie[*i]>=35 && cookie[*i]<=43) || (cookie[*i]>=45 && cookie[*i]<=58) || (cookie[*i]>=60 && cookie[*i]<=91)){
        return true;
    }
    else {
        return false;
    }
}

bool checkAlpha(const char requete[], int i) {
    return ((requete[i] >= 'a' && requete[i] <= 'z') || (requete[i] >= 'A' && requete[i] <= 'Z'));
}

bool checkDigit(const char requete[], int i) {
    return (requete[i] >= '0' && requete[i] <= '9');
}