#ifndef STRUCTURE_H
#define STRUCTURE_H

typedef struct Noeud Noeud;
struct Noeud {   
    // int indice;
    // On remplace l'indice par un pointeur vers un char
    char *valeur;
    int longueur;

    char *tag;

    Noeud *fils;
    int nombreFils;

    // Noeud *pere; //? Pourra être utile plus tard
};

// On utilise une pile pour parcourir l'arbre
// Pour conserver l'indice du noeud et savoir où on en est dans la liste des fils
typedef struct pile pile;
struct pile {
    Noeud *noeud;
    int indice; // On utilise cet indice pour savoir où on en est dans la liste des fils du noeud
    pile *suivant;
};

enum Header {
    CONNECTION,
    CONTENT_LENGTH,
    CONTENT_TYPE,
    COOKIE,
    TRANSFER_ENCODING,
    EXPECT,
    HOST,
    GENERIC,
    CRLF
};
typedef enum Header Header;

#endif
