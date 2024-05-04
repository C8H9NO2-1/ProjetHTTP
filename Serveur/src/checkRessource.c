/*
 * Ce fichier contient les fonctions qui servent à la vérification de la ressource demandée
 * Notamment son existence
 * Mais on vérifie aussi si l'utilisateur ne va pas dans un endroit dans lequel il n'a pas le droit d'aller
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "header/checkRessource.h"

/*
 * Cette fonction vérifie que l'utilisateur reste bien dans le dossier dans lequel il a le droit d'être
 */
bool checkPath(char *path, int len) {
    // Il faut vérifier que l'on ne remonte pas plus loin que ce qui est possible
    // Par exemple si on trouve .. avant de trouver quelque chose...
    // On va utiliser un compteur et arrêter la fonction dès qu'il devient négatif

    int count = 0;

    for (int i = 1; i < len; i++) {
        if (path[i] == '/') {
            count ++;
        } else if (i < len - 1 && path[i] == '.' && path[i + 1] == '.') {
            count--;
            i += 2;
        }
        if (count < 0) {
            return false;
        }
    }
    return true;
}

/*
 * On veut vérifier l'existence de la ressource
 * Il y a plusieurs scénarios:
 * - Soit on a un champ Host avec le nom du répertoire et dans ce cas c'est très facile
 * - Soit on a un champ Host avec une adresse IP et dans ce cas je ne sais pas trop quoi faire (on a une adresse IP pour tous nos sites web ???)
 * - Soit on a pas de champ Host et dans ce cas il faut explorer les ressources présentes dans notre base de données pour voir si la ressource demandée est unique
 */

/*
 * Cette fonction est le cas très facile, lorsqu'on a le nom du répertoire
 * On devrait pouvoir étendre les capacités de cette fonction pour pouvoir lire le contenu de la ressource
 */
bool checkExistence(char *path, int lenPath, char *host, int lenHost) {

    FILE *file;

    char *root = "racine/";

    int len = strlen(root) + lenPath + lenHost;
    char *fileName = malloc((len + 1) * sizeof(char));
    strcpy(fileName, root);

    // On fait des copies des paramètres pour s'assurer que ce qu'on fait est correct
    // Car sinon, on va avoir un problème de longueur
    char *pathCopy = malloc((lenPath + 1) * sizeof(char));
    for (int i = 0; i < lenPath; i++) {
        pathCopy[i] = path[i];
    }
    pathCopy[lenPath] = '\0';

    char *hostCopy = malloc((lenHost + 1) * sizeof(char));
    for (int i = 0; i < lenHost; i++) {
        hostCopy[i] = host[i];
    }
    hostCopy[lenHost] = '\0';

    strcat(fileName, hostCopy);
    strcat(fileName, pathCopy);

    free(pathCopy);
    free(hostCopy);

    printf("path of the file from the root system => %s\n", fileName);

    file = fopen(fileName, "r");
    free(fileName);

    if (file == NULL) {
        printf("La ressource n'existe pas\n");
        return false;
    } else {
        printf("La ressource existe\n");
        fclose(file);
        return true;
    }
}
