#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "structure.h"
#include "functions.h"
#include "affichage.h"

bool checkVchar(char requete[], int *i, int longueur, Noeud *noeud){ // Si le noeud est NULL alors *i ne bouge pas

    if ((requete[*i] >=33) && (requete[*i]<=126)){
        if (noeud==NULL){
            return true;
        }

        else{
            createFilsSimple("VCHAR", requete + *i, 1, noeud);
            (*i)++;
            return true;
        }
    }

    else{
        if (noeud==NULL){
            return false;
        }

        else{
            free(noeud);
            return false;
        }
    }
}

bool checkObs_Text(char requete[], int *i, int longueur, Noeud *noeud){ // Si le noeud est NULL alors *i ne bouge pas

    if (((int)requete[*i] <0) && ((int)requete[*i]>=-128)){

        if (noeud==NULL){
            return true;
        }

        else{
            createFilsSimple("obs-text", requete + *i, 1, noeud);
            (*i)++;
            return true;
        }
    }

    else{
        if (noeud==NULL){
            return false;
        }

        else{
            free(noeud);
            return false;
        }
    }

}

bool checkField_Vchar(char requete[], int *i, int longueur, Noeud *noeud){ // Si le noeud est NULL alors *i ne bouge pas
    const int indice= *i;
    
    if ((checkVchar(requete, i, longueur, NULL)) || (checkObs_Text(requete, i, longueur, NULL))){
        if(noeud==NULL){
            return true;
        }

        else if(checkVchar(requete, i, longueur, NULL)){ // Si c'est un Field-Var on le stock
            noeud->valeur = requete + indice;
            noeud->tag = "field-vchar";
            noeud->nombreFils = 1;
            noeud->fils = malloc(1 * sizeof(Noeud));
            noeud->longueur=1;
            checkVchar(requete, i, longueur, &noeud->fils[0]);
            return true;
        }

        else{
            noeud->valeur = requete + indice; // Sinon on stock un OBS-text
            noeud->tag = "field-vchar";
            noeud->nombreFils = 1;
            noeud->fils = malloc(1 * sizeof(Noeud));
            noeud->longueur=1;
            checkObs_Text(requete, i, longueur, &noeud->fils[0]);
            return true;
        }

    }

    else{
        if (noeud==NULL){
            return false;
        }

        else{
            free(noeud);
            return false;
        }
    }

}

bool checkObs_Fold(char requete[], int *i, int longueur, Noeud *noeud){ // Si le noeud est NULL alors *i ne bouge pas
    const int indice= *i;
    int nombreFils=0;
    int compteur=0;
    //printf("Test2 \n");

    if (!checkCRLFBool(requete, longueur, *i)){

        if (noeud==NULL){
            //printf("Chakalal\n");
            return false;
        }

        else{
            free(noeud);
            //printf("Maaais noooon grooos\n");
            return false;
        }
    }

    //printf("Test1 \n");
    nombreFils++;
    (*i)=(*i)+2;

    if (((requete[*i]!=32)) && (requete[*i]!=9)){

        if (noeud==NULL){
            (*i)=indice;
            return false;
        }

        else{
            (*i)=indice;
            free(noeud);
            return false;
        }
    }

    nombreFils++;
    (*i)++;

    while(((requete[*i]==32)) || (requete[*i]==9)){
        nombreFils++;
        (*i)++;
    }

    if (noeud==NULL){
        (*i)=indice;
        return true;
    }
     // Maintenant on stock tout 
    noeud->valeur = requete + indice;
    noeud->tag = "obs-fold";
    noeud->nombreFils = nombreFils;
    noeud->fils = malloc(nombreFils * sizeof(Noeud));
    noeud->longueur=*i - indice;
    (*i)=indice;

    createFilsSimple("CRLF", requete + *i, 2, &noeud->fils[compteur]);
    compteur++;
    (*i)=(*i)+2;

    while(((requete[*i]==32)) || (requete[*i]==9)){
        if (requete[*i]==32){
            createFilsSimple("SP", requete + *i, 1, &noeud->fils[compteur]);
        }
        else{
            createFilsSimple("HTAB", requete + *i, 1, &noeud->fils[compteur]);
        }
        printf("%d \n", compteur);
        compteur++;
        (*i)++;
    }

    return true;

}

