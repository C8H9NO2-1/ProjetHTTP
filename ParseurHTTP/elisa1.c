#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "structure.h"
#include "elisa1.h"
#include "affichage.h"

#define VERIFICATION1() if (*i >= longueur) { \
free(noeud); \
return false; } \

#define VERIFICATION() if (*i >= longueur) { \
return false; } \

void freeArbre(Noeud *racine) {
    for (int i = 0; i < racine->nombreFils; i++) {
        if (racine->fils[i].nombreFils != 0) {
            freeArbre(&racine->fils[i]);
        }
    }
    free(racine->fils);
}

int main(int argc, char *argv[]) {
    
    char cookie[] = "COoKiE: 	  n+q#_iP&W9EoHoE=\"/{i>}uPeh9TZ9]V\"; 4qdK8ZQdCyBMF.k=\"#j.=cFZ@$?^&-$4\"; k1KU^_X&opaKHta=:]xa&s<TDKnZPM*; zN&yiA`MFeSoGaa=*wwlhxR28?L![32  ";
    Noeud *test = malloc(sizeof(Noeud));

    int i = 0;

    if (!checkCookieHeader(cookie, &i, strlen(cookie), test)) {
        printf("Hello world\n");
        test = NULL;
    }

    if (test != NULL) {
        printArbre(test, 0);
        freeArbre(test);
    }
    
    free(test);

    return 0;
}

bool checkCookieHeader(char cookie[], int *i, int longueur, Noeud *noeud) { //Cookie-header = "Cookie" ":" OWS cookie-string OWS

    VERIFICATION1()

    int nombreFils = 5;
    const int indice = *i;

    Noeud *filsCookie = malloc(sizeof(Noeud));
    if (!checkCookie(cookie, filsCookie)) {
        free(filsCookie);
        free(noeud);
        *i = indice;
        return false;
    }
    *i=6;

    if (cookie[*i] != 58) { //? ":" = 58
        free(filsCookie);
        free(noeud);
        *i = indice;
        return false;
    }
    (*i)++;

    // Noeud *filsOWS1 = malloc(sizeof(Noeud));
    checkOWS(cookie, i, longueur, NULL);


    Noeud *filsCookieString = malloc(sizeof(Noeud));
    if (!checkCookieString(cookie, i, longueur, filsCookieString)) {
        free(filsCookieString);
        free(filsCookie);
        free(noeud);
        *i = indice;
        return false;
    }

    // Noeud *filsOWS2 = malloc(sizeof(Noeud));
    checkOWS(cookie, i, longueur, NULL);
   
    noeud->fils = malloc(nombreFils * sizeof(Noeud));
    noeud->valeur = cookie + indice;
    noeud->longueur = *i - indice;
    noeud->nombreFils = nombreFils;
    noeud->tag = "Cookie-header";

    *i = indice; // On réinitialise i
    
    noeud->fils[0] = *filsCookie;
    (*i) += noeud->fils[0].longueur;
    
    createFilsSimple("case_insensitive_string", cookie + *i, 1, &noeud->fils[1]);
    (*i)++;
    
    checkOWS(cookie, i, longueur, &noeud->fils[2]);
    // noeud->fils[2] = *filsOWS1;
    // (*i) += noeud->fils[2].longueur;

    noeud->fils[3] = *filsCookieString;
    (*i) += noeud->fils[3].longueur;

    checkOWS(cookie, i, longueur, &noeud->fils[4]);
    // noeud->fils[4] = *filsOWS2;
    // (*i) += noeud->fils[4].longueur;

    free(filsCookie);
    free(filsCookieString);

    return true;
}


bool checkCookie(char cookie[], Noeud *noeud){
    noeud->valeur = cookie;
    noeud->longueur = 6;
    noeud->tag = "case_insensitive_string";
    noeud->fils = NULL;
    noeud->nombreFils = 0;

    if (cookie[0] != 'C' && cookie[0] !='c' ) {
        printf("not c");
        return false;
    }
    if (cookie[1] != 'o' && cookie[1]!='O') {
        return false;
    }
    if (cookie[2] != 'o' && cookie[2]!='O') {
        return false;
    }
    if (cookie[3] != 'k' && cookie[3]!='K') {
        return false;
    }
    if (cookie[4] != 'i' && cookie[4]!='I') {
        return false;
    }
    if (cookie[5] != 'e' && cookie[5] != 'E') {
        return false;
    }

    return true;
}

