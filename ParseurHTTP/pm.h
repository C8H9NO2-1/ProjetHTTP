#ifndef PM_H
#define PM_H

//! Fonctions qui sont très génériques
//! Elles ne font aucun stockage, elle vérifie juste des syntaxes spécifiques qui peuvent être utiles autre part
bool checkAlpha(const char requete[], int i);
bool checkDigit(const char requete[], int i);
bool checkHexdig(const char requete[], int i);
bool checkUnreserved(const char requete[], int i);
bool checkPctEncoded(const char requete[], int i);
bool checkSubDelims(const char requete[], int i);
void createFilsSimple(char nom[], int i, int longueur, Noeud *noeud);

bool checkStartLine(const char requete[], int *i, int longueur, Noeud *noeud); //? start-line = method SP request-target SP HTTP-version CRLF

/**
 * @brief Vérifie qu'une partie de la chaîne requete est bien une method
 * 
 * @param requete Requête HTTP en cours de parsing
 * @param i Indice de début de la chaîne à vérifier
 * @param longueur Longueur de requete
 * @param noeud Pointeur vers le noeud dans lequel on va stocker la method
 * @return true Si la syntaxe est correcte
 * @return false Sinon
 */
bool checkMethod(const char requete[], int *i, const int longueur, Noeud *noeud); //? method = 1*tchar

/**
 * @brief Vérifie si un caractère est bien un tchar
 * 
 * @param requete Requête HTTP en cours de parsing
 * @param i Indice du caractère à vérifier dans requête
 * @param noeud Pointeur vers le noeud dans lequel on va stocker le caractère
 * @return true Si requête[i] est un tchar
 * @return false Sinon
 */
bool checkTChar(const char requete[], int i, Noeud *noeud); //? tchar = "!" / "#" / "$" / "%" / "&" / "'" / "*" / "+" / "-" / "." /"^" / "_" / "`" / "|" / "~" / DIGIT / ALPHA

bool checkRequestTarget(const char requete[], int *i, const int longueur, Noeud *noeud); //? request-target = absolute-path [ "?" query ]

bool checkAbsolutePath(const char requete[], int *i, const int longueur, Noeud *noeud); //? absolute-path = 1*( "/" segment )

bool checkSegment(const char requete[], int *i, const int longueur, Noeud *noeud); //? segment = *pchar

bool checkPChar(const char requete[], int *i, const int longueur, Noeud *noeud); //? pchar = unreserved / pct-encoded / sub-delims / ":" / "@"

bool checkQuery(const char requete[], int *i, const int longueur, Noeud *noeud); //? query = *( pchar / "/" / "?" )

bool checkHTTPVersion(const char requete[], int *i, const int longueur, Noeud *noeud); //? HTTP-version = HTTP-name "/" DIGIT "." DIGIT

#endif