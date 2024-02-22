#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "parseur.h"

int main(int argc, char *argv[]) {

    char chaineMessage[] = "startVKiMupi-.4 LCTKq-.fin\n";

    Noeud *racine = malloc(sizeof(Noeud));

    bool test = message(chaineMessage, 20, racine);

    printf("%s\n", racine->tag);

    free(racine);

    return 0;
}