bool checkOWS(char cookie[], int *i, int longueur, Noeud *noeud) { //OWS = *( SP / HTAB )
   //! Même formule qu'avec checkTChar, il y a deux éxécutions différentes, en fonction de la valeur de noeud

    int compteur = 0;
    const int indice = *i;

    // On compte le nombre de fils dans le champ courant
    while (*i < longueur && (cookie[*i] == 32 || cookie[*i] == 9)) {
        (*i)++;
        compteur++;
    }

    if (noeud != NULL) {
        // On stocke les données nécessaires pour le noeud courant
        noeud->valeur = cookie + indice;
        noeud->longueur = *i - indice;
        noeud->tag = "OWS";
        noeud->nombreFils = compteur;
        if (compteur > 0) {
            noeud->fils = malloc(compteur * sizeof(Noeud));
        }

        // On réinitialise l'indice i pour la suite de la fonction
        *i = indice;

        // On remplit le tableau des fils du noeud
        for (int j = 0; j < compteur; j++) {
            if (cookie[*i] == 32) {
                createFilsSimple("SP", cookie + *i, 1, &noeud->fils[j]);
            } else {
                createFilsSimple("HTAB", cookie + *i, 1, &noeud->fils[j]);               
            }
            (*i)++;
        }
    }
    
    return true;
}


bool checkCookieString(char cookie[], int *i, int longueur, Noeud *noeud){ //cookie-pair *( ";" SP cookie-pair )

    VERIFICATION()

    const int indice=*i;
    int compteur=0;

    // Noeud *filsCookiePair1 = malloc(sizeof(Noeud));
    if (!checkCookiePair(cookie, i, longueur, NULL)) {
        free(noeud);
        *i = indice;
        return false;
    }
    compteur++;

    while (*i < longueur && cookie[*i]==59){ //";"=59
        (*i)+=2;
        if(cookie[(*i)-1]!=32 || !checkCookiePair(cookie, i, longueur, NULL)){
            // free(noeud);
            *i = indice;
            return false;
        }
        compteur+=3;
    }

    if (compteur>1){ // Il y a au moins un "( ";" SP cookie-pair )"
        noeud->valeur = cookie + indice;
        noeud->longueur = *i - indice;
        noeud->tag = "cookie-string";
        noeud->fils = malloc(compteur * sizeof(Noeud));
        noeud->nombreFils = compteur;

        // On réinitialise l'indice i pour la suite de la fonction
        *i = indice;
        int j = 0;
        // On remplit le tableau des fils du noeud cookie-string
        // On remplit le premier fils cookie-pair
        // noeud->fils[j]=*filsCookiePair1;
        // (*i) += noeud->fils[j].longueur;
        checkCookiePair(cookie, i, longueur, &noeud->fils[j]);
        j++;

        while (*i < longueur && cookie[*i]==59 && j < compteur){ //";"=59
            createFilsSimple("case_insensitive_string", cookie + *i, 1, &noeud->fils[j]);
            j++;
            (*i)+=2;
            createFilsSimple("SP", cookie + (*i) - 1, 1, &noeud->fils[j]);
            j++;
            if(cookie[(*i)-1]!=32 || !checkCookiePair(cookie, i, longueur, &noeud->fils[j])){
                // free(noeud);
                *i = indice;
                return false;
            }
            j++;
        }
    }
    else {
        noeud->valeur = cookie + indice;
        noeud->longueur = *i - indice;
        noeud->tag = "cookie-string";
        noeud->fils = malloc(sizeof(Noeud));
        noeud->nombreFils = 1;
        // noeud->fils[0]=*filsCookiePair1;
        checkCookiePair(cookie, i, longueur, &noeud->fils[0]);
    }
    return true ;
}

bool checkCookiePair(char cookie[], int *i, int longueur, Noeud *noeud){ //cookie-pair = cookie-name "=" cookie-value

    VERIFICATION()

    const int indice=*i;
    Noeud *filsCookieName = malloc(sizeof(Noeud));

    if (!checkCookieName(cookie, i, longueur, filsCookieName)){
        // free(noeud);
        free(filsCookieName);
        *i = indice;
        return false;
    }
    if (cookie[*i]!=61){//"="=61
        // free(noeud);
        freeArbre(filsCookieName);
        free(filsCookieName);
        *i = indice;
        return false;
    }
    (*i)++;
    
    Noeud *filsCookieValue = malloc(sizeof(Noeud));
    if (!checkCookieValue(cookie, i, longueur, filsCookieValue)){
        // free(noeud);
        freeArbre(filsCookieName);
        free(filsCookieName);
        free(filsCookieValue);
        *i = indice;
        return false;
    }

    if (noeud!=NULL){
        noeud->fils = malloc(3 * sizeof(Noeud));
        noeud->valeur = cookie + indice;
        noeud->longueur = *i - indice;
        noeud->nombreFils = 3;
        noeud->tag = "cookie-pair";
    
        *i = indice; // On réinitialise i
        
        noeud->fils[0] = *filsCookieName;
        (*i) += noeud->fils[0].longueur;

        createFilsSimple("case_insensitive_string", cookie + *i, 1, &noeud->fils[1]);
        (*i)++;

        noeud->fils[2] = *filsCookieValue;
        (*i) += noeud->fils[2].longueur;
    } else {
        freeArbre(filsCookieName);
        freeArbre(filsCookieValue);
    }

    free(filsCookieName);
    free(filsCookieValue);

    return true;
}

