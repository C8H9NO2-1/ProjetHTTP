#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "structure.h"
#include "tom.h"
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
    
    // char requete[] = "hoSt:[f731:542:6e:57c5:c3a:d3:e6f:e]		";
	// char requete[] = "hOsT: 			138.199.75.219:84766 ";
	// char requete[] = "Host: [Ve2509c.1oGFNz9rfZi+;*=)*+:++*:$&($+K7B6]  "; //! Ce test n'est pas censé fonctionner de la façon dont il fonctionne actuellement => Solved √
	// char requete[] = "hosT:    *;('*==S~7nGt.a+='!$'$*NxQWy%bAmV%6C";
	// char requete[] = "hosT:[1f2f:2532:922f:93d6:8fc:4605:239.44.246.129]:0176";
	// char requete[] = "HOST:[::b886:ec3f:f56:a366:c3e:f1bd:1ce]:9164 	 ";
	// char requete[] = "HOsT:[::c83c:7e0c:2b58:cf7c:aea1:166.202.243.242]";
	// char requete[] = "hOSt:[21::d7c0:2ab:34ca:f88:4616]:3663 		";
	// char requete[] = "HoST:[8748::5790:9b7c:203.221.115.212]";

    //! Batterie de tests qui ne devraient pas fonctionner (ou du moins pas de la manière dont on le pense à première vue)
    // char requete[] = "hoSt:[f731:542:pe:57c5:c3a:d3:e6f:e]		";
    // char requete[] = "hOsT: 			138.199.75.219:84a66 ";
    // char requete[] = "hsT:";
    // char requete[] = "hOsT	138.199.75.219:84a66 ";
    // char requete[] = "hOSt:[21::d7c0:2ab:34ca:f88:]:3663 		";
    char requete[] = "hOSt:[21::d7c0:2ab:34ca:f88]: 		";
    
    //* Annotations
    //? Pourquoi es-ce qu'on ignore le premier caractère si celui est un [

    //? Problème si on a ::: au lieu de ::, le code considère que c'est une syntaxe valide
    //? Alors que ce n'est pas le cas

    // 7c98:56ae:f99c:839a:1571:03d8:5969:4e0e
    // 47.122.200.37

    //! Première ligne de la grammaire
    // char requete[] = "7c98:56ae:f99c:839a:1571:03d8:5969:4e0e";
    // char requete[] = "7c98:56ae:f99c:839a:1571:03d8:47.122.200.37";

    //! Deuxième ligne de la grammaire
    // char requete[] = "::56ae:f99c:839a:1571:03d8:5969:4e0e";
    // char requete[] = "::56ae:f99c:839a:1571:03d8:47.122.200.37";

    //! Troisième ligne de la grammaire
    // char requete[] = "7c98::f99c:839a:1571:03d8:5969:4e0e";
    // char requete[] = "7c98::f99c:839a:1571:03d8:47.122.200.37";
    // char requete[] = "::f99c:839a:1571:03d8:5969:4e0e";
    // char requete[] = "::f99c:839a:1571:03d8:47.122.200.37";

    //! Quatrième ligne de la grammaire
    // char requete[] = "7c98:56ae::839a:1571:03d8:5969:4e0e";
    // char requete[] = "7c98:56ae::839a:1571:03d8:47.122.200.37";
    // char requete[] = "7c98::839a:1571:03d8:5969:4e0e";
    // char requete[] = "7c98::839a:1571:03d8:47.122.200.37";
    // char requete[] = "::839a:1571:03d8:5969:4e0e";
    // char requete[] = "::839a:1571:03d8:47.122.200.37";

    //! Cinquième ligne de la grammaire
    // char requete[] = "7c98:56ae:f99c::1571:03d8:5969:4e0e";
    // char requete[] = "7c98:56ae:f99c::1571:03d8:47.122.200.37";
    // char requete[] = "7c98:56ae::1571:03d8:5969:4e0e";
    // char requete[] = "7c98:56ae::1571:03d8:47.122.200.37";
    // char requete[] = "7c98::1571:03d8:5969:4e0e";
    // char requete[] = "7c98::1571:03d8:47.122.200.37";
    // char requete[] = "::1571:03d8:5969:4e0e";
    // char requete[] = "::1571:03d8:47.122.200.37";

    //! Sixième ligne de la grammaire
    // char requete[] = "7c98:56ae:f99c:839a::03d8:5969:4e0e";
    // char requete[] = "7c98:56ae:f99c:839a::03d8:47.122.200.37";
    // char requete[] = "7c98:56ae:f99c::03d8:5969:4e0e";
    // char requete[] = "7c98:56ae:f99c::03d8:47.122.200.37";
    // char requete[] = "7c98:56ae::03d8:5969:4e0e";
    // char requete[] = "7c98:56ae::03d8:47.122.200.37";
    // char requete[] = "7c98::03d8:5969:4e0e";
    // char requete[] = "7c98::03d8:47.122.200.37";
    // char requete[] = "::03d8:5969:4e0e";
    // char requete[] = "::03d8:47.122.200.37";

    //! Septième ligne de la grammaire
    // char requete[] = "7c98:56ae:f99c:839a:1571::5969:4e0e";
    // char requete[] = "7c98:56ae:f99c:839a:1571::47.122.200.37";
    // char requete[] = "7c98:56ae:f99c:839a::5969:4e0e";
    // char requete[] = "7c98:56ae:f99c:839a::47.122.200.37";
    // char requete[] = "7c98:56ae:f99c::5969:4e0e";
    // char requete[] = "7c98:56ae:f99c::47.122.200.37";
    // char requete[] = "7c98:56ae::5969:4e0e";
    // char requete[] = "7c98:56ae::47.122.200.37";
    // char requete[] = "7c98::5969:4e0e";
    // char requete[] = "7c98::47.122.200.37";
    // char requete[] = "::5969:4e0e";
    // char requete[] = "::47.122.200.37";

    //! Huitième ligne de la grammaire
    // char requete[] = "7c98:56ae:f99c:839a:1571:03d8::4e0e";
    // char requete[] = "7c98:56ae:f99c:839a:1571::4e0e";
    // char requete[] = "7c98:56ae:f99c:839a::4e0e";
    // char requete[] = "7c98:56ae:f99c::4e0e";
    // char requete[] = "7c98:56ae::4e0e";
    // char requete[] = "7c98::4e0e";
    // char requete[] = "::4e0e";

    //! Neuvième ligne de la grammaire
    // char requete[] = "7c98:56ae:f99c:839a:1571:03d8:5969::";
    // char requete[] = "7c98:56ae:f99c:839a:1571:03d8::";
    // char requete[] = "7c98:56ae:f99c:839a:1571::";
    // char requete[] = "7c98:56ae:f99c:839a::";
    // char requete[] = "7c98:56ae:f99c::";
    // char requete[] = "7c98:56ae::";
    // char requete[] = "7c98::";
    // char requete[] = "::";

    //! Autres tests
    // char requete[] = "7c98:56ae:9c:839a:1571:03d8:5969:4e0e";
    // char requete[] = "7c98:56ae:fc:839a:1571:03d8:5969:4ee";
    // char requete[] = "798:56ae:f99c:839a::03d8:5969:4e0e";
    // char requete[] = "7c98:56ae:f99c:3a:15:03d8:5969:4e0e";
    // char requete[] = "7c98:56ae:fc:839a:1571:03d8:5969:40e";

    //! Tests qui fonctionnent mais qui ne sont pas censés
    // char requete[] = "[7c98:56ae:f99c:::1571:03d8:5969:4e0e]"; //* Réglé √
    // char requete[] = "[Ve2509c.1oGFNz9rfZi+;*=)*+:++*:$&($+K7B6]"; //* Réglé √
    
    // char requete[] = "47.122.200.37";

    // char requete[] = "Ve2509c.1oGFNz9rfZi+;*=)*+:++*:$&($+K7B6";

    Noeud *test = malloc(sizeof(Noeud));

    int i = 0;

    if (!checkHostHeader(requete, &i, strlen(requete), test)) {
        printf("Hello world\n");
        test = NULL;
    }

    // if (!checkIPfuture(requete, &i, test)) {
    //     printf("Hello world\n");
    //     test = NULL;
    // }

    if (test != NULL) {
        printArbre(test, 0);
        freeArbre(test);
    }

    free(test);
    
    return 0;
}

