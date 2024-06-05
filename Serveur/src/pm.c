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
#include <math.h>

#include "header/api.h"
#include "header/elisa.h"
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
 * Cette fonction vérifie que l'utilisateur reste bien dans le dossier dans lequel il a le droit d'être
 * ! Cette fonction est obsolète maintenant que l'algorithme de dot removal à été implémenté
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

    char defaultFile[100];
    if (strncmp(host, "www.testphp.com", len) == 0) {
        strcpy(defaultFile, "/main.php");
    } else {
        strcpy(defaultFile, "/index.html");
    }
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
 * Cette fonction vérifie que le type de la ressource demandée correspond bien à un champ dans le header Accept
 * On pourrait avoir plusieurs représentations d'une même ressource mais pour le moment non
 * Donc cette fonction est relativement simple
 * ? Il faudra peut-être ajouter un paramètre pour le type du contenu
 */
bool acceptHeaderVerification(void *root, ContentType content) {
    _Token *r = searchTree(root, "header-field");
    _Token *token = r;

    char comp[] = "accept:";
    int len = strlen(comp);

    bool b = false;
    bool found = false;

    while (token != NULL) {
        int l;
        char *s;
        s = getElementValue(token->node, &l);
        if (compareCaseInsensitive(s, comp, len)) {
            found = true;
            /*printf("=> %.*s\n", l, s);*/
            char *header = malloc((l + 1) * sizeof(char));
            sprintf(header, "%.*s", l, s);
            // On regarde si le pattern */* est présent
            // Si c'est le cas alors c'est gagné
            if (auxAccept(header, "*/*")) {
                b = true;
            }

            // Sinon on regarde les autres patterns en se basant sur le type de la ressource
            // On fait deux vérifications:
            // - Une première qui sera un peu générique
            // - Une deuxième plus fine
            switch (content) {
                case HTML:
                case CSS:
                case JAVASCRIPT:
                    if (auxAccept(header, "text/*")) {
                        b = true;
                    }
                    break;
                case PNG:
                case JPEG:
                case GIF:
                    if (auxAccept(header, "image/*")) {
                        b = true;
                    }
                    break;
            }
            switch (content) {
                case HTML:
                    if (auxAccept(header, "text/html")) {
                        b = true;
                    }
                    break;
                case CSS:
                    if (auxAccept(header, "text/css")) {
                        b = true;
                    }
                    break;
                case JAVASCRIPT:
                    if (auxAccept(header, "text/javascript")) {
                        b = true;
                    }
                    break;
                case PNG:
                    if (auxAccept(header, "image/png")) {
                        b = true;
                    }
                    break;
                case JPEG:
                    if (auxAccept(header, "image/jpeg")) {
                        b = true;
                    }
                    break;
                case GIF:
                    if (auxAccept(header, "image/gif")) {
                        b = true;
                    }
                    break;
            }
            free(header);
        }
        token = token->next;
    }

    purgeElement(&r);

    if (found) {
        return b;
    }

    // Si il n'y a pas de header Accept, c'est qu'on est bon
    return true;
}

bool auxAccept(char *header, const char *str) {
    char *temp;
    if ((temp = strstr(header, str)) != NULL) {
        if (priorityVerification(temp + strlen(str))) {
            return true;
        }
    }

    return false;
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
    } else {
        // Par défaut on renvoie une page html
        result = HTML;
    }

    free(temp);

    return result;
}

/*
 * Le type de retour de cette fonction devra être changé plus tard
 * Cette fonction détermine l'encoding à utiliser en fonction de ce que le client demande
 */
