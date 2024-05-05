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
 * Les deux derniers cas sont les mêmes car on a une seule adresse IP pour tous nos sites web => 127.0.0.1 ou [::1] (il faut faire attention avec les @IPv6 car il y a plusieurs façon des les écrires), donc celle-ci ne peut pas nous aider à les identifier
 *
 * Il y a aussi le cas où la ressource demandée est / dans ce cas il faut renvoyer le fichier par défaut (ou tous les fichiers nécessaires, je ne sais pas)
 */

/*
 * Cette fonction est le cas très facile, lorsqu'on a le nom du répertoire
 * On devrait pouvoir étendre les capacités de cette fonction pour pouvoir lire le contenu de la ressource
 */
bool checkExistenceWithHost(char *path, int lenPath, char *host, int lenHost) {

    FILE *file;

    char *root = "racine/";

    int len = strlen(root) + lenPath + lenHost;
    char *fileName = malloc((len + 1) * sizeof(char));
    strcpy(fileName, root);

    // On fait des copies des paramètres pour s'assurer que ce qu'on fait est correct
    // Car sinon, on va avoir un problème de longueur
    char *pathCopy = copy(path, lenPath);
    char *hostCopy = copy(host, lenHost);

    strcat(fileName, hostCopy);
    strcat(fileName, pathCopy);

    free(pathCopy);
    free(hostCopy);

    /*printf("path of the file from the root system => %s\n", fileName);*/

    file = fopen(fileName, "r");
    free(fileName);

    if (file == NULL) {
        return false;
    } else {
        fclose(file);
        return true;
    }
}

/*
 * Cette fonction s'occupe de vérifier les deux derniers cas
 * Pour ça, on vérifie si la ressource demandée est unique dans notre architecture
 * On devrait pouvoir étendre les fonctionnalités pour lire la ressource
 */
bool checkExistence(char *path, int len) {
    // On va coder en dur nos trois sites web
    char *webSite1 = "racine/www.hilopt.com";
    char *webSite2 = "racine/www.paphypu.fr";
    char *webSite3 = "racine/www.wichopool.com";

    char *pathCopy = copy(path, len);

    int count = 0; // On va compter le nombre de fois où la ressource apparait
    int last = 0;

    FILE *file = NULL;
    // Premier site web
    int lenFileName = strlen(webSite1) + len;
    char *fileName1 = malloc((lenFileName + 1) * sizeof(char));
    strcpy(fileName1, webSite1);
    strcat(fileName1, pathCopy);
    /*printf("path of the file: %s\n", fileName1);*/
    file = fopen(fileName1, "r");
    if (file != NULL) {
        last = 1;
        count++;
        fclose(file);
    }

    // Deuxième site web
    lenFileName = strlen(webSite2) + len;
    char *fileName2 = malloc((lenFileName + 1) * sizeof(char));
    strcpy(fileName2, webSite2);
    strcat(fileName2, pathCopy);
    /*printf("path of the file: %s\n", fileName2);*/
    file = fopen(fileName2, "r");
    if (file != NULL) {
        last = 2;
        count++;
        fclose(file);
    }

    // Troisième site web
    lenFileName = strlen(webSite3) + len;
    char *fileName3 = malloc((lenFileName + 1) * sizeof(char));
    strcpy(fileName3, webSite3);
    strcat(fileName3, pathCopy);
    /*printf("path of the file: %s\n", fileName3);*/
    file = fopen(fileName3, "r");
    if (file != NULL) {
        last = 3;
        count++;
        fclose(file);
    }

    if (count == 1) {
        switch (last) {
            case 1:
                printf("Le chemin est %s\n", fileName1);
                break;
            case 2:
                printf("Le chemin est %s\n", fileName2);
                break;
            case 3:
                printf("Le chemin est %s\n", fileName3);
                break;
            default:
                printf("Problème dans la fonction\n");
        }
    }

    free(pathCopy);

    free(fileName1);
    free(fileName2);
    free(fileName3);

    return (count == 1);
}

/*
 * Cette fonction permet de gérer le cas par défaut (/)
 * Le cas par défaut est le fichier index.html
 */
bool defaultPath(char *host, int len) {

    char *hostCopy = copy(host, len);
    
    if (strcmp(hostCopy, "www.hilopt.com") == 0) {
        printf("Fichiers par défaut pour www.hilopt.com\n");
    } else if (strcmp(hostCopy, "www.paphypu.fr") == 0) {
        printf("Fichiers par défaut pour www.paphypu.fr\n");
    } else if (strcmp(hostCopy, "www.wichopool.com") == 0) {
        printf("Fichiers par défaut pour www.wichopool.com\n");
    } else {
        printf("Problème dans le champ host\n");
    }

    return true;
}

char *copy(char *str, int len) {
    char *strCopy = malloc((len + 1) * sizeof(char));
    for (int i = 0; i < len; i++) {
        strCopy[i] = str[i];
    }
    strCopy[len] = '\0';

    return strCopy;
}