bool checkCookieName(char cookie[], int *i, int longueur, Noeud *noeud){ //cookie-name = 1*tchar

    VERIFICATION()

    return checkToken(cookie, i, longueur, noeud, "cookie-name");

    // int compteur = 0;
    // int indice = *i;
    // // On compte le nombre de caractères dans le champ courant
    // while (*i < longueur && checkTChar(cookie, *i, NULL)) {
    //     (*i)++;
    //     compteur++;
    // }

    // // Si on a 0 tchar, on renvoie false
    // if (compteur < 1) {
    //     free(noeud);
    //     return false;
    // }
    // if (noeud!=NULL){
    //     // On stocke les données nécessaires pour le noeud courant
    //     noeud->valeur = cookie + indice;
    //     noeud->longueur = *i - indice;
    //     noeud->tag = "cookie-name";
    //     noeud->fils = malloc(compteur * sizeof(Noeud));
    //     noeud->nombreFils = compteur;

    //     // On réinitialise l'indice i pour la suite de la fonction
    //     *i = indice;

    //     // On remplit le tableau des fils du noeud cookie-name
    //     for (int j = 0; j < compteur; j++) {
    //         checkTChar(cookie, *i, &noeud->fils[j]); 
    //         (*i)++;
    //     }
    // }
    // return true;
}

bool checkTChar(char requete[], int i, Noeud *noeud) {
    //! Cette fonction a deux éxécution différentes:
    //* - Une qui ne fait que vérifier syntaxiquement un caractère
    //* - Une qui vérifie syntaxiquement un caractère mais stocke aussi ce caractère

    // On stocke les données nécessaires pour le noeud courant
    if (noeud != NULL) {
        noeud->fils = NULL;
        noeud->valeur = requete + i;
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
    // if (noeud != NULL) {
    //     free(noeud);
    // }
    return false;
}

bool checkCookieValue(char cookie[], int *i, int longueur, Noeud *noeud){ //cookie-value = ( DQUOTE *cookie-octet DQUOTE ) / *cookie-octet 
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
    if (noeud!=NULL){
        // On stocke les données nécessaires pour le noeud courant
        noeud->valeur = cookie + indice;
        noeud->longueur = *i - indice;
        noeud->tag = "cookie-value";
        noeud->fils = malloc(compteur * sizeof(Noeud));
        noeud->nombreFils = compteur;

        // On réinitialise l'indice i pour la suite de la fonction
        *i = indice;

        // On remplit le tableau des fils du noeud cookie-value
        for (int j = 0; j < compteur; j++) {
            if (cookie[*i]==34){
                createFilsSimple("DQUOTE", cookie + *i, 1, &noeud->fils[j]);
            }
            else {
                createFilsSimple("cookie-octet", cookie + *i, 1, &noeud->fils[j]);
            }
            (*i)++;
        } 
    } 
    return true;
}

bool checkCookieOctet(const char cookie[], int *i){ //cookie-octet = %x21 / %x23-2B / %x2D-3A / %x3C-5B / %x5D-7E

    if (cookie[*i]==33 || (cookie[*i]>=35 && cookie[*i]<=43) || (cookie[*i]>=45 && cookie[*i]<=58) || (cookie[*i]>=60 && cookie[*i]<=91)|| (cookie[*i]>=93 && cookie[*i]<=126)){
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

void createFilsSimple(char nom[], char *i, int longueur, Noeud *noeud) {
    noeud->tag = nom;
    noeud->fils = NULL;
    noeud->valeur = i;
    noeud->longueur = longueur;
    noeud->nombreFils = 0;
}

bool checkToken(char requete[], int *i, int longueur, Noeud *noeud, char nom[]) {

    VERIFICATION()

    int compteur = 0;
    const int indice = *i;

    // On compte le nombre de caractères dans le champ courant
    while (*i < longueur && checkTChar(requete, *i, NULL)) {
        (*i)++;
        compteur++;
    }

    // Si on a 0 tchar, on renvoie false
    if (compteur < 1) {
        // if (noeud != NULL) {
        //     free(noeud);
        // }
        (*i) = indice;
        return false;
    }

    if (noeud != NULL) {
        // On stocke les données nécessaires pour le noeud courant
        noeud->valeur = requete + indice;
        noeud->longueur = *i - indice;
        noeud->tag = nom;
        noeud->fils = malloc(compteur * sizeof(Noeud));
        noeud->nombreFils = compteur;

        // On réinitialise l'indice i pour la suite de la fonction
        *i = indice;

        // On remplit le tableau des fils du noeud method
        for (int j = 0; j < compteur; j++) {
            checkTChar(requete, *i, &noeud->fils[j]); // On a pas besoin de récupérer la valeur de retour cette fois-ci
            (*i)++;
        }
    }
    
    return true;
}