bool checkField_Content(char requete[], int *i, int longueur, Noeud *noeud){ // Si le noeud est NULL alors *i ne bouge pas
    const int indice= *i;
    int nombreFils=0;
    int compteur=0;

    int indice2;
    int nombreFils2;

    bool optionel=true;

    printf("Bonsoir2 \n");

    if (!checkField_Vchar(requete, i, longueur, NULL)){
        if (noeud==NULL){
            return false;
        }

        else{
            printf("Bonsoir \n");
            free(noeud); /* segFault fuite de mémoire*/
            return false;
        }
    }

    if (noeud==NULL){
        return true;
    }

    (*i)++;
    nombreFils++;

    indice2=*i;
    nombreFils2=nombreFils;

    if (((requete[*i]!=32)) && (requete[*i]!=9)){ // On teste pour voir si il y a des choses optionnel ou non
        optionel=false;
    }

    if (optionel==true){
        nombreFils++;
        (*i)++;
        while(((requete[*i]==32)) || (requete[*i]==9)){
            nombreFils++;
            (*i)++;
        }

        if (!checkField_Vchar(requete, i, longueur, NULL)){
            optionel=false;
        }
        nombreFils++;
        (*i)++;
    }

    if (optionel==false){//Indice2= *i+1 et NombreFils=1
        noeud->fils = malloc(nombreFils2 * sizeof(Noeud));
        noeud->valeur = requete + indice;
        noeud->nombreFils = nombreFils2;
        noeud->tag = "field-content";
        noeud->longueur = indice2 - indice;
        *i=indice;
        checkField_Vchar(requete, i, longueur, &noeud->fils[0]);
        return true;
    }

    else{
        noeud->fils = malloc(nombreFils * sizeof(Noeud));
        noeud->valeur = requete + indice;
        noeud->nombreFils = nombreFils;
        noeud->tag = "field-content";
        noeud->longueur = *i - indice;
        *i=indice;
        checkField_Vchar(requete, i, longueur, &noeud->fils[compteur]);
        compteur++;
        while(((requete[*i]==32)) || (requete[*i]==9)){
            if (requete[*i]==32){
                createFilsSimple("SP", requete + *i, 1, &noeud->fils[compteur]);
            }
            else{
                createFilsSimple("HTAB", requete + *i, 1, &noeud->fils[compteur]);
            }
            compteur++;
            (*i)++;
        }
        checkField_Vchar(requete, i, longueur, &noeud->fils[compteur]);
        (*i)++;
    }
    return true;
}

bool checkField_Value(char requete[], int *i, int longueur, Noeud *noeud){ //erreur  segmentation fault
    printf("Dudur \n");
    const int indice= *i;
    int nombreFils=0;

    int compteur=0; //Ce int servira à créer les différents noeuds
    printf("Dudurqvfqav \n");
    noeud->valeur = requete + indice;
    printf("Dudur3 \n");
    noeud->tag = "field-value";
    printf("Dudur2 \n");

    while (((checkField_Content(requete, i, longueur, NULL))) || (checkObs_Fold(requete, i, longueur,NULL))){
        if(checkField_Content(requete, i, longueur, NULL)){
            (*i)++;
        }
        {
            Noeud *test = malloc(sizeof(Noeud)); // Ce noeud nous sert à avancer (*i)
            checkObs_Fold(requete, i, longueur,test);
            free(test);
        }
        nombreFils++;
    }

    noeud->nombreFils = nombreFils;
    noeud->fils = malloc(nombreFils * sizeof(Noeud));

    while (((checkField_Content(requete, i, longueur, NULL)) || (checkObs_Fold(requete, i, longueur,NULL)))){
        if(checkField_Content(requete, i, longueur, NULL)){
            checkField_Content(requete, i, longueur, &noeud->fils[compteur]);
            compteur++;
        }

        else{
            checkObs_Fold(requete, i, longueur, &noeud->fils[compteur] );
            compteur++;
        }
    }
    

    noeud->longueur = *i - indice;
    return true;
}


