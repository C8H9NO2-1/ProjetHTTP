#ifndef STRUCTURE_H
#define STRUCTURE_H

typedef struct Noeud Noeud;
struct Noeud
{   
    int indice;
    int longueur;

    char *tag;

    Noeud *fils;
    int nombreFils;

    // Noeud *pere; //? Pourra être utile plus tard
};

#endif
