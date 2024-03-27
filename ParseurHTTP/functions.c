#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "structure.h"
#include "affichage.h"
#include "functions.h"

#define VERIFICATION1() if (*i >= longueur) { \
free(noeud); \
return false; } \

#define VERIFICATION() if (*i >= longueur) { \
return false; } \

// int main(int argc, char *argv[]) {
    
//     // char requete[] = "GET /index.html HTTP/1.0\r\n";
//     // char requete[] = "EKr1czBS+P*RAja /6Q@_C;IqPpdWi:I/4:b9Hra7UELY2tN/!xDzvl9mpYm)Y.j/MS!zM,C2P'!Z0l@ HTTP/4.0\r\n";
//     // char requete[] = "CoNNEctIon: , Keep-alive,     	keep-alive, 	close,	test,";
//     char requete[] = "CONNeCtION:, 		, ,5_p8ck0lJ|vM-lx	,";
//     char cookie[] = "COoKiE: 	  n+q#_iP&W9EoHoE=\"/{i>}uPeh9TZ9]V\"h; 4qdK8ZQdCyBMF.k=\"#j.=cFZ@$?^&-$4\"; k1KU^_X&opaKHta=:]xa&s<TDKnZPM*; zN&yiA`MFeSoGaa=*wwlhxR28?L![32";

//     char transferEncoding[] = "TRaNSFEr-enCODiNG:  ,20Kmy%d_Yg8t.MS; 	eh6owY1JpoLr99n=\"k_*	lx:H~ rV\\-\\R\\	!Pa,v\"";

//     Noeud *test = malloc(sizeof(Noeud));

//     int i = 0;

//     if (!checkCookieHeader(cookie, &i, strlen(cookie), test)) {
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

//! Fonctions pour le parseur final
int checkCRLF(char requete[], const int longueur, int i) {
    while ((i+1) < longueur && (requete[i] != 13 || requete[i + 1] != 10)) {
        i++;
    }
    return i;
}

bool checkCRLFBool(char requete[], const int longueur, int i) {
    if ((i +1) >= longueur ) {
        return false;
    }
    if (requete[i] == 13 && requete[i + 1] == 10) {
        return true;
    }
    return false;
}

int compteHeader(char requete[], int i, int longueur, Header tabHeader[]) {

    int indice = i;
    int j = i;
    int k = 0;

    if (tabHeader == NULL) {
        while (j < longueur) {
            indice = j;
            Noeud *test = malloc(sizeof(Noeud));
            if (checkConnectionHeader(requete, &j, longueur, test)) {
                // printf("j => %d\n", j);
                if (!checkCRLFBool(requete, longueur,j)) {
                    return k;
                } else {
                    k++;
                    j += 2;
                }
                continue;
            }

            j = indice;
            test = malloc(sizeof(Noeud));
            if(checkCookieHeader(requete, &j, longueur, test)) {
                if (!checkCRLFBool(requete, longueur,j)) {
                    return k;
                } else {
                    k++;
                    j += 2;
                }
                continue;
            }

            // j = indice;
            // test = malloc(sizeof(Noeud));
            // if(checkContentLengthHeader(requete, &j, longueur, test)) {
            //     if (!checkCRLFBool(requete, longueur,j)) {
                    // return k;
                    // } else {
                    //     k++;
                    //     j += 2;
                    // }
                    // continue;
            // }
            // j = indice
            // test = malloc(sizeof(Noeud));
            // if(checkContentTypeHeader(requete, &j, longueur,  test)) {
            //     if (!checkCRLFBool(requete, longueur,j)) {
                    // return k;
                    // } else {
                    //     k++;
                    //     j += 2;
                    // }
                    // continue;
            // }

            j = indice;
            test = malloc(sizeof(Noeud));
            if(checkTransferEncodingHeader(requete, &j, longueur, test)) {
                if (!checkCRLFBool(requete, longueur,j)) {
                    return k;
                } else {
                    k++;
                    j += 2;
                }
                continue;
            }

            j = indice;
            test = malloc(sizeof(Noeud));
            if(checkExpectHeader(requete, &j, longueur,  test)) {
                if (!checkCRLFBool(requete, longueur,j)) {
                    return k;
                } else {
                    k++;
                    j += 2;
                }
                continue;
            }

            j = indice; 
            test = malloc(sizeof(Noeud));
            if(checkHostHeader(requete, &j, longueur,  test)) {
                if (!checkCRLFBool(requete, longueur,j)) {
                    return k;
                } else {
                    k++;
                    j += 2;
                }
                continue;
            }

            return k;
        }
    } else {
        while (j < longueur) {
            indice = j;
            Noeud *test = malloc(sizeof(Noeud));
            if (checkConnectionHeader(requete, &j, longueur, test)) {
                if (!checkCRLFBool(requete, longueur,j)) {
                    return k;
                } else {
                    tabHeader[k] = CONNECTION;
                    k++;
                    j += 2;
                }
                continue;
            }

            j = indice;
            test = malloc(sizeof(Noeud));
            if(checkCookieHeader(requete, &j, longueur, test)) {
                if (!checkCRLFBool(requete, longueur,j)) {
                    return k;
                } else {
                    tabHeader[k] = COOKIE;
                    k++;
                    j += 2;
                }
                continue;
            }

            // j = indice;
            // test = malloc(sizeof(Noeud));
            // if(checkContentLengthHeader(requete, &j, longueur, test)) {
            //     if (!checkCRLFBool(requete, longueur,j)) {
                    // return k;
                    // } else {
                        // tabHeader[k] = CONTENT_LENGTH;
                    //     k++;
                    //     j += 2;
                    // }
                    // continue;
            // }
            // j = indice
            // test = malloc(sizeof(Noeud));
            // if(checkContentTypeHeader(requete, &j, longueur,  test)) {
            //     if (!checkCRLFBool(requete, longueur,j)) {
                    // return k;
                    // } else {
                        // tabHeader[k] = CONTENT_TYPE;
                    //     k++;
                    //     j += 2;
                    // }
                    // continue;
            // }

            j = indice;
            test = malloc(sizeof(Noeud));
            if(checkTransferEncodingHeader(requete, &j, longueur, test)) {
                if (!checkCRLFBool(requete, longueur,j)) {
                    return k;
                } else {
                    tabHeader[k] = TRANSFER_ENCODING;
                    k++;
                    j += 2;
                }
                continue;
            }

            j = indice;
            test = malloc(sizeof(Noeud));
            if(checkExpectHeader(requete, &j, longueur,  test)) {
                if (!checkCRLFBool(requete, longueur,j)) {
                    return k;
                } else {
                    tabHeader[k] = EXPECT;
                    k++;
                    j += 2;
                }
                continue;
            }

            j = indice; 
            test = malloc(sizeof(Noeud));
            if(checkHostHeader(requete, &j, longueur,  test)) {
                if (!checkCRLFBool(requete, longueur,j)) {
                    return k;
                } else {
                    tabHeader[k] = HOST;
                    k++;
                    j += 2;
                }
                continue;
            }

            return k;
        }
    }
    return k;
}

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

void freeArbre(Noeud *racine) {
    for (int i = 0; i < racine->nombreFils; i++) {
        if (racine->fils[i].nombreFils != 0) {
            freeArbre(&racine->fils[i]);
        }
    }
    free(racine->fils);
}

//!===============================================================================
//? Fonctions utiles pour parser la start-line

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
        *i = indice;
        return false;
    }

    (*i) += 4;

    if (requete[*i] != '/') {
        *i = indice;
        return false;
    }

    (*i)++;

    if (!checkDigit(requete, *i)) {
        *i = indice;
        return false;
    }

    (*i)++;

    if (requete[*i] != '.') {
        *i = indice;
        return false;
    }

    (*i)++;

    if (!checkDigit(requete, *i)) {
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
    createFilsSimple("HTTP-name", requete + indice, 4, &noeud->fils[0]);
    createFilsSimple("case_insensitive_string", requete + indice + 4, 1, &noeud->fils[1]);
    createFilsSimple("DIGIT", requete + indice + 5, 1, &noeud->fils[2]);
    createFilsSimple("case_insensitive_string", requete + indice + 6, 1, &noeud->fils[3]);
    createFilsSimple("DIGIT", requete + indice + 7, 1, &noeud->fils[4]);

    return true;
}

