#ifndef TOM_H
#define TOM_H

#include "structure.h"

bool checkExpectString(char requete[], int *i, Noeud *noeud); // "Expect"

bool checkHostString(char requete[], int *i, Noeud *noeud); // "Host"

bool checkExpect(char requete[], int *i, Noeud *noeud); // "100-continue"

bool checkHost(char requete[], int *i, int longueur, Noeud *noeud); //

bool checkOWS(char requete[], int *i, int longueur, Noeud *noeud); // OWS = *( SP / HTAB )

bool checkIPV4(char requete[], int *i, Noeud *noeud, bool stocker);

int CompteurHexdig(char requete[], int *i);

bool checkIPV6(char requete[], int *i, Noeud *noeud);

bool checkIPfuture(char requete[], int *i, Noeud *noeud);

bool checkDecoctet(char requete[], int *i, Noeud *noeud, bool stocker);

bool checkIPliteral(char requete[], int *i, Noeud *noeud);

bool checkREGNAME(char requete[], int *i, Noeud *noeud);

bool checkUriHost(char requete[], int *i, int longueur, Noeud *noeud);

bool checkPort(char requete[], int *i, int longueur, Noeud *noeud);

// Fonctions qui sont très génériques
// Elles ne font aucun stockage, elles vérifient juste des syntaxes spécifiques qui peuvent être utiles autre part
bool checkAlpha(char requete[], int i);
bool checkDigit(char requete[], int i);
bool checkHexdig(char requete[], int i);
bool checkUnreserved(char requete[], int i);
bool checkPctEncoded(char requete[], int i);
bool checkSubDelims(char requete[], int i);


// Fonction utile pour la création de feuilles dans l'arbre
void createFilsSimple(char nom[], char *i, int longueur, Noeud *noeud);

// Fonction qui transforme une sous chaîne en minuscule pour pouvoir comparer les chaînes non sensibles à la casse
void sousChaineMinuscule(char chaine1[], char chaine2[], int i, int j);

/**
 * @brief Vériie si une chaîne est bien l'expect-header d'une requête HTTP
 * 
 * @param requete Requête HTTP en cours de parsing
 * @param i Pointeur vers l'indice de début de la chaîne à vérifier
 * @param longueur Longueur de requete
 * @param noeud Pointeur vers le noeud dans lequel on va stocker l'expect-header
 * @return true Si la syntaxe est correcte
 * @return false Sinon
 */
bool checkExpectHeader(char requete[], int *i,int longueur, Noeud *noeud) ; // Expect-header = "Expect" ":" OWS Expect OWS

/**
 * @brief Vériie si une chaîne est bien l'host-header d'une requête HTTP
 * 
 * @param requete Requête HTTP en cours de parsing
 * @param i Pointeur vers l'indice de début de la chaîne à vérifier
 * @param longueur Longueur de requete
 * @param noeud Pointeur vers le noeud dans lequel on va stocker l'host-header
 * @return true Si la syntaxe est correcte
 * @return false Sinon
 */
bool checkHostHeader(char requete[], int *i, int longueur, Noeud *noeud) ; // Host-header = "Host" ":" OWS Host OWS
#endif