#ifndef TOM_H
#define TOM_H


/*Fonction vérifiant la sémantique du champ host*/
bool semanticHost(void *root, int version);

/*Vérifie si l'adresse ipv6 est  127.0.0.1, dans la taille la sentinelle n'est pas compté, elle est à l'emplacement inside[taille]*/
bool isIPV6(char *inside, int taille);


#endif