//==============================Fonctions générales==============================

bool checkAlpha(char requete[], int i) {
    return ((requete[i] >= 'a' && requete[i] <= 'z') || (requete[i] >= 'A' && requete[i] <= 'Z'));
}

bool checkDigit(char requete[], int i) {
    return (requete[i] >= '0' && requete[i] <= '9');
}

bool checkHexdig(char requete[], int i) {
    if ((requete[i] >= 'a' && requete[i] <= 'f') || (requete[i] >= 'A' && requete[i] <= 'F') || (requete[i] >= '0' && requete[i] <= '9')) {
        return true;
    }
    return false;
}

bool checkUnreserved(char requete[], int i) {
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

bool checkPctEncoded(char requete[], int i) {
    return (requete[i] == '%' && checkHexdig(requete, i + 1) && checkHexdig(requete, i + 2));
}

bool checkSubDelims(char requete[], int i) {
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

void sousChaineMinuscule(char chaine1[], char chaine2[], int i, int j) {
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

bool checkOWS(char requete[], int *i, int longueur, Noeud *noeud) {
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

bool checkExpectString(char requete[], int *i, Noeud *noeud) {
    const int indice = *i; // On conserve l'indice de début
    int n = 6; // Taille de la sous-chaîne qui contient potentiellement "Connection"
    char *chaineExpect = malloc((n + 1) * sizeof(char)); // On ajoute un caractère pour le \0

    sousChaineMinuscule(requete, chaineExpect, *i, *i + n);

    if (strcmp(chaineExpect, "expect") != 0) {
        // free(noeud);
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

bool checkHostString(char requete[], int *i, Noeud *noeud) {
    const int indice = *i; // On conserve l'indice de début
    int n = 4; // Taille de la sous-chaîne qui contient potentiellement "Connection"
    char *chaineHost = malloc((n + 1) * sizeof(char)); // On ajoute un caractère pour le \0

    sousChaineMinuscule(requete, chaineHost, *i, *i + n);

    if (strcmp(chaineHost, "host") != 0) {
        // free(noeud);
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

bool checkExpect(char requete[], int *i, Noeud *noeud){
    const int indice = *i; // On conserve l'indice de début
    int n = 12; // Taille de la sous-chaîne qui contient potentiellement "100-continue"
    char *chaineContinue = malloc((n + 1) * sizeof(char)); // On ajoute un caractère pour le \0

    sousChaineMinuscule(requete, chaineContinue, *i, *i + n);

    if (strcmp(chaineContinue, "100-continue") != 0) {
        // free(noeud);
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

bool checkDecoctet(char requete[], int *i, Noeud *noeud, bool stocker){ // if stocker = true on stock si false on stock pas

    const int indice = *i; // On conserve l'indice de début
    int NombreFils;
    //noeud->fils = NULL;
    //noeud->longueur = *i - indice;
    noeud->valeur = requete + indice;
    //noeud->nombreFils = ;
    noeud->tag = "dec-octet";
    int tailleIP = 0;

    while (checkDigit(requete, indice + tailleIP)) {
        tailleIP++;
    }

    if (tailleIP == 0){
        // free (noeud);
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
                        // noeud->longueur = *i - indice;
                        noeud->longueur = 3;
                        noeud->nombreFils = NombreFils ;
                        createFilsSimple("Digit", requete + *i, 1, &noeud->fils[0]);
                        (*i)++;
                        createFilsSimple("Digit", requete + *i, 1, &noeud->fils[1]);
                        (*i)++;
                        createFilsSimple("Digit", requete + *i, 1, &noeud->fils[2]);
                        (*i)++;
                        return true;
                    } else {
                        (*i) += 3;
                        // free(noeud);
                        return true;
                    }
                }
                else {
                    // free (noeud);
                    return false;
                }

            }

            else if (requete[indice + 1] >= 0 && requete[indice + 1] >= 4){
                if (checkDigit(requete, indice +2 )){

                    if (stocker){
                        NombreFils =3;
                        noeud->fils = malloc(NombreFils*sizeof(Noeud)) ;
                        // noeud->longueur = *i - indice;
                        noeud->longueur = 3;
                        noeud->nombreFils = NombreFils ;
                        createFilsSimple("Digit", requete + *i, 1, &noeud->fils[0]);
                        (*i)++;
                        createFilsSimple("Digit", requete + *i, 1, &noeud->fils[1]);
                        (*i)++;
                        createFilsSimple("Digit", requete + *i, 1, &noeud->fils[2]);
                        (*i)++;

                        return true;
                    } else {
                        (*i) += 3;
                        // free(noeud);
                        return true;
                     }

                }
                else{
                    // free(noeud);
                    return false;
                }

            }
            else {
                // free(noeud);
                return false;
            }
        } 
        else if (requete[indice]=='1') {
            if (stocker) {
                NombreFils = 3;
                noeud->fils = malloc(NombreFils*sizeof(Noeud));
                // noeud->longueur = *i - indice;
                noeud->longueur = 3;
                noeud->nombreFils = NombreFils ;
                createFilsSimple("Digit", requete + *i, 1, &noeud->fils[0]);
                (*i)++;
                createFilsSimple("Digit", requete + *i, 1, &noeud->fils[1]);
                (*i)++;
                createFilsSimple("Digit", requete + *i, 1, &noeud->fils[2]);
                (*i)++;

                return true;
            }
            else {
                (*i) += 3;
                // free(noeud);
                return true;
            }

        }
        else {
            // free (noeud);
            return false;
        }
    } else if (tailleIP == 2) {
        if (requete[indice] != '0'){

            if (stocker){ 
                NombreFils =2;
                noeud->fils = malloc(NombreFils*sizeof(Noeud)) ;
                // noeud->longueur = *i - indice;
                noeud->longueur = 2;
                noeud->nombreFils = NombreFils ;
                createFilsSimple("Digit", requete + *i, 1, &noeud->fils[0]);
                (*i)++;
                createFilsSimple("Digit", requete + *i, 1, &noeud->fils[1]);
                (*i)++;

                return true;
            }
            else{
                (*i) += 2;
                // free(noeud);
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
            // noeud->longueur = *i - indice;
            noeud->longueur = 1;
            noeud->nombreFils = NombreFils ;
            createFilsSimple("Digit", requete + *i, 1, &noeud->fils[0]);
            (*i)++;

            return true;
        }
        else {
            (*i)++;
            // free(noeud);
            return true;
        }

    }

    else {
        printf("Si ceci s'affiche : Gros Probleme dans la fonction checkDecoctet");
        // free (noeud);
        return false;
    }
}

bool checkIPfuture(char requete[], int *i, Noeud *noeud) {

   
    int length = strlen(requete);
    
    const int indice = *i; // On conserve l'indice de début
    int NombreFils=0;
   // noeud->fils = malloc(NombreFils*sizeof(Noeud));
    noeud->valeur = requete + indice;
   // noeud->longueur = *i - indice;
   // noeud->nombreFils = NombreFils;
    noeud->tag = "IPvFuture";


    if (*i < length && requete[*i] != 'v' && requete[*i] != 'V') { // Code ascii pour "v"
        // free(noeud);
        *i=indice;
        return false;
    } else {
        NombreFils++;
        (*i)++;
    }
    if (*i < length && !checkHexdig(requete, *i)){
        // free (noeud);
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
        // free(noeud);
        *i=indice;
        return false;
    }
    else{
        NombreFils++;
        (*i)++;  
    }

    if (*i < length && !checkUnreserved(requete, *i) && !checkSubDelims(requete, *i) && requete[*i] != 58 ){ // 58 est le code ascii de ":"
        // free(noeud);
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
                createFilsSimple("Alpha", requete + *i, 1, &petit->fils[0]);
            }
            else if (checkDigit(requete, *i)){
                createFilsSimple("Digit", requete + *i, 1, &petit->fils[0]);
            }
            else {
                createFilsSimple("case_insensitive_string", requete + *i, 1, &petit->fils[0]);
            }

        }
        else if (checkSubDelims(requete, *i)){
            petit->tag = "sub-delims";
            petit->fils = malloc(sizeof(Noeud));
            petit->valeur = requete + *i;
            petit->longueur = 1;
            petit->nombreFils = 1;
            createFilsSimple("case_insensitive_string", requete + *i, 1, &petit->fils[0]);
        }
        else if(requete[*i] == 58){
            petit->tag = "case_insensitive_string";
            petit->fils = NULL;
            petit->valeur = requete + *i;
            petit->longueur = 1;
            petit->nombreFils = 0;
        }

        else {
            printf("GROS BUG DANS LA FONCTION IPFUTURE");
        }
        j++;
        (*i)++;
    }

    // createFilsSimple("case_insensitive_string", requete + *i, 1, &noeud->fils[j]);

    // (*i)++;


    return true;

}

int CompteurHexdig(char requete[], int *i){
     int indice= *i;
     int CompteurHexdig=0;

    while (checkHexdig(requete, indice)){
        CompteurHexdig++;
        indice++;
    }

    return CompteurHexdig;
}

bool checkIPV6(char requete[], int *i, Noeud *noeud) {
    // Ne pas oublier de faire *i=indice; avant chaque return false pour ne pas créer de problèmes lors du 2e if de checkIPliteral

    const int length = strlen(requete);

    //Potentiellement faire un tableau des tailles de H16

    printf("IPV6 : %d\n", *i);
    
    const int indice = *i; // On conserve l'indice de début
    int indice2;

    int NombreFils = 0;

    int j = 0; // J nous servira plus tard pour créer les différents noeuds

    Noeud *petit; // A l'aide du noeud petit on va créer les noeuds H16 puis digit
    int somme; // Nous sert à "localiser" les noeuds
    
    //! We might need to change the initial value of m
    int m = 0;

    int k = 0; //k nous sert à accéder au tableau du compteur de digit des h16

    int CompteurH16=0;
    int Compteur=0;
    int tab1[9]; //Ce tableau nous sert à garder en mémoire la taille de chaque H16

    int tab2[8]; //Ce tableau fais la meme chose que le premier mais pour les H16 après "::"
    int CompteurH16_bis=0; //Idem
    
    // bool ls32=false; // Ce boolean nous sera utile pour verifier si on se trouve dans le ls32 ou non
    int ls32 = 0; //? 0 = pas de ls32, 1 = h16:h16, 2 = ipv4

    bool interrupteur = true;
    bool cas_1 = true;

    printf("Test\n");
    
    //noeud->fils = malloc(NombreFils*sizeof(Noeud));
    noeud->valeur = requete + indice;
    //noeud->longueur = ;
    //noeud->nombreFils = ;
    noeud->tag = "IPv6address";

    while (interrupteur) { // Cette boucle while va nous permettre de compter le nombre de H16 ou de (H16 ":") avant les "::"
        Compteur = CompteurHexdig(requete, i);
        printf("\nCompteur : %d\n", Compteur);

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
            // free (noeud);
            printf("Erreur dans la fonction IPV6\n");
            *i=indice;
            return false;
        } else {
            if (requete[*i + Compteur]==':') {
                printf("Test :\n");
                if (CompteurH16 == 6) {  //A partir de H16 = 6 on peut soit être dans le cas 1 ou les deux derniers, on active donc une option nous permettant de ne plus tout jeter si ce qui vient après n'est pas H16 ou ":" "::"
                    cas_1=false;
                    printf("Test 1\n");
                }
                if(requete[*i + Compteur + 1]==':') {
                    printf("Test 2 :\n");
                    *i = *i + Compteur + 1;
                    interrupteur = false;
                }
                else{
                    printf("Test 2 ");
                    *i = *i + Compteur + 1;
                    printf("i : %d\n", *i);
                }
            } else {
                printf("Test 3\n");
                if (CompteurH16 != 8 && requete[*i + Compteur] != '.') { //Si Il y a 8 H16 alors on est dans le cas 1 avec 2 H16 dans ls32, donc c'est valide de ne pas avoir de ":" après le 8e H16
                // free(noeud);
                    *i=indice;
                    return false;
                } else{
                    if (requete[*i + Compteur]=='.') { //Ce qu'on a pris pour un H16 était en réalité un dec-octet, on corrigera notre erreur plus tard
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

    printf("\nLine %d => CompteurH16 : %d\n\n", __LINE__, CompteurH16);

    printf("i : %d\n", *i);

    printf("Test 4\n");

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

            petit->tag = "H16";
            petit->fils = malloc(tab1[a]*sizeof(Noeud));
            petit->valeur = requete + indice + somme; //ETNBZ
            petit->longueur = tab1[a];
            petit->nombreFils = tab1[a];

            m=0;

            while (m < tab1[a]) {
                createFilsSimple("Hexdig", requete + indice + somme + m , 1, &petit->fils[m]);
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

        printf("Test IPV4\n");

        NombreFils= 6 + 6 + 1; // 6 H16 + 6 :  + 1 addresseipv4
        noeud->fils = malloc(NombreFils*sizeof(Noeud));
        noeud->longueur = *i - indice;
        noeud->nombreFils = NombreFils ;

        if (checkIPV4(requete, i, &noeud->fils[NombreFils - 1], true)) {

            printf("Test 2 IPV4\n");

            somme =0; // Nous sert à "localiser" leqs noeuds
            m=0;

            int a = 0;

            while (j < NombreFils - 1) {

                Noeud *petit = &noeud->fils[j]; // A l'aide du noeud petit on va créer les noeuds H16 puis digit 

                petit->tag = "H16";
                petit->fils = malloc(tab1[a]*sizeof(Noeud));
                petit->valeur = requete + indice + somme; //ETNBZ
                petit->longueur = tab1[a];
                petit->nombreFils = tab1[a];

                m=0;

                while (m < tab1[a]) {
                    createFilsSimple("Hexdig", requete + indice + somme + m, 1, &petit->fils[m]);
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
            // free (noeud);
            *i=indice;
            return false;
        }
    } else {  // Maintenant le plan c'est de compter les H16 de l'autre coté des ":" pour comparer leur nombre à ceux d'avant et de déterminer le cas où l'on se trouve
        bool pointsConsecutifs = false; // On vérifie qu'on a pas trop de : qui se suivent
        while (interrupteur) { // Cette boucle while va nous permettre de compter le nombre de H16 ou de (H16 ":") après les "::"
            printf("\nLine %d => i : %d\n",__LINE__, *i);
            Compteur = CompteurHexdig(requete, i);
            printf("Compteur : %d\n", Compteur);
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
                    printf("CompteurH16_bis : %d\n", CompteurH16_bis);
                    break;
                
                default:
                interrupteur=false;
                break;
            }

            if (CompteurH16_bis < 0 || CompteurH16_bis > 7) {
                // free (noeud);
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
                        if (requete[*i + Compteur]=='.') { //Ce qu'on a pris pour un H16 était en réalité un dec-octet, on corrige notre erreur
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

    printf("i => %d\n", *i);

    //TODO Remove this line
    // return true;

    printf("\nLine %d => CompteurH16 : %d\n\n", __LINE__, CompteurH16);

    printf("Test 5\n");

    // OK alors maintenant l'idée c'est de vérifier si après le dernier h16 on a un ":" ou non.
    // Si il n'y a pas de ":" alors on est dans le cas 1 de ls32 (ou dans le cas où il n'y a que 1 H16)
    // Si il y a un ":" alors on est avant le ls32 (ou dans le dernier cas)

    printf("i : %d\n", *i);
    printf("CompteurH16 : %d\n", CompteurH16);
    printf("CompteurH16bis : %d\n", CompteurH16_bis);

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

    printf("CompteurH16_bis : %d\n", CompteurH16_bis);

    switch (CompteurH16_bis) { // Dans ce switch on va "exclure" tous les cas faux, cad les cas ou il ya plus de 7 h16 a gauche sans ls32 à droite
        case 0:
            // if (((CompteurH16<1 || CompteurH16 >7) && ls32 == 0) || ((CompteurH16<1 || CompteurH16 >5) && ls32 != 0) ) { // (!ls32 est égal à ls32==False non ?)
            if ((CompteurH16 > 7 && ls32 == 0) || (CompteurH16 > 5 && ls32 != 0) ) {
                // free (noeud);
                *i=indice;
                return false;
                break;
            }
            else {
                break;
            }
        case 1: 
            // if (((CompteurH16<1 || CompteurH16 >6) && ls32 == 0) || ((CompteurH16<1 || CompteurH16 >4) && ls32 != 0) ) {
            if ((CompteurH16 > 6 && ls32 == 0) || (CompteurH16 >4 && ls32 != 0) ) {
                // free (noeud);
                *i=indice;
                return false;
            }
            break;
        case 2:
            // if (((CompteurH16<1 || CompteurH16 >5) && ls32 != 0) || ls32 == 0){
            if ((CompteurH16 > 6 && ls32 != 0) || ls32 == 0) {
                // free (noeud);
                *i=indice;
                return false;
            }
            break;
        case 3:
            // if (((CompteurH16<1 || CompteurH16 >4) && ls32 != 0) || ls32 == 0) {
            if ((CompteurH16 > 4 && ls32 != 0) || ls32 == 0) {
                // free (noeud);
                *i=indice;
                return false;
            }
            break;
        case 4:
            // if (((CompteurH16<1 || CompteurH16 >3) && ls32 != 0) || ls32 == 0) {
            if ((CompteurH16 > 3 && ls32 != 0) || ls32 == 0) {
                // free (noeud);
                *i=indice;
                return false;
            }
            break;
        case 5:
            // if (((CompteurH16<1 || CompteurH16 >2) && ls32) || !ls32) {
            if ((CompteurH16 > 2 && ls32 != 0) || ls32 == 0) {
                // free (noeud);
                *i=indice;
                return false;
            }
            break;
        case 6:
            if (((CompteurH16!=1) && ls32 != 0) || ls32 == 0){
                // free (noeud);
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


    printf("CompteurH16 23 : %d\n", CompteurH16);
    printf("CompteurH16_bis 23 : %d\n", CompteurH16_bis);

    // NombreFils += 1+2*(CompteurH16 + CompteurH16_bis); // Pour chaque H16 1 ":" + le 2e ":" du milieu de la fonction
    // Il faut prendre en compte le cas où il n'y a pas de h16 à gauche
    if (CompteurH16 <= 0) {
        NombreFils += 2 + 2 * CompteurH16_bis;
        // if (ls32 == 1) {
        //     NombreFils += 2;
        // }
    } else {
        NombreFils += 1 + 2 * (CompteurH16 + CompteurH16_bis);
    }

    if (CompteurH16_bis == 1 && ls32 == 0) {
        NombreFils--; // On a compté 1 ":" de trop
    }

    noeud->fils = malloc(NombreFils*sizeof(Noeud));
    noeud->longueur = *i - indice;
    noeud->nombreFils = NombreFils;
    printf("NombreFils : %d\n", NombreFils);

    j=0;
    // k=0;
    *i=indice;

    printf("i => %d\n", *i);

    somme =0; // Nous sert à "localiser" leqs noeuds
    m=0;
    int a = 0;

    while (j < 2 * CompteurH16) {

        Noeud *petit1 = &noeud->fils[j]; // A l'aide du noeud petit on va créer les noeuds H16 puis Hexdig

        petit1->tag = "H16";
        petit1->fils = malloc(tab1[a]*sizeof(Noeud));
        petit1->valeur = requete + indice + somme; //ETNBZ
        petit1->longueur = tab1[a];
        petit1->nombreFils = tab1[a];
        
        m=0;

        while (m < tab1[a]){
            createFilsSimple("Hexdig", requete + indice + somme + m , 1, &petit1->fils[m]);
            m++;
            (*i)++;
        }
        j++;
        somme= somme + tab1[a]; // Ne pas oublier de compter le ":"
        createFilsSimple("case_insensitive_string", requete + indice + somme, 1, &noeud->fils[j]);
        (*i)++;
        somme++; // Ne pas oublier de compter le ":"
        j++;
        // k++;
        a++;
    }

    // printArbre(noeud, 0);
    // printf("\n\n\n");

    createFilsSimple("case_insensitive_string", requete + *i, 1, &noeud->fils[j]); // On fait le noeud pour les "::" , mais le premier a déja été fait juste &vant
    (*i)++;
    j++;

    if (CompteurH16 <= 0) {
        createFilsSimple("case_insensitive_string", requete + *i, 1, &noeud->fils[j]);
        (*i)++;
        j++;
        somme++;
    }

    // m++;

    // j=0;
    // k=0;
    a = 0;
    // int numberOfChildsUntilNow = j;

    somme++;

    int number = 0;
    if (ls32 == 0) {
        number = NombreFils;
    } else if (ls32 == 1) {
        number = NombreFils - 3;
    } else {
        number = NombreFils - 1;
    }

    printf("number => %d\n", number);

    // while (j < 2 * CompteurH16_bis + numberOfChildsUntilNow) {
    while (j < number) {

        Noeud *petit1 = &noeud->fils[j]; // A l'aide du noeud petit on va créer les noeuds H16 puis Hexdig

        petit1->tag = "H16";
        petit1->fils = malloc(tab2[a]*sizeof(Noeud));
        petit1->valeur = requete + indice + somme; //ETNBZ
        petit1->longueur = tab2[a];
        petit1->nombreFils = tab2[a];
        
        m=0;

        while (m < tab2[a]) {
            createFilsSimple("Hexdig", requete + indice + somme + m , 1, &petit1->fils[m]);
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
        // j++;
        k++;
        a++;

    }

    // numberOfChildsUntilNow = j;

    // printArbre(noeud, 0);
    // printf("\n\n");

    //Maintenant on s'occupe de ls32
    if (ls32 != 0) {

        if (ls32 == 2) {
            printf("j : %d\n", j);
            checkIPV4(requete, i, &noeud->fils[j], true);
        } else {
            indice2 = *i;

            Noeud *petit2 = &noeud->fils[j];
            petit2->tag = "H16";
            petit2->fils = malloc(tab2[CompteurH16_bis] * sizeof(Noeud));
            petit2->valeur = requete + indice + somme; //ETNBZ
            petit2->longueur = tab2[CompteurH16_bis];
            petit2->nombreFils = tab2[CompteurH16_bis];

            m = 0;

            while (m < tab2[CompteurH16_bis]) {
                createFilsSimple("Hexdig", requete + indice + somme + m, 1, &petit2->fils[m]);
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
            petit2->tag = "H16";
            petit2->fils = malloc(tab2[CompteurH16_bis + 1] * sizeof(Noeud));
            petit2->valeur = requete + indice + somme; //ETNBZ
            petit2->longueur = tab2[CompteurH16_bis + 1];
            petit2->nombreFils = tab2[CompteurH16_bis + 1];

            m = 0;

            while (m < tab2[CompteurH16_bis + 1]) {
                createFilsSimple("Hexdig", requete + indice + somme + m, 1, &petit2->fils[m]);
                m++;
                (*i)++;
            }
        }

    }

    return true;

}

bool checkIPliteral(char requete[], int *i, Noeud *noeud) {

    printf("IPLITERAL i = %d\n", *i);

    const int indice = *i; // On conserve l'indice de début
    bool ipv6 = false;

    if (requete[*i] == '[') { // Code ascii pour [
        (*i)++;
    } else {
        return false;
    }

    printf("Line => %d \t i => %d\n", __LINE__, *i);

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
    // (*i)++;

    printf("Line => %d \t i => %d\n", __LINE__, *i);

    if (requete[*i] == ']') { // Code ascii pour ]
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
    // (*i)++;

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
            // free(noeud);
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
        if(requete[*i] != '.'){  // 46 est le code ascii du point "."
            // free(noeud);
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
        // free(noeud);
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
        // int suppr=0;
        // while (suppr<nombreFils){
        //     free(&noeud->fils[j]);
        //     suppr++;
        // }
        free(noeud->fils);
        // free(noeud);
        
        *i=indice;
    }

    return true;

}

bool checkREGNAME(char requete[], int *i, Noeud *noeud) {
    const int indice = *i; // On conserve l'indice de début
    int nombreFils=0;

    noeud->valeur = requete + indice;
    noeud->tag = "reg-name";

    printf("i => %d\n", *i);

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

    printf("i => %d\n", *i);

    // Cette boucle while ne sert qu'a déterminer le nombre de fils que regname possède
    // while (checkUnreserved(requete, *i) || checkSubDelims(requete, *i) || checkPctEncoded(requete, *i)) {
    //     nombreFils++;
    //     (*i)++;
    // }

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
            printf("GROS BUG DANS LA FONCTION checkREGNAME");
        }
        j++;
        (*i)++;
    }

    return true;
}

bool checkHost(char requete[], int *i, int longueur, Noeud *noeud){

    printf("i 2 => %d\n", *i);

    const int indice = *i; // On conserve l'indice de début
    int nombreFils = 0;

    // noeud->fils = malloc(nombreFils * sizeof(Noeud));
    //noeud->longueur = *i - indice;
    noeud->valeur = requete + indice;
    // noeud->nombreFils = nombreFils ;
    noeud->tag = "Host";

    Noeud *tempUriHost = malloc(sizeof(Noeud));
    printf("i 2 => %d\n", *i);
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

    // if (checkIPliteral(requete, i, &noeud->fils[0])){
    //     noeud->longueur = *i - indice;
    //     return true;

    // }
    // else if (checkIPV4(requete, i, &noeud->fils[0], true)){ //pas sur du true là, ya surement un moyen plus efficace de faire cette histoire là
    //     noeud->longueur = *i - indice;
    //     return true;

    // }
    // else if(checkREGNAME(requete, i, &noeud->fils[0])){
    //     noeud->longueur = *i - indice;
    //     return true;

    // }

    free(tempUriHost);
    free(tempPort);

    return true;

}

bool checkUriHost(char requete[], int *i, int longueur, Noeud *noeud) {
    const int indice = *i; // On conserve l'indice de début
    int nombreFils=1;

    noeud->fils = malloc(nombreFils * sizeof(Noeud));
    //noeud->longueur = *i - indice;
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
    
    // else if (checkIPV4(requete, i, &noeud->fils[0], true)) { //pas sur du true là, ya surement un moyen plus efficace de faire cette histoire là
    //     noeud->longueur = *i - indice;
    //     return true;

    // } else if(checkREGNAME(requete, i, &noeud->fils[0])) {
    //     noeud->longueur = *i - indice;
    //     return true;

    // }

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

    if (!checkExpect(requete, i, &noeud->fils[j])) {
        // for (int k = 0; k < nombreFils; k ++) {
		// 	if (k != j) {
		// 		free(&noeud->fils[k]);
		// 	}
		// }

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

    noeud->longueur = *i - indice;

    return true;
}

bool checkHostHeader(char requete[], int *i, int longueur, Noeud *noeud) {
    int nombreFils=5;
    const int indice= *i;

    noeud->fils = malloc(nombreFils * sizeof(Noeud));
    noeud->valeur = requete + indice;
    // On ne connaît pas encore noeud->longueur
    noeud->nombreFils = nombreFils;
    noeud->tag = "Host-header";

    int j = 0; // On utilise cet indice pour compter les fils

    if (!checkHostString(requete, i, &noeud->fils[j])) {
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

    printf("i 1 => %d\n", *i);

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
        j++;
    }

    checkOWS(requete, i, longueur, &noeud->fils[j]);
    j++;

    printf("i 1 => %d\n", *i);

    if (!checkHost(requete, i, longueur, &noeud->fils[j])) {
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
