#ifndef FUNCTIONS_H
#define FUNCTIONS_H

//! Fonctions pour le parseur final
int checkCRLF(char requete[], const int longueur, int i);
bool checkCRLFBool(char requete[], const int longueur, int i);
int compteHeader(char requete[], int i, int longueur, Header tabHeader[]);

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
//! Fonction qui permet de vérifier et de créer un tchar
bool checkTChar(char requete[], int i, Noeud *noeud); //? tchar = "!" / "#" / "$" / "%" / "&" / "'" / "*" / "+" / "-" / "." /"^" / "_" / "`" / "|" / "~" / DIGIT / ALPHA
//! Fonction qui permet de vérifier et de créer un token
bool checkToken(char requete[], int *i, const int longueur, Noeud *noeud, char nom[]); //? token = 1*tchar
//! Fonction qui permet de vérifier et de créer un OWS
bool checkOWS(char requete[], int *i, const int longueur, Noeud *noeud); //? OWS = *( SP / HTAB )
//! Fonction qui transforme une sous chaîne en minuscule pour pouvoir comparer les chaînes non sensibles à la casse
void sousChaineMinuscule(const char chaine1[], char chaine2[], int i, int j);
//! Fonction qui permet de libérer de la mémoire
void freeArbre(Noeud *racine);

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

/**
 * @brief Vérifie si une chaîne est bien un connection-header
 * 
 * @param requete Requête HTTP en cours de parsing
 * @param i Pointeur vers l'indice de début de la chaîne à vérifier
 * @param longueur longueur de requete
 * @param noeud Pointeur vers le noeud dans lequel on va stocker le connection-header
 * @return true Si la syntaxe est correcte
 * @return false Sinon
 */
bool checkConnectionHeader(char requete[], int *i, const int longueur, Noeud *noeud); //? Connection-header = "Connection" ":" OWS Connection OWS

/**
 * @brief Vérifie si une chaîne est bien un connection-string
 * 
 * @param requete Requête HTTP en cours de parsing
 * @param i Pointeur vers l'indice de début de la chaîne à vérifier
 * @param noeud Pointeur vers le noeud dans lequel on va stocker le connection-option
 * @return true Si la syntaxe est correcte
 * @return false Sinon
 */
bool checkConnectionString(char requete[], int *i, Noeud *noeud); //? "Connection"

/**
 * @brief Vérifie si une chaîne est bien un connection
 * 
 * @param requete Requête HTTP en cours de parsing
 * @param i Pointeur vers l'indice de début de la chaîne à vérifier
 * @param longueur longueur de requete
 * @param noeud Pointeur vers le noeud dans lequel on va stocker le connection-option
 * @return true Si la syntaxe est correcte
 * @return false Sinon
 */
bool checkConnection(char requete[], int *i, const int longueur, Noeud *noeud); //? *( "," OWS ) connection-option *( OWS "," [ OWS connection-option ] )

/**
 * @brief Vérifie si une chaîne est bien un connection-option
 * 
 * @param requete Requête HTTP en cours de parsing
 * @param i Pointeur vers l'indice de début de la chaîne à vérifier
 * @param longueur longueur de requete
 * @param noeud Pointeur vers le noeud dans lequel on va stocker le connection-option
 * @return true Si la syntaxe est correcte
 * @return false Sinon
 */
bool checkConnectionOption(char requete[], int *i, const int longueur, Noeud *noeud); //? connection-option = token

//!====================================================================================================
//? Fonctions utiles pour parser le cookie-header

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

/**
 * @brief Vérifie si une chaîne est bien un cookie
 * 
 * @param cookie en-tête cookie en cours de parsing
 * @param i Pointeur vers l'indice de début de la chaîne à vérifier
 * @param noeud Pointeur vers le noeud dans lequel on va stocker le cookie
 * @return true Si la syntaxe est correcte
 * @return false Sinon
 */
bool checkCookie(char cookie[], int *i, Noeud *noeud);

/**
 * @brief Vérifie si une chaîne est bien un cookie-string
 * 
 * @param cookie en-tête cookie en cours de parsing
 * @param i Pointeur vers l'indice de début de la chaîne à vérifier
 * @param longueur Longueur de l'en-tête
 * @param noeud Pointeur vers le noeud dans lequel on va stocker le cookie-string
 * @return true Si la syntaxe est correcte
 * @return false Sinon
 */
bool checkCookieString(char cookie[], int *i, int longueur, Noeud *noeud);

