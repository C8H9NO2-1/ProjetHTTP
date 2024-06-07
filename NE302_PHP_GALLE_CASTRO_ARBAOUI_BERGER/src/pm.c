/*
 * Ce fichier contient les fonctions qui servent à la vérification de la ressource demandée
 * Notamment son existence
 * Mais on vérifie aussi si l'utilisateur ne va pas dans un endroit dans lequel il n'a pas le droit d'aller
 * Mais aussi à la vérification de la start-line et du header Connection (sémantique)
 */

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
    if (strncmp(s, "GET", l) == 0) {
        *method = GET;
    } else if (strncmp(s, "HEAD", l) == 0) {
        *method = HEAD;
    } else if (strncmp(s, "POST", l) == 0) {
        *method = POST;
    } else {
        printf("Erreur => method inconnue\n");
        b = false;
    }
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
    if (strncmp(s2, "HTTP/1.0", l2) == 0) {
        *version = 0;
    } else if (strncmp(s2, "HTTP/1.1", l2) == 0) {
        *version = 1;
    } else if (strncmp(s2, "HTTP/1.", l2 - 1) == 0) { // Il faut supporter toutes les sous-versions
        *version = 1; // On considère que la requête a été faite avec HTTP/1.1
    } else {
        printf("Erreur => version non supportée\n");
        *version = -1;
        b = false;
    }
    purgeElement(&tokenVersion);

    return b;
}

