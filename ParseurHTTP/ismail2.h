#ifndef IS2_H
#define IS2_H


bool checkContentTypeHeader(char requete[],int *i,int length,Noeud *noeud);
bool compareCaseInse(char chaine1[], char chaine2[]);
bool checkConType( char requete[] ,Noeud *noeud);
bool checkMediaType( char requete[],int *i,int length,Noeud *noeud);

bool checkParameter(char requete[],int *i,int length,Noeud *noeud);

bool checkQuotedString(char transferEncoding[], int *i, int longueur, Noeud *noeud);
bool checkQdtext(char transferEncoding[], int i, Noeud *noeud);
bool checkQuotedPair(char transferEncoding[], int *i, int longueur, Noeud *noeud);

bool checkCRLF(char requete[],int *i,const int length);
void freeArbre(Noeud *racine);
void createFilsSimple(char nom[], char *i, int longueur, Noeud *noeud);


#endif