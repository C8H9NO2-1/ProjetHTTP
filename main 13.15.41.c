#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "parseur.h"

int main(int argc, char *argv[]) {

    char chaineMessage[] = "startOpPap .kkrDqK-:fin\n";

    Noeud *racine = malloc(sizeof(Noeud));

    bool test = message(chaineMessage, 27, racine);

    if (!test) {
        return 1;
    }

    printf("%s\n", racine->tag);
    for (int i = 0; i < racine->nombreFils; i++) {
        printf("\t%s:\n", racine->tab[i].tag);
        if (racine->tab[i].nombreFils == 0) {
            printf("\t\t\"");
            for (int j = 0; j < racine->tab[i].longueur; j++) {
                if (racine->tab[i].tag[0] != 'L') {
                    printf("%c", chaineMessage[racine->tab[i].indice + j]);
                } else {
                    printf("LF");
                }
            }
            printf("\"\n");
        } else {
            for (int j = 0; j < racine->tab[i].nombreFils; j++) {
                printf("\t\t%s: ", racine->tab[i].tab[j].tag);
                printf("\"");
                for (int k = 0; k < racine->tab[i].tab[j].longueur; k++) {
                    printf("%c", chaineMessage[racine->tab[i].tab[j].indice + k]);
                }
                printf("\"\n");
            }
        }
    }

    free(racine);

    return 0;
}