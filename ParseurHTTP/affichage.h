#ifndef AFFICHAGE_H
#define AFFICHAGE_H

/**
 * @brief Fonction récursive qui affiche l'arbre à partir du noeud passé en paramètre
 * 
 * @param requete Requête HTTP en train d'être parsée
 * @param noeud Pointeur vers la racine de l'arbre à afficher
 * @param nombreTab Nombre de tabulations à mettre avant l'affichage
 */
void printArbre(const char requete[], Noeud *noeud, int nombreTab);

#endif