/**
 * @brief Vérifie si une chaîne est bien un cookie-pair
 * 
 * @param cookie en-tête cookie en cours de parsing
 * @param i Pointeur vers l'indice de début de la chaîne à vérifier
 * @param longueur Longueur de l'en-tête
 * @param noeud Pointeur vers le noeud dans lequel on va stocker le cookie-pair
 * @return true Si la syntaxe est correcte
 * @return false Sinon
 */
bool checkCookiePair(char cookie[], int *i, int longueur, Noeud *noeud);

/**
 * @brief Vérifie si une chaîne est bien un cookie-name
 * 
 * @param cookie en-tête cookie en cours de parsing
 * @param i Pointeur vers l'indice de début de la chaîne à vérifier
 * @param longueur Longueur de l'en-tête
 * @param noeud Pointeur vers le noeud dans lequel on va stocker le cookie-name
 * @return true Si la syntaxe est correcte
 * @return false Sinon
 */
bool checkCookieName(char cookie[], int *i, int longueur, Noeud *noeud);

/**
 * @brief Vérifie si une chaîne est bien un cookie-value
 * 
 * @param cookie en-tête cookie en cours de parsing
 * @param i Pointeur vers l'indice de début de la chaîne à vérifier
 * @param longueur Longueur de l'en-tête
 * @param noeud Pointeur vers le noeud dans lequel on va stocker le cookie-value
 * @return true Si la syntaxe est correcte
 * @return false Sinon
 */
bool checkCookieValue(char cookie[], int *i, int longueur, Noeud *noeud);

/**
 * @brief Vérifie si une chaîne est bien un cookie-octet
 * 
 * @param cookie en-tête cookie en cours de parsing
 * @param i Pointeur vers l'indice de début de la chaîne à vérifier
 * @return true Si la syntaxe est correcte
 * @return false Sinon
 */
bool checkCookieOctet(const char cookie[], int *i);

//!====================================================================================================
//? Fonctions utiles pour parser le transfer-encoding-header

/**
 * @brief Vérifie si une chaîne est bien l'en-tête transfer-encoding d'une requête HTTP
 * 
 * @param transferEncoding en-tête transfer-encoding en cours de parsing
 * @param i Pointeur vers l'indice de début de la chaîne à vérifier
 * @param longueur Longueur de l'en-tête
 * @param noeud Pointeur vers le noeud dans lequel on va stocker l'en-tête'
 * @return true Si la syntaxe est correcte
 * @return false Sinon
 */
bool checkTransferEncodingHeader(char transferEncoding[], int *i, int longueur, Noeud *noeud);

/**
 * @brief Vérifie si une chaîne est bien un transfer-encoding mot
 * 
 * @param transferEncoding en-tête transfer-encoding en cours de parsing
 * @param i Pointeur vers l'indice de début de la chaîne à vérifier
 * @param noeud Pointeur vers le noeud dans lequel on va stocker le transfer-coding
 * @return true Si la syntaxe est correcte
 * @return false Sinon
 */
bool checkTransferEncodingMot(char transferEncoding[], int *i, Noeud *noeud);

/**
 * @brief Vérifie si une chaîne est bien un transfer-encoding
 * 
 * @param transferEncoding en-tête transfer-encoding en cours de parsing
 * @param i Pointeur vers l'indice de début de la chaîne à vérifier
 * @param longueur Longueur de l'en-tête
 * @param noeud Pointeur vers le noeud dans lequel on va stocker le transfer-extension
 * @return true Si la syntaxe est correcte
 * @return false Sinon
 */
bool checkTransferEncoding(char transferEncoding[], int *i, int longueur, Noeud *noeud);

/**
 * @brief Vérifie si une chaîne est bien un transfer-coding
 * 
 * @param transferEncoding en-tête transfer-encoding en cours de parsing
 * @param i Pointeur vers l'indice de début de la chaîne à vérifier
 * @param longueur Longueur de l'en-tête
 * @param noeud Pointeur vers le noeud dans lequel on va stocker le transfer-extension
 * @return true Si la syntaxe est correcte
 * @return false Sinon
 */
bool checkTransferCoding(char transferEncoding[], int *i, int longueur, Noeud *noeud);

/**
 * @brief Vérifie si une chaîne est bien un transfer-extension
 * 
 * @param transferEncoding en-tête transfer-encoding en cours de parsing
 * @param i Pointeur vers l'indice de début de la chaîne à vérifier
 * @param longueur Longueur de l'en-tête
 * @param noeud Pointeur vers le noeud dans lequel on va stocker le transfer-extension
 * @return true Si la syntaxe est correcte
 * @return false Sinon
 */
bool checktransferExtension(char transferEncoding[], int *i, int longueur, Noeud *noeud);

