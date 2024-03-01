#ifndef ELISA2_H
#define ELISA2_H
bool checkTransferEncodingHeader(const char transferEncoding[], int *i, int longueur, Noeud *noeud);
void createFilsSimple(char nom[], int i, int longueur, Noeud *noeud);
bool checkTransferEncodingMot(const char transferEncoding[], Noeud *noeud);
bool checkOWS(const char transferEncoding[], int *i, int longueur, Noeud *noeud);
bool checkTransferEncoding(const char transferEncoding[], int *i, int longueur, Noeud *noeud);
bool checkTransferCoding(const char transferEncoding[], int *i, int longueur, Noeud *noeud);
bool checktransferextension(const char transferEncoding[], int *i, int longueur, Noeud *noeud);
bool checkTransferParameter(const char transferEncoding[], int *i, int longueur, Noeud *noeud);
bool checkQuotedString(const char transferEncoding[], int *i, int longueur, Noeud *noeud);
bool checkQdtext(const char transferEncoding[], int i, Noeud *noeud);
bool checkQuotedPair(const char transferEncoding[], int *i, int longueur, Noeud *noeud);
bool checkTChar(const char requete[], int i, Noeud *noeud);
bool checkAlpha(const char requete[], int i);
bool checkDigit(const char requete[], int i);
#endif