#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "structure.h"
#include "tom.h"
#include "affichage.h"


//==============================Fonctions générales==============================

int main(){
    printf("Ce main ne sert qu'a voir si le programme compile et s'execute sans crasher et faire exploser mon ordinateur, merci de l'ignorer");
    return(1);
}

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
        free(noeud);
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
        free(noeud);
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
        free(noeud);
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
    int tailleIP=0;
    while (checkDigit(requete, indice + tailleIP)){
        tailleIP++;
    }

    if (tailleIP == 0){
        free (noeud);
        return false;
    }
    // pas oubleir de (*i)++;
    else if (tailleIP >= 3){
        if (requete[indice]== '2'){
            if(requete[indice + 1 ]=='5'){
                if (requete[indice + 2] >= '0' && requete[indice + 2] <= '5'){

                    if (stocker){

                        NombreFils =3;
                        noeud->fils = malloc(NombreFils*sizeof(Noeud)) ;
                        noeud->longueur = *i - indice;
                        noeud->nombreFils = NombreFils ;
                        createFilsSimple("Digit", requete + *i, 1, &noeud->fils[*i]);
                        (*i)++;
                        createFilsSimple("Digit", requete + *i, 1, &noeud->fils[*i]);
                        (*i)++;
                        createFilsSimple("Digit", requete + *i, 1, &noeud->fils[*i]);
                        (*i)++;
                        return true;
                     }
                     else{
                        free(noeud);
                        return true;
                     }
                }
                else{
                    free (noeud);
                    return false;
                }

            }

            else if (requete[indice + 1] >= 0 && requete[indice + 1] >= 4){
                if (checkDigit(requete, indice +2 )){

                    if (stocker){
                        NombreFils =3;
                        noeud->fils = malloc(NombreFils*sizeof(Noeud)) ;
                        noeud->longueur = *i - indice;
                        noeud->nombreFils = NombreFils ;
                        createFilsSimple("Digit", requete + *i, 1, &noeud->fils[*i]);
                        (*i)++;
                        createFilsSimple("Digit", requete + *i, 1, &noeud->fils[*i]);
                        (*i)++;
                        createFilsSimple("Digit", requete + *i, 1, &noeud->fils[*i]);
                        (*i)++;

                        return true;
                    }
                     else{
                        free(noeud);
                        return true;
                     }

                }
                else{
                    free(noeud);
                    return false;
                }

            }
            else {
                free(noeud);
                return false;
            }
        }
        else if (requete[indice]==1){ 

            if (stocker){
                NombreFils =3;
                noeud->fils = malloc(NombreFils*sizeof(Noeud)) ;
                noeud->longueur = *i - indice;
                noeud->nombreFils = NombreFils ;
                createFilsSimple("Digit", requete + *i, 1, &noeud->fils[*i]);
                (*i)++;
                createFilsSimple("Digit", requete + *i, 1, &noeud->fils[*i]);
                (*i)++;
                createFilsSimple("Digit", requete + *i, 1, &noeud->fils[*i]);
                (*i)++;

                return true;
            }
            else{
                    free(noeud);
                    return true;
             }

        }
        else {
            free (noeud);
            return false;
        }
    }

    else if (tailleIP == 2){
        if (requete[indice] != '0'){

            if (stocker){
                 NombreFils =2;
                noeud->fils = malloc(NombreFils*sizeof(Noeud)) ;
                noeud->longueur = *i - indice;
                noeud->nombreFils = NombreFils ;
                createFilsSimple("Digit", requete + *i, 1, &noeud->fils[*i]);
                (*i)++;
                createFilsSimple("Digit", requete + *i, 1, &noeud->fils[*i]);
                (*i)++;

                return true;
            }
            else{
            free(noeud);
            return true;
            }
        }
        else{
            free (noeud);
            return false;
        }
    }

    else if (tailleIP == 1){
        if (stocker){
            NombreFils =1;
            noeud->fils = malloc(NombreFils*sizeof(Noeud)) ;
            noeud->longueur = *i - indice;
            noeud->nombreFils = NombreFils ;
            createFilsSimple("Digit", requete + *i, 1, &noeud->fils[*i]);
            (*i)++;

            return true;
        }
        else{
        free(noeud);
        return true;
        }

    }

    else {
        printf("Si ceci s'affiche : Gros Probleme dans la fonction checkDecoctet");
        free (noeud);
        return false;
    }
}