bool acceptEncodingHeaderVerification(void *root, EncodingState *coding) {
    // À priori, nous n'avons aucun encoding pour nos ressources
    // cependant si le client refuse que la ressource soit envoyé sans content-coding,
    // alors il va falloir coder la ressource avec les options que l'on a à notre disposition

    // On cherche pour savoir si l'encoding identity (sans encoding) a une valeur de préférence nulle
    _Token *r, *token;
    r = searchTree(root, "header-field");
    token = r;

    char comp[] = "accept-encoding:";
    int len = strlen(comp);

    bool b = false;
    bool found = false;

    while (token != NULL) {
        int l;
        char *s;
        s = getElementValue(token->node, &l);
        if (compareCaseInsensitive(s, comp, len)) {
            found = true;
            /*printf("=> %.*s\n", l, s);*/
            char *header = malloc((l + 1) * sizeof(char));
            sprintf(header, "%.*s", l, s);

            // On veut d'abord vérifier le cas le plus simple (la présence du champ identity avec une valeur non nulle)
            char *temp = strstr(header, "identity");
            if (temp != NULL) {
                float priority = priorityValue(temp + strlen("identity"));
                b = priority != 0;
                if (!b) {
                    // Il faut vérifier l'encoding acceptable
                    *coding = maximumPriority(header);
                }
            } else if (auxAcceptEncoding(header)) {
                // On doit ensuite vérifier que le symbole * n'est pas présent avec une priorité nulle
                // On ne peut donc pas tout à fait utiliser la fonction auxAccept car ce n'est pas son job
                b = true;
            } else {
                // Finalement, on renvoie faux mais on doit déterminer l'encodage prioritaire à utiliser parmi les trois (gzip, compress, deflate)
                b = false;
                *coding = maximumPriority(header);
            }
            free(header);
        }
        token = token->next;
    }

    purgeElement(&r);

    if (found) {
        return b;
    }

    return true;
}

/*
 * Cette fonction sert uniquement à vérifier si le symbole * n'est pas présent avec une priorité nulle
 */
bool auxAcceptEncoding(char *header) {
    char *temp;
    char *str = "*";
    if ((temp = strstr(header, str)) != NULL) {
        // Si le symbole est présent, on vérifie que la priorité est non nulle
        if (priorityVerification(temp + strlen(str))) {
            return true;
        } else {
            return false;
        }
    }
    return true;
}

/*
 * Cette fonction n'est pas codé pour être utilisable autre que pour Accept-Encoding
 * Ce n'est pas terrible mais c'est pour ne pas trop me compliquer la vie
 */
EncodingState maximumPriority(char *header) {
    EncodingState coding = GZIP;
    float values[3] = {0, 0, 0};

    char *temp;
    char *strs[3] = { "gzip", "deflate", "compress" };

    for (int i = 0; i < 3; i++) {
        if ((temp = strstr(header, strs[i])) != NULL) {
            values[i] = priorityValue(temp + strlen(strs[i]));
        }
    }

    int maxIndex = 0;
    for (int i = 1; i < 3; i++) {
        if (values[i] > values[maxIndex]) {
            maxIndex = i;
        }
    }

    switch (maxIndex) {
        case 0:
            coding = GZIP;
            break;
        case 1:
            coding = DEFLATE;
            break;
        case 2:
            coding = COMPRESS;
            break;
        default:
            printf("Erreur critique dans la fonction maximumPriority\n");
    }

    return coding;
}

float priorityValue(char *str) {
    int i = 0;
    // On commence par vérifier si il y a un ;
    if (str[i] != ';') {
        return 1; // Car cela veut dire qu'il n'y a pas de poids
    }
    i++;

    // Ensuite on cherche 'q'
    while (str[i] != '\0' && str[i] != 'q') {
        if (str[i] != 9 && str[i] != 32) {
            return 0; // Car c'est mal écrit donc on ne peut en tirer de conclusion
        }
        i++;
    }

    if (str[i] == '\0') {
        return 0;
    }
    i++;

    if (str[i] != '\0' && str[i] != '=') {
        return 0;
    }
    i++;

    // On vérifie le premier digit est soit 0 soit 1
    if (str[i] != '\0') {
        if (str[i] == '1') {
            return 1;
        } else if (str[i] != '0') {
            return 0;
        }
    } else {
        return 0;
    }
    i++;

    if (str[i] != '\0' && str[i] != '.') {
        return 0;
    }
    i++;

    if (str[i] == '\0') {
        return 0;
    }

    // Il faut calculer la valeur de retour
    float result = 0;
    for (int k = 0; k < 3; k++) {
        if (str[i + k] >= '0' && str[i + k] <= '9') {
            result += (float) (str[i + k] - '0') / pow(10, k + 1);
        } else {
            return result;
        }
    }

    return result;
    return 0;
}
