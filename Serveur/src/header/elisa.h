#ifndef ELISA_H
#define ELISA_H

typedef enum EncodingState EncodingState;
enum EncodingState {
    CHUNKED, COMPRESS, DEFLATE, GZIP, NONE
};

typedef struct listeEncodage {
    EncodingState value; // valeur de l'encodage à faire 
    struct listeEncodage *next; // next pointe vers le prochain encodage à faire.
} listeEncodage;

bool semanticCookie(void *root);
bool semanticTransferCodings(void *root, listeEncodage *liste, int version);
void sousChaineMinuscule2(const char *chaine1, char chaine2[], int n, int j);
void purgeListeEncodage(listeEncodage **r);

#endif
