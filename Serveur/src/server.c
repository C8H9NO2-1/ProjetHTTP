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

#include "header/colours.h"

#define REPONSE1 "HTTP/1.0 200 OK\r\nContent-Type: text/html\r\n"
#define REPONSE2 "\r\n<html><head><title>Test</title></head><body><p>This is a test</p></body></html>"
#define REPONSE3 "Connection: close\r\n"

//! Pour exécuter: cat ... | ./server

int main(int argc, char *argv[]) {
    message *requete;

    int c;
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
            if (r2 != NULL) { //TODO Il faudra bien prendre en compte le fait que le header peut être le nom d'un site ou une adresse ip (dans le cas d'une @ip, il faut aller dans le else)
                valueHost = getElementValue(r2->node, &lengthHost);
                if (lengthTarget == 1 && valueTarget[0] == '/') {
                    file = defaultPath(valueHost, lengthHost);
                } else {
                     file = checkExistenceWithHost(valueTarget, lengthTarget, valueHost, lengthHost);
                }
            } else {
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

            if (!testStartLine) {
                if (version == -1) {
                    // Dans ce cas, on a une erreur de version
                }
            }

            purgeElement(&r);
            purgeElement(&r2);
            purgeElement(&r3);

            //! Interface debugging du serveur:
            //!================================
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
            /*if (semanticTransferCodings(root, encoding, version)) {
                green();
                printf("Les valeurs dans le champ Transfer-Encoding sont bien reconnues\n");
                reset();
                listeEncodage *temp = encoding;
                if (temp->value != NONE) {
                    printf("Il faut:\n");
                    while (temp != NULL) {
                        switch (temp->value) {
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
            }*/

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

            //!================================
            purgeTree(root);
        } else {
            red();
            printf("Impossible de parser la requête\n");
            reset();
        }



        //error(404, 0, "keep-alive", requete->clientId);
        reponse(200,0,"text/html",50, "liste.txt" ,"keep-alive", requete->clientId);


        //writeDirectClient(requete->clientId, REPONSE1, strlen(REPONSE1));
        //writeDirectClient(requete->clientId, REPONSE3, strlen(REPONSE3));
        //writeDirectClient(requete->clientId, REPONSE2, strlen(REPONSE2));
        /*writeDirectClient(requete->clientId, "\r\n", strlen("\r\n"));*/
        /*while ((c = getchar()) != EOF) {*/
            /*char temp[2]; */
            /*temp[0] = (char) c;*/
            /*temp[1] = '\0';*/
            /*writeDirectClient(requete->clientId, temp, 1);*/
        /*}*/
        //endWriteDirectClient(requete->clientId);
        if (connection == CLOSE) {
            requestShutdownSocket(requete->clientId);
        }

        freeRequest(requete);
    }

    return 1;
}
