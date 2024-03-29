#ifndef AFFICHAGE_H
#define AFFICHAGE_H

#include "structure.h"

/**
 * @brief Fonction récursive qui affiche l'arbre à partir du noeud passé en
 * paramètre
 *
 * @param noeud Pointeur vers la racine de l'arbre à afficher
 * @param nombreTab Nombre de tabulations à mettre avant l'affichage
 */
void printArbre(Noeud *noeud, int nombreTab);

#endif
