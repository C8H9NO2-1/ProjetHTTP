#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "structure.h"
#include "functions.h"
#include "affichage.h"

int main(int argc, char *argv[]) {
    
    // char requete[] = "GET /index.html HTTP/1.0\r\n";
    // char requete[] = "EKr1czBS+P*RAja /6Q@_C;IqPpdWi:I/4:b9Hra7UELY2tN/!xDzvl9mpYm)Y.j/MS!zM,C2P'!Z0l@ HTTP/4.0\r\n";
    // char requete[] = "CoNNEctIon: , Keep-alive,     	keep-alive, 	close,	test,";
    // char requete[] = "CONNeCtION:, 		, ,5_p8ck0lJ|vM-lx	,";
    char cookie[] = "COoKiE: 	  n+q#_iP&W9EoHoE=\"/{i>}uPeh9TZ9]V\"; 4qdK8ZQdCyBMF.k=\"#j.=cFZ@$?^&-$4\"; k1KU^_X&opaKHta=:]xa&s<TDKnZPM*; zN&yiA`MFeSoGaa=*wwlhxR28?L![32";

    char transferEncoding[] = "TRaNSFEr-enCODiNG:  ,20Kmy%d_Yg8t.MS; 	eh6owY1JpoLr99n=\"k_*	lx:H~ rV\\-\\R\\	!Pa,v\"";

    Noeud *test = malloc(sizeof(Noeud));

    int i = 0;

    if (!checkTransferEncodingHeader(transferEncoding, &i, strlen(transferEncoding), test)) {
        printf("Hello world\n");
        test = NULL;
    }

    if (test != NULL) {
        printArbre(test, 0);
    }

    free(test);
    
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
    if (noeud != NULL) {
        free(noeud);
    }

    return false;
}

