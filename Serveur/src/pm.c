/*
 * Ce fichier contient les fonctions qui servent à la vérification de la ressource demandée
 * Notamment son existence
 * Mais on vérifie aussi si l'utilisateur ne va pas dans un endroit dans lequel il n'a pas le droit d'aller
 * Mais aussi à la vérification de la start-line et du header Connection (sémantique)
 */

//TODO Il faut peut-être aussi trouver un moyen de renvoyer le type du fichier

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "header/api.h"
#include "header/pm.h"

#define MAX_LENGTH 100

// Dans cette fonction, on ne vérifie pas la request-target, je sais ce n'est pas terrible mais c'est mieux comme ça
// Je changerais peut-être si j'ai le temps
bool semanticStartLine(void *root, Method *method, int *version) {
    // On commence par vérifier la method
    bool b = true;
    _Token *tokenMethod;
    tokenMethod = searchTree(root, "method");
    int l;
    char *s;
    s = getElementValue(tokenMethod->node, &l);
    char *methodName = copy(s, l);
    if (strcmp(methodName, "GET") == 0) {
        *method = GET;
    } else if (strcmp(methodName, "HEAD") == 0) {
        *method = HEAD;
    } else {
        printf("Erreur => method inconnue\n");
        b = false;
    }
    free(methodName);
    purgeElement(&tokenMethod);

    if (!b) {
        return false;
    }

    // On vérifie ensuite que la version de HTTP soit couverte par notre serveur (soit 1.0 soit 1.1)
    _Token *tokenVersion;
    tokenVersion = searchTree(root, "HTTP-version");
    int l2;
    char *s2;
    s2 = getElementValue(tokenVersion->node, &l2);
    char *versionName = copy(s2, l2);
    if (strcmp(versionName, "HTTP/1.0") == 0) {
        *version = 0;
    } else if (strcmp(versionName, "HTTP/1.1") == 0) {
        *version = 1;
    } else {
        printf("Erreur => version non supportée\n");
        b = false;
    }
    free(versionName);
    purgeElement(&tokenVersion);

    return b;
}

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
FILE* checkExistenceWithHost(char *path, int lenPath, char *host, int lenHost) {

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

    file = fopen(fileName, "r");
    free(fileName);

    return file; //! Il faudra bien penser à faire des fclose
}

/*
 * Cette fonction s'occupe de vérifier les deux derniers cas
 * Pour ça, on vérifie si la ressource demandée est unique dans notre architecture
 */
FILE* checkExistence(char *path, int len) {
    // On lit le fichier liste qui contient les noms des serveurs
    FILE *file = fopen("liste.txt", "r");
    if (file == NULL) {
        printf("Erreur => Impossible d'accéder à la structure du serveur web\n");
        return false;
    }

    char *pathCopy = copy(path, len);
    int count = 0; // On va compter le nombre de fois où la ressource apparait
    int last = 0; // Contient l'indice du site web dans la liste qui contient la ressource

    char *base = "racine/";
    int lenBase = strlen(base);

    // On lit le contenu du fichier
    char website[MAX_LENGTH] = "";
    int k = 0;
    while (fgets(website, MAX_LENGTH, file) != NULL) {
        k++;
        // Il faut envoyer le dernier \n du nom du dossier
        int temp = strlen(website);
        website[temp - 1] = '\0';

        // On vérifie ensuite ce qu'on veut vérifier
        // C'est-à-dire l'unicité ou non de la ressource
        FILE *ressource = NULL;
        int lenFileName = lenBase + strlen(website) + len;
        char *fileName = malloc((lenFileName + 1) * sizeof(char));
        strcpy(fileName, base);
        strcat(fileName, website);
        strcat(fileName, pathCopy);
        /*printf("path of the file: %s\n", fileName);*/
        ressource = fopen(fileName, "r");
        if (ressource != NULL) {
            last = k;
            count++;
            fclose(ressource);
        }

        free(fileName);
    }

    if (count != 1) {
        fclose(file);
        free(pathCopy);
        return NULL;
    }

    // Si la ressource est unique, on continue et il faut l'exploiter

    // Il faut revenir au début du fichier
    rewind(file);
    for (int i = 0; i < last; i++) {
        fgets(website, MAX_LENGTH, file);
    }
    fclose(file);
    // On enlève le \n à la fin
    int temp = strlen(website);
    website[temp - 1] = '\0';
    FILE *ressource = NULL;
    int lenFileName = lenBase + strlen(website) + len;
    char *fileName = malloc((lenFileName + 1) * sizeof(char));
    strcpy(fileName, base);
    strcat(fileName, website);
    strcat(fileName, pathCopy);
    /*printf("Chemin vers la ressource unique => %s\n", fileName);*/
    ressource = fopen(fileName, "r");

    free(pathCopy);
    free(fileName);

    return ressource; //! Meme chose ici, penser a faire des fclose dans les fonctions appelantes
}

/*
 * Cette fonction permet de gérer le cas par défaut (/)
 * Le cas par défaut est le fichier index.html (Modification nécéssaire)
 */
FILE* defaultPath(char *host, int len) {
    char *base = "racine/";
    int lenBase = strlen(base);

    char *hostCopy = copy(host, len);

    char *defaultFile = "/index.html";
    int lenFile = strlen(defaultFile);

    int lenFileName = lenBase + len + lenFile;
    char *fileName = malloc((lenFileName + 1) * sizeof(char));
    strcpy(fileName, base);
    strcat(fileName, hostCopy);
    strcat(fileName, defaultFile);

    FILE *file = fopen(fileName, "r");

    return file;
}

char *copy(char *str, int len) {
    char *strCopy = malloc((len + 1) * sizeof(char));
    for (int i = 0; i < len; i++) {
        strCopy[i] = str[i];
    }
    strCopy[len] = '\0';

    return strCopy;
}