bool checkLastHeader(char requete[], int *i, int longueur, Noeud *noeud){  //( field-name ":" OWS field-value OWS ) fonction pas encore tester
    const int indice= *i;
    int nombreFils=5;

    noeud->fils = malloc(nombreFils * sizeof(Noeud));
    noeud->valeur = requete + indice;
    // On ne connaît pas encore noeud->longueur
    noeud->nombreFils = nombreFils;
    noeud->tag = "header-field";

    Noeud *field_name=&noeud->fils[0];  

    field_name->fils = malloc(1 * sizeof(Noeud));
    field_name->valeur = requete + indice;
    // On ne connaît pas encore noeud->longueur
    field_name->nombreFils = 1;
    field_name->tag = "field-name";  

    int suppr_fils=0; //Compteur qui nous servira à supprimer les fils si jamais on à un false

    if(!checkToken( requete,i, longueur, &field_name->fils[0], "Tchar")){
        while(suppr_fils != nombreFils){
            free(&noeud->fils[suppr_fils]);
            suppr_fils++;
        }
        free(noeud);
        return false;
    }
    field_name->longueur= *i - indice;

    if(requete[*i]!=':'){
        while(suppr_fils != nombreFils){
            free(&noeud->fils[suppr_fils]);
            suppr_fils++;
        }
        free(noeud);
        return false;
    }
    else{
    (&noeud->fils[1])->fils = NULL;
    (&noeud->fils[1])->valeur = requete + indice;
    // On ne connaît pas encore noeud->longueur
    (&noeud->fils[1])->nombreFils = 0;
    (&noeud->fils[1])->tag = "incensitive-case-string";  
    (&noeud->fils[1])->longueur=1;

    (*i)++;

    }

    if(!checkOWS(requete, i, longueur, &noeud->fils[2])){
        while(suppr_fils != nombreFils){
            free(&noeud->fils[suppr_fils]);
            suppr_fils++;
        }
        free(noeud);
        return false;
    }

    if(!checkField_Value(requete,  i, longueur, &noeud->fils[3])){
        while(suppr_fils != nombreFils){
            free(&noeud->fils[suppr_fils]);
            suppr_fils++;
        }
        free(noeud);
        return false;
    }


    if(!checkOWS(requete, i, longueur, &noeud->fils[4])){
        while(suppr_fils != nombreFils){
            free(&noeud->fils[suppr_fils]);
            suppr_fils++;
        }
        free(noeud);
        return false;
    }

    noeud->longueur = *i - indice;
    return true;
}

int main(){

    Noeud *test_noeud = malloc(sizeof(Noeud));
    bool test;

    /*int temp=0;
    int *i=&temp;
    test=checkVchar("w", i, 1, NULL); // !=33 et ~=126
    if (test){
        printf("C'est carré \n");
    }
    else{
        printf("Maaaais nooon \n");
    }
    checkVchar("w", i, 1, test_noeud);

    if (test_noeud != NULL) {
        printArbre(test_noeud, 0);
        freeArbre(test_noeud);
    }*/

    /*int temp=0;
    int *i=&temp;
    test=checkObs_Text("€", i, 1, NULL); // ÿ=255 et €=128
    if (test){
        printf("C'est carré \n");
    }
    else{
        printf("Maaaais nooon \n");
    }
    checkObs_Text("ÿ", i, 1, test_noeud);

    if (test_noeud != NULL) {
        printArbre(test_noeud, 0);
        freeArbre(test_noeud);
    }*/

    /*int temp=0;
    int *i=&temp;
    test=checkField_Vchar("a", i, 1, NULL); // ÿ=255 et €=128 et !=33 et ~=126
    if (test){
        printf("C'est carré \n");
    }
    else{
        printf("Maaaais nooon \n");
    }
    checkField_Vchar("€", i, 1, test_noeud);

    if (test_noeud != NULL) {
        printArbre(test_noeud, 0);
        freeArbre(test_noeud);
    }*/

    /*char sujet[]= {13, 10, 32 , 32 , 32, 9,9,9,9,9,9,32,32,32, '\0'};
    int temp=0;
    int *i=&temp;
    test=checkObs_Fold(sujet, i, 10000, NULL); // ÿ=255 et €=128 et !=33 et ~=126
    if (test){
        printf("C'est carré \n");
    }
    else{
        printf("Maaaais nooon \n");
    }
    checkObs_Fold(sujet, i, 100, test_noeud);

    if (test_noeud != NULL) {
        printArbre(test_noeud, 0);
        freeArbre(test_noeud);
    }*/

    /*char sujet[]= {255, 32 , 32 , 32, 9,9,9,9,9,9,32,32,255, '\0'};
    int temp=0;
    int *i=&temp;
    test=checkField_Content(sujet, i, 10000, NULL); // ÿ=255 et €=128 et !=33 et ~=126
    if (test){
        printf("C'est carré \n");
    }
    else{
        printf("Maaaais nooon \n");
    }
    checkField_Content(sujet, i, 100, test_noeud);
    printf("Shees \n");

    if (test_noeud != NULL) {
        printArbre(test_noeud, 0);
        freeArbre(test_noeud);
    }*/

    printf("Chef \n");
    char sujet[]= {255, 32 , 32 , 32, 9,9,9,9,9,9,32,32,255,13, 10, 32 , 32 , 32, 9,9,9,9,9,9,32,32,32,255, 32 , 32 , 32, 9,9,9,9,9,9,32,32,255, '\0'};
    int temp=0;
    int *i=&temp;
    test=checkField_Value(sujet, i, 100, NULL); // ÿ=255 et €=128 et !=33 et ~=126
    if (test){
        printf("C'est carré \n");
    }
    else{
        printf("Maaaais nooon \n");
    }
    checkField_Value(sujet, i, 100, test_noeud);
    printf("Shees \n");

    if (test_noeud != NULL) {
        printArbre(test_noeud, 0);
        freeArbre(test_noeud);
    }


    return 0;
}