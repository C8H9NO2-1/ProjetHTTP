#ifndef IS_H
#define IS_H


/**
 * @brief verifie si la requete est bien correspand a Content-Length-header
 * 
 * @param requete la requete correspandante
 * @param i Pointeur 
 * @return tr
*/

// void freeArbre(Noeud *racine);

bool checkContentLenHeader(char requete[], int *i, int length, Noeud*Noeud);

// bool checkOWS(t char requete[], int *i, int longueur, Noeud *noeud); //OWS = *( SP / HTAB )
bool compareCaseInse(char chaine1[], char chaine2[]);
bool checkDigitLen(char requete[],int *i, int length,Noeud *noeud);// Content-Length = 1*DIGIT
bool checkConLen( char requete[] ,Noeud *noeud); // "Content-Length"

bool checkDigitS( char requete[],int i ,Noeud *noeud);
bool checkCRLF(char requete[],int *i,const int length);

#endif
