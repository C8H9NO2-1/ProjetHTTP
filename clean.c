#include "parseur.h"
#include "clean.h"

void nettoie1(Noeud *debut) {
    free(debut);
}

void nettoie2(Noeud *debut, Noeud *tabMilieu, int longueurTab) {
    free(debut);

    for (int i = 0; i < longueurTab; i++) {
        for (int k = 0; k < tabMilieu[i].nombreFils; k++) {
            Noeud *temp = tabMilieu[i].tab;
            free(temp[k]);
        }
    }
    free(tabMilieu);
}