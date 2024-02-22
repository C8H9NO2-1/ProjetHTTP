#ifndef PARSEUR_H
#define PARSEUR_H

#include <stdbool.h>

typedef struct Noeud Noeud;
struct Noeud {
    int indice;
    int longueur;

    char *tag;

    Noeud *tab;
    int nombreFils;

    // struct Noeud *pere;
};

bool message(char chaineMessage[], int longueur, Noeud *racine);

bool debut(char chaineMessage[], Noeud *noeud);

bool compteMilieu(char chaineMessage[],int *i, int *compteurTrucs, int longueur);

bool motponct(char chaineMessage[],int *i);

bool nombresep(char chaineMessage[],int *i);

void noeudMilieu(char chaineMessage[], int i, int compteurTrucs, Noeud tabFilsMilieu[]);

void motFils(Noeud tabFilsMilieu[], int indice);

void chiffre(int longueurNombre,Noeud *tabFilsMilieu, int j);

bool fin(char chaineMessage[], int indice, Noeud *noeud);

void ponct(Noeud *ne, int indice);

#endif