/**
 * @brief Vérifie si une chaîne est bien un transfer-parameter
 * 
 * @param transferEncoding en-tête transfer-encoding en cours de parsing
 * @param i Pointeur vers l'indice de début de la chaîne à vérifier
 * @param longueur Longueur de l'en-tête
 * @param noeud Pointeur vers le noeud dans lequel on va stocker le transfer-parameter
 * @return true Si la syntaxe est correcte
 * @return false Sinon
 */
bool checkTransferParameter(char transferEncoding[], int *i, int longueur, Noeud *noeud);

/**
 * @brief Vérifie si une chaîne est bien un quoted-string
 * 
 * @param transferEncoding en-tête transfer-encoding en cours de parsing
 * @param i Pointeur vers l'indice de début de la chaîne à vérifier
 * @param longueur Longueur de l'en-tête
 * @param noeud Pointeur vers le noeud dans lequel on va stocker le transfer-coding-param
 * @return true Si la syntaxe est correcte
 * @return false Sinon
 */
bool checkQuotedString(char transferEncoding[], int *i, int longueur, Noeud *noeud);

/**
 * @brief Vérifie si une chaîne est bien un qdtext
 * 
 * @param transferEncoding en-tête transfer-encoding en cours de parsing
 * @param i Pointeur vers l'indice de début de la chaîne à vérifier
 * @param noeud Pointeur vers le noeud dans lequel on va stocker le qdtext
 * @return true Si la syntaxe est correcte
 * @return false Sinon
 */
bool checkQdtext(char transferEncoding[], int i, Noeud *noeud);

/**
 * @brief Vérifie si une chaîne est bien un quoted-pair
 * 
 * @param transferEncoding en-tête transfer-encoding en cours de parsing
 * @param i Pointeur vers l'indice de début de la chaîne à vérifier
 * @param longueur Longueur de l'en-tête
 * @param noeud Pointeur vers le noeud dans lequel on va stocker le quoted-pair
 * @return true Si la syntaxe est correcte
 * @return false Sinon
 */
bool checkQuotedPair(char transferEncoding[], int *i, int longueur, Noeud *noeud);

//!====================================================================================================
//? Fonctions utiles pour parser le expect-header

/**
 * @brief Vérifie si une chaîne est bien l'expect-header d'une requête HTTP
 * 
 * @param requete Requête HTTP en cours de parsing
 * @param i Pointeur vers l'indice de début de la chaîne à vérifier
 * @param longueur Longueur de requete
 * @param noeud Pointeur vers le noeud dans lequel on va stocker l'expect-header
 * @return true Si la syntaxe est correcte
 * @return false Sinon
 */
bool checkExpectHeader(char requete[], int *i,int longueur, Noeud *noeud);

/**
 * @brief Vérifie si une chaîne est bien un expect-string
 * 
 * @param requete Requête HTTP en cours de parsing
 * @param i Pointeur vers l'indice de début de la chaîne à vérifier
 * @param noeud Pointeur vers le noeud dans lequel on va stocker l'expect-string
 * @return true Si la syntaxe est correcte
 * @return false Sinon
 */
bool checkExpectString(char requete[], int *i, Noeud *noeud);

/**
 * @brief Vérifie si une chaîne est bien un expect
 * 
 * @param requete Requête HTTP en cours de parsing
 * @param i Pointeur vers l'indice de début de la chaîne à vérifier
 * @param noeud Pointeur vers le noeud dans lequel on va stocker l'expect
 * @return true Si la syntaxe est correcte
 * @return false Sinon
 */
bool checkExpect(char requete[], int *i, Noeud *noeud);

//!====================================================================================================
//? Fonctions utiles pour parser le host-header

/**
 * @brief Vérifie si une chaîne est bien l'host-header d'une requête HTTP
 * 
 * @param requete Requête HTTP en cours de parsing
 * @param i Pointeur vers l'indice de début de la chaîne à vérifier
 * @param longueur Longueur de requete
 * @param noeud Pointeur vers le noeud dans lequel on va stocker l'host-header
 * @return true Si la syntaxe est correcte
 * @return false Sinon
 */
bool checkHostHeader(char requete[], int *i, int longueur, Noeud *noeud);

/**
 * @brief Vérifie si une chaîne est bien un host-string
 * 
 * @param requete Requête HTTP en cours de parsing
 * @param i Pointeur vers l'indice de début de la chaîne à vérifier
 * @param noeud Pointeur vers le noeud dans lequel on va stocker l'host-string
 * @return true Si la syntaxe est correcte
 * @return false Sinon
 */
bool checkHostString(char requete[], int *i, Noeud *noeud);

