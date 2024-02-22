#ifndef PARSEUR_H
#define PARSEUR_H

typedef struct {
    int indice;
    int longueur;

    char *tag;

    struct Noeud *tab;
    int nombreFils;

    // struct Noeud *pere;
} Noeud;

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