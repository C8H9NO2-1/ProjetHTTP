#ifndef ELISA2_H
#define ELISA2_H

bool checkTransferEncodingHeader(char transferEncoding[], int *i, int longueur, Noeud *noeud);

void createFilsSimple(char nom[], char *i, int longueur, Noeud *noeud);

bool checkTransferEncodingMot(char transferEncoding[], int *i, Noeud *noeud);

bool checkOWS(char transferEncoding[], int *i, int longueur, Noeud *noeud);

bool checkTransferEncoding(char transferEncoding[], int *i, int longueur, Noeud *noeud);

bool checkTransferCoding(char transferEncoding[], int *i, int longueur, Noeud *noeud);

bool checktransferextension(char transferEncoding[], int *i, int longueur, Noeud *noeud);

bool checkTransferParameter(char transferEncoding[], int *i, int longueur, Noeud *noeud);

bool checkQuotedString(char transferEncoding[], int *i, int longueur, Noeud *noeud);

bool checkQdtext(char transferEncoding[], int i, Noeud *noeud);

bool checkQuotedPair(char transferEncoding[], int *i, int longueur, Noeud *noeud);








bool checkTChar(char requete[], int i, Noeud *noeud);




bool checkAlpha(const char requete[], int i);

bool checkDigit(const char requete[], int i);

void sousChaineMinuscule(const char chaine1[], char chaine2[], int i, int j);
bool checkToken(char requete[], int *i, int longueur, Noeud *noeud, char nom[]);
#endif