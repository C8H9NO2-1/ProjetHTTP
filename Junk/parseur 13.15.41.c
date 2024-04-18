#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "parseur.h"
#include "clean.h"

bool message(char chaineMessage[], int longueur, Noeud *racine) {

    racine->indice = 0;
    racine->longueur = longueur;

    racine->tag = "message";

    int compteur = 0; // Compte le nombre de fils de ce noeud

    //! Création du fils debut
    Noeud *filsDebut = malloc(sizeof(Noeud));

    if (!debut(chaineMessage, filsDebut)) {
        nettoie1(filsDebut);
        return false;
    }
    compteur++;
    int i = 5;
    int compteurTrucs = 0;
    Noeud *tabFilsMilieu = NULL;
    if (!compteMilieu(chaineMessage, &i, &compteurTrucs, longueur)){
        nettoie1(filsDebut);
        return false;
    }
    else {
        if(compteurTrucs < 2){
            nettoie1(filsDebut);
            return false;
        }
        compteurTrucs *= 2;
        compteur += compteurTrucs;
        tabFilsMilieu=malloc(compteurTrucs * sizeof(Noeud));
        noeudMilieu(chaineMessage, i, compteurTrucs, tabFilsMilieu);
    }
    Noeud *tabFin=malloc(2*sizeof(Noeud));
    Noeud *ponctoption= NULL;
    bool isfin = false;
    if (chaineMessage[i]=='.'||chaineMessage[i]==','||chaineMessage[i]=='!'||chaineMessage[i]=='?'||chaineMessage[i]==':'){
        ponctoption = malloc(sizeof(Noeud));
        ponct(ponctoption, i);
        i++;
        compteur++;
        isfin=fin(chaineMessage,i,tabFin);
    } else {
        isfin = fin(chaineMessage, i, tabFin);
    }
    if (!isfin){
        return false;
    }
    compteur += 2;

    Noeud *fils = malloc(compteur * sizeof(Noeud));
    int l = 0;
    fils[l] = *filsDebut;
    for (int k = 0; k < compteurTrucs; k++) {
        fils[k + 1] = tabFilsMilieu[k];
    }
    l += compteurTrucs;
    if (ponctoption != NULL) {
        fils[l] = *ponctoption;
        l++;
    }
    l++;
    fils[l] = tabFin[0];
    l++;
    fils[l] = tabFin[1];

    racine->tab = fils;
    racine->nombreFils = compteur;

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
                    motFils(tabFilsMilieu, l);
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
        caractere[i].indice = tabFilsMilieu[indice].indice + i;
        caractere[i].tag="ALPHA";
        caractere[i].longueur=1;
        caractere[i].nombreFils=0;
        caractere[i].tab=NULL;
        i++;
    }
    caractere[i].indice = tabFilsMilieu[indice].indice + i;
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

bool fin(char chaineMessage[], int indice, Noeud *noeud){
    noeud[0].indice = indice;
    noeud[0].longueur = 3;
    noeud[0].tag = "fin";
    noeud[0].tab = NULL;
    noeud[0].nombreFils = 0;

    noeud[1].indice = indice+3;
    noeud[1].longueur = 1;
    noeud[1].tag = "LF";
    noeud[1].tab = NULL;
    noeud[1].nombreFils = 0;

    if (chaineMessage[indice] != 'F' && chaineMessage[indice] != 'f') {
        return false;
    }
    if (chaineMessage[indice +1] != 'I' && chaineMessage[indice +1] != 'i') {
        return false;
    }
    if (chaineMessage[indice +2] != 'N' && chaineMessage[indice +2] != 'n') {
        return false;
    }
    if (chaineMessage[indice +3] != 0x0A) {
        return false;
    }
    return true;    
}

void ponct(Noeud *ne, int indice){
    ne[indice].tag="Ponct";
    ne[indice].indice=indice;
    ne[indice].tab=NULL;
    ne[indice].nombreFils=0;
    ne[indice].longueur=1;
}