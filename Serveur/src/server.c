#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "header/request.h"
#include "header/response.h"
#include "header/api.h"
#include "header/elisa.h"
#include "header/pm.h"
#include "header/fileLen.h"
#include "header/tom.h"
#include "header/phpServer.h"

#include "header/colours.h"

#define REPONSE1 "HTTP/1.0 200 OK\r\nContent-Type: text/html\r\n"
#define REPONSE2 "\r\n<html><head><title>Test</title></head><body><p>This is a test</p></body></html>"
#define REPONSE3 "Connection: close\r\n"

int main(int argc, char *argv[]) {
    message *requete;

    ConnectionState connection = CLOSE;

    system("ls racine > liste.txt");

    while (true) {
        if ((requete = getRequest(8080)) == NULL) {
            return -1;
        }

        printf("###########################################\n");
        printf("Demande reçue depuis le client %d\n", requete->clientId);
        printf("Client [%d] [%s:%d]\n", requete->clientId, inet_ntoa(requete->clientAddress->sin_addr), htons(requete->clientAddress->sin_port));
        printf("Contenu de la demande %.*s\n\n", requete->len, requete->buf);

        printf("===========\n");

        if (parseur(requete->buf, requete->len) != 0) {
            _Token *r, *r2;
            void *root;

            root = getRootTree();

            Method method;
            int version = 0;
            bool testStartLine = semanticStartLine(root, &method, &version);

            /*On recupere la request-target (il n'y en a qu'une normalement)*/
            r = searchTree(root, "request-target");
            int lengthTarget;
            char *valueTarget;
            valueTarget = getElementValue(r->node, &lengthTarget);
            /*printf("%.*s\n", l, s);*/

            FILE *file = NULL;
            r2 = searchTree(root, "host");
            int lengthHost = 0;
            char *valueHost = NULL;
            int dns = -1; // On veut savoir si c'est le nom d'un site web ou une @IP
            bool testHost = semanticHost(root, version, &dns);
            if (testHost && dns == 0) { //TODO Il faudra bien prendre en compte le fait que le header peut être le nom d'un site ou une adresse ip (dans le cas d'une @ip, il faut aller dans le else)
                valueHost = getElementValue(r2->node, &lengthHost);
                if (lengthTarget == 1 && valueTarget[0] == '/') {
                    //! Si on est sur le site PHP, il faut donner un fichier
                    //! par défaut différent de index.html que l'on veut
                    //! protéger
                    if (strncmp(valueHost, "www.testphp.fr", lengthHost) == 0) {
                        file = NULL;
                        const char *temp = "/main.php";
                        const int lengthTemp = strlen(temp);
                        valueTarget = malloc(lengthTemp * sizeof(char));
                        strncpy(valueTarget, temp, lengthTemp);
                        lengthTarget = lengthTemp;
                    }
                    file = defaultPath(valueHost, lengthHost);
                } else {
                     file = checkExistenceWithHost(valueTarget, lengthTarget, valueHost, lengthHost);
                }
            } else if (testHost) {
                file = checkExistence(valueTarget, lengthTarget);
            }
            /*printf("%.*s\n", l2, s2);*/

            _Token *r3 = searchTree(root, "Connection-header");
            if (r3 != NULL) {
                semanticConnection(root, &connection, version);
            } else {
                if (version == 1) {
                    connection = KEEPALIVE;
                } else if (version == 0) {
                    connection = CLOSE;
                }
            }
            
            //! On renvoie les potentielles erreurs
            //!====================================

            bool errorSent = false; // Si on a envoyé une erreur, on ne doit pas en envoyer une autre

            red();
            //? On récupère la méthode pour savoir si on doit envoyer du contexte ou non
            bool get = false;
            if (method == GET) {
                get = true;
            } else {
                get = false;
            }
            //? On récupère aussi l'état de la connection
            char close[50];
            if (connection == KEEPALIVE) {
                const char *temp = "keep-alive";
                strcpy(close, temp);
                close[strlen(temp)] = '\0';
            } else {
                const char *temp = "close";
                strcpy(close, temp);
                close[strlen(temp)] = '\0';
            }
            //? Erreur dans la startline
            if (!testStartLine) {
                errorSent = true;
                if (version == -1) {
                    // Dans ce cas, on a une erreur de version
                    printf("\nErreur 505\n\n");
                    error(505, 1, close, requete->clientId, get);
                } else {
                    // On a une erreur de méthode
                    printf("\nErreur 501\n\n");
                    error(501, version, close, requete->clientId, get);
                }
            }

            //? Erreur dans le champ host
            if (!errorSent && !testHost) {
                printf("\nErreur 400\n\n");
                error(400, version, close, requete->clientId, get);
                errorSent = true;
            }

            //? Erreur dans la request-target
            if (!errorSent && file == NULL) {
                printf("\nErreur 404\n\n");
                error(404, version, close, requete->clientId, get);
                errorSent = true;
            }
            
            reset();


            //? Si on a envoyé une erreur, on ne fait aucune autre vérification
            //! Interface debugging du serveur:
            //!================================
            if (!errorSent) {
                printf("\n\n==========\n");

                //? Méthode
                printf("Méthode demandée par le client: ");
                green();
                switch (method) {
                    case GET:
                        printf("GET\n");
                        break;
                    case HEAD:
                        printf("HEAD\n");
                        break;
                    case POST:
                        printf("POST\n");
                        break;
                }
                reset();

                //? Ressource demandée
                if (file != NULL) {
                    green();
                    printf("La ressource demandée existe sur le serveur\n");
                    reset();
                } else {
                    red();
                    printf("La ressource demandée n'existe pas sur le serveur\n");
                    reset();
                }

                //? Version HTTP
                switch (version) {
                    case 0:
                        printf("La version du client est HTTP/1.0\n");
                        break;
                    case 1:
                        printf("La version du client est supérieure ou égale à HTTP/1.1\n");
                        break;
                    default:
                        red();
                        printf("La version du client nous est inconnue\n");
                        reset();
                        break;
                }

                //? Gestion de la connexion
                purple();
                switch (connection) {
                    case CLOSE:
                        printf("Il faut fermer la connexion après la réponse\n");
                        break;
                    case KEEPALIVE:
                        printf("Il ne faut pas fermer la connexion après la réponse\n");
                        break;
                }
                reset();

                //? Accept
                ContentType ressourceType = typeFromPath(valueTarget, lengthTarget);
                if (file == NULL) {
                    ressourceType = PHP;
                }

                //? On commence par vérifier si on doit traiter une requête PHP
                bool php = false;
                if (ressourceType == PHP) {
                    printf("Il faut traiter le code PHP du fichier\n");
                    php = true;
                }

                //! Pour le moment on fait comme ça, même si ce n'est pas terrible
                if (!php) {
                    //? Si tout c'est bien passé on envoie la réponse au client
                    //? On a beaucoup de choses à vérifier
                    //? La connection est déjà faite au moment des erreurs (dans close)
                    //? On récupère ensuite le type du fichier
                    char type[50];
                    if (ressourceType == HTML) {
                        const char *temp = "text/html";
                        strcpy(type, temp);
                        type[strlen(temp)] = '\0';
                    } else if (ressourceType == CSS) {
                        const char *temp = "text/css";
                        strcpy(type, temp);
                        type[strlen(temp)] = '\0';
                    } else if (ressourceType == JAVASCRIPT) {
                        const char *temp = "text/javascript";
                        strcpy(type, temp);
                        type[strlen(temp)] = '\0';
                    } else if (ressourceType == PNG) {
                        const char *temp = "image/png";
                        strcpy(type, temp);
                        type[strlen(temp)] = '\0';
                    } else if (ressourceType == JPEG) {
                        const char *temp = "image/jpeg";
                        strcpy(type, temp);
                        type[strlen(temp)] = '\0';
                    } else if (ressourceType == GIF) {
                        const char *temp = "image/gif";
                        strcpy(type, temp);
                        type[strlen(temp)] = '\0';
                    } else if (ressourceType == ICO) {
                        const char *temp = "image/vnd.microsoft.icon";
                        strcpy(type, temp);
                        type[strlen(temp)] = '\0';
                    } else if (ressourceType == DEFAULT) {
                        const char *temp = "application/octet-stream";
                        strcpy(type, temp);
                        type[strlen(temp)] = '\0';
                    }

                    long int length = 0;
                    if (file != NULL) {
                        length = calulContenLen(file);
                    }
                    if (method == GET) {
                        reponse2(200, version, type, length, file, close, requete->clientId);
                    } else {
                        reponse2(200, version, type, length, NULL, close, requete->clientId);
                    }
                } else {
                    //! Si on traite une requête PHP, on envoie une réponse vide au client
                    //! C'est temporaire
                    if (method == POST) {
                        // On recupere le payload du message
                        _Token *rP;
                        rP = searchTree(root, "message-body");
                        int lengthBody;
                        char *valueBody;
                        valueBody = getElementValue(rP->node, &lengthBody);
                        printf("%.*s\n", lengthBody, valueBody);

                        char *valueBodyCopy = malloc((lengthBody + 1) * sizeof(char));
                        strncpy(valueBodyCopy, valueBody, lengthBody);
                        valueBodyCopy[lengthBody] = '\0';

                        // On récupère le Content-Type header
                        _Token *rCT;
                        rCT = searchTree(root, "media-type");
                        int lengthContentType;
                        char *valueContentType;
                        valueContentType = getElementValue(rCT->node, &lengthContentType);
                        /*printf("%.*s\n", lengthContentType, valueContentType);*/

                        // On récupère le Content-Length header
                        _Token *rCL;
                        rCL = searchTree(root, "Content-Length");
                        int lengthContentLength;
                        char *valueContentLength;
                        valueContentLength = getElementValue(rCL->node, &lengthContentLength);
                        /*printf("%.*s\n", lengthContentLength, valueContentLength);*/

                        // On récupère le Cookie-header
                        if (!semanticCookie(root)) {
                            red();
                            printf("Erreur => Cookie header faux\n");
                            reset();
                        }
                        _Token *rC;
                        rC = searchTree(root, "cookie-pair");
                        int lengthCookie;
                        char *valueCookie;
                        valueCookie = getElementValue(rC->node, &lengthCookie);
                        printf("%.*s\n", lengthCookie, valueCookie);

                        if (lengthContentType == 0) {
                            error(400, 1, close, requete->clientId, true);
                        } else if (lengthContentLength == 0) {
                            error(411, 1, close, requete->clientId, true);
                        } else {
                            char *path = phpPath(valueTarget, lengthTarget, valueHost, lengthHost);
                            phpServerResponse(path, 1, "close", method, requete->clientId, valueBodyCopy, valueContentLength, lengthContentLength,
                                    valueContentType, lengthContentType, valueCookie, lengthCookie);
                        }
                        
                        purgeElement(&rP);
                        purgeElement(&rCT);
                        purgeElement(&rCL);
                    } else {
                        // Sinon on ne passe rien dans les stdin

                        _Token *rC;
                        rC = searchTree(root, "cookie-pair");
                        int lengthCookie;
                        char *valueCookie;
                        if (rC == NULL) {
                            valueCookie = NULL;
                            lengthCookie = 0;
                        } else {
                            valueCookie = getElementValue(rC->node, &lengthCookie);
                            printf("%.*s\n", lengthCookie, valueCookie);
                        }
                        char *path = phpPath(valueTarget, lengthTarget, valueHost, lengthHost);
                        phpServerResponse(path, 1, "close", method, requete->clientId, NULL, NULL, 0, NULL, 0, valueCookie, lengthCookie);
                    }
                    /*reponse2(200, version, "text/html", 0, NULL, close, requete->clientId);*/
                }

            }
            //!================================
            purgeElement(&r);
            purgeElement(&r2);
            purgeElement(&r3);

            purgeTree(root);
        } else {
            red();
            printf("Impossible de parser la requête\n");
            error(400, 1, "close", requete->clientId, false);
            reset();
        }

        if (connection == CLOSE) {
            requestShutdownSocket(requete->clientId);
        }

        freeRequest(requete);
    }

    return 1;
}