/**
 * @brief Vérifie si une chaîne est bien un host
 * 
 * @param requete Requête HTTP en cours de parsing
 * @param i Pointeur vers l'indice de début de la chaîne à vérifier
 * @param longueur Longueur de requete
 * @param noeud Pointeur vers le noeud dans lequel on va stocker l'host
 * @return true Si la syntaxe est correcte
 * @return false Sinon
 */
bool checkHost(char requete[], int *i, int longueur, Noeud *noeud);

/**
 * @brief Vérifie si une chaîne est bien un uri-host
 * 
 * @param requete Requête HTTP en cours de parsing
 * @param i Pointeur vers l'indice de début de la chaîne à vérifier
 * @param longueur Longueur de requete
 * @param noeud Pointeur vers le noeud dans lequel on va stocker l'uri-host
 * @return true Si la syntaxe est correcte
 * @return false Sinon
 */
bool checkUriHost(char requete[], int *i, int longueur, Noeud *noeud);

/**
 * @brief Vérifie si une chaîne est bien un port
 * 
 * @param requete Requête HTTP en cours de parsing
 * @param i Pointeur vers l'indice de début de la chaîne à vérifier
 * @param longueur Longueur de requete
 * @param noeud Pointeur vers le noeud dans lequel on va stocker le port
 * @return true Si la syntaxe est correcte
 * @return false Sinon
 */
bool checkPort(char requete[], int *i, int longueur, Noeud *noeud);

/**
 * @brief Vérifie si une chaîne est bien un IP-literal
 * 
 * @param requete Requête HTTP en cours de parsing
 * @param i Pointeur vers l'indice de début de la chaîne à vérifier
 * @param noeud Pointeur vers le noeud dans lequel on va stocker le IP-literal
 * @return true Si la syntaxe est correcte
 * @return false Sinon
 */
bool checkIPliteral(char requete[], int *i, Noeud *noeud);

/**
 * @brief Vérifie si une chaîne est bien un IPv4address
 * 
 * @param requete Requête HTTP en cours de parsing
 * @param i Pointeur vers l'indice de début de la chaîne à vérifier
 * @param noeud Pointeur vers le noeud dans lequel on va stocker le IPv4address
 * @param stocker true Si on doit stocker le résultat dans l'arbre, false sinon
 * @return true Si la syntaxe est correcte
 * @return false Sinon
 */
bool checkIPV4(char requete[], int *i, Noeud *noeud, bool stocker);

/**
 * @brief Vérifie si une chaîne est bien un reg-name
 * 
 * @param requete Requête HTTP en cours de parsing
 * @param i Pointeur vers l'indice de début de la chaîne à vérifier
 * @param noeud Pointeur vers le noeud dans lequel on va stocker le reg-name
 * @return true Si la syntaxe est correcte
 * @return false Sinon
 */
bool checkREGNAME(char requete[], int *i, Noeud *noeud);

/**
 * @brief Vérifie si une chaîne est bien un IPv6address
 * 
 * @param requete Requête HTTP en cours de parsing
 * @param i Pointeur vers l'indice de début de la chaîne à vérifier
 * @param noeud Pointeur vers le noeud dans lequel on va stocker le IPv6address
 * @return true Si la syntaxe est correcte
 * @return false Sinon
 */
bool checkIPV6(char requete[], int *i, Noeud *noeud);

/**
 * @brief Vérifie si une chaîne est bien un IPvFuture
 * 
 * @param requete Requête HTTP en cours de parsing
 * @param i Pointeur vers l'indice de début de la chaîne à vérifier
 * @param noeud Pointeur vers le noeud dans lequel on va stocker le IFvFuture
 * @return true Si la syntaxe est correcte
 * @return false Sinon
 */
bool checkIPfuture(char requete[], int *i, Noeud *noeud);

/**
 * @brief Vérifie si une chaîne est bien un dec-octet
 * 
 * @param requete Requête HTTP en cours de parsing
 * @param i Pointeur vers l'indice de début de la chaîne à vérifier
 * @param noeud Pointeur vers le noeud dans lequel on va stocker le dec-octet
 * @param stocker true Si on doit stocker le résultat dans l'arbre, false sinon
 * @return true Si la syntaxe est correcte
 * @return false Sinon
 */
bool checkDecoctet(char requete[], int *i, Noeud *noeud, bool stocker);

/**
 * @brief Compte le nombre de caractère hexa
 * 
 * @param requete Chaîne dans laquelle on compte
 * @param i Pointeur vers la position dans la chaîne
 * @return int Nombre de caractère hexa consécutifs
 */
int CompteurHexdig(char requete[], int *i);

#endif
