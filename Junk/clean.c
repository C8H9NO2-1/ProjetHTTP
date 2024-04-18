#include <stdlib.h>
#include <stdio.h>

#include "parseur.h"
#include "clean.h"

void nettoie1(Noeud *debut) {
    free(debut);
}

void nettoie2(Noeud *debut, Noeud *tabMilieu, int longueurTab) {
    free(debut);

    free(tabMilieu);
}