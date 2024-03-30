#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "structure.h"
#include "pm.h"
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

// int main(int argc, char *argv[]) {

//     // char requete[] = "GET /index.html HTTP/1.0\r\n";
//     // char requete[] = "EKr1czBS+P*RAja /6Q@_C;IqPpdWi:I/4:b9Hra7UELY2tN/!xDzvl9mpYm)Y.j/MS!zM,C2P'!Z0l@ HTTP/4.0\r\n ";
//     char requete[] = "CoNNEctIon: , Keep-alive,     	keep-alive, 	close,	test,  ";
//     // char requete[] = "CONNeCtION:, 		, ,5_p8ck0lJ|vM-lx	,";

//     Noeud *test = malloc(sizeof(Noeud));

//     int i = 0;

//     if (!checkConnectionHeader(requete, &i, strlen(requete), test)) {
//         printf("Hello world\n");
//         test = NULL;
//     }

//     if (test != NULL) {
//         printArbre(test, 0);
//         freeArbre(test);
//     }

//     free(test);

//     return 0;
// }

//!==============================Fonctions générales==============================

bool checkAlpha(const char requete[], int i) {
    return ((requete[i] >= 'a' && requete[i] <= 'z') || (requete[i] >= 'A' && requete[i] <= 'Z'));
}

bool checkDigit(const char requete[], int i) {
    return (requete[i] >= '0' && requete[i] <= '9');
}