bool semanticConnection(void *root, ConnectionState *state, int version) {
    //? Es-ce possible d'avoir plusieurs Connection-headers ?
    // A priori oui mais dans ce cas, il faut regarder pour voir si les options n'entrent pas en conflit

    // On regarde les options de connexions pour voir ce qu'on doit faire
    bool close = false;
    bool keepAlive = false;
    _Token *r, *tok;
    r = searchTree(root, "connection-option");
    tok = r;
    while (tok != NULL) {
        int l;
        char *s;
        s = getElementValue(tok->node, &l);
        if (compareCaseInsensitive(s, "close", l)) {
            close = true;
        } else if (compareCaseInsensitive(s, "keep-alive", l)) {
            keepAlive = true;
        } else {
            printf("Option de connexion inconnue\n");
        }
        tok = tok->next;
    }

    purgeElement(&r);

    // On liste tous les cas possibles
    if (close && keepAlive) {
        printf("Impossible de déterminer la bonne option de connexion\n");
        return false;
    }

    if (close) {
        *state = CLOSE;
    } else if (version == 1) {
        *state = KEEPALIVE;
    } else if (version == 0 && keepAlive) {
        *state = KEEPALIVE;
    } else {
        *state = CLOSE;
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
    //! Changement drastique de cette fonction, en se basant sur la RFC 3986

    FILE *file;

    char *root = "racine/";

    // Il faut normaliser le chemin
    char *pathNormalized = URINormalization(path, lenPath);
    int newLenPath = strlen(pathNormalized);

    // Il faut normaliser le champ host
    char *hostNormalized = URINormalization(host, lenHost);
    int newLenHost = strlen(hostNormalized);

    int len = strlen(root) + newLenPath + newLenHost;
    char *fileName = malloc((len + 1) * sizeof(char));
    strcpy(fileName, root);

    strncat(fileName, hostNormalized, newLenHost);
    strncat(fileName, pathNormalized, newLenPath);

    file = fopen(fileName, "r");
    free(fileName);
    free(pathNormalized);
    free(hostNormalized);

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

    // On normalise le chemin
    char *pathNormalized = URINormalization(path, len);
    int newLen = strlen(pathNormalized);

    int count = 0; // On va compter le nombre de fois où la ressource apparait
    int last = 0; // Contient l'indice du site web dans la liste qui contient la ressource

    char *base = "racine/";
    int lenBase = strlen(base);

    // On lit le contenu du fichier
    char website[MAX_LENGTH] = "";
    int k = 0;
    while (fgets(website, MAX_LENGTH, file) != NULL) {
        k++;
        // Il faut enlever le dernier \n du nom du dossier
        int temp = strlen(website);
        website[temp - 1] = '\0';

        // On vérifie ensuite ce qu'on veut vérifier
        // C'est-à-dire l'unicité ou non de la ressource
        FILE *ressource = NULL;
        int lenFileName = lenBase + strlen(website) + newLen;
        char *fileName = malloc((lenFileName + 1) * sizeof(char));
        strcpy(fileName, base);
        strcat(fileName, website);
        strncat(fileName, pathNormalized, newLen);
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
        free(pathNormalized);
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
    int lenFileName = lenBase + strlen(website) + newLen;
    char *fileName = malloc((lenFileName + 1) * sizeof(char));
    strcpy(fileName, base);
    strcat(fileName, website);
    strncat(fileName, pathNormalized, newLen);
    /*printf("Chemin vers la ressource unique => %s\n", fileName);*/
    ressource = fopen(fileName, "r");

    free(pathNormalized);
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

    char defaultFile[] = "/index.html";
    int lenFile = strlen(defaultFile);

    // On normalise le champ host
    char *hostNormalized = URINormalization(host, len);
    int newLen = strlen(hostNormalized);

    int lenFileName = lenBase + newLen + lenFile;
    char *fileName = malloc((lenFileName + 1) * sizeof(char));
    strcpy(fileName, base);
    strncat(fileName, hostNormalized, newLen);
    strcat(fileName, defaultFile);

    FILE *file = fopen(fileName, "r");

    free(fileName);
    free(hostNormalized);

    return file;
}

/*
 * Cette fonction va faire toute la normalisation nécessaire
 */
char *URINormalization(char *path, int len) {
    // On commence par faire une copie assez grande
    // Cette copie sera probablement trop grande mais ce n'est pas grave
    char *result1 = malloc((len + 1) * sizeof(char));
    int j = 0; // Cet indice va nous permettre de parcourir result

    /* Il va falloir faire les 2 normalisations à la fois
     * - Changer toutes les lettres en minuscule (Sauf pour percent-encoded qui doivent tous être en majuscule)
     * - Changer tous les %.. qui correspondent à des unreserved
     */
    for (int i = 0; i < len; i++) {
        if (path[i] == '%') {
            int temp = convertHexdig(path + i + 1);
            if (isUnreserved(temp)) {
                result1[j] = (char) temp;
            } else {
                result1[j] = '%';
                for (int k = 1; k <= 2; k++) {
                    if (path[i + k] >= 'a' && path[i + k] <= 'f') {
                        result1[j + k] = path[i + k] - 'a' + 'A';
                    } else {
                        result1[j + k] = path[i + k];
                    }
                }
                j += 2;
            }
            i += 2; // On passe les deux caractères suivant
        } else {
            if (path[i] >= 'A' && path[i] <= 'Z') {
                result1[j] = path[i] - 'A' + 'a';
            } else {
                result1[j] = path[i];
            }
        }
        // Il ne faut pas oublier d'incrémenter j
        j++;
    }

    // On ajoute la sentinelle à la fin
    result1[j] = '\0';

    /*? On applique ensuite l'algorithme de "dot removal"*/
    char *result2 = dotRemoval(result1, j);

    free(result1);

    return result2;
}

int convertHexdig(char *str) {
    int result = 0;

    for (int i = 0; i < 2; i++) {
        if (str[i] >= '0' && str[i] <= '9') {
            result = str[i] - '0' + (result * 16 * i);
        } else if (str[i] >= 'A' && str[i] <= 'F') {
            result = str[i] - 'A' + 10 + (result * 16 * i);
        } else if (str[i] >= 'a' && str[i] <= 'f') {
            result = str[i] - 'a' + 10 + (result * 16 * i);
        }
    }

    /*printf("Result => %d\n", result);*/

    return result;
}

bool isUnreserved(int x) {
    bool b = false;

    if (x >= 'A' && x <= 'Z') {
        b = true;
    } else if (x >= 'a' && x <= 'z') {
        b = true;
    } else if (x >= '0' && x <= '9') {
        b = true;
    } else {
        switch (x) {
            case '-':
            case '.':
            case '_':
            case '~':
                b = true;
                break;
            default:
                break;
        }
    }

    return b;
}

char *dotRemoval(char *path, int len) {
    /*? On ne va pas faire exactement comme la RFC 3986, on va en effet utiliser un indice et un buffer et non deux buffers*/
    /*! Il y a quelques situations que nous allons ignorer, il faudra voir si cela posera des problèmes plus tard*/
    char *temp = malloc((len + 1) * sizeof(char));
    int j = 0; // Cet indice va nous permettre de parcourir les 2 chaines en même temps

    bool done = false;
    for (int i = 0; i < len; i++) {
        // A
        if (!done && (i + 2) < len && strncmp(path, "../", 3) == 0) {
            i += 2;
        } else if (!done && (i + 1) < len && strncmp(path, "./", 2) == 0) {
            i += 1;
        }
        // B
        else if ((i + 2) < len && strncmp(path + i, "/./", 3) == 0) {
            i += 1; // On veut garder le dernier /
        }
        // C
        else if ((i + 3) < len && strncmp(path + i, "/../", 4) == 0) {
            i += 2; // On veut garder le dernier /
            // On retire le dernier segment dans la chaine de résultat
            if (j > 0) {
                j--;
            }
            while (j > 0 && temp[j] != '/') {
                j--;
            }
        }
        // D
        else if ((i + 1) == len && path[i] == '.') {
            i += 1;
        } else if ((i + 2) == len && strncmp(path + i, "..", 2) == 0) {
            i += 2;
        }
        // E
        else {
            temp[j] = path[i];
            j++;
            i++;
            while (i < len && path[i] != '/') {
                temp[j] = path[i];
                j++;
                i++;
            }
            i--; // Peut être inutile, don't know
        }
    }

    temp[j] = '\0';

    // On rend tout cela un peu plus propre en faisant une chaine de la bonne longueur maintenant qu'on la connait
    char *result = malloc((j + 1) * sizeof(char));
    strncpy(result, temp, j);
    result[j] = '\0';

    free(temp);

    return result;
}

/*
 * Cette fonction fait une comparaison sans tenir compte de la casse de str1
 * str2 doit être tout en minuscule pour que la fonction fonctionne
 */
bool compareCaseInsensitive(char *str1, char *str2, int len) {
    bool b = true;

    for (int i = 0; i < len; i++) {
        if (str1[i] >= 'A' && str1[i] <= 'Z') {
            if (str2[i] != str1[i] - 'A' + 'a') {
                b = false;
            }
        } else if (str1[i] != str2[i]) {
            b = false;
        }
    }

    return b;
}

/*
 * Cette fonction vérifie que le paramètre q n'est pas nul
 * J'ai pris quelques libertées dans la fonction mais à priori ça fonctionne
 */
bool priorityVerification(char *str) {
    int i = 0;
    // On commence par vérifier si il y a un ;
    if (str[i] != ';') {
        return true; // Car cela veut dire qu'il n'y a pas de poids
    }
    i++;

    // Ensuite on cherche 'q'
    while (str[i] != '\0' && str[i] != 'q') {
        if (str[i] != 9 && str[i] != 32) {
            return false;
        }
        i++;
    }

    if (str[i] == '\0') {
        return false;
    }
    i++;

    if (str[i] != '\0' && str[i] != '=') {
        return false;
    }
    i++;

    // On vérifie le premier digit est soit 0 soit 1
    if (str[i] != '\0') {
        if (str[i] == '1') {
            return true;
        } else if (str[i] != '0') {
            return false;
        }
    } else {
        return false;
    }
    i++;

    if (str[i] != '\0' && str[i] != '.') {
        return false;
    }
    i++;

    if (str[i] == '\0') {
        return false;
    }

    for (int k = 0; k < 3; k++) {
        if (str[i + k] > '0' && str[i + k] <= '9') {
            return true;
        } else if (str[i + k] != '0') {
            return false;
        }
    }

    return false;
}

bool extensionMatch(const char *name, const char *ext) {
    int len = strlen(name);
    int extLen = strlen(ext);

    return len >= extLen && !strcmp(name + len - extLen, ext);
}

ContentType typeFromPath(char *path, int len) {
    char *temp = URINormalization(path, len);

    ContentType result;

    if (extensionMatch(temp, ".html")) {
        result = HTML;
    } else if (extensionMatch(temp, ".css")) {
        result = CSS;
    } else if (extensionMatch(temp, ".js")) {
        result = JAVASCRIPT;
    } else if (extensionMatch(temp, ".png")) {
        result = PNG;
    } else if (extensionMatch(temp, ".jpg") || extensionMatch(temp, ".jpeg")) {
        result = JPEG;
    } else if (extensionMatch(temp, ".gif")) {
        result = GIF;
    } else if (extensionMatch(temp, ".php")) {
        result = PHP;
    } else if (extensionMatch(temp, ".ico")) {
        result = ICO;
    } else {
        // Par défaut on renvoie un flux d'octet
        result = DEFAULT;
    }

    free(temp);

    return result;
}

char* phpPath(char *path, int lenPath, char *host, int lenHost) {
    //! Changement drastique de cette fonction, en se basant sur la RFC 3986

    char *root = "/racine/";

    // Il faut normaliser le chemin
    char *pathNormalized = URINormalization(path, lenPath);
    int newLenPath = strlen(pathNormalized);

    // Il faut normaliser le champ host
    char *hostNormalized = URINormalization(host, lenHost);
    int newLenHost = strlen(hostNormalized);

    int len = strlen(root) + newLenPath + newLenHost;
    char *fileName = malloc((len + 1) * sizeof(char));
    strcpy(fileName, root);

    strncat(fileName, hostNormalized, newLenHost);
    strncat(fileName, pathNormalized, newLenPath);

    free(pathNormalized);
    free(hostNormalized);

    return fileName;
}