bool checkToken(char requete[], int *i, int longueur, Noeud *noeud, char nom[]) {
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

//!===============================================================================
//? Fonctions utiles pour parser la start-line
bool checkStartLine(char requete[], int *i, int longueur, Noeud *noeud) {
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

    return true;
}

bool checkMethod(char requete[], int *i, int longueur, Noeud *noeud) {
    return checkToken(requete, i, longueur, noeud, "method");
}

bool checkRequestTarget(char requete[], int *i, const int longueur, Noeud *noeud) {
    // On remplit ce qu'on sait déjà du noeud
    noeud->valeur = requete + *i;
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
        createFilsSimple("case_insensitive_string", requete + *i, 1, filsQuery1);
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

bool checkAbsolutePath(char requete[], int *i, const int longueur, Noeud *noeud) {
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
    if (noeud != NULL) {
        free(noeud);
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
    int nombreFils = 5;
    const int indice = *i;

    noeud->fils = malloc(nombreFils * sizeof(Noeud));
    noeud->valeur = requete + indice;
    // On ne connaît pas encore noeud->longueur
    noeud->nombreFils = nombreFils;
    noeud->tag = "Connection-header";

    int j = 0; // On utilise cet indice pour compter les fils

    if (!checkConnectionString(requete, i, &noeud->fils[j])) {
		for (int k = 0; k < nombreFils; k ++) {
			if (k != j) {
				free(&noeud->fils[k]);
			}
		}
        free(noeud);
        *i = indice;
        return false;
    }
    j++;

    if (requete[*i] != ':') {
        for (int k = 0; k < nombreFils; k ++) {
			if (k != j) {
				free(&noeud->fils[k]);
			}
		}
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
        for (int k = 0; k < nombreFils; k ++) {
			if (k != j) {
				free(&noeud->fils[k]);
			}
		}
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
        free(noeud);
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
        free(noeud);
        *i = indice;
        return false;
    }
    compteur++;

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
            *i = indiceTemp;
        } else {
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
    return checkToken(requete, i, longueur, noeud, "connection-option");
}

//!===============================================================================
//? Fonctions utiles pour parser le cookie-header

bool checkCookieHeader(char cookie[], int *i, int longueur, Noeud *noeud) { //Cookie-header = "Cookie" ":" OWS cookie-string OWS
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
    (*i)++;

    Noeud *filsOWS1 = malloc(sizeof(Noeud));
    checkOWS(cookie, i, longueur, filsOWS1);


    Noeud *filsCookieString = malloc(sizeof(Noeud));
    if (!checkCookieString(cookie, i, longueur, filsCookieString)){
        free(noeud);
        *i = indice;
        return false;
    }

    Noeud *filsOWS2 = malloc(sizeof(Noeud));
    checkOWS(cookie, i, longueur, filsOWS2);
   
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
    
    noeud->fils[2] = *filsOWS1;
    (*i) += noeud->fils[2].longueur;

    noeud->fils[3] = *filsCookieString;
    (*i) += noeud->fils[3].longueur;

    noeud->fils[4] = *filsOWS2;
    (*i) += noeud->fils[4].longueur;

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


bool checkCookieString(char cookie[], int *i, int longueur, Noeud *noeud){ //cookie-pair *( ";" SP cookie-pair )
    const int indice=*i;
    int compteur=0;
    Noeud *filsCookiePair1 = malloc(sizeof(Noeud));
    if (!checkCookiePair(cookie, i, longueur, filsCookiePair1)){
        free(noeud);
        *i = indice;
        return false;
    }
    compteur++;

    while (*i < longueur && cookie[*i]==59){ //";"=59
        (*i)+=2;
        if(cookie[(*i)-1]!=32 || !checkCookiePair(cookie, i, longueur, NULL)){
            free(noeud);
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
        noeud->fils[j]=*filsCookiePair1;
        (*i) += noeud->fils[j].longueur;
        j++;

        while (*i < longueur && cookie[*i]==59 && j < compteur){ //";"=59
            createFilsSimple("case_insensitive_string", cookie + *i, 1, &noeud->fils[j]);
            j++;
            (*i)+=2;
            createFilsSimple("SP", cookie + (*i) - 1, 1, &noeud->fils[j]);
            j++;
            if(cookie[(*i)-1]!=32 || !checkCookiePair(cookie, i, longueur, &noeud->fils[j])){
                free(noeud);
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
        noeud->fils[0]=*filsCookiePair1;
    }
    return true ;
}

bool checkCookiePair(char cookie[], int *i, int longueur, Noeud *noeud){ //cookie-pair = cookie-name "=" cookie-value
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
    (*i)++;
    
    Noeud *filsCookieValue = malloc(sizeof(Noeud));
    if (!checkCookieValue(cookie, i, longueur, filsCookieValue)){
        free(noeud);
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
    }
    return true;
}

bool checkCookieName(char cookie[], int *i, int longueur, Noeud *noeud){ //cookie-name = 1*tchar
    int compteur = 0;
    int indice = *i;
    // On compte le nombre de caractères dans le champ courant
    while (*i < longueur && checkTChar(cookie, *i, NULL)) {
        (*i)++;
        compteur++;
    }

    // Si on a 0 tchar, on renvoie false
    if (compteur < 1) {
        free(noeud);
        return false;
    }
    if (noeud!=NULL){
        // On stocke les données nécessaires pour le noeud courant
        noeud->valeur = cookie + indice;
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
    }
    return true;
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

//!===============================================================================
//? Fonctions utiles pour parser le transfer-encoding-header

bool checkTransferEncodingHeader(char transferEncoding[], int *i, int longueur, Noeud *noeud){//Transfer-Encoding-header = "Transfer-Encoding" ":" OWS Transfer-Encoding OWS
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
    checkOWS(transferEncoding, i, longueur, filsOWS1);

    Noeud *filsTransferEncoding = malloc(sizeof(Noeud));
    if (!checkTransferEncoding(transferEncoding, i, longueur, filsTransferEncoding)){
        free(noeud);
        *i = indice;
        return false;
    }

    Noeud *filsOWS2 = malloc(sizeof(Noeud));
    checkOWS(transferEncoding, i, longueur, filsOWS2);

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
    
    noeud->fils[2] = *filsOWS1;
    (*i) += noeud->fils[2].longueur;

    noeud->fils[3] = *filsTransferEncoding;
    (*i) += noeud->fils[3].longueur;

    noeud->fils[4] = *filsOWS2;
    (*i) += noeud->fils[4].longueur;

    return true;

}

bool checkTransferEncodingMot(char transferEncoding[], Noeud *noeud){
    noeud->valeur = transferEncoding;
    noeud->longueur = 17;
    noeud->tag = "case_insensitive_string";
    noeud->fils = NULL;
    noeud->nombreFils = 0;
    char transferEncodingMinuscule[18];
    sousChaineMinuscule(transferEncoding, transferEncodingMinuscule, 0, 17);
    int a=strcmp("transfer-encoding", transferEncodingMinuscule);
    if (a!=0){
        free(noeud);
        return false ;
    }
    return true;
}

bool checkTransferEncoding(char transferEncoding[], int *i, int longueur, Noeud *noeud){
    //Transfer-Encoding = *( "," OWS ) transfer-coding *( OWS "," [ OWS transfer-coding ] )
    int compteur=0;
    int const indice=*i;
    while ((*i)<longueur && transferEncoding[*i]==44){
        (*i)++;
        compteur++;
        checkOWS(transferEncoding, i, longueur, NULL);
        compteur++;
    }
    if (!checkTransferCoding(transferEncoding, i, longueur, NULL)){
            free(noeud);
            *i = indice;
            return false;
    }

    // (*i)++;
    compteur++;
    checkOWS(transferEncoding, i, longueur, NULL);
    while (*i<longueur && transferEncoding[*i]==44){
        compteur+=2;
        (*i)++;
        // int iTemp=(*i);
        checkOWS(transferEncoding, i, longueur, NULL);
        if(checkTransferCoding(transferEncoding, i, longueur, NULL)){
                if (*i>=longueur) {
                    compteur+=2;
                    break;
                }
                (*i)++;
                compteur+=2;
        }
    }

    noeud->valeur = transferEncoding + indice;
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
            createFilsSimple("case_insensitive_string", transferEncoding + *i, 1, &noeud->fils[j]);
            (*i)++;
            j++;
            checkOWS(transferEncoding, i, longueur, &noeud->fils[j]);
        }
        else if (checkTransferCoding(transferEncoding, i, longueur, NULL)){
            *i=iTemp;
            checkTransferCoding(transferEncoding, i, longueur, &noeud->fils[j]);
        }
        else {
            checkOWS(transferEncoding, i, longueur, &noeud->fils[j]);
        }
    }
    return true;  
}

bool checkTransferCoding(char transferEncoding[], int *i, int longueur, Noeud *noeud){
    //transfer-coding = "chunked" / "compress" / "deflate" / "gzip" / transfer-extension
    const int indice=*i;

    if (noeud!=NULL){
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
    if (strcmp("compress", transferEncodingMinuscule)==0){
        if (noeud!=NULL){
            noeud->longueur = 8;
            (*i)=indice;
            createFilsSimple("case_insensitive_string", transferEncoding + *i, 8, &noeud->fils[0]);
        }
        (*i)+=8;
        return true;
    }

    sousChaineMinuscule(transferEncoding, transferEncodingMinuscule, *i, (*i)+7);
    if (strcmp("deflate", transferEncodingMinuscule)==0){
        if (noeud!=NULL){
            noeud->longueur = 7;
            (*i)=indice;
            createFilsSimple("case_insensitive_string", transferEncoding + *i, 7, &noeud->fils[0]);
        }
        (*i)+=7;
        return true;
    }

    sousChaineMinuscule(transferEncoding, transferEncodingMinuscule, *i, (*i)+4);
    if (strcmp("gzip", transferEncodingMinuscule)==0){
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
        Noeud *filstransferExtension=malloc(sizeof(Noeud));
        checktransferExtension(transferEncoding, i, longueur, filstransferExtension);
        if (noeud!=NULL){
            noeud->longueur = (*i)-indice;
            noeud->fils[0]=*filstransferExtension;
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

bool checktransferExtension(char transferEncoding[], int *i, int longueur, Noeud *noeud){
    //transfer-extension = token *( OWS ";" OWS transfer-parameter )
    int compteur=0;
    int const indice=*i;
    int point_virgule=longueur;
    while (((*i)<longueur)&&(checkTChar(transferEncoding, *i, NULL))){
        compteur++;
        (*i)++; 
    }
    if (compteur==0){
        free(noeud);
        (*i)=indice;
        return false;
    }
    checkOWS(transferEncoding, i, longueur, NULL);
    if (transferEncoding[*i]==59){
        point_virgule=*i;
    }
    while (*i<longueur && transferEncoding[*i]==59){
        compteur+=2;
        (*i)++;
        checkOWS(transferEncoding, i, longueur, NULL);
        if (checkTransferParameter(transferEncoding, i, longueur, NULL)){
            compteur+=2;
            (*i)++;
            checkOWS(transferEncoding, i, longueur, NULL);
        }
    }
    if (noeud!=NULL){
        noeud->valeur = transferEncoding + indice;
        noeud->longueur = *i - indice;
        noeud->tag = "Transfer-Extension";
        noeud->fils = malloc(compteur * sizeof(Noeud));
        noeud->nombreFils =compteur;

        // On réinitialise l'indice i pour la suite de la fonction
        *i = indice;
        for (int j=0; j<compteur; j++){
            int iTemp=*i;
            if (*i<point_virgule && checkTChar(transferEncoding, *i, NULL)){
                checkTChar(transferEncoding, *i, &noeud->fils[j]);
                (*i)++;
            }
            else if(transferEncoding[*i]==59){
                checkOWS(transferEncoding, i, longueur, &noeud->fils[j]);
                j++;
                createFilsSimple("case_insensitive_string", transferEncoding + *i, 1, &noeud->fils[j]);
                (*i)++;
            }
            else if(checkTransferParameter(transferEncoding, i, longueur, NULL)){
                *i=iTemp;
                checkTransferParameter(transferEncoding, i, longueur, &noeud->fils[j]);
                (*i)++;
            }
            else {
                *i=iTemp;
                checkOWS(transferEncoding, i, longueur, &noeud->fils[j]);
            }
        }
    }
    return true;
}

bool checkTransferParameter(char transferEncoding[], int *i, int longueur, Noeud *noeud){
    //transfer-parameter = token BWS "=" BWS ( token / quoted-string )
    //BWS=OWS
    int compteur=0;
    const int indice=*i;
    while ((*i)<longueur && checkTChar(transferEncoding, *i, NULL)){
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

    if (noeud!=NULL){
        noeud->valeur = transferEncoding + indice;
        noeud->longueur = *i - indice;
        noeud->tag = "transfer-parameter";
        noeud->fils = malloc(compteur * sizeof(Noeud));
        noeud->nombreFils =compteur;

        *i = indice;

        for (int j=0;j<compteur;j++){
            int iTemp=*i;
            if (checkTChar(transferEncoding, *i, NULL)){
                checkTChar(transferEncoding, *i, &noeud->fils[j]);
                (*i)++;
            }
            // else if (transferEncoding[*i]==61){
            //     checkOWS(transferEncoding, i, longueur, &noeud->fils[j]);
            //     j++;
            //     createFilsSimple("case_insensitive_string", *i, 1, &noeud->fils[j]);
            //     printf("je suis un égal \n");
            //     (*i)++;
            //     j++;
            //     checkOWS(transferEncoding, i, longueur, &noeud->fils[j]);
            // }
            else if (checkQuotedString(transferEncoding, i, longueur, NULL)){
                *i=iTemp;
                checkQuotedString(transferEncoding, i, longueur, &noeud->fils[j]);
            }
            else {
                (*i)=iTemp;
                checkOWS(transferEncoding, i, longueur, &noeud->fils[j]);
                j++;
                createFilsSimple("case_insensitive_string", transferEncoding + *i, 1, &noeud->fils[j]);
                (*i)++;
                j++;
                checkOWS(transferEncoding, i, longueur, &noeud->fils[j]);
            }
            // (*i)++;
        }
    }
    return true;
}

bool checkQuotedString(char transferEncoding[], int *i, int longueur, Noeud *noeud){
    //quoted-string = DQUOTE *( qdtext / quoted-pair ) DQUOTE
    int compteur=0;
    int const indice =(*i);
    if (transferEncoding[*i]!=34){
        (*i)=indice;
        free(noeud);
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
        free(noeud);
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
            // (*i)++;
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
        free(noeud);
        return false;
    }
}

bool checkQuotedPair(char transferEncoding[], int *i, int longueur, Noeud *noeud){
    //quoted-pair = "\" ( HTAB / SP / VCHAR / obs-text )
    int indice= *i;
    if (noeud!=NULL){
        noeud->valeur = transferEncoding + *i;
        noeud->longueur = 2;
        noeud->tag = "quoted-pair";
        noeud->fils = malloc(2*sizeof(Noeud));
        noeud->nombreFils =2;
    }
    if (transferEncoding[*i]!=92){
        free(noeud);
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
        free(noeud);
        (*i)=indice;
        return false;
    }
    (*i)++;

    if (noeud == NULL) {
        (*i)--;
    }

    return true;
}