//!===============================================================================
//? Fonctions utiles pour parser le connection-header

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
        free(noeud->fils);
        free(noeud);
        *i = indice;
        return false;
    }
    j++;

    if (requete[*i] != ':') {
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

//!===============================================================================
//? Fonctions utiles pour parser le cookie-header

bool checkCookieHeader(char cookie[], int *i, int longueur, Noeud *noeud) { //Cookie-header = "Cookie" ":" OWS cookie-string OWS

    VERIFICATION1()

    int nombreFils = 5;
    const int indice = *i;

    Noeud *filsCookie = malloc(sizeof(Noeud));
    if (!checkCookie(cookie, i, filsCookie)) {
        free(filsCookie);
        free(noeud);
        *i = indice;
        return false;
    }
    (*i) += 6;

    if (cookie[*i] != 58) { //? ":" = 58
        free(filsCookie);
        free(noeud);
        *i = indice;
        return false;
    }
    (*i)++;

    checkOWS(cookie, i, longueur, NULL);


    Noeud *filsCookieString = malloc(sizeof(Noeud));
    if (!checkCookieString(cookie, i, longueur, filsCookieString)) {
        free(filsCookieString);
        free(filsCookie);
        free(noeud);
        *i = indice;
        return false;
    }

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

    noeud->fils[3] = *filsCookieString;
    (*i) += noeud->fils[3].longueur;

    checkOWS(cookie, i, longueur, &noeud->fils[4]);

    free(filsCookie);
    free(filsCookieString);

    return true;
}

bool checkCookie(char cookie[], int *i, Noeud *noeud) {
    noeud->valeur = cookie + *i;
    noeud->longueur = 6;
    noeud->tag = "case_insensitive_string";
    noeud->fils = NULL;
    noeud->nombreFils = 0;
    char cookieMinuscule[7];
    sousChaineMinuscule(cookie, cookieMinuscule, *i, *i + 6);
    int a=strcmp("cookie", cookieMinuscule);
    if (a!=0){
        // free(noeud);
        return false ;
    }
    return true;
}

bool checkCookieString(char cookie[], int *i, int longueur, Noeud *noeud){ //cookie-pair *( ";" SP cookie-pair )

    VERIFICATION()

    const int indice=*i;
    int compteur=0;

    if (!checkCookiePair(cookie, i, longueur, NULL)) {
        // free(noeud);
        *i = indice;
        return false;
    }
    compteur++;

    while (*i < longueur && cookie[*i]==59){ //";"=59
        (*i)+=2;
        if(cookie[(*i)-1]!=32 || !checkCookiePair(cookie, i, longueur, NULL)){
            *i = indice;
            return false;
        }
        compteur+=3;
    }

    if (compteur>1) { // Il y a au moins un "( ";" SP cookie-pair )"
        noeud->valeur = cookie + indice;
        noeud->longueur = *i - indice;
        noeud->tag = "cookie-string";
        noeud->fils = malloc(compteur * sizeof(Noeud));
        noeud->nombreFils = compteur;

        // On réinitialise l'indice i pour la suite de la fonction
        *i = indice;
        int j = 0;
        checkCookiePair(cookie, i, longueur, &noeud->fils[j]);
        j++;

        while (*i < longueur && cookie[*i]==59 && j < compteur){ //";"=59
            createFilsSimple("case_insensitive_string", cookie + *i, 1, &noeud->fils[j]);
            j++;
            (*i)+=2;
            createFilsSimple("SP", cookie + (*i) - 1, 1, &noeud->fils[j]);
            j++;
            if(cookie[(*i)-1]!=32 || !checkCookiePair(cookie, i, longueur, &noeud->fils[j])) {
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

        *i = indice;

        checkCookiePair(cookie, i, longueur, &noeud->fils[0]);
    }
    return true;
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
        freeArbre(filsCookieName);
        free(filsCookieName);
        *i = indice;
        return false;
    }
    (*i)++;
    
    Noeud *filsCookieValue = malloc(sizeof(Noeud));
    if (!checkCookieValue(cookie, i, longueur, filsCookieValue)){
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
}

bool checkCookieValue(char cookie[], int *i, int longueur, Noeud *noeud){ //cookie-value = ( DQUOTE *cookie-octet DQUOTE ) / *cookie-octet 
    const int indice=*i;
    int compteur=0;
    if (cookie[*i]==34) { 
        compteur++;
        (*i)++;
        while (checkCookieOctet(cookie, i) && *i<longueur){
            compteur++;
            (*i)++;
        }
        if (cookie[*i]==34) {
            compteur++;
            (*i)++;
        }
        else {
            // free(noeud);
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
            if (cookie[*i]==34) {
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

//!===============================================================================
//? Fonctions utiles pour parser le transfer-encoding-header

bool checkTransferEncodingHeader(char transferEncoding[], int *i, int longueur, Noeud *noeud){//Transfer-Encoding-header = "Transfer-Encoding" ":" OWS Transfer-Encoding OWS

    VERIFICATION1()

    int nombreFils = 5;
    const int indice = *i;
    Noeud *filsTransferEncodingMot = malloc(sizeof(Noeud));
    if (!checkTransferEncodingMot(transferEncoding, i, filsTransferEncodingMot)) {
        free(filsTransferEncodingMot);
        free(noeud);
        *i = indice;
        return false;
    }
    (*i) += 17;
    if (transferEncoding[*i] != 58) { //? ":" = 58
        free(noeud);
        free(filsTransferEncodingMot);
        *i = indice;
        return false;
    }
    (*i)++;

    checkOWS(transferEncoding, i, longueur, NULL);

    Noeud *filsTransferEncoding = malloc(sizeof(Noeud));
    if (!checkTransferEncoding(transferEncoding, i, longueur, filsTransferEncoding)){
        free(noeud);
        free(filsTransferEncodingMot);
        free(filsTransferEncoding);
        *i = indice;
        return false;
    }

    checkOWS(transferEncoding, i, longueur, NULL);

    noeud->fils = malloc(nombreFils * sizeof(Noeud));
    noeud->valeur = transferEncoding + indice;
    noeud->longueur = *i - indice;
    noeud->nombreFils = nombreFils;
    noeud->tag = "Transfer-Encoding-header";

    *i = indice; // On réinitialise i
    
    noeud->fils[0] = *filsTransferEncodingMot;
    (*i) += noeud->fils[0].longueur;
    
    createFilsSimple("case_insensitive_string", transferEncoding + *i, 1, &noeud->fils[1]);
    (*i)++;
    
    checkOWS(transferEncoding, i, longueur, &noeud->fils[2]);

    noeud->fils[3] = *filsTransferEncoding;
    (*i) += noeud->fils[3].longueur;

    checkOWS(transferEncoding, i, longueur, &noeud->fils[4]);

    // On nettoie les fils temporaires
    free(filsTransferEncodingMot);
    free(filsTransferEncoding);

    return true;

}

bool checkTransferEncodingMot(char transferEncoding[], int *i, Noeud *noeud) {
    noeud->valeur = transferEncoding + *i;
    noeud->longueur = 17;
    noeud->tag = "case_insensitive_string";
    noeud->fils = NULL;
    noeud->nombreFils = 0;
    char transferEncodingMinuscule[18];
    sousChaineMinuscule(transferEncoding, transferEncodingMinuscule, *i, *i + 17);
    int a=strcmp("transfer-encoding", transferEncodingMinuscule);
    if (a!=0){
        return false ;
    }
    return true;
}

bool checkTransferEncoding(char transferEncoding[], int *i, int longueur, Noeud *noeud) {
    //Transfer-Encoding = *( "," OWS ) transfer-coding *( OWS "," [ OWS transfer-coding ] )

    VERIFICATION()

    int compteur = 0;
    int const indice = *i;
    bool reachCoding = false; // Indique si on a atteint le premier transfer-coding pour savoir si on inverse OWS et ,
    bool owsChecked = false;

    while ((*i)<longueur && transferEncoding[*i] == 44) {
        (*i)++;
        compteur++;
        checkOWS(transferEncoding, i, longueur, NULL);
        compteur++;
    }

    if (!checkTransferCoding(transferEncoding, i, longueur, NULL)) {
            *i = indice;
            return false;
    }
    compteur++;

    checkOWS(transferEncoding, i, longueur, NULL);
    while (*i<longueur && transferEncoding[*i]==44) {
        compteur+=2;
        (*i)++;

        checkOWS(transferEncoding, i, longueur, NULL);

        if(checkTransferCoding(transferEncoding, i, longueur, NULL)) {
            if (*i>=longueur) {
                compteur+=2;
                break;
            } else {
                compteur+=2;
                checkOWS(transferEncoding, i, longueur, NULL);
            }
        }
    }


    noeud->valeur = transferEncoding + indice;
    noeud->tag = "Transfer-Encoding";
    noeud->fils = malloc(compteur * sizeof(Noeud));
    noeud->nombreFils =compteur;

    // On réinitialise l'indice i pour la suite de la fonction
    *i = indice;

    // On remplit le tableau des fils du noeud method
    for (int j = 0; j < compteur; j++) {

        int iTemp=*i;
        if(transferEncoding[*i]==44) {
            if (!owsChecked && reachCoding) {
                checkOWS(transferEncoding, i, longueur, &noeud->fils[j]);
                j++;
            }
            createFilsSimple("case_insensitive_string", transferEncoding + *i, 1, &noeud->fils[j]);
            (*i)++;
            if (!reachCoding) {
                j++;
                checkOWS(transferEncoding, i, longueur, &noeud->fils[j]);
            }
            owsChecked = false;
        }
        else if (checkTransferCoding(transferEncoding, i, longueur, NULL)){
            *i=iTemp;
            if (!owsChecked && reachCoding) {
                checkOWS(transferEncoding, i, longueur, &noeud->fils[j]);
                j++;
            }
            checkTransferCoding(transferEncoding, i, longueur, &noeud->fils[j]);
            reachCoding = true;
            owsChecked = false;
        }
        else {
            checkOWS(transferEncoding, i, longueur, &noeud->fils[j]);
            owsChecked = true;
        }
    }

    noeud->longueur = *i - indice;

    return true;  
}

bool checkTransferCoding(char transferEncoding[], int *i, int longueur, Noeud *noeud){
    //transfer-coding = "chunked" / "compress" / "deflate" / "gzip" / transfer-extension

    VERIFICATION()

    const int indice=*i;

    if (noeud!=NULL) {
        noeud->valeur = transferEncoding + indice;
        noeud->tag = "transfer-coding";
        noeud->fils = malloc(sizeof(Noeud));
        noeud->nombreFils = 1;
    }

    (*i)=indice;
    char transferEncodingMinuscule[9];
    sousChaineMinuscule(transferEncoding, transferEncodingMinuscule, *i, (*i)+7);
    if (strcmp("chunked", transferEncodingMinuscule)==0){
        if (noeud!=NULL){
            noeud->longueur = 7;
            (*i)=indice;
            createFilsSimple("case_insensitive_string", transferEncoding + *i, 7, &noeud->fils[0]);
        }
        (*i)+=7;
        return true;
    }

    sousChaineMinuscule(transferEncoding, transferEncodingMinuscule, *i, (*i)+8);
    if (strcmp("compress", transferEncodingMinuscule)==0) {
        if (noeud!=NULL){
            noeud->longueur = 8;
            (*i)=indice;
            createFilsSimple("case_insensitive_string", transferEncoding + *i, 8, &noeud->fils[0]);
        }
        (*i)+=8;
        return true;
    }

    sousChaineMinuscule(transferEncoding, transferEncodingMinuscule, *i, (*i)+7);
    if (strcmp("deflate", transferEncodingMinuscule)==0) {
        if (noeud!=NULL){
            noeud->longueur = 7;
            (*i)=indice;
            createFilsSimple("case_insensitive_string", transferEncoding + *i, 7, &noeud->fils[0]);
        }
        (*i)+=7;
        return true;
    }

    sousChaineMinuscule(transferEncoding, transferEncodingMinuscule, *i, (*i)+4);
    if (strcmp("gzip", transferEncodingMinuscule)==0) {
        if (noeud!=NULL){
            noeud->longueur = 4;
            (*i)=indice;
            createFilsSimple("case_insensitive_string", transferEncoding + *i, 4, &noeud->fils[0]);
        }
        (*i)+=4;
        return true;
    }
    if (checktransferExtension(transferEncoding, i, longueur, NULL)){
        *i=indice;
        Noeud *filsTransferExtension=malloc(sizeof(Noeud));
        checktransferExtension(transferEncoding, i, longueur, filsTransferExtension);
        if (noeud!=NULL){
            noeud->longueur = (*i)-indice;
            noeud->fils[0]=*filsTransferExtension;
        } else {
            freeArbre(filsTransferExtension);
        }
        free(filsTransferExtension);
        return true;
    }

    else{
        (*i)=indice;
        return false;
    }

    return true;
}

bool checktransferExtension(char transferEncoding[], int *i, int longueur, Noeud *noeud) {
    //transfer-extension = token *( OWS ";" OWS transfer-parameter )

    VERIFICATION()

    int compteur = 0;
    int const indice = *i;
    bool owsChecked = false; // Booléen utilisé pour savoir si on a eu un OWS juste avant ou non
    
    if (!checkToken(transferEncoding, i, longueur, NULL, "")) {
        (*i) = indice;
        return false;
    }
    compteur++;

    // On vérifie si on a une suite au token
    int const indice2 = *i; // On garde l'indice en mémoire

    checkOWS(transferEncoding, i, longueur, NULL);

    while (*i<longueur && transferEncoding[*i]==59) {
        compteur+=2;
        (*i)++;
        checkOWS(transferEncoding, i, longueur, NULL);
        if (!checkTransferParameter(transferEncoding, i, longueur, NULL)) {
            compteur -= 2;
            (*i) = indice2;
            break;
        }
        compteur += 2;
        checkOWS(transferEncoding, i, longueur, NULL);
    }

    if (noeud!=NULL) {
        noeud->valeur = transferEncoding + indice;
        noeud->longueur = *i - indice;
        noeud->tag = "transfer-extension";
        noeud->fils = malloc(compteur * sizeof(Noeud));
        noeud->nombreFils =compteur;

        // On réinitialise l'indice i pour la suite de la fonction
        *i = indice;

        checkToken(transferEncoding, i, longueur, &noeud->fils[0], "token");

        for (int j = 1; j < compteur; j++) {
            int iTemp = *i;

            if(transferEncoding[*i]==59) {
                if (!owsChecked) {
                    checkOWS(transferEncoding, i, longueur, &noeud->fils[j]);
                    j++;
                }
                createFilsSimple("case_insensitive_string", transferEncoding + *i, 1, &noeud->fils[j]);
                (*i)++;
                owsChecked = false;
            }
            else if(checkTransferParameter(transferEncoding, i, longueur, NULL)) {
                *i=iTemp;
                if (!owsChecked) {
                    checkOWS(transferEncoding, i, longueur, &noeud->fils[j]);
                    j++;
                }
                checkTransferParameter(transferEncoding, i, longueur, &noeud->fils[j]);
                owsChecked = false;
            }
            else {
                *i=iTemp;
                checkOWS(transferEncoding, i, longueur, &noeud->fils[j]);
                owsChecked = true;
            }
        }
    }
    return true;
}

bool checkTransferParameter(char transferEncoding[], int *i, int longueur, Noeud *noeud){
    //transfer-parameter = token BWS "=" BWS ( token / quoted-string )

    VERIFICATION()

    //BWS=OWS
    int compteur=0;
    const int indice=*i;

    if (!checkToken(transferEncoding, i, longueur, NULL, "")) {
        free(noeud);
        (*i) = indice;
        return false;
    }
    compteur++;

    checkOWS(transferEncoding, i, longueur, NULL);
    compteur++;
    if (transferEncoding[*i]!=61) {
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
        if (!checkToken(transferEncoding, i, longueur, NULL, "")) {
            (*i) = indice;
            return false;
        }
        compteur++;
    }

    if (noeud!=NULL){
        noeud->valeur = transferEncoding + indice;
        noeud->longueur = *i - indice;
        noeud->tag = "transfer-parameter";
        noeud->fils = malloc(compteur * sizeof(Noeud));
        noeud->nombreFils =compteur;

        *i = indice;

        for (int j=0;j<compteur;j++){
            int iTemp=*i;
            if (checkToken(transferEncoding, i, longueur, NULL, "token")) {
                (*i) = iTemp;
                checkToken(transferEncoding, i, longueur, &noeud->fils[j], "token");
            } else if (checkQuotedString(transferEncoding, i, longueur, NULL)){
                *i=iTemp;
                checkQuotedString(transferEncoding, i, longueur, &noeud->fils[j]);
            } else {
                (*i)=iTemp;
                checkOWS(transferEncoding, i, longueur, &noeud->fils[j]);
                j++;
                createFilsSimple("case_insensitive_string", transferEncoding + *i, 1, &noeud->fils[j]);
                (*i)++;
                j++;
                checkOWS(transferEncoding, i, longueur, &noeud->fils[j]);
            }
        }
    }
    return true;
}

bool checkQuotedString(char transferEncoding[], int *i, int longueur, Noeud *noeud){
    //quoted-string = DQUOTE *( qdtext / quoted-pair ) DQUOTE

    VERIFICATION()

    int compteur = 0;
    int const indice = (*i);

    if (transferEncoding[*i]!=34){
        (*i)=indice;
        return false;
    }
    (*i)++;
    compteur++;

    while(((*i)<longueur) && (checkQdtext(transferEncoding, *i, NULL)||checkQuotedPair(transferEncoding, i, longueur, NULL))){
        compteur++;
        (*i)++;
    }
    if (transferEncoding[*i]!=34){
        (*i)=indice;
        return false;
    }
    (*i)++;
    compteur++;

    if (noeud!=NULL){
        noeud->valeur = transferEncoding + indice;
        noeud->longueur = *i - indice;
        noeud->tag = "quoted-string";
        noeud->fils = malloc(compteur * sizeof(Noeud));
        noeud->nombreFils =compteur;

        *i = indice;

        for (int j=0;j<compteur;j++){
            if (transferEncoding[*i]==34){
                createFilsSimple("dquote", transferEncoding + *i, 1, &noeud->fils[j]);
                (*i)++;
            }
            else if (checkQdtext(transferEncoding, *i, NULL)){
                checkQdtext(transferEncoding, *i, &noeud->fils[j]);
                (*i)++;
            }
            else{
                checkQuotedPair(transferEncoding, i, longueur, &noeud->fils[j]);
            }
        }
    }
    return true;
}

bool checkQdtext(char transferEncoding[], int i, Noeud *noeud){
    // qdtext = HTAB / SP / "!" / %x23-5B ; '#'-'['
    //          / %x5D-7E ; ']'-'~'
    //         / obs-text
    // obs-text = %x80-FF

    if (transferEncoding[i]==9 || transferEncoding[i]==32 || transferEncoding[i]==33 || (transferEncoding[i]>=35 && transferEncoding[i]<=91) || (transferEncoding[i]>=93 && transferEncoding[i]<=126) || (transferEncoding[i]>=128 && transferEncoding[i]<=255)) {
        if (noeud!=NULL){
            createFilsSimple("qdtext", transferEncoding + i, 1, noeud);
        }
        return true;
    } else {
        return false;
    }
}

bool checkQuotedPair(char transferEncoding[], int *i, int longueur, Noeud *noeud){
    //quoted-pair = "\" ( HTAB / SP / VCHAR / obs-text )

    VERIFICATION()

    int indice= *i;
    if (noeud!=NULL){
        noeud->valeur = transferEncoding + *i;
        noeud->longueur = 2;
        noeud->tag = "quoted-pair";
        noeud->fils = malloc(2*sizeof(Noeud));
        noeud->nombreFils =2;
    }
    if (transferEncoding[*i]!=92){
        return false;
    }
    if (noeud!=NULL){
        createFilsSimple("case_insensitive_string",transferEncoding + *i, 1, &noeud->fils[0]);
    }
    (*i)++;
    if (transferEncoding[*i]==9){
        if (noeud!=NULL){
            createFilsSimple("HTAB", transferEncoding + *i, 1, &noeud->fils[1]);
        }
    }
    else if (transferEncoding[*i]==32){
        if (noeud!=NULL){
            createFilsSimple("SP", transferEncoding + *i, 1, &noeud->fils[1]);
        }
    }
    else if (transferEncoding[*i]>=33 && transferEncoding[*i]<= 126){
        if (noeud!=NULL){
            createFilsSimple("VCHAR", transferEncoding + *i, 1, &noeud->fils[1]);
        }
    }
    else if (transferEncoding[*i]>=128 && transferEncoding[*i]<=255){
        if (noeud!=NULL){
            createFilsSimple("obs-text", transferEncoding + *i, 1, &noeud->fils[1]);
        }
    }
    else {
        (*i)=indice;
        return false;
    }
    (*i)++;

    if (noeud == NULL) {
        (*i)--;
    }

    return true;
}

//!===============================================================================
//? Fonctions utiles pour parser le expect-header

bool checkExpectHeader(char requete[], int *i, int longueur, Noeud *noeud) {

    VERIFICATION1()

    int nombreFils=5;
    const int indice= *i;

    noeud->fils = malloc(nombreFils * sizeof(Noeud));
    noeud->valeur = requete + indice;
    // On ne connaît pas encore noeud->longueur
    noeud->nombreFils = nombreFils;
    noeud->tag = "Expect-header";

    int j = 0; // On utilise cet indice pour compter les fils

    if (!checkExpectString(requete, i, &noeud->fils[j])) {
        free(noeud->fils);
        free(noeud);
        *i = indice;
        return false;
    }
    j++;

    if (requete[*i] != ':') {
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

    if (!checkExpect(requete, i, &noeud->fils[j])) {

        if (noeud->fils[j - 1].nombreFils > 0) {
            free(noeud->fils[j - 1].fils);
        } 

        free(noeud->fils);
        free(noeud);
        *i = indice;
        return false;
    }
    j++;

    checkOWS(requete, i, longueur, &noeud->fils[j]);
    j++;

    return true;
}

bool checkExpectString(char requete[], int *i, Noeud *noeud) {
    const int indice = *i; // On conserve l'indice de début
    int n = 6; // Taille de la sous-chaîne qui contient potentiellement "Connection"
    char *chaineExpect = malloc((n + 1) * sizeof(char)); // On ajoute un caractère pour le \0

    sousChaineMinuscule(requete, chaineExpect, *i, *i + n);

    if (strcmp(chaineExpect, "expect") != 0) {
        free(chaineExpect);
        *i = indice;
        return false;
    }

    (*i) += n;
    free(chaineExpect);

    // Si tout s'est bien passé, on créé le noeud contenant "Expect"
    noeud->fils = NULL;
    noeud->valeur = requete + indice;
    noeud->longueur = *i - indice;
    noeud->nombreFils = 0;
    noeud->tag = "case_insensitive_string";

    return true;
}

bool checkExpect(char requete[], int *i, Noeud *noeud) {
    const int indice = *i; // On conserve l'indice de début
    int n = 12; // Taille de la sous-chaîne qui contient potentiellement "100-continue"
    char *chaineContinue = malloc((n + 1) * sizeof(char)); // On ajoute un caractère pour le \0

    sousChaineMinuscule(requete, chaineContinue, *i, *i + n);

    if (strcmp(chaineContinue, "100-continue") != 0) {
        free(chaineContinue);
        *i = indice;
        return false;
    }

    (*i) += n;
    free(chaineContinue);

    // Si tout s'est bien passé, on créé le noeud contenant "100-continue"
    noeud->fils = NULL;
    noeud->valeur = requete + indice;
    noeud->longueur = *i - indice;
    noeud->nombreFils = 0;
    noeud->tag = "case_insensitive_string";

    return true;    
}

//!===============================================================================
//? Fonctions utiles pour parser le host-header

bool checkHostHeader(char requete[], int *i, int longueur, Noeud *noeud) {

    VERIFICATION1()

    int nombreFils=5;
    const int indice= *i;

    noeud->fils = malloc(nombreFils * sizeof(Noeud));
    noeud->valeur = requete + indice;
    // On ne connaît pas encore noeud->longueur
    noeud->nombreFils = nombreFils;
    noeud->tag = "Host-header";

    int j = 0; // On utilise cet indice pour compter les fils

    if (!checkHostString(requete, i, &noeud->fils[j])) {
        free(noeud->fils);
        free(noeud);
        *i = indice;
        return false;
    }
    j++;

    if (requete[*i] != ':') {
        free(noeud->fils);
        free(noeud);
        *i = indice;
        return false;
    } else {
        createFilsSimple("case_insensitive_string", requete + *i, 1, &noeud->fils[j]);
        (*i)++;
        j++;
    }

    checkOWS(requete, i, longueur, &noeud->fils[j]);
    j++;

    if (!checkHost(requete, i, longueur, &noeud->fils[j])) {
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

bool checkHostString(char requete[], int *i, Noeud *noeud) {
    const int indice = *i; // On conserve l'indice de début
    int n = 4; // Taille de la sous-chaîne qui contient potentiellement "Connection"
    char *chaineHost = malloc((n + 1) * sizeof(char)); // On ajoute un caractère pour le \0

    sousChaineMinuscule(requete, chaineHost, *i, *i + n);

    if (strcmp(chaineHost, "host") != 0) {
        free(chaineHost);
        *i = indice;
        return false;
    }

    (*i) += n;
    free(chaineHost);

    // Si tout s'est bien passé, on créé le noeud contenant "Host"
    noeud->fils = NULL;
    noeud->valeur = requete + indice;
    noeud->longueur = *i - indice;
    noeud->nombreFils = 0;
    noeud->tag = "case_insensitive_string";

    return true;
}

bool checkHost(char requete[], int *i, int longueur, Noeud *noeud) {

    const int indice = *i; // On conserve l'indice de début
    int nombreFils = 0;

    // noeud->fils = malloc(nombreFils * sizeof(Noeud));
    //noeud->longueur = *i - indice;
    noeud->valeur = requete + indice;
    // noeud->nombreFils = nombreFils ;
    noeud->tag = "Host";

    Noeud *tempUriHost = malloc(sizeof(Noeud));
    if (!checkUriHost(requete, i, longueur, tempUriHost)) {
        free(tempUriHost);
        *i = indice;
        return false;
    }
    nombreFils++;

    Noeud *tempPort = malloc(sizeof(Noeud));
    if (*i < longueur && requete[*i] == ':') {
        nombreFils += 2;
        (*i)++;
        checkPort(requete, i, longueur, tempPort);
    }

    noeud->fils = malloc(nombreFils * sizeof(Noeud));
    noeud->nombreFils = nombreFils;
    noeud->longueur = *i - indice;

    *i = indice;

    noeud->fils[0] = *tempUriHost;
    *i += tempUriHost->longueur;

    if (nombreFils > 1) {
        createFilsSimple("case_insensitive_string", requete + *i, 1, &noeud->fils[1]);
        (*i)++;

        noeud->fils[2] = *tempPort;
        *i += tempPort->longueur;
    }

    free(tempUriHost);
    free(tempPort);

    return true;
}

bool checkUriHost(char requete[], int *i, int longueur, Noeud *noeud) {
    const int indice = *i; // On conserve l'indice de début
    int nombreFils=1;

    noeud->fils = malloc(nombreFils * sizeof(Noeud));
    noeud->valeur = requete + indice;
    noeud->nombreFils = nombreFils ;
    noeud->tag = "host";


    if (checkIPliteral(requete, i, &noeud->fils[0])) {
        noeud->longueur = *i - indice;
        return true;

    } else {
        *i = indice;

        if (checkIPV4(requete, i, &noeud->fils[0], true)) {
            noeud->longueur = *i - indice;
            return true;
        } else {
            *i = indice;
            if(checkREGNAME(requete, i, &noeud->fils[0])) {
                noeud->longueur = *i - indice;
                return true;
            }
        }
    }

    free(noeud->fils);

    *i = indice;
    
    return false;
}

bool checkPort(char requete[], int *i, int longueur, Noeud *noeud) {
    const int indice = *i;
    int nombreFils = 0;

    while (*i < longueur && checkDigit(requete, *i)) {
        (*i)++;
        nombreFils++;
    }

    if (nombreFils > 0) {
        noeud->fils = malloc(nombreFils * sizeof(Noeud));
    }
    noeud->nombreFils = nombreFils;
    noeud->longueur = *i - indice;
    noeud->tag = "port";
    noeud->valeur = requete + indice;

    *i = indice;

    for (int j = 0; j < nombreFils; j++) {
        createFilsSimple("DIGIT", requete + *i, 1, &noeud->fils[j]);
        (*i)++;
    }

    return true;
}

bool checkIPliteral(char requete[], int *i, Noeud *noeud) {

    const int indice = *i; // On conserve l'indice de début
    bool ipv6 = false;

    if (requete[*i] == '[') {
        (*i)++;
    } else {
        return false;
    }

    Noeud *temp = malloc(sizeof(Noeud));
    if(!checkIPV6(requete,  i, temp)) {
        *i = indice + 1;
        if (!checkIPfuture(requete,  i, temp)) {
            free(temp);
            return false;
        }
    } else {
        ipv6 = true;
    }
    freeArbre(temp);
    free(temp);

    if (requete[*i] == ']') {
        (*i)++;
    }
    else {
        return false;
    }

    noeud->fils = malloc(3 * sizeof(Noeud));
    noeud->valeur = requete + indice;
    noeud->longueur = *i - indice;
    noeud->nombreFils = 3;
    noeud->tag = "IP-literal";

    *i = indice;

    createFilsSimple("case_insensitive_string", requete + *i, 1, &noeud->fils[0]);
    (*i)++;

    if (ipv6) {
        checkIPV6(requete, i, &noeud->fils[1]);
    } else {
        checkIPfuture(requete, i, &noeud->fils[1]);
    }

    createFilsSimple("case_insensitive_string", requete + *i, 1, &noeud->fils[2]);
    (*i)++;

    return true;
}

bool checkIPV4(char requete[], int *i, Noeud *noeud, bool stocker) {

    const int indice = *i; // On conserve l'indice de début
    int nombreFils=7;

    noeud->valeur = requete + indice;
    noeud->nombreFils = nombreFils;
    noeud->fils = malloc(nombreFils * sizeof(Noeud));
    noeud->tag = "IPv4address";

    int j = 0; //compteur de Fils

    while (j < 6) {
        if (!checkDecoctet(requete,  i, &noeud->fils[j], stocker)) {
            if (stocker) {
                for (int k = 0; k < j; k++) {
                    if (noeud->fils[k].nombreFils > 0) {
                        free(noeud->fils[k].fils);
                    }
                }
            }
            free(noeud->fils);
            return false;
        }
        j++;
        if(requete[*i] != '.') {
            if (stocker) {
                for (int k = 0; k < j; k++) {
                    if (noeud->fils[k].nombreFils > 0) {
                        free(noeud->fils[k].fils);
                    }
                }
            }
            free(noeud->fils);
            return false;
        }
        createFilsSimple("case_insensitive_string", requete + *i, 1, &noeud->fils[j]);
        (*i)++;
        j++;
    }

    if (!checkDecoctet(requete,  i, &noeud->fils[j], stocker)) {
        if (stocker) {
            for (int k = 0; k < j; k++) {
                if (noeud->fils[k].nombreFils > 0) {
                    free(noeud->fils[k].fils);
                }
            }
        }
        free(noeud->fils);
        return false;
    }
    j++;
    noeud->longueur = *i - indice;

    if (!stocker) {
        free(noeud->fils);
        *i=indice;
    }

    return true;
}

bool checkREGNAME(char requete[], int *i, Noeud *noeud) {
    const int indice = *i; // On conserve l'indice de début
    int nombreFils=0;

    noeud->valeur = requete + indice;
    noeud->tag = "reg-name";

    while (true) {
        if (checkUnreserved(requete, *i) || checkSubDelims(requete, *i)) {
            nombreFils++;
            (*i)++;
        } else if (checkPctEncoded(requete, *i)) {
            nombreFils++;
            (*i) += 3;
        } else {
            break;
        }
    }

    noeud->nombreFils = nombreFils ;
    
    if (nombreFils > 0) {
        noeud->fils = malloc(nombreFils * sizeof(Noeud));
    }

    noeud->longueur = *i - indice;

    //On remet les comptes à 0 et on crée maintenant les noeuds
    *i=indice;

    int j = 0;


    while (true) {
        Noeud *petit = &noeud->fils[j]; // Ce noeud va nous permettre de créer les plus petits Noeuds Alpha, Digit et Hexdig facilemment si besoin
        if(checkUnreserved(requete, *i)) {
            petit->tag = "unreserved";
            petit->fils = malloc(sizeof(Noeud));
            petit->valeur = requete + *i;
            petit->longueur = 1;
            petit->nombreFils = 1;

            if (checkAlpha(requete, *i)) {
                createFilsSimple("Alpha", requete + *i, 1, &petit->fils[0]);
            }
            else if (checkDigit(requete, *i)){
                createFilsSimple("Digit", requete + *i, 1, &petit->fils[0]);
            }
            else {
                createFilsSimple("case_insensitive_string", requete + *i, 1, &petit->fils[0]);
            }

        } else if (checkSubDelims(requete, *i)){
            petit->tag = "sub-delims";
            petit->fils = malloc(sizeof(Noeud));
            petit->valeur = requete + *i;
            petit->longueur = 1;
            petit->nombreFils = 1;
            createFilsSimple("case_insensitive_string", requete + *i, 1, &petit->fils[0]);
        }
        else if(checkPctEncoded(requete, *i)){
            petit->tag = "pct-encoded";
            petit->fils = malloc(3*sizeof(Noeud));
            petit->valeur = requete + *i;
            petit->longueur = 3;
            petit->nombreFils = 3;
            createFilsSimple("case_insensitive_string", requete + *i, 1, &petit->fils[0]);
            (*i)++;
            createFilsSimple("HEXDIG", requete + *i, 1, &petit->fils[1]);
            (*i)++;
            createFilsSimple("HEXDIG", requete + *i, 1, &petit->fils[2]);
        }

        else {
            break;
        }
        j++;
        (*i)++;
    }

    return true;
}

bool checkIPV6(char requete[], int *i, Noeud *noeud) {
    // Ne pas oublier de faire *i=indice; avant chaque return false pour ne pas créer de problèmes lors du 2e if de checkIPliteral

    const int length = strlen(requete);

    //Potentiellement faire un tableau des tailles de H16
    
    const int indice = *i; // On conserve l'indice de début
    // int indice2;

    int NombreFils = 0;

    int j = 0; // J nous servira plus tard pour créer les différents noeuds

    // Noeud *petit; // A l'aide du noeud petit on va créer les noeuds H16 puis digit
    int somme; // Nous sert à "localiser" les noeuds
    
    //! We might need to change the initial value of m
    int m = 0;

    // int k = 0; //k nous sert à accéder au tableau du compteur de digit des h16

    int CompteurH16=0;
    int Compteur=0;
    int tab1[9]; //Ce tableau nous sert à garder en mémoire la taille de chaque H16

    int tab2[8]; //Ce tableau fais la meme chose que le premier mais pour les H16 après "::"
    int CompteurH16_bis=0; //Idem
    
    // bool ls32=false; // Ce boolean nous sera utile pour verifier si on se trouve dans le ls32 ou non
    int ls32 = 0; //? 0 = pas de ls32, 1 = h16:h16, 2 = ipv4

    bool interrupteur = true;
    bool cas_1 = true;
    
    noeud->valeur = requete + indice;
    noeud->tag = "IPv6address";

    while (interrupteur) { // Cette boucle while va nous permettre de compter le nombre de H16 ou de (H16 ":") avant les "::"
        Compteur = CompteurHexdig(requete, i);

        switch (Compteur) {
            case 0:
                break;
            case 1:
            case 2:
            case 3:
            case 4:
                // printf("CompteurH16 before : %d\n", CompteurH16);
                if (CompteurH16 < 8) {
                    tab1[CompteurH16] = Compteur;
                    CompteurH16++;
                }
                // printf("CompteurH16 after : %d\n", CompteurH16);
                break;
            
            default:
                if (cas_1) {
                    CompteurH16=-1;
                }
                else{
                    interrupteur=false;
                }
                break;
        }

        if (CompteurH16 < 0 || CompteurH16 > 8) {
            *i=indice;
            return false;
        } else {
            if (requete[*i + Compteur] == ':') {
                if (CompteurH16 == 6) {  //A partir de H16 = 6 on peut soit être dans le cas 1 ou les deux derniers, on active donc une option nous permettant de ne plus tout jeter si ce qui vient après n'est pas H16 ou ":" "::"
                    cas_1=false;
                }
                if (requete[*i + Compteur + 1] == ':') {
                    *i = *i + Compteur + 1;
                    interrupteur = false;
                }
                else{
                    *i = *i + Compteur + 1;
                }
            } else {
                if (CompteurH16 != 8 && requete[*i + Compteur] != '.') { //Si Il y a 8 H16 alors on est dans le cas 1 avec 2 H16 dans ls32, donc c'est valide de ne pas avoir de ":" après le 8e H16
                    *i=indice;
                    return false;
                } else{
                    if (requete[*i + Compteur] == '.') { //Ce qu'on a pris pour un H16 était en réalité un dec-octet, on corrigera notre erreur plus tard
                        CompteurH16--;
                        interrupteur=false;
                    }
                    else{
                        *i=*i+Compteur;
                        interrupteur=false;
                    }
                }

            }

        }
    }

    interrupteur=true;

    if (CompteurH16 == 8) { // C'est le cas 1.1 où ls32 = 2 H16, on corrige donc notre erreur

        NombreFils = 8 + 7; //? 8 H16 + 7 ":"
        noeud->fils = malloc(NombreFils*sizeof(Noeud));
        noeud->longueur = *i - indice;
        noeud->nombreFils = NombreFils ;

        somme = 0; // Nous sert à "localiser" les noeuds
        m = 0;
        int a = 0;

        while (j < NombreFils) {

            Noeud *petit = &noeud->fils[j]; // A l'aide du noeud petit on va créer les noeuds H16 puis digit

            petit->tag = "h16";
            petit->fils = malloc(tab1[a]*sizeof(Noeud));
            petit->valeur = requete + indice + somme; //ETNBZ
            petit->longueur = tab1[a];
            petit->nombreFils = tab1[a];

            m=0;

            while (m < tab1[a]) {
                createFilsSimple("HEXDIG", requete + indice + somme + m , 1, &petit->fils[m]);
                m++;
            }
            somme = somme + tab1[a]; // Ne pas oublier de compter le ":" //! Tu l'as oublié 🙃
            j++;
            if (j < NombreFils) {
                createFilsSimple("case_insensitive_string", requete + indice + somme, 1, &noeud->fils[j]);
                j++;
                somme++;
            }
            a++;
        }

        return true;
    } else if (CompteurH16==6 && requete [*i] !=':' && requete[*i + 1] != ':'){ //On traite le cas 1.2

        NombreFils= 6 + 6 + 1; //? 6 H16 + 6 :  + 1 addresseipv4
        noeud->fils = malloc(NombreFils*sizeof(Noeud));
        noeud->longueur = *i - indice;
        noeud->nombreFils = NombreFils ;

        if (checkIPV4(requete, i, &noeud->fils[NombreFils - 1], true)) {

            somme =0; // Nous sert à "localiser" leqs noeuds
            m=0;

            int a = 0;

            while (j < NombreFils - 1) {

                Noeud *petit = &noeud->fils[j]; // A l'aide du noeud petit on va créer les noeuds H16 puis digit 

                petit->tag = "h16";
                petit->fils = malloc(tab1[a]*sizeof(Noeud));
                petit->valeur = requete + indice + somme; //ETNBZ
                petit->longueur = tab1[a];
                petit->nombreFils = tab1[a];

                m=0;

                while (m < tab1[a]) {
                    createFilsSimple("HEXDIG", requete + indice + somme + m, 1, &petit->fils[m]);
                    m++;
                }

                somme = somme + tab1[a]; // Ne pas oublier de compter le ":" //! Tu l'as oublié 🙃
                j++;
                createFilsSimple("case_insensitive_string", requete + indice + somme, 1, &noeud->fils[j]);
                j++;

                somme++;
                a++;
            }
            return true;
        } else {
            *i=indice;
            return false;
        }
    } else {  // Maintenant le plan c'est de compter les H16 de l'autre coté des ":" pour comparer leur nombre à ceux d'avant et de déterminer le cas où l'on se trouve
        // bool pointsConsecutifs = false; // On vérifie qu'on a pas trop de : qui se suivent
        while (interrupteur) { // Cette boucle while va nous permettre de compter le nombre de H16 ou de (H16 ":") après les "::"
            Compteur = CompteurHexdig(requete, i);

            switch (Compteur) {
                case 0:
                    break;
                case 1:
                case 2:
                case 3:
                case 4:
                    if (CompteurH16_bis <= 8) {
                        tab2[CompteurH16_bis] = Compteur;
                    }
                    CompteurH16_bis++;
                    break;
                
                default:
                interrupteur=false;
                break;
            }

            if (CompteurH16_bis < 0 || CompteurH16_bis > 7) {
                *i=indice;
                return false;
            } else {
                if (requete[*i + Compteur]==':') {
                    if (*i < length && requete[*i + Compteur + 1] == ':') {
                        *i = *i + Compteur + 1;
                        interrupteur = false;
                    } else {
                        *i = *i + Compteur + 1;
                    }
                } else {
                    if (requete[*i + Compteur] != '.') { //Si il n'y a pas de "." ou de ":" alors on arrête de compter sans savoir ce qui se trouve après (on regardera plus tard)
                        *i = *i + Compteur;
                        interrupteur = false;
                    } else {
                        if (requete[*i + Compteur] == '.') { //Ce qu'on a pris pour un H16 était en réalité un dec-octet, on corrige notre erreur
                            // CompteurH16_bis--;
                            interrupteur=false;
                        } else {
                            *i=*i+Compteur;
                            interrupteur=false;
                        }
                    }
                }
            }
        }
    }

    // OK alors maintenant l'idée c'est de vérifier si après le dernier h16 on a un ":" ou non.
    // Si il n'y a pas de ":" alors on est dans le cas 1 de ls32 (ou dans le cas où il n'y a que 1 H16)
    // Si il y a un ":" alors on est avant le ls32 (ou dans le dernier cas)

     // On recule d'un cran pour vérifier si il y a un ":" ou non

    Noeud *test = malloc(sizeof(Noeud)); // Ce noeud nous servira juste à tester si ipv6 continet une @ ipv4
    if (requete[*i - 1] != ':' && CompteurH16_bis > 1) {  // Vérifier IPV4 pour ls32 : Créer une option dans la fonction ipv4 pour pouvoir ne pas créer de noeud et ensuite l'utiliser pour check si c'est une adresse ipv4, plus haut et dans une autre fiction
        ls32=1;
        CompteurH16_bis--;
        CompteurH16_bis--;
    } else if(checkIPV4(requete, i, test, false)) {
        ls32=2;
        CompteurH16_bis--;
    }
    free(test);
    // On a donc dans compteurH16_bis le nombre exact de H16 hors ls32

    switch (CompteurH16_bis) { // Dans ce switch on va "exclure" tous les cas faux, cad les cas ou il ya plus de 7 h16 a gauche sans ls32 à droite
        case 0:
            if ((CompteurH16 > 7 && ls32 == 0) || (CompteurH16 > 5 && ls32 != 0) ) {
                *i=indice;
                return false;
                break;
            }
            else {
                break;
            }
        case 1: 
            if ((CompteurH16 > 6 && ls32 == 0) || (CompteurH16 >4 && ls32 != 0) ) {
                *i=indice;
                return false;
            }
            break;
        case 2:
            if ((CompteurH16 > 6 && ls32 != 0) || ls32 == 0) {
                *i=indice;
                return false;
            }
            break;
        case 3:
            if ((CompteurH16 > 4 && ls32 != 0) || ls32 == 0) {
                *i=indice;
                return false;
            }
            break;
        case 4:
            if ((CompteurH16 > 3 && ls32 != 0) || ls32 == 0) {
                *i=indice;
                return false;
            }
            break;
        case 5:
            if ((CompteurH16 > 2 && ls32 != 0) || ls32 == 0) {
                *i=indice;
                return false;
            }
            break;
        case 6:
            if (((CompteurH16!=1) && ls32 != 0) || ls32 == 0){
                *i=indice;
                return false;
            }
            break;
        
        default:
            interrupteur=false;
            break;
    }

    // Si on est arrivé jusqu'ici ça veut dire que tout va bien et que l'on peut ENFIN STOCKER HAHA PRENDS CA IPV6 FONCTION DE MERDE //! Elle t'as montré qui c'était le patron
    NombreFils=0;
    if (ls32 == 1) {
        NombreFils += 3;
    } else if (ls32 == 2) {
        NombreFils++;
    }
    // Il faut prendre en compte le cas où il n'y a pas de h16 à gauche
    if (CompteurH16 <= 0) {
        NombreFils += 2 + 2 * CompteurH16_bis;
    } else {
        NombreFils += 1 + 2 * (CompteurH16 + CompteurH16_bis);
    }

    if (CompteurH16_bis == 1 && ls32 == 0) {
        NombreFils--; // On a compté 1 ":" de trop
    }

    noeud->fils = malloc(NombreFils*sizeof(Noeud));
    noeud->longueur = *i - indice;
    noeud->nombreFils = NombreFils;

    j=0;
    *i=indice;

    somme =0; // Nous sert à "localiser" leqs noeuds
    m=0;
    int a = 0;

    while (j < 2 * CompteurH16) {

        Noeud *petit1 = &noeud->fils[j]; // A l'aide du noeud petit on va créer les noeuds H16 puis Hexdig

        petit1->tag = "h16";
        petit1->fils = malloc(tab1[a]*sizeof(Noeud));
        petit1->valeur = requete + indice + somme; //ETNBZ
        petit1->longueur = tab1[a];
        petit1->nombreFils = tab1[a];
        
        m=0;

        while (m < tab1[a]){
            createFilsSimple("HEXDIG", requete + indice + somme + m , 1, &petit1->fils[m]);
            m++;
            (*i)++;
        }
        j++;
        somme= somme + tab1[a]; // Ne pas oublier de compter le ":"
        createFilsSimple("case_insensitive_string", requete + indice + somme, 1, &noeud->fils[j]);
        (*i)++;
        somme++; // Ne pas oublier de compter le ":"
        j++;
        a++;
    }

    createFilsSimple("case_insensitive_string", requete + *i, 1, &noeud->fils[j]); // On fait le noeud pour les "::" , mais le premier a déja été fait juste avant
    (*i)++;
    j++;

    if (CompteurH16 <= 0) {
        createFilsSimple("case_insensitive_string", requete + *i, 1, &noeud->fils[j]);
        (*i)++;
        j++;
        somme++;
    }

    a = 0;

    somme++;

    int number = 0;
    if (ls32 == 0) {
        number = NombreFils;
    } else if (ls32 == 1) {
        number = NombreFils - 3;
    } else {
        number = NombreFils - 1;
    }

    while (j < number) {

        Noeud *petit1 = &noeud->fils[j]; // A l'aide du noeud petit on va créer les noeuds H16 puis Hexdig

        petit1->tag = "h16";
        petit1->fils = malloc(tab2[a]*sizeof(Noeud));
        petit1->valeur = requete + indice + somme; //ETNBZ
        petit1->longueur = tab2[a];
        petit1->nombreFils = tab2[a];
        
        m=0;

        while (m < tab2[a]) {
            createFilsSimple("HEXDIG", requete + indice + somme + m , 1, &petit1->fils[m]);
            m++;
            (*i)++;
        }
        somme += tab2[a];

        if ((CompteurH16_bis !=1 || ls32 != 0) && j < number) {
            j++;
            createFilsSimple("case_insensitive_string", requete + indice + somme, 1, &noeud->fils[j]);
            (*i)++;
            j++;
        }
        else {
            j++;
        }

        somme++; // Ne pas oublier de compter le ":"
        // k++;
        a++;

    }

    //Maintenant on s'occupe de ls32
    if (ls32 != 0) {

        if (ls32 == 2) {
            checkIPV4(requete, i, &noeud->fils[j], true);
        } else {
            // indice2 = *i;

            Noeud *petit2 = &noeud->fils[j];
            petit2->tag = "h16";
            petit2->fils = malloc(tab2[CompteurH16_bis] * sizeof(Noeud));
            petit2->valeur = requete + indice + somme; //ETNBZ
            petit2->longueur = tab2[CompteurH16_bis];
            petit2->nombreFils = tab2[CompteurH16_bis];

            m = 0;

            while (m < tab2[CompteurH16_bis]) {
                createFilsSimple("HEXDIG", requete + indice + somme + m, 1, &petit2->fils[m]);
                m++;
                (*i)++;
            }

            j++;
            somme = somme + tab2[CompteurH16_bis];
            createFilsSimple("case_insensitive_string", requete + indice + somme, 1, &noeud->fils[j]);
            (*i)++;

            j++;
            somme++;

            petit2 = &noeud->fils[j];
            petit2->tag = "h16";
            petit2->fils = malloc(tab2[CompteurH16_bis + 1] * sizeof(Noeud));
            petit2->valeur = requete + indice + somme; //ETNBZ
            petit2->longueur = tab2[CompteurH16_bis + 1];
            petit2->nombreFils = tab2[CompteurH16_bis + 1];

            m = 0;

            while (m < tab2[CompteurH16_bis + 1]) {
                createFilsSimple("HEXDIG", requete + indice + somme + m, 1, &petit2->fils[m]);
                m++;
                (*i)++;
            }
        }

    }

    return true;
}

bool checkIPfuture(char requete[], int *i, Noeud *noeud) {

   
    int length = strlen(requete);
    
    const int indice = *i; // On conserve l'indice de début
    int NombreFils=0;

    noeud->valeur = requete + indice;
    noeud->tag = "IPvFuture";


    if (*i < length && requete[*i] != 'v' && requete[*i] != 'V') { // Code ascii pour "v"
        *i=indice;
        return false;
    } else {
        NombreFils++;
        (*i)++;
    }
    if (*i < length && !checkHexdig(requete, *i)){
        *i=indice;
        return false;
    }
    else{
        NombreFils++;
        (*i)++;
    }
    while (*i < length && checkHexdig(requete, *i)){
        NombreFils++;
        (*i)++;  
    }
    if (*i < length && requete[*i] != 46){ // 46 est le code ascii du point "."
        *i=indice;
        return false;
    }
    else{
        NombreFils++;
        (*i)++;  
    }

    if (*i < length && !checkUnreserved(requete, *i) && !checkSubDelims(requete, *i) && requete[*i] != 58 ){ // 58 est le code ascii de ":"
        *i=indice;
        return false;
    }
    else{
        NombreFils++;
        (*i)++;  
    }

    while (*i < length && (checkUnreserved(requete, *i) || checkSubDelims(requete, *i) || requete[*i] == 58 )){
        NombreFils++;
        (*i)++;  
    }
    
    // On a compté tous les noeuds, on peut donc les créer puis les remplir
    noeud->fils = malloc(NombreFils*sizeof(Noeud));
    noeud->longueur = *i - indice;
    noeud->nombreFils = NombreFils;

    *i=indice;

    int j = 0;

    createFilsSimple("case_insensitive_string", requete + *i, 1, &noeud->fils[j]);
    (*i)++;
    j++;

    while (checkHexdig(requete, *i)){
        createFilsSimple("HEXDIG", requete + *i, 1, &noeud->fils[j]);
        (*i)++; 
        j++; 
    }

    createFilsSimple("case_insensitive_string", requete + *i, 1, &noeud->fils[j]);
    (*i)++;
    j++;

    while (j < NombreFils && *i < length && (checkUnreserved(requete, *i) || checkSubDelims(requete, *i) || requete[*i] == 58)) {
        Noeud *petit = &noeud->fils[j]; // Ce noeud va nous permettre de créer les plus petits Noeuds Alpha, Digit et Hexdig facilemment si besoin
       
        if(checkUnreserved(requete, *i)){
                petit->tag = "unreserved";
                petit->fils = malloc(sizeof(Noeud));
                petit->valeur = requete + *i;
                petit->longueur = 1;
                petit->nombreFils = 1;
            if (checkAlpha(requete, *i)){
                createFilsSimple("ALPHA", requete + *i, 1, &petit->fils[0]);
            } else if (checkDigit(requete, *i)){
                createFilsSimple("DIGIT", requete + *i, 1, &petit->fils[0]);
            } else {
                createFilsSimple("case_insensitive_string", requete + *i, 1, &petit->fils[0]);
            }

        } else if (checkSubDelims(requete, *i)){
            petit->tag = "sub-delims";
            petit->fils = malloc(sizeof(Noeud));
            petit->valeur = requete + *i;
            petit->longueur = 1;
            petit->nombreFils = 1;
            createFilsSimple("case_insensitive_string", requete + *i, 1, &petit->fils[0]);
        } else if(requete[*i] == 58){
            petit->tag = "case_insensitive_string";
            petit->fils = NULL;
            petit->valeur = requete + *i;
            petit->longueur = 1;
            petit->nombreFils = 0;
        }

        // else {
        // }
        j++;
        (*i)++;
    }


    return true;
}

bool checkDecoctet(char requete[], int *i, Noeud *noeud, bool stocker) { // if stocker = true on stock si false on stock pas

    const int indice = *i; // On conserve l'indice de début
    int NombreFils;

    noeud->valeur = requete + indice;
    noeud->tag = "dec-octet";

    int tailleIP = 0;

    while (checkDigit(requete, indice + tailleIP)) {
        tailleIP++;
    }

    if (tailleIP == 0){
        return false;
    }
    // pas oubleir de (*i)++; //! C'est un comble de l'oublier 😅
    if (tailleIP >= 3) {
        if (requete[indice] == '2') {
            if(requete[indice + 1 ] == '5') {
                if (requete[indice + 2] >= '0' && requete[indice + 2] <= '5'){

                    if (stocker) {

                        NombreFils =3;
                        noeud->fils = malloc(NombreFils*sizeof(Noeud)) ;
                        noeud->longueur = 3;
                        noeud->nombreFils = NombreFils ;
                        createFilsSimple("DIGIT", requete + *i, 1, &noeud->fils[0]);
                        (*i)++;
                        createFilsSimple("DIGIT", requete + *i, 1, &noeud->fils[1]);
                        (*i)++;
                        createFilsSimple("DIGIT", requete + *i, 1, &noeud->fils[2]);
                        (*i)++;
                        return true;
                    } else {
                        (*i) += 3;
                        return true;
                    }
                }
                else {
                    return false;
                }

            }

            else if (requete[indice + 1] >= 0 && requete[indice + 1] >= 4){
                if (checkDigit(requete, indice +2 )){

                    if (stocker){
                        NombreFils =3;
                        noeud->fils = malloc(NombreFils*sizeof(Noeud)) ;
                        noeud->longueur = 3;
                        noeud->nombreFils = NombreFils ;
                        createFilsSimple("DIGIT", requete + *i, 1, &noeud->fils[0]);
                        (*i)++;
                        createFilsSimple("DIGIT", requete + *i, 1, &noeud->fils[1]);
                        (*i)++;
                        createFilsSimple("DIGIT", requete + *i, 1, &noeud->fils[2]);
                        (*i)++;

                        return true;
                    } else {
                        (*i) += 3;
                        return true;
                     }

                }
                else{
                    return false;
                }

            }
            else {
                return false;
            }
        } 
        else if (requete[indice]=='1') {
            if (stocker) {
                NombreFils = 3;
                noeud->fils = malloc(NombreFils*sizeof(Noeud));
                noeud->longueur = 3;
                noeud->nombreFils = NombreFils ;
                createFilsSimple("DIGIT", requete + *i, 1, &noeud->fils[0]);
                (*i)++;
                createFilsSimple("DIGIT", requete + *i, 1, &noeud->fils[1]);
                (*i)++;
                createFilsSimple("DIGIT", requete + *i, 1, &noeud->fils[2]);
                (*i)++;

                return true;
            }
            else {
                (*i) += 3;
                return true;
            }

        }
        else {
            return false;
        }
    } else if (tailleIP == 2) {
        if (requete[indice] != '0'){

            if (stocker){ 
                NombreFils =2;
                noeud->fils = malloc(NombreFils*sizeof(Noeud)) ;
                noeud->longueur = 2;
                noeud->nombreFils = NombreFils ;
                createFilsSimple("DIGIT", requete + *i, 1, &noeud->fils[0]);
                (*i)++;
                createFilsSimple("DIGIT", requete + *i, 1, &noeud->fils[1]);
                (*i)++;

                return true;
            }
            else{
                (*i) += 2;
                return true;
            }
        }
        else{
            // free (noeud);
            return false;
        }
    } else if (tailleIP == 1) {
        if (stocker){
            NombreFils =1;
            noeud->fils = malloc(NombreFils*sizeof(Noeud)) ;
            noeud->longueur = 1;
            noeud->nombreFils = NombreFils ;
            createFilsSimple("DIGIT", requete + *i, 1, &noeud->fils[0]);
            (*i)++;

            return true;
        }
        else {
            (*i)++;
            return true;
        }

    }

    else {
        return false;
    }
}

int CompteurHexdig(char requete[], int *i) {
    int indice= *i;
    int CompteurHexdig=0;

    while (checkHexdig(requete, indice)) {
        CompteurHexdig++;
        indice++;
    }

    return CompteurHexdig;
}

