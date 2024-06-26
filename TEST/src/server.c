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

#include "header/colours.h"

int main(int argc, char *argv[]) {
    message *requete;

    /*int c;*/
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

            FILE *file = NULL;
            r2 = searchTree(root, "host");
            int lengthHost = 0;
            char *valueHost = NULL;
            int dns = -1; // On veut savoir si c'est le nom d'un site web ou une @IP
            bool testHost = semanticHost(root, version, &dns);
            if (testHost && dns == 0) {
                valueHost = getElementValue(r2->node, &lengthHost);
                if (lengthTarget == 1 && valueTarget[0] == '/') {
                    file = defaultPath(valueHost, lengthHost);
                } else {
                     file = checkExistenceWithHost(valueTarget, lengthTarget, valueHost, lengthHost);
                }
            } else if (testHost) {
                file = checkExistence(valueTarget, lengthTarget);
            }

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

            purgeElement(&r);
            purgeElement(&r2);
            purgeElement(&r3);

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

                //? Transfert Encoging
                listeEncodage *encoding = malloc(sizeof(listeEncodage));
                encoding->value = NONE;
                encoding->next = NULL;
                if (semanticTransferCodings(root, encoding, version)) {
                    green();
                    printf("Les valeurs dans le champ Transfer-Encoding sont bien reconnues\n");
                    reset();
                    listeEncodage *temp = encoding;
                    if (temp->value != NONE) {
                        printf("Il faut:\n");
                        while (temp != NULL) {
                            switch (temp->value) {
                                case NONE:
                                    printf("-> Euh\n");
                                    break;
                                case CHUNKED:
                                    printf("-> decoder chunk\n");
                                    break;
                                case GZIP:
                                    printf("-> decoder gzip\n");
                                    break;
                                case DEFLATE:
                                    printf("-> decoder deflate\n");
                                    break;
                                case COMPRESS:
                                    printf("-> decoder compress\n");
                                    break;
                            }
                            temp = temp->next;
                        }
                        purgeListeEncodage(&encoding);
                    }
                } else {
                    red();
                    printf("Les valeurs du champ Transfer-Encoding ne sont pas reconnues\n");
                    reset();
                }

                //? Accept
                ContentType ressourceType = typeFromPath(valueTarget, lengthTarget);
                if (acceptHeaderVerification(root, ressourceType)) {
                    green();
                    printf("Le client accepte le type de notre représentation\n");
                    reset();
                    printf("Celle-ci est: ");
                    blue();
                    switch (ressourceType) {
                        case HTML:
                            printf("text/html\n");
                            break;
                        case CSS:
                            printf("text/css\n");
                            break;
                        case JAVASCRIPT:
                            printf("text/javascript\n");
                            break;
                        case PNG:
                            printf("image/png\n");
                            break;
                        case JPEG:
                            printf("image/jpeg\n");
                            break;
                        case GIF:
                            printf("image/gif\n");
                            break;
                    }
                    reset();
                } else {
                    red();
                    printf("Le client n'accepte pas le type de notre représentation\n");
                    reset();
                }

                //? Accept-Encoding
                EncodingState coding;
                if (acceptEncodingHeaderVerification(root, &coding)) {
                    green();
                    printf("Le client accepte l'encoding de notre représentation (il n'y en a pas)\n");
                    reset();
                } else {
                    red();
                    printf("Le client n'accepte pas l'encoding de notre représentation\n");
                    reset();
                    printf("Cependant, il accepte l'encoding suivant: ");
                    blue();
                    switch (coding) {
                        case GZIP:
                            printf("gzip\n");
                            break;
                        case DEFLATE:
                            printf("deflate\n");
                            break;
                        case COMPRESS:
                            printf("compress\n");
                            break;
                        default:
                            red();
                            printf("Erreur lors de la vérification de Accept-Encoding\n");
                            break;
                    }
                    reset();
                }
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
            }
            //!================================
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
