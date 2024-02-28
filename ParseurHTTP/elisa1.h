#ifndef ELISA1_H
#define ELISA1_H

/**
 * @brief Vériie si une chaîne est bien l'en-tête cookie d'une requête HTTP
 * 
 * @param cookie en-tête cookie en cours de parsing
 * @param i Pointeur vers l'indice de début de la chaîne à vérifier
 * @param longueur Longueur de l'en-tête
 * @param noeud Pointeur vers le noeud dans lequel on va stocker l'en-tête'
 * @return true Si la syntaxe est correcte
 * @return false Sinon
 */
bool checkCookieHeader(const char cookie[], int *i, int longueur, Noeud *noeud);//? "Cookie" ":" OWS cookie-string OWS

bool checkCookie(const char cookie[], Noeud *noeud);

bool checkOWS(const char cookie[], int *i, int longueur, Noeud *noeud);

bool checkCookieString(const char cookie[], int *i, int longueur, Noeud *noeud);

bool checkCookiePair(const char cookie[], int *i, int longueur, Noeud *noeud);

bool checkCookieName(const char cookie[], int *i, int longueur, Noeud *noeud);

bool checkTChar(const char requete[], int i, Noeud *noeud);

bool checkCookieValue(const char cookie[], int *i, int longueur, Noeud *noeud);

bool checkCookieOctet(const char cookie[], int *i);

bool checkAlpha(const char requete[], int i);
bool checkDigit(const char requete[], int i);
void createFilsSimple(char nom[], int i, int longueur, Noeud *noeud);
#endif