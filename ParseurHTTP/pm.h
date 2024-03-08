#ifndef PM_H
#define PM_H

//! Fonctions qui sont très génériques
//! Elles ne font aucun stockage, elles vérifient juste des syntaxes spécifiques qui peuvent être utiles autre part
bool checkAlpha(const char requete[], int i);
bool checkDigit(const char requete[], int i);
bool checkHexdig(const char requete[], int i);
bool checkUnreserved(const char requete[], int i);
bool checkPctEncoded(const char requete[], int i);
bool checkSubDelims(const char requete[], int i);
//! Fonction utile pour la création de feuilles dans l'arbre
void createFilsSimple(char nom[], char *i, int longueur, Noeud *noeud);
//! Fonction qui permet de vérifier et de créer un token
bool checkToken(char requete[], int *i, const int longueur, Noeud *noeud, char nom[]); //? token = 1*tchar
//! Fonction qui transforme une sous chaîne en minuscule pour pouvoir comparer les chaînes non sensibles à la casse
void sousChaineMinuscule(const char chaine1[], char chaine2[], int i, int j);

//!====================================================================================================
//? Fonctions utiles pour parser la start-line
/**
 * @brief Vériie si une chaîne est bien la start-line d'une requête HTTP
 * 
 * @param requete Requête HTTP en cours de parsing
 * @param i Pointeur vers l'indice de début de la chaîne à vérifier
 * @param longueur Longueur de requete
 * @param noeud Pointeur vers le noeud dans lequel on va stocker la start-line
 * @return true Si la syntaxe est correcte
 * @return false Sinon
 */
bool checkStartLine(char requete[], int *i, int longueur, Noeud *noeud); //? start-line = method SP request-target SP HTTP-version CRLF

/**
 * @brief Vérifie qu'une partie de la chaîne requete est bien une method
 * 
 * @param requete Requête HTTP en cours de parsing
 * @param i Pointeur vers l'indice de début de la chaîne à vérifier
 * @param longueur Longueur de requete
 * @param noeud Pointeur vers le noeud dans lequel on va stocker la method
 * @return true Si la syntaxe est correcte
 * @return false Sinon
 */
bool checkMethod(char requete[], int *i, const int longueur, Noeud *noeud); //? method = token

/**
 * @brief Vérifie si un caractère est bien un tchar
 * 
 * @param requete Requête HTTP en cours de parsing
 * @param i Indice du caractère à vérifier dans requête
 * @param noeud Pointeur vers le noeud dans lequel on va stocker le caractère
 * @return true Si requête[i] est un tchar
 * @return false Sinon
 */
bool checkTChar(char requete[], int i, Noeud *noeud); //? tchar = "!" / "#" / "$" / "%" / "&" / "'" / "*" / "+" / "-" / "." /"^" / "_" / "`" / "|" / "~" / DIGIT / ALPHA

/**
 * @brief Vérifie si une chaîne est bien une request-target
 * 
 * @param requete Requête HTTP en cours de parsing
 * @param i Pointeur vers l'indice de début de la chaîne à vérifier
 * @param longueur Longueur de requete
 * @param noeud Pointeur vers le noeud dans lequel on va stocker la request-target
 * @return true Si la syntaxe est correcte
 * @return false Sinon
 */
bool checkRequestTarget(char requete[], int *i, const int longueur, Noeud *noeud); //? request-target = absolute-path [ "?" query ]

/**
 * @brief Vérifie si une chaîne est bien un absolute-path
 * 
 * @param requete Requête HTTP en cours de parsing
 * @param i Pointeur vers l'indice de début de la chaîne à vérifier
 * @param longueur Longueur de requete
 * @param noeud Pointeur vers le noeud dans lequel on va stocker l'absolute-path
 * @return true Si la syntaxe est correcte
 * @return false Sinon
 */
bool checkAbsolutePath(char requete[], int *i, const int longueur, Noeud *noeud); //? absolute-path = 1*( "/" segment )

/**
 * @brief Vérifie si une chaîne est bien un segment
 * 
 * @param requete Requête HTTP en cours de parsing
 * @param i Pointeur vers l'indice de début de la chaîne à vérifier
 * @param longueur Longueur de requete
 * @param noeud Pointeur vers le noeud dans lequel on va stocker le segment
 * @return true Si la syntaxe est correcte
 * @return false Sinon
 */
bool checkSegment(char requete[], int *i, const int longueur, Noeud *noeud); //? segment = *pchar

/**
 * @brief Vérifie si une chaîne est bien un pchar
 * 
 * @param requete Requête HTTP en cours de parsing
 * @param i Pointeur vers l'indice de début de la chaîne à vérifier
 * @param longueur Longueur de requete
 * @param noeud Pointeur vers le noeud dans lequel on va stocker le pchar
 * @return true Si la syntaxe est correcte
 * @return false Sinon
 */
bool checkPChar(char requete[], int *i, const int longueur, Noeud *noeud); //? pchar = unreserved / pct-encoded / sub-delims / ":" / "@"

/**
 * @brief Vérifie si une chaîne est bien un query
 * 
 * @param requete Requête HTTP en cours de parsing
 * @param i Pointeur vers l'indice de début de la chaîne à vérifier
 * @param longueur Longueur de requete
 * @param noeud Pointeur vers le noeud dans lequel on va stocker le query
 * @return true Si la syntaxe est correcte
 * @return false Sinon
 */
bool checkQuery(char requete[], int *i, const int longueur, Noeud *noeud); //? query = *( pchar / "/" / "?" )

/**
 * @brief Vérifie si une chaîne est bien une HTTP-version
 * 
 * @param requete Requête HTTP en cours de parsing
 * @param i Pointeur vers l'indice de début de la chaîne à vérifier
 * @param longueur Longueur de requete
 * @param noeud Pointeur vers le noeud dans lequel on va stocker la HTTP-version
 * @return true Si la syntaxe est correcte
 * @return false Sinon
 */
bool checkHTTPVersion(char requete[], int *i, const int longueur, Noeud *noeud); //? HTTP-version = HTTP-name "/" DIGIT "." DIGIT

//!====================================================================================================
//? Fonctions utiles pour parser le connection-header

bool checkConnectionHeader(char requete[], int *i, const int longueur, Noeud *noeud); //? Connection-header = "Connection" ":" OWS Connection OWS

bool checkConnectionString(char requete[], int *i, Noeud *noeud); //? "Connection"

bool checkOWS(char requete[], int *i, const int longueur, Noeud *noeud); //? OWS = *( SP / HTAB )

bool checkConnection(char requete[], int *i, const int longueur, Noeud *noeud); //? *( "," OWS ) connection-option *( OWS "," [ OWS connection-option ] )

bool checkConnectionOption(char requete[], int *i, const int longueur, Noeud *noeud); //? connection-option = token

#endif