bool checkHexdig(const char requete[], int i) {
    if ((requete[i] >= 'a' && requete[i] <= 'f') || (requete[i] >= 'A' && requete[i] <= 'F') || (requete[i] >= '0' && requete[i] <= '9')) {
        return true;
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

void createFilsSimple(char nom[], char *i, int longueur, Noeud *noeud) {
    noeud->tag = nom;
    noeud->fils = NULL;
    noeud->valeur = i;
    noeud->longueur = longueur;
    noeud->nombreFils = 0;
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

void sousChaineMinuscule(const char chaine1[], char chaine2[], int i, int j) {
    int diff = 'a' - 'A';
    int n = strlen(chaine1);
    for (int k = i; k < j; k++) {
        if (k < n) {
            if (chaine1[k] >= 'A' && chaine1[k] <= 'Z') {
                chaine2[k - i] = chaine1[k] + diff;
            } else {
                chaine2[k - i] = chaine1[k];
            }
        }
    }
    chaine2[j - i] = '\0';
}

//!===============================================================================

bool checkStartLine(char requete[], int *i, int longueur, Noeud *noeud) {

    VERIFICATION1()

    int nombreFils = 6;
    const int indice = *i;

    Noeud *filsMethod = malloc(sizeof(Noeud));
    if (!checkMethod(requete, i, longueur, filsMethod)) {
        free(noeud);
        free(filsMethod);
        *i = indice;
        return false;
    }

    if (requete[*i] != 32) { //? SP = 32
        free(noeud);
        freeArbre(filsMethod);
        free(filsMethod);
        *i = indice;
        return false;
    }

    (*i)++;

    Noeud *filsRequestTarget = malloc(sizeof(Noeud));
    if (!checkRequestTarget(requete, i, longueur, filsRequestTarget)) {
        free(noeud);
        free(filsRequestTarget);
        freeArbre(filsMethod);
        free(filsMethod);
        *i = indice;
        return false;
    }

    if (requete[*i] != 32) {
        free(noeud);
        freeArbre(filsMethod);
        free(filsMethod);
        freeArbre(filsRequestTarget);
        free(filsRequestTarget);
        *i = indice;
        return false;
    }

    (*i)++;

    Noeud *filsVersion = malloc(sizeof(Noeud));
    if (!checkHTTPVersion(requete, i, longueur, filsVersion)) {
        free(noeud);
        freeArbre(filsMethod);
        free(filsMethod);
        freeArbre(filsRequestTarget);
        free(filsRequestTarget);
        free(filsVersion);
        *i = indice;
        return false;
    }

    // printf("%c\n", requete[*i]);
    if (requete[*i] != 13) { //? CR = 13
        free(noeud);
        freeArbre(filsMethod);
        free(filsMethod);
        freeArbre(filsRequestTarget);
        free(filsRequestTarget);
        freeArbre(filsVersion);
        free(filsVersion);
        *i = indice;
        return false;
    }

    (*i)++;

    if (requete[*i] != 10) { //? LF = 10
        free(noeud);
        freeArbre(filsMethod);
        free(filsMethod);
        freeArbre(filsRequestTarget);
        free(filsRequestTarget);
        freeArbre(filsVersion);
        free(filsVersion);
        *i = indice;
        return false;
    }

    (*i)++;

    // Si tout s'est passé, on remplit le noeud start-line et on créé les noeuds fils qui ne le sont pas déjà
    noeud->fils = malloc(nombreFils * sizeof(Noeud));
    noeud->valeur = requete + indice;
    noeud->longueur = *i - indice;
    noeud->nombreFils = nombreFils;
    noeud->tag = "start-line";

    *i = indice; // On réinitialise i

    noeud->fils[0] = *filsMethod;
    (*i) += noeud->fils[0].longueur;

    createFilsSimple("SP", requete + *i, 1, &noeud->fils[1]);
    (*i)++;

    noeud->fils[2] = *filsRequestTarget;
    (*i) += noeud->fils[2].longueur;

    createFilsSimple("SP", requete + *i, 1, &noeud->fils[3]);
    (*i)++;

    noeud->fils[4] = *filsVersion;
    (*i) += noeud->fils[4].longueur;

    createFilsSimple("CRLF", requete + *i, 2, &noeud->fils[5]);
    (*i) += noeud->fils[5].longueur;

    free(filsMethod);
    free(filsRequestTarget);
    free(filsVersion);

    return true;
}

bool checkMethod(char requete[], int *i, int longueur, Noeud *noeud) {

    VERIFICATION()

    return checkToken(requete, i, longueur, noeud, "method");
}

bool checkRequestTarget(char requete[], int *i, const int longueur, Noeud *noeud) {

    VERIFICATION()

    // On remplit ce qu'on sait déjà du noeud
    noeud->valeur = requete + *i;
    noeud->tag = "request-target";

    // On est certain d'avoir une absolute-path comme fils
    Noeud *filsPath = malloc(sizeof(Noeud));
    Noeud *filsQuery1 = NULL;
    Noeud *filsQuery2 = NULL;

    if (!checkAbsolutePath(requete, i, longueur, filsPath)) {
        // filsPath = NULL;
        free(filsPath);
        return false;
    }

    // On regarde si il y a une query
    if (requete[*i] == '?') {
        filsQuery1 = malloc(sizeof(Noeud));
        filsQuery2 = malloc(sizeof(Noeud));
        createFilsSimple("case_insensitive_string", requete + *i, 1, filsQuery1);
        (*i)++;
        checkQuery(requete, i, longueur, filsQuery2);
        noeud->nombreFils = 3;
        noeud->fils = malloc(3 * sizeof(Noeud));
        noeud->fils[0] = *filsPath;
        noeud->fils[1] = *filsQuery1;
        noeud->fils[2] = *filsQuery2;
        noeud->longueur = filsPath->longueur + filsQuery1->longueur + filsQuery2->longueur;

        free(filsQuery1);
        free(filsQuery2);
    } else {
        noeud->nombreFils = 1;
        noeud->fils = malloc(sizeof(Noeud));
        noeud->fils[0] = *filsPath;
        noeud->longueur = filsPath->longueur;
    }

    free(filsPath);

    return true;
}

bool checkAbsolutePath(char requete[], int *i, const int longueur, Noeud *noeud) {

    VERIFICATION()

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
        // free(noeud);
        return false;
    }

    // On stocke les données nécessaires pour le noeud courant
    noeud->valeur = requete + indice;
    noeud->longueur = *i - indice;
    noeud->tag = "absolute-path";
    noeud->fils = malloc(2 * compteur * sizeof(Noeud));
    noeud->nombreFils = 2 * compteur;

    // On réinitialise l'indice i pour la suite de la fonction
    *i = indice;

    // On remplit le tableau des fils du noeud
    for (int j = 0; j < 2 * compteur; j += 2) {
        // On crée le fils contenant juste le caractère '/'
        createFilsSimple("case_insensitive_string", requete + *i, 1, &noeud->fils[j]);
        (*i)++;
        checkSegment(requete, i, longueur, &noeud->fils[j + 1]); // On a pas besoin de récupérer la valeur de retour cette fois-ci
    }

    return true;
}

bool checkSegment(char requete[], int *i, const int longueur, Noeud *noeud) {
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
        noeud->valeur = requete + indice;
        noeud->longueur = *i - indice;
        noeud->tag = "segment";
        noeud->nombreFils = compteur;
        if (compteur > 0) {
            noeud->fils = malloc(compteur * sizeof(Noeud));
        }

        // On réinitialise l'indice i pour la suite de la fonction
        *i = indice;

        // On remplit le tableau des fils du noeud
        for (int j = 0; j < compteur; j++) {
            checkPChar(requete, i, longueur, &noeud->fils[j]); // On a pas besoin de récupérer la valeur de retour cette fois-ci
            (*i)++;
        }
    }

    return true;
}

bool checkPChar(char requete[], int *i, const int longueur, Noeud *noeud) {
    //! Même formule qu'avec checkTChar, il y a deux éxécutions différentes, en fonction de la valeur de noeud

    // On a deux cas, soit c'est un nombre encodé en hexadécimal, soit ça n'en est pas un
    if (((*i + 2) < longueur) && (checkPctEncoded(requete, *i))) {
        // On stocke les données nécessaires pour le noeud courant
        if (noeud != NULL) {
            noeud->fils = NULL;
            noeud->valeur = requete + *i;
            noeud->longueur = 3;
            noeud->nombreFils = 0;
            noeud->tag = "pchar";
        }

        (*i) += 2; // On fait en sorte que pour la fonction appelante, on doit juste incrémenter i de 1

        return true;
    } else {
        if (noeud != NULL) {
            noeud->fils = NULL;
            noeud->valeur = requete + *i;
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

    // Si la syntaxe n'est pas correcte, on libère la mémoire et on renvoie false
    // if (noeud != NULL) {
    //     free(noeud);
    // }

    return false;
}

bool checkQuery(char requete[], int *i, const int longueur, Noeud *noeud) {
    int compteur = 0;
    const int indice = *i;

    // On compte le nombre de fils dans le champ courant

    while (*i < longueur && (checkPChar(requete, i, longueur, NULL) || requete[*i] == '/' || requete[*i] == '?')) {
        (*i)++;
        compteur++;
    }

    // On stocke les données nécessaires pour le noeud courant
    noeud->valeur = requete + indice;
    noeud->longueur = *i - indice;
    noeud->tag = "query";
    noeud->nombreFils = compteur;
    if (compteur > 0) {
        noeud->fils = malloc(compteur * sizeof(Noeud));
    }

    // On réinitialise l'indice i pour la suite de la fonction
    *i = indice;

    // On remplit le tableau des fils du noeud
    for (int j = 0; j < compteur; j++) {
        if (requete[*i] == '/' || requete[*i] == '?') {
            createFilsSimple("case_insensitive_string", requete + *i, 1, &noeud->fils[j]);
            (*i)++;
            continue;
        }
        checkPChar(requete, i, longueur, &noeud->fils[j]);
        (*i)++;
    }

    return true;
}

bool checkHTTPVersion(char requete[], int *i, const int longueur, Noeud *noeud) {

    VERIFICATION()

    if (*i + 8 >= longueur) {
        return false;
    }

    int longTemp = 8; // On compte les 4 lettres du mot HTTP plus les versions
    const int indice = *i; // On garde en mémoire l'indice de début
    const int nombreFils = 5;

    if (requete[*i] != 'H' || requete[*i + 1] != 'T' || requete[*i + 2] != 'T' || requete[*i + 3] != 'P') {
        // free(noeud);
        *i = indice;
        return false;
    }

    (*i) += 4;

    if (requete[*i] != '/') {
        // free(noeud);
        *i = indice;
        return false;
    }

    (*i)++;

    if (!checkDigit(requete, *i)) {
        // free(noeud);
        *i = indice;
        return false;
    }

    (*i)++;

    if (requete[*i] != '.') {
        // free(noeud);
        *i = indice;
        return false;
    }

    (*i)++;

    if (!checkDigit(requete, *i)) {
        // free(noeud);
        *i = indice;
        return false;
    }

    (*i)++;

    // Si tout s'est bien passé, on créé tous les noeuds fils et on remplit le noeud de la version HTTP
    noeud->fils = malloc(nombreFils * sizeof(Noeud));
    noeud->valeur = requete + indice;
    noeud->longueur = longTemp;
    noeud->nombreFils = nombreFils;
    noeud->tag = "HTTP-version";

    // On créé les fils

    // noeud->fils[0].fils = NULL;
    // noeud->fils[0].indice = indice;
    // noeud->fils[0].longueur = 4;
    // noeud->fils[0].nombreFils = 0;
    // noeud->fils[0].tag = "HTTP-name";

    createFilsSimple("HTTP-name", requete + indice, 4, &noeud->fils[0]);
    createFilsSimple("case_insensitive_string", requete + indice + 4, 1, &noeud->fils[1]);
    createFilsSimple("DIGIT", requete + indice + 5, 1, &noeud->fils[2]);
    createFilsSimple("case_insensitive_string", requete + indice + 6, 1, &noeud->fils[3]);
    createFilsSimple("DIGIT", requete + indice + 7, 1, &noeud->fils[4]);

    return true;
}

//!===============================================================================

bool checkConnectionHeader(char requete[], int *i, const int longueur, Noeud *noeud) {

    VERIFICATION1()

    int nombreFils = 5;
    const int indice = *i;

    noeud->fils = malloc(nombreFils * sizeof(Noeud));
    noeud->valeur = requete + indice;
    // On ne connaît pas encore noeud->longueur
    noeud->nombreFils = nombreFils;
    noeud->tag = "Connection-header";

    int j = 0; // On utilise cet indice pour compter les fils

    if (!checkConnectionString(requete, i, &noeud->fils[j])) {
		// for (int k = 0; k < nombreFils; k ++) {
		// 	if (k != j) {
		// 		free(&noeud->fils[k]);
		// 	}
		// }
        free(noeud->fils);
        free(noeud);
        *i = indice;
        return false;
    }
    j++;

    if (requete[*i] != ':') {
        // for (int k = 0; k < nombreFils; k ++) {
		// 	if (k != j) {
		// 		free(&noeud->fils[k]);
		// 	}
		// }
        free(noeud->fils);
        free(noeud);
        *i = indice;
        return false;
    } else {
        createFilsSimple("case_insensitive_string", requete + *i, 1, &noeud->fils[j]);
        (*i)++;
    }
    j++;

    checkOWS(requete, i, longueur, &noeud->fils[j]);
    j++;

    if (!checkConnection(requete, i, longueur, &noeud->fils[j])) {
        // for (int k = 0; k < nombreFils; k ++) {
		// 	if (k != j) {
		// 		free(&noeud->fils[k]);
		// 	}
		// }
        free(noeud->fils);
        free(noeud);
        *i = indice;
        return false;
    }
    j++;

    checkOWS(requete, i, longueur, &noeud->fils[j]);
    j++;

    noeud->longueur = *i - indice;

    return true;
}

bool checkConnectionString(char requete[], int *i, Noeud *noeud) {
    const int indice = *i; // On conserve l'indice de début
    int n = 10; // Taille de la sous-chaîne qui contient potentiellement "Connection"
    char *chaineConnection = malloc((n + 1) * sizeof(char)); // On ajoute un caractère pour le \0

    sousChaineMinuscule(requete, chaineConnection, *i, *i + n);

    if (strcmp(chaineConnection, "connection") != 0) {
        // free(noeud);
        free(chaineConnection);
        *i = indice;
        return false;
    }

    (*i) += n;
    free(chaineConnection);

    // Si tout s'est bien passé, on créé le noeud contenant "Connection"
    noeud->fils = NULL;
    noeud->valeur = requete + indice;
    noeud->longueur = *i - indice;
    noeud->nombreFils = 0;
    noeud->tag = "case_insensitive_string";

    return true;
}

bool checkOWS(char requete[], int *i, const int longueur, Noeud *noeud) {
    //! Même formule qu'avec checkTChar, il y a deux éxécutions différentes, en fonction de la valeur de noeud

    int compteur = 0;
    const int indice = *i;

    // On compte le nombre de fils dans le champ courant
    while (*i < longueur && (requete[*i] == 32 || requete[*i] == 9)) {
        (*i)++;
        compteur++;
    }

    if (noeud != NULL) {
        // On stocke les données nécessaires pour le noeud courant
        noeud->valeur = requete + indice;
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
            if (requete[*i] == 32) {
                createFilsSimple("SP", requete + *i, 1, &noeud->fils[j]);
            } else {
                createFilsSimple("HTAB", requete + *i, 1, &noeud->fils[j]);
            }
            (*i)++;
        }
    }

    return true;
}

bool checkConnection(char requete[], int *i, const int longueur, Noeud *noeud) {

    VERIFICATION()

    // On vérifie si on a une ou plusieurs virgule et on en compte le nombre
    int compteur = 0;
    const int indice = *i; // On stocke l'indice du début de la chaîne
    int indiceTempSortie = *i; // Cet indice est utilisé avant de vérifier un OWS car on ne sait pas si celui-ci sera le dernier ou pas
    int indiceTemp = *i;

    while (*i < longueur && requete[*i] == ',') {
        (*i)++;
        checkOWS(requete, i, longueur, NULL);
        compteur += 2;
    }

    // On créé ensuite le premier fils connection-option
    Noeud *filsConnectionOption = malloc(sizeof(Noeud));
    if (!checkConnectionOption(requete, i, longueur, filsConnectionOption)) {
        free(filsConnectionOption);
        // free(noeud);
        *i = indice;
        return false;
    }
    compteur++;
    free(filsConnectionOption->fils);
    free(filsConnectionOption);

    // On a un problème car après la rulename Connection on a un OWS
    // Or ici on cherche aussi un ou plusieurs OWS chacun suivi d'une virgule
    // Ainsi on doit avancer mais pas trop

    // On vérifie si on a quelque chose après l'OWS suivant
    indiceTempSortie = *i; // Indice avant l'OWS
    checkOWS(requete, i, longueur, NULL);
    compteur++;

    // On vérifie en boucle si on a des virgules ou d'autres option de connection
    while (*i < longueur && requete[*i] == ',') {
        compteur++;
        (*i)++;
        // On passe ensuite l'OWS suivant
        indiceTempSortie = *i;
        checkOWS(requete, i, longueur, NULL);
        compteur++;
        // On vérifie si on a une virgule ou une connection-option
        Noeud *temp = malloc(sizeof(Noeud)); // Ce n'est pas très propre mais c'est ce que j'ai trouvé de plus simple
        indiceTemp = *i; // On veut pouvoir revenir en arrière si on a pas de connectionOption
        if (!checkConnectionOption(requete, i, longueur, temp)) {
            free(temp);
            *i = indiceTemp;
        } else {
            free(temp->fils);
            free(temp);
            compteur++; // On compte le champ connection-option
            // On passe ensuite l'OWS suivant
            indiceTempSortie = *i;
            checkOWS(requete, i, longueur, NULL);
            compteur++;
        }
    }

    // Si on sort de la boucle parce qu'il nous manque une virgule, on réinitialise l'indice et on décrémente le compteur
    // Pour ne pas compter le dernier OWS
    if (requete[*i] != ',') { // Cette ligne peut peut-être poser des problèmes si *i est trop grand mais à priori non
        compteur--;
        *i = indiceTempSortie;
    }

    // Si tout s'est bien passé, on créé tous les noeuds fils et le noeud parent
    noeud->fils = malloc(compteur * sizeof(Noeud));
    noeud->valeur = requete + indice;
    noeud->longueur = *i - indice;
    noeud->nombreFils = compteur;
    noeud->tag = "Connection";

    // On réinitialise *i
    *i = indice;

    int j = 0; // On l'utilise pour compter le nombre de fils
    // On remplit les premiers fils
    while (*i < longueur && requete[*i] == ',') {
        createFilsSimple("case_insensitive_string", requete + *i, 1, &noeud->fils[j]);
        (*i)++;
        j++;
        checkOWS(requete, i, longueur, &noeud->fils[j]);
        j++;
    }

    checkConnectionOption(requete, i, longueur, &noeud->fils[j]);
    j++;

    indiceTempSortie = *i; // Indice avant l'OWS
    if (j < compteur) {
        checkOWS(requete, i, longueur, &noeud->fils[j]);
        j++;
    }

    // On vérifie en boucle si on a des virgules ou d'autres option de connection
    //! Dans cette boucle on fait attention car il se peut qu'on atteigne un j trop grand
    //! On met donc des if partout
    while (*i < longueur && requete[*i] == ',') {
        createFilsSimple("case_insensitive_string", requete + *i, 1, &noeud->fils[j]);
        (*i)++;
        j++;
        if (j < compteur) {
            // On passe ensuite l'OWS suivant
            indiceTempSortie = *i;
            checkOWS(requete, i, longueur, &noeud->fils[j]);
            j++;
        }
        // On vérifie si on a une virgule ou une connection-option
        indiceTemp = *i; // On veut pouvoir revenir en arrière si on a pas de connectionOption
        if (j < compteur && !checkConnectionOption(requete, i, longueur, &noeud->fils[j])) {
            *i = indiceTemp;
        } else {
            j++;
            // On passe ensuite l'OWS suivant
            indiceTempSortie = *i;
            if (j < compteur) {
                checkOWS(requete, i, longueur, &noeud->fils[j]);
                j++;
            }
        }
    }

    if (requete[*i] != ',') { // Cette ligne peut peut-être poser des problèmes si *i est trop grand mais à priori non
        *i = indiceTempSortie;
    }

    return true;
}

bool checkConnectionOption(char requete[], int *i, int longueur, Noeud *noeud) {

    VERIFICATION()

    return checkToken(requete, i, longueur, noeud, "connection-option");
}
