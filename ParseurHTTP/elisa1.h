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
bool checkCookieHeader(char cookie[], int *i, int longueur, Noeud *noeud);//? "Cookie" ":" OWS cookie-string OWS

bool checkCookie(char cookie[], int *i, Noeud *noeud);

bool checkOWS(char cookie[], int *i, int longueur, Noeud *noeud);

bool checkCookieString(char cookie[], int *i, int longueur, Noeud *noeud);

bool checkCookiePair(char cookie[], int *i, int longueur, Noeud *noeud);

bool checkCookieName(char cookie[], int *i, int longueur, Noeud *noeud);

bool checkTChar(char requete[], int i, Noeud *noeud);

bool checkCookieValue(char cookie[], int *i, int longueur, Noeud *noeud);

bool checkCookieOctet(const char cookie[], int *i);

bool checkAlpha(const char requete[], int i);
bool checkDigit(const char requete[], int i);
void createFilsSimple(char nom[], char *i, int longueur, Noeud *noeud);

bool checkToken(char requete[], int *i, int longueur, Noeud *noeud, char nom[]);

void sousChaineMinuscule(const char chaine1[], char chaine2[], int i, int j);

#endif