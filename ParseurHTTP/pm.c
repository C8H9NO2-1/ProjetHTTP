#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "structure.h"
#include "pm.h"
#include "affichage.h"

int main(int argc, char *argv[]) {
    
    char requete[] = "GET /index.html HTTP/1.0\r\n";

    Noeud *test = malloc(sizeof(Noeud));

    int i = 0;

    if (!checkStartLine(requete, &i, strlen(requete), test)) {
        printf("Hello world\n");
        test = NULL;
    }

    if (test != NULL) {
        printArbre(requete, test, 0);
    }
    
    return 0;
}

//!==============================Fonctions générales==============================

bool checkAlpha(const char requete[], int i) {
    return ((requete[i] >= 'a' && requete[i] <= 'z') || (requete[i] >= 'A' && requete[i] <= 'Z'));
}

bool checkDigit(const char requete[], int i) {
    return (requete[i] >= '0' && requete[i] <= '9');
}

bool checkHexdig(const char requete[], int i) {
    if (requete[i] == '%') {
        if (((requete[i + 1] >= 'a' && requete[i + 1] <= 'f') || (requete[i + 1] >= 'A' && requete[i + 1] <= 'F')) && ((requete[i + 2] >= 'a' && requete[i + 2] <= 'f') || (requete[i + 2] >= 'A' && requete[i + 2] <= 'F'))) {
            return true;
        }
    }
    return false;
}

bool checkUnreserved(const char requete[], int i) {
    switch (requete[i]) {
        case '-':
        case '.':
        case '_':
        case '~':
            return true;
            break;
        
        default:
            break;
    }

    return (checkAlpha(requete, i) || checkDigit(requete, i));
}

bool checkPctEncoded(const char requete[], int i) {
    return (requete[i] == '%' && checkHexdig(requete, i + 1) && checkHexdig(requete, i + 2));
}

bool checkSubDelims(const char requete[], int i) {
    switch (requete[i]) {
        case '!':
        case '$':
        case '&':
        case 39 : //? ' = 39 en ASCII
        case '(':
        case ')':
        case '*':
        case '+':
        case ',':
        case ';':
        case '=':
            return true;
            break;
        
        default:
            return false;
            break;
    }
}

void createFilsSimple(char nom[], int i, int longueur, Noeud *noeud) {
    noeud->tag = nom;
    noeud->fils = NULL;
    noeud->indice = i;
    noeud->longueur = longueur;
    noeud->nombreFils = 0;
}

//!===============================================================================

bool checkStartLine(const char requete[], int *i, int longueur, Noeud *noeud) {
    int nombreFils = 6;
    const int indice = *i;

    Noeud *filsMethod = malloc(sizeof(Noeud));
    if (!checkMethod(requete, i, longueur, filsMethod)) {
        free(noeud);
        *i = indice;
        return false;
    }

    if (requete[*i] != 32) { //? SP = 32
        free(noeud);
        *i = indice;
        return false;
    }

    (*i)++;

    Noeud *filsRequestTarget = malloc(sizeof(Noeud));
    if (!checkRequestTarget(requete, i, longueur, filsRequestTarget)) {
        free(noeud);
        *i = indice;
        return false;
    }

    if (requete[*i] != 32) {
        free(noeud);
        *i = indice;
        return false;
    }

    (*i)++;

    Noeud *filsVersion = malloc(sizeof(Noeud));
    if (!checkHTTPVersion(requete, i, longueur, filsVersion)) {
        free(noeud);
        *i = indice;
        return false;
    }

    printf("%c\n", requete[*i]);
    if (requete[*i] != 13) { //? CR = 13
        free(noeud);
        *i = indice;
        printf("Test CR\n");
        return false;
    }

    (*i)++;

    if (requete[*i] != 10) { //? LF = 10
        free(noeud);
        *i = indice;
        return false;
    }

    (*i)++;

    // Si tout s'est passé, on remplit le noeud start-line et on créé les noeuds fils qui ne le sont pas déjà
    noeud->fils = malloc(nombreFils * sizeof(Noeud));
    noeud->indice = indice;
    noeud->longueur = *i - indice;
    noeud->nombreFils = nombreFils;
    noeud->tag = "start-line";

    *i = indice; // On réinitialise i
    
    noeud->fils[0] = *filsMethod;
    (*i) += noeud->fils[0].longueur;
    
    createFilsSimple("SP", *i, 1, &noeud->fils[1]);
    (*i)++;
    
    noeud->fils[2] = *filsRequestTarget;
    (*i) += noeud->fils[2].longueur;

    createFilsSimple("SP", *i, 1, &noeud->fils[3]);
    (*i)++;

    noeud->fils[4] = *filsVersion;
    (*i) += noeud->fils[4].longueur;

    createFilsSimple("CRLF", *i, 2, &noeud->fils[5]);
    (*i) += noeud->fils[5].longueur;

    return true;
}

