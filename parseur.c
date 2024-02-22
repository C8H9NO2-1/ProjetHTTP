#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "parseur.h"

bool message(char chaineMessage[], int longueur, Noeud *racine) {

    racine->indice = 0;
    racine->longueur = longueur;

    racine->tag = "message";

    int compteur = 0; // Compte le nombre de fils de ce noeud

    //! Création du fils debut
    Noeud *filsDebut = malloc(sizeof(Noeud));

    if (!debut(chaineMessage, filsDebut)) {
        free(filsDebut);
        return false;
    }
    compteur++;
    int i = 5;
    int compteurTrucs = 0;
    if (!compteMilieu(chaineMessage, &i, &compteurTrucs, longueur)){
        return false;
    }
    else {
        if(compteurTrucs < 2){
            return false;
        }
        compteurTrucs *= 2;
        Noeud *tabFilsMilieu=malloc(compteurTrucs * sizeof(Noeud));
        noeudMilieu(chaineMessage, i, compteurTrucs, tabFilsMilieu);
    }

    return true;
}

bool debut(char chaineMessage[], Noeud *noeud) {
    noeud->indice = 0;
    noeud->longueur = 5;
    noeud->tag = "debut";

    noeud->tab = NULL;
    noeud->nombreFils = 0;

    if (chaineMessage[0] != 'S' && chaineMessage[0] != 's') {
        return false;
    }
    if (chaineMessage[1] != 'T' && chaineMessage[1] != 't') {
        return false;
    }
    if (chaineMessage[2] != 'A' && chaineMessage[2] != 'a') {
        return false;
    }
    if (chaineMessage[3] != 'R' && chaineMessage[3] != 'r') {
        return false;
    }
    if (chaineMessage[4] != 'T' && chaineMessage[4] != 't') {
        return false;
    }

    return true;
}

bool compteMilieu(char chaineMessage[],int *i, int *compteurTrucs, int longueur){
    char c;
    while (*i<longueur){
        bool b=false;
        c=chaineMessage[*i];
        if ((c>=65 && c<=90) || (c>=97 && c<=122)){
            b=motponct(chaineMessage, i);
        }
        if (c>=48 && c<=57){
            b=nombresep(chaineMessage, i);
        }
        if (!b){
            if (*compteurTrucs<2){
                return false;
            }
            else{
                return true;
            }
        }
        else{
            (*compteurTrucs)++;
        }
    }
    return false;
}

bool motponct(char chaineMessage[],int *i){
    char c=chaineMessage[*i];
    int k=*i;
    while ((c>=65 && c<=90) || (c>=97 && c<=122)){
        k++;
        c=chaineMessage[k];
    }
    if (c==32 || c==9 || c=='-' || c=='_'){
        k++;
        c=chaineMessage[k];
        if (c==',' || c=='.' || c=='!' || c=='?' || c==':') {
            k++;
            *i=k;
            return true;
        }
        else {
            return false;
        }
    }
    else {
        return false;
    }
}

bool nombresep(char chaineMessage[],int *i){
    char c=chaineMessage[*i];
    int k=*i;
    while (c>=48 && c<=57){
        k++;
        c=chaineMessage[k];
    }
    if (c==32 || c==9 || c=='-' || c=='_'){
        k++;
        *i=k;
        return true;
    }
    else {
        return false;
    }
}

void noeudMilieu(char chaineMessage[], int i, int compteurTrucs, Noeud tabFilsMilieu[]){
    int k=5;
    int j=k;
    int longTemp=0;
    bool alarme=true;
    for(int l=0;l<compteurTrucs;l++){
        while(k<i && alarme){
            char c=chaineMessage[k];
            if (c==32 || c==9 || c=='-' || c=='_') {
                alarme=false;
                if (chaineMessage[k-1]>=48 && chaineMessage[k-1]<=57){
                    tabFilsMilieu[l].tag="nombre";   
                    tabFilsMilieu[l].indice=j;
                    tabFilsMilieu[l].longueur=longTemp;
                    chiffre(longTemp, &tabFilsMilieu[l], j);
                    l++;
                    tabFilsMilieu[l].tag="separateur";   
                    tabFilsMilieu[l].indice=k;
                    tabFilsMilieu[l].longueur=1;
                    tabFilsMilieu[l].nombreFils=0;
                    tabFilsMilieu[l].tab=NULL;

                }
                else {
                    longTemp++;
                    k++;
                    tabFilsMilieu[l].tag="mot";
                    tabFilsMilieu[l].indice=j;
                    tabFilsMilieu[l].longueur=longTemp;
                    motFils(tabFilsMilieu, j);
                    l++;
                    tabFilsMilieu[l].tag="ponct";
                    tabFilsMilieu[l].indice=k;
                    tabFilsMilieu[l].longueur=1;
                    tabFilsMilieu[l].nombreFils=0;
                    tabFilsMilieu[l].tab=NULL;
                }
            }
            longTemp++;
            k++;
        }
        alarme=true;
        longTemp=0;
        j=k;
    }
}


void motFils(Noeud tabFilsMilieu[], int indice){
    int i=0;
    Noeud *caractere= malloc(tabFilsMilieu[indice].longueur * sizeof(Noeud));
    while (i < tabFilsMilieu[indice].longueur - 1){
        caractere[i].tag="ALPHA";
        caractere[i].longueur=1;
        caractere[i].nombreFils=0;
        caractere[i].tab=NULL;
        i++;
    }
    caractere[i].tag="Separateur";
    caractere[i].longueur=1;
    caractere[i].nombreFils=0;
    caractere[i].tab=NULL; 
    tabFilsMilieu[indice].nombreFils=tabFilsMilieu[indice].longueur;
    tabFilsMilieu[indice].tab=caractere;
}

void chiffre(int longueurNombre,Noeud *tabFilsMilieu, int j){
    Noeud *tabnombre=malloc(longueurNombre*sizeof(Noeud));
    for (int i=0;i<longueurNombre;i++){
        tabnombre[i].tag="DIGIT";
        tabnombre[i].indice=j+i;
        tabnombre[i].longueur=1;
        tabnombre[i].nombreFils=0;
        tabnombre[i].tab=NULL;
    }
    tabFilsMilieu->nombreFils=longueurNombre;
    tabFilsMilieu->tab=tabnombre;
}