bool checkIPfuture(char requete[], int *i, Noeud *noeud){
    const int indice = *i; // On conserve l'indice de début
    int NombreFils=0;
   // noeud->fils = malloc(NombreFils*sizeof(Noeud));
    noeud->valeur = requete + indice;
   // noeud->longueur = *i - indice;
   // noeud->nombreFils = NombreFils;
    noeud->tag = "IPvFuture";


    if (requete[*i] != 118){ // Code ascii pour "v"
        free(noeud);
        *i=indice;
        return false;
    }   
    else{
        NombreFils++;
        (*i)++;
    }
    if (!checkHexdig(requete, *i)){
        free (noeud);
        *i=indice;
        return false;
    }
    else{
        NombreFils++;
        (*i)++;
    }
    while (checkHexdig(requete, *i)){
        NombreFils++;
        (*i)++;  
    }

    if (requete[*i] != 46){ // 46 est le code ascii du point "."
        free(noeud);
        *i=indice;
        return false;
    }
    else{
        NombreFils++;
        (*i)++;  
    }

    if(!checkUnreserved(requete, *i) || !checkSubDelims(requete, *i) || requete[*i] != 58 ){ // 58 est le code ascii de ":"
        free(noeud);
        *i=indice;
        return false;
    }
    else{
        NombreFils++;
        (*i)++;  
    }

    while (!checkUnreserved(requete, *i) || !checkSubDelims(requete, *i) || requete[*i] != 58 ){
        NombreFils++;
        (*i)++;  
    }

    // On a compté tous les noeuds, on peut donc les créer puis les remplir
    noeud->fils = malloc(NombreFils*sizeof(Noeud));
    noeud->longueur = *i - indice;
    noeud->nombreFils = NombreFils;
    *i=indice;

    int *j=0; //Compteur de fils
    createFilsSimple("case_insensitive_string", requete + *i, 1, &noeud->fils[*j]);
    (*i)++;
    (*j)++;

    while (checkHexdig(requete, *i)){
        createFilsSimple("HEXDIG", requete + *i, 1, &noeud->fils[0]);
        (*i)++; 
        (*j)++; 
    }
    Noeud *petit= &noeud->fils[*j]; // Ce noeud va nous permettre de créer les plus petits Noeuds Alpha, Digit et Hexdig facilemment si besoin

    while (checkUnreserved(requete, *i) || checkSubDelims(requete, *i) || requete[*i] == 58) {
        if(checkUnreserved(requete, *i)){
            if (checkAlpha(requete, *i)){
                petit->tag = "unreserved";
                petit->fils = malloc(sizeof(Noeud));
                petit->valeur = requete + *i;
                petit->longueur = 1;
                petit->nombreFils = 1;
                createFilsSimple("Alpha", requete + *i, 1, &petit->fils[0]);
            }
            else if (checkDigit(requete, *i)){
                petit->tag = "unreserved";
                petit->fils = malloc(sizeof(Noeud));
                petit->valeur = requete + *i;
                petit->longueur = 1;
                petit->nombreFils = 1;
                createFilsSimple("Digit", requete + *i, 1, &petit->fils[0]);
            }
            else {
                petit->tag = "unreserved";
                petit->fils = malloc(sizeof(Noeud));
                petit->valeur = requete + *i;
                petit->longueur = 1;
                petit->nombreFils = 1;
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
        (*j)++;
        (*i)++;
    }

    createFilsSimple("case_insensitive_string", requete + *i, 1, &noeud->fils[*j]);

    (*j)++;
    (*i)++;

    return true;

}

int CompteurDigit(char requete[], int *i){
     int indice= *i;
     int CompteurDigit=0;

    while (checkDigit(requete, indice)){
        CompteurDigit++;
        indice++;
    }

    return CompteurDigit;
}

bool checkIPV6(char requete[], int *i, Noeud *noeud){
    // Ne pas oublier de faire *i=indice; avant chaque return false pour ne pas créer de problèmes lors du 2e if de checkIPliteral

    //Potentiellement faire un tableau des tailles de H16
    
    const int indice = *i; // On conserve l'indice de début
    int indice2;

    int NombreFils = 0;
    int *j=0;            // J nous servira plus tard pour créer les différents noeuds
    Noeud *petit; // A l'aide du noeud petit on va créer les noeuds H16 puis digit
    int somme ; // Nous sert à "localiser" les noeuds
    int *m;

    int *k=0; //k nous sert à accéder au tableau du compteur de digit des h16

    int CompteurH16=0;
    int Compteur=0;
    int tab1[9]; //Ce tableau nous sert à garder en mémoire la taille de chaque H16

    int tab2[8]; //Ce tableau fais la meme chose que le premier mais pour les H16 après "::"
    int CompteurH16_bis=0; //Idem
    bool ls32=false; // Ce boolean nous sera utile pour verifier si on se trouve dans le ls32 ou non

    bool interrupteur= true;
    bool cas_1 = true;

    Noeud * test; // Ce noeud nous servira juste à tester si ipv6 continet une @ ipv4
    
    //noeud->fils = malloc(NombreFils*sizeof(Noeud));
    noeud->valeur = requete + indice;
    //noeud->longueur = ;
    //noeud->nombreFils = ;
    noeud->tag = "IPv6address";

    while (interrupteur){ // Cette boucle while va nous permettre de compter le nombre de H16 ou de (H16 ":") avant les "::"
        Compteur=CompteurDigit(requete,*i);
        switch (Compteur) {
            case 0:
                break;
            case 1:
            case 2:
            case 3:
            case 4:
                if (CompteurH16 <= 8){
                    tab1[CompteurH16]=Compteur;
                }
                CompteurH16++;
                break;
            
            default:
            if (cas_1){
                CompteurH16=-1;
            }
            else{
                interrupteur=false;
            }
            break;
        }

        if (CompteurH16 < 0 || CompteurH16 > 8){
            free (noeud);
            *i=indice;
            return false;
        }


        else {
            if (requete[*i + Compteur]==":"){
                if (CompteurH16==6){  //A partir de H16 = 6 on peut soit être dans le cas 1 ou les deux derniers, on active donc une option nous permettant de ne plus tout jeter si ce qui vient après n'est pas H16 ou ":" "::"
                    cas_1=false;     
                }
                else if(requete[*i + Compteur + 1]==":"){
                    interrupteur=false;
                }
                else{
                    *i=*i+Compteur + 1;
                }
            }

            else{
                if (CompteurH16!=8 || requete[*i + Compteur]!="."){ //Si Il y a 8 H16 alors on est dans le cas 1 avec 2 H16 dans ls32, donc c'est valide de ne pas avoir de ":" après le 8e H16
                free(noeud);
                *i=indice;
                return false;
                }

                else{
                    if (requete[*i + Compteur]=="."){ //Ce qu'on a pris pour un H16 était en réalité un dec-octet, on corrigera notre erreur plus tard
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

    if (CompteurH16==8){ // C'est le cas 1.1 où ls32 = 2 H16, on corrige donc notre erreur
        NombreFils= 8 + 7;
        noeud->fils = malloc(NombreFils*sizeof(Noeud));
        noeud->longueur = *i - indice;
        noeud->nombreFils = NombreFils ;

        Noeud *petit = &noeud->fils[*j]; // A l'aide du noeud petit on va créer les noeuds H16 puis digit
        somme =0; // Nous sert à "localiser" leqs noeuds
        *m=0;

        while (*j < NombreFils){

            petit->tag = "H16";
            petit->fils = malloc(tab1[*j]*sizeof(Noeud));
            petit->valeur = requete + indice + somme; //ETNBZ
            petit->longueur = tab1[*j];
            petit->nombreFils = tab1[*j];
            *m=0;
            while (*m < tab1[*j]){
                createFilsSimple("Digit", requete + indice + somme + *m , 1, &petit->fils[*m]);
                (*m)++;
            }
            somme= somme + tab1[*j] + 1; // Ne pas oublier de compter le ":"
            (*j)++;

        }
        return true;
    }

    else if(CompteurH16==6 && requete [*i]!=":" &&requete[*i + 1] != ":"){ //On traite le cas 1.2
        NombreFils= 6 + 6; // 6 H16 + 5 :  + 1 addresseipv4
        noeud->fils = malloc(NombreFils*sizeof(Noeud));
        noeud->longueur = *i - indice;
        noeud->nombreFils = NombreFils ;

        if (checkIPV4(requete,*i, &noeud->fils[15], true)){

            Noeud *petit = &noeud->fils[*j]; // A l'aide du noeud petit on va créer les noeuds H16 puis digit 
            somme =0; // Nous sert à "localiser" leqs noeuds
            *m=0;

            while (*j < NombreFils - 1){

                petit->tag = "H16";
                petit->fils = malloc(tab1[*j - *k]*sizeof(Noeud));
                petit->valeur = requete + indice + somme; //ETNBZ
                petit->longueur = tab1[*j];
                petit->nombreFils = tab1[*j];
                *m=0;
                while (*m < tab1[*j]){
                    createFilsSimple("Digit", requete + indice + somme + *m, 1, &petit->fils[*m]);
                    (*m)++;
                }
                (*j)++;
                createFilsSimple("Incensitive case string", requete + indice + somme + *m + 1, 1, &noeud->fils[*j]);
                somme= somme + tab1[*j] + 1; // Ne pas oublier de compter le ":", c'est pour ça que le +1 est là !
                (*j)++;
                (*k)++;

            }
            return true;
        }

        else{
            free (noeud);
            *i=indice;
            return false;
        }
    }

    else {  // Maintenant le plan c'est de compter les H16 de l'autre coté des ":" pour comparer leur nombre à ceux d'avant et de déterminer le cas où l'on se trouve
        while (interrupteur){ // Cette boucle while va nous permettre de compter le nombre de H16 ou de (H16 ":") après les "::"
            Compteur=CompteurDigit(requete,*i);
            switch (Compteur) {
                case 0:
                    break;
                case 1:
                case 2:
                case 3:
                case 4:
                    if (CompteurH16_bis <= 8){
                        tab2[CompteurH16_bis]=Compteur;
                    }
                    CompteurH16_bis++;
                    break;
                
                default:
                interrupteur=false;
                break;
            }

            if (CompteurH16_bis < 0 || CompteurH16_bis > 7){
                free (noeud);
                *i=indice;
                return false;
            }


            else {
                if (requete[*i + Compteur]==":"){
                    *i=*i+Compteur + 1;
                }

                else{
                    if (requete[*i + Compteur]!="."){ //Si il n'y a pas de "." ou de ":" alors on arrête de compter sans savoir ce qui se trouve après (on regardera plus tard)
                    *i=*i+Compteur;
                    interrupteur= false;
                    }

                    else{
                        if (requete[*i + Compteur]=="."){ //Ce qu'on a pris pour un H16 était en réalité un dec-octet, on corrige notre erreur
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

    }

    // OK alors maintenant l'idée c'est de vérifier si après le dernier h16 on a un ":" ou non.
    // Si il n'y a pas de ":" alors on est dans le cas 1 de ls32 (ou dans le cas où il n'y a que 1 H16)
    // Si il y a un ":" alors on est avant le ls32 (ou dans le dernier cas)

    if (requete[*i ] != ":" && CompteurH16_bis > 1){  // Vérifier IPV4 pour ls32 : Créer une option dans la fonction ipv4 pour pouvoir ne pas créer de noeud et ensuite l'utiliser pour check si c'est une adresse ipv4, plus haut et dans une autre fiction
        ls32=true;
        CompteurH16_bis--;
        CompteurH16_bis--;
    }

    else if(checkIPV4(requete,*i, test, false)){
        ls32=true;
        free (test);
    }
    // On a donc dans compteurH16_bis le nombre exact de H16 hors ls32

    switch (CompteurH16_bis) { // Dans ce switch on va "exclure" tous les cas faux, cad les cas ou il ya plus de 7 h16 a gauche sans ls32 à droite
    case 0:
        if (((CompteurH16<1 || CompteurH16 >7) && !ls32) ||((CompteurH16<1 || CompteurH16 >5) && ls32) ){ // (!ls32 est égal à ls32==False non ?) 
            free (noeud);
            *i=indice;
            return false;
            break;
        }
        else{
            break;
        }
    case 1: 
        if (((CompteurH16<1 || CompteurH16 >6) && !ls32) ||((CompteurH16<1 || CompteurH16 >4) && ls32) ){
            free (noeud);
            *i=indice;
            return false;
            break;
            }
    case 2:
        if (((CompteurH16<1 || CompteurH16 >5) && ls32) || !ls32){
            free (noeud);
            *i=indice;
            return false;
            break;
        }
    case 3:
        if (((CompteurH16<1 || CompteurH16 >4) && ls32) || !ls32) {
            free (noeud);
            *i=indice;
            return false;
            break;
            }
    case 4:
        if (((CompteurH16<1 || CompteurH16 >3) && ls32) || !ls32){
            free (noeud);
            *i=indice;
            return false;
            break;
            }
    case 5:
        if (((CompteurH16<1 || CompteurH16 >2) && ls32) || !ls32){
            free (noeud);
            *i=indice;
            return false;
            break;
            }
    case 6:
        if (((CompteurH16!=1) && ls32) || !ls32){
            free (noeud);
            *i=indice;
            return false;
            break;
            }
        
    
    default:
    interrupteur=false;
    break;
    }
    // Si on est arrivé jusqu'ici ça veut dire que tout va bien et que l'on peut ENFIN STOCKER HAHA PRENDS CA IPV6 FONCTION DE MERDE
    NombreFils=0;
    if (ls32){
        NombreFils++;
    }


    NombreFils= 1+2*(CompteurH16 + CompteurH16_bis); // Pour chaque H16 1 ":" + le 2e ":" du milieu de la fonction
    if (CompteurH16_bis == 1){
        NombreFils--; // On a compté 1 ":" de trop
    }
    noeud->fils = malloc(NombreFils*sizeof(Noeud));
    noeud->longueur = *i - indice;
    noeud->nombreFils = NombreFils ;

    *j=0;
    *k=0;
    *i=indice;

    Noeud *petit1 = &noeud->fils[*j]; // A l'aide du noeud petit on va créer les noeuds H16 puis digit
    somme =0; // Nous sert à "localiser" leqs noeuds
    *m=0;

    while (*j < 2*CompteurH16){

        petit1->tag = "H16";
        petit1->fils = malloc(tab1[*j- *k]*sizeof(Noeud));
        petit1->valeur = requete + indice + somme; //ETNBZ
        petit1->longueur = tab1[*j];
        petit1->nombreFils = tab1[*j];
        *m=0;
        while (*m < tab1[*j]){
            createFilsSimple("Digit", requete + indice + somme + *m , 1, &petit1->fils[*m]);
            (*m)++;
            (*i)++;
        }
        (*j++);
        createFilsSimple("Incensitive case string", requete + indice + somme + *m + 1, 1, &noeud->fils[*j]);
        somme= somme + tab1[*j] + 1; // Ne pas oublier de compter le ":"
        (*j)++;
        (*k)++;

    }

    createFilsSimple("Incensitive case string", requete + *i, 1, &petit1->fils[*m]); // On fait le noeud pour les "::" , mais le premier a déja été fait juste &vant
    (*i)++;
    (*m)++;

    (*j)=0;
    (*k)=0;

    somme=somme+1;

    while (*j < 2*CompteurH16_bis){

        petit1->tag = "H16";
        petit1->fils = malloc(tab2[*j - *k]*sizeof(Noeud));
        petit1->valeur = requete + indice + somme; //ETNBZ
        petit1->longueur = tab2[*j];
        petit1->nombreFils = tab2[*j];
        while (*m < tab1[*j]){
            createFilsSimple("Digit", requete + indice + somme + *m , 1, &petit1->fils[*m]);
            (*m)++;
            (*i)++;
        }
        if ((CompteurH16_bis) !=1){
            (*j)++;
            createFilsSimple("Incensitive case string", requete + indice + somme + *m + 1, 1, &noeud->fils[*j]);
        }
        else{
            (*j)++;
        }
        somme= somme + tab2[*j] + 1; // Ne pas oublier de compter le ":"
        (*j)++;
        (*k)++;

    }

    //Maintenant on s'occupe de ls32
    if (ls32){
        Noeud *petit2 = &petit1->fils[*m];
        petit2->tag = "LS32";
        petit2->fils = malloc(3*sizeof(Noeud));
        petit2->valeur = requete + *i; 
        //petit1->longueur = 1;
        //petit1->nombreFils = tab2[*j];
        if (checkIPV4(requete,*i,&petit2->fils[0] , false)){
            free(&petit2->fils[1]);
            free(&petit2->fils[2]);
            petit2->longueur = 1;
            petit2->nombreFils = 1;
            checkIPV4(requete,*i,&petit2->fils[0] , true);
        }
        else{
            indice2= *i;

            (*j)=0;
            while (*j < 3){

                petit2->tag = "H16";
                petit2->fils = malloc(tab2[CompteurH16_bis + *j - *k]*sizeof(Noeud));
                petit2->valeur = requete + indice + somme; //ETNBZ
                petit2->longueur = tab1[CompteurH16_bis + *j - *k];
                petit2->nombreFils = tab1[CompteurH16_bis + *j];
                while (*m < tab1[*j]){
                    createFilsSimple("Digit", requete + indice + somme + *m, 1, &petit2->fils[*m]);
                    (*m)++;
                    (*i)++;
                }
                    if (*j ==0){
                        (*j)++;
                        createFilsSimple("Incensitive case string", requete + indice + somme + *m + 1, 1, &petit1->fils[*j]);
                    }
                    else{
                        (*j)++;
                    }
                somme= somme + tab2[*j] + 1; // Ne pas oublier de compter le ":"
                (*j)++;
                (*k)++;

            }

            petit2->longueur = *i - indice2;
            petit2->nombreFils = 3;
        }

    }
    return true;

}

bool checkIPliteral(char requete[], int *i, Noeud *noeud){
    const int indice = *i; // On conserve l'indice de début
    noeud->fils = malloc(3*sizeof(Noeud));
    noeud->valeur = requete + indice;
    noeud->longueur = 3;
    noeud->nombreFils = 3;
    noeud->tag = "IP-literal";

    if (requete[indice]==91){ // Code ascii pour [
        createFilsSimple("case_insensitive_string", requete + indice, 1, &noeud->fils[0]);
        (*i)++;
    }
    else{
        free(noeud);
        return false;
    }


    if(!checkIPV6(requete, *i, &noeud->fils[1]) || !checkIPfuture(requete, *i, &noeud->fils[1])){
        free(noeud);
        return false;
    }
    (*i)++;

    if (requete[indice+2]==93){ // Code ascii pour ]
        createFilsSimple("case_insensitive_string", requete + indice+2, 1, &noeud->fils[2]);
        (*i)++;
    }
    else{
        free(noeud);
        return false;
    }
    return true;

}


bool checkIPV4(char requete[], int *i, Noeud *noeud, bool stocker){
    const int indice = *i; // On conserve l'indice de début
    int nombreFils=7;

    noeud->valeur = requete + indice;
    noeud->nombreFils = nombreFils;
    noeud->fils = malloc(nombreFils * sizeof(Noeud));
    noeud->tag = "IPv4address";

    int j=0; //compteur de Fils
    while (j < 6){
        if (!checkDecoctet(requete, *i, &noeud->fils[j], stocker)){
            free(noeud);
            return false;
        }
        j++;
        if(requete[*i] != 46){  // 46 est le code ascii du point "."
            free(noeud);
            return false;
        }
        createFilsSimple("case_insensitive_string", requete + *i, 1, &noeud->fils[j]);
        (*i)++;
        j++;
    }
    if (!checkDecoctet(requete, *i, &noeud->fils[j], stocker)){
        free(noeud);
        return false;
    }
    j++;
    noeud->longueur = *i - indice;
    if (!stocker){
        int suppr=0;
        while (suppr<nombreFils){
            free(&noeud->fils[j]);
            suppr++;
        }
        free(noeud);
        
        *i=indice;
    }
    return true;

}

bool checkREGNAME(char requete[], int *i, Noeud *noeud){
    const int indice = *i; // On conserve l'indice de début
    int nombreFils=0;

    noeud->valeur = requete + indice;
    noeud->tag = "reg-name";


    // Cette boucle while ne sert qu'a déterminer le nombre de fils que regname possède
    while (checkUnreserved(requete, *i) || checkSubDelims(requete, *i) || checkPctEncoded(requete, *i)) {
        nombreFils++;
        (*i)++;
    }


    noeud->nombreFils = nombreFils ;
    noeud->fils = malloc(nombreFils * sizeof(Noeud));
    noeud->longueur = *i - indice;

    //On remet les comptes à 0 et on crée maintenant les noeuds
    *i=indice;
    int *j=0;

    Noeud *petit= &noeud->fils[*j]; // Ce noeud va nous permettre de créer les plus petits Noeuds Alpha, Digit et Hexdig facilemment si besoin

    while (checkUnreserved(requete, *i) || checkSubDelims(requete, *i) || checkPctEncoded(requete, *i)) {
        if(checkUnreserved(requete, *i)){
            if (checkAlpha(requete, *i)){
                petit->tag = "unreserved";
                petit->fils = malloc(sizeof(Noeud));
                petit->valeur = requete + *i;
                petit->longueur = 1;
                petit->nombreFils = 1;
                createFilsSimple("Alpha", requete + *i, 1, &petit->fils[0]);
            }
            else if (checkDigit(requete, *i)){
                petit->tag = "unreserved";
                petit->fils = malloc(sizeof(Noeud));
                petit->valeur = requete + *i;
                petit->longueur = 1;
                petit->nombreFils = 1;
                createFilsSimple("Digit", requete + *i, 1, &petit->fils[0]);
            }
            else {
                petit->tag = "unreserved";
                petit->fils = malloc(sizeof(Noeud));
                petit->valeur = requete + *i;
                petit->longueur = 1;
                petit->nombreFils = 1;
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
            printf("GROS BUG DANS LA FONCTION checkREGNAME");
        }
        (*j)++;
        (*i)++;
    }

    return true;
}

bool checkHost(char requete[], int *i, int longueur, Noeud *noeud){
    const int indice = *i; // On conserve l'indice de début
    int nombreFils=1;

    noeud->fils = malloc(nombreFils * sizeof(Noeud));
    //noeud->longueur = *i - indice;
    noeud->valeur = requete + indice;
    noeud->nombreFils = nombreFils ;
    noeud->tag = "Host";


    if (checkIPliteral(requete, i, &noeud->fils[0])){
        noeud->longueur = *i - indice;
        return true;

    }
    else if (checkIPV4(requete, i, &noeud->fils[0], true)){ //pas sur du true là, ya surement un moyen plus efficace de faire cette histoire là
        noeud->longueur = *i - indice;
        return true;

    }
    else if(checkREGNAME(requete, i, &noeud->fils[0])){
        noeud->longueur = *i - indice;
        return true;

    }
    free(noeud);
    return false;

}


bool checkExpectHeader(char requete[], int *i, int longueur, Noeud *noeud) {
    int nombreFils=5;
    const int indice= *i;

    noeud->fils = malloc(nombreFils * sizeof(Noeud));
    noeud->valeur = requete + indice;
    // On ne connaît pas encore noeud->longueur
    noeud->nombreFils = nombreFils;
    noeud->tag = "Expect-header";

    int j = 0; // On utilise cet indice pour compter les fils

    if (!checkExpectString(requete, i, &noeud->fils[j])) {
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

    if (!checkExpect(requete, i, &noeud->fils[j])) {
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

    if (!checkHost(requete, i, longueur, &noeud->fils[j])) {
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

    return true;
}