bool checkMethod(const char requete[], int *i, int longueur, Noeud *noeud) {
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
    noeud->tag = "method";
    noeud->fils = malloc(compteur * sizeof(Noeud));
    noeud->nombreFils = compteur;

    // On réinitialise l'indice i pour la suite de la fonction
    *i = indice;

    // On remplit le tableau des fils du noeud method
    for (int j = 0; j < compteur; j++) {
        checkTChar(requete, *i, &noeud->fils[j]); // On a pas besoin de récupérer la valeur de retour cette fois-ci
        (*i)++;
    }
    
    return true;
}

bool checkRequestTarget(const char requete[], int *i, const int longueur, Noeud *noeud) {
    // On remplit ce qu'on sait déjà du noeud
    noeud->indice = *i;
    noeud->tag = "request-target";
    
    // On est certain d'avoir une absolute-path comme fils
    Noeud *filsPath = malloc(sizeof(Noeud));
    Noeud *filsQuery1 = NULL;
    Noeud *filsQuery2 = NULL;

    if (!checkAbsolutePath(requete, i, longueur, filsPath)) {
        filsPath = NULL;
        return false;
    }

    // On regarde si il y a une query
    if (requete[*i] == '?') {
        filsQuery1 = malloc(sizeof(Noeud));
        filsQuery2 = malloc(sizeof(Noeud));
        createFilsSimple("case-insensitive-string", *i, 1, filsQuery1);
        (*i)++;
        checkQuery(requete, i, longueur, filsQuery2);
        noeud->nombreFils = 3;
        noeud->fils = malloc(3 * sizeof(Noeud));
        noeud->fils[0] = *filsPath;
        noeud->fils[1] = *filsQuery1;
        noeud->fils[2] = *filsQuery2;
        noeud->longueur = filsPath->longueur + filsQuery1->longueur + filsQuery2->longueur;
    } else {
        noeud->nombreFils = 1;
        noeud->fils = malloc(sizeof(Noeud));
        noeud->fils[0] = *filsPath;
        noeud->longueur = filsPath->longueur;
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

bool checkAbsolutePath(const char requete[], int *i, const int longueur, Noeud *noeud) {
    int compteur = 0;
    const int indice = *i;

    // On compte le nombre de fils dans le champ courant
    while ((*i < longueur) && (requete[*i] == '/')) {
        (*i)++;
        checkSegment(requete, i, longueur, NULL);
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
    noeud->tag = "absolute-path";
    noeud->fils = malloc(2 * compteur * sizeof(Noeud));
    noeud->nombreFils = 2 * compteur;

    // On réinitialise l'indice i pour la suite de la fonction
    *i = indice;

    // On remplit le tableau des fils du noeud method
    for (int j = 0; j < 2 * compteur; j += 2) {
        // On crée le fils contenant juste le caractère '/'
        createFilsSimple("case-insensitive-string", *i, 1, &noeud->fils[j]);
        (*i)++;
        checkSegment(requete, i, longueur, &noeud->fils[j + 1]); // On a pas besoin de récupérer la valeur de retour cette fois-ci
    }
    
    return true;
}

bool checkSegment(const char requete[], int *i, const int longueur, Noeud *noeud) {
    //! Même formule qu'avec checkTChar, il y a deux éxécutions différentes, en fonction de la valeur de noeud

    int compteur = 0;
    const int indice = *i;

    // On compte le nombre de fils dans le champ courant
    while (*i < longueur && checkPChar(requete, i, longueur, NULL)) {
        (*i)++;
        compteur++;
    }

    if (noeud != NULL) {    
        // On stocke les données nécessaires pour le noeud courant
        noeud->indice = indice;
        noeud->longueur = *i - indice;
        noeud->tag = "segment";
        noeud->nombreFils = compteur;
        if (compteur > 0) {
            noeud->fils = malloc(compteur * sizeof(Noeud));
        }

        // On réinitialise l'indice i pour la suite de la fonction
        *i = indice;

        // On remplit le tableau des fils du noeud method
        for (int j = 0; j < compteur; j++) {
            checkPChar(requete, i, longueur, &noeud->fils[j]); // On a pas besoin de récupérer la valeur de retour cette fois-ci
            (*i)++;
        }
    }
    
    return true;
}

bool checkPChar(const char requete[], int *i, const int longueur, Noeud *noeud) {
    //! Même formule qu'avec checkTChar, il y a deux éxécutions différentes, en fonction de la valeur de noeud

    // On a deux cas, soit c'est un nombre encodé en hexadécimal, soit ça n'en est pas un
    if (((*i + 2) < longueur) && (checkHexdig(requete, *i))) {
        // On stocke les données nécessaires pour le noeud courant
        printf("Test hexa\n");
        if (noeud != NULL) {
            noeud->fils = NULL;
            noeud->indice = *i;
            noeud->longueur = 3;
            noeud->nombreFils = 0;
            noeud->tag = "pchar";
        }

        (*i) += 2; // On fait en sorte que pour la fonction appelante, on doit juste incrémenter i de 1

        return true;
    } else {
        if (noeud != NULL) {
            noeud->fils = NULL;
            noeud->indice = *i;
            noeud->longueur = 1;
            noeud->nombreFils = 0;
            noeud->tag = "pchar";
        }

        // On vérifie que la syntaxe est correcte
        switch (requete[*i]) {
            case ':':
            case '@':
                return true;
                break;
        
            default:
                break;
        }

        if (checkUnreserved(requete, *i) || checkSubDelims(requete, *i)) {
            return true;
        }
    }

    // Si la syntaxe n'est pas correcte, on pense bien à libérer la mémoire
    if (noeud != NULL) {
        free(noeud);
    }

    return false;
}

bool checkQuery(const char requete[], int *i, const int longueur, Noeud *noeud) {
    int compteur = 0;
    const int indice = *i;

    // On compte le nombre de fils dans le champ courant
    while (*i < longueur && (checkPChar(requete, i, longueur, NULL) || requete[*i] == '/' || requete[*i] == '?')) {
        (*i)++;
        compteur++;
    }

    // On stocke les données nécessaires pour le noeud courant
    noeud->indice = indice;
    noeud->longueur = *i - indice;
    noeud->tag = "query";
    noeud->nombreFils = compteur;
    if (compteur > 0) {
        noeud->fils = malloc(compteur * sizeof(Noeud));
    }

    // On réinitialise l'indice i pour la suite de la fonction
    *i = indice;

    // On remplit le tableau des fils du noeud method
    for (int j = 0; j < compteur; j++) {
        if (requete[*i] == '/' || requete[*i] == '?') {
            createFilsSimple("case-insensitive-string", *i, 1, &noeud->fils[j]);
        }
        checkPChar(requete, i, longueur, &noeud->fils[j]);
        (*i)++;
    }
    
    return true;
}

bool checkHTTPVersion(const char requete[], int *i, const int longueur, Noeud *noeud) {
    int longTemp = 8; // On compte les 4 lettres du mot HTTP plus les versions
    const int indice = *i; // On garde en mémoire l'indice de début
    const int nombreFils = 5;

    if (requete[*i] != 'H' || requete[*i + 1] != 'T' || requete[*i + 2] != 'T' || requete[*i + 3] != 'P') {
        free(noeud);
        *i = indice;
        return false;
    }

    (*i) += 4;

    if (requete[*i] != '/') {
        free(noeud);
        *i = indice;
        return false;
    }

    (*i)++;

    if (!checkDigit(requete, *i)) {
        free(noeud);
        *i = indice;
        return false;
    }

    (*i)++;

    if (requete[*i] != '.') {
        free(noeud);
        *i = indice;
        return false;
    }

    (*i)++;

    if (!checkDigit(requete, *i)) {
        free(noeud);
        *i = indice;
        return false;
    }

    (*i)++;

    // Si tout s'est bien passé, on créé tous les noeuds fils et on remplit le noeud de la version HTTP
    noeud->fils = malloc(nombreFils * sizeof(Noeud));
    noeud->indice = indice;
    noeud->longueur = longTemp;
    noeud->nombreFils = nombreFils;
    noeud->tag = "HTTP-version";

    // On créé les fils

    noeud->fils[0].fils = NULL;
    noeud->fils[0].indice = indice;
    noeud->fils[0].longueur = 4;
    noeud->fils[0].nombreFils = 0;
    noeud->fils[0].tag = "HTTP-name";

    createFilsSimple("case-insensitive-string", indice + 4, 1, &noeud->fils[1]);
    createFilsSimple("DIGIT", indice + 5, 1, &noeud->fils[2]);
    createFilsSimple("case-insensitive-string", indice + 6, 1, &noeud->fils[3]);
    createFilsSimple("DIGIT", indice + 7, 1, &noeud->fils[4]);

    return true;
}