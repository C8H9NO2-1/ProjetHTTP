#include <stdlib.h>
#include <stdio.h>

char *lireFichier(const char *nom) {
    FILE *testFile;

    testFile = fopen(nom, "r");
    if (testFile == NULL) {
        perror("Impossible d'ouvrir le fichier ");
        return 0;
    }

    int compteur = 0;
    int caractereActuel = 0;

    do {
        caractereActuel = fgetc(testFile); // On lit le caractère
        compteur++;
        // printf("%c", caractereActuel); // On l'affiche
    } while (caractereActuel != EOF); // On continue tant que fgetc n'a pas retourné EOF (fin de fichier)

    char *req = malloc(compteur * sizeof(char));
    int i = 0;
    rewind(testFile);

    for (int i = 0; i < compteur; i++) {
        req[i] = fgetc(testFile);
    }

    req[compteur - 1] = '\0';

    // printf("%s\n", req);

    fclose(testFile);

    return req;
}
