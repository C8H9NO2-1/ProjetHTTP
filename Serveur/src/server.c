#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "header/request.h"
#include "header/api.h"
#include "header/pm.h"

#define REPONSE1 "HTTP/1.0 200 OK\r\nContent-Type: text/html\r\n"
#define REPONSE2 "\r\n<html><head><title>Test</title></head><body><p>This is a test</p></body></html>"
#define REPONSE3 "Connection: close\r\n"

//! Pour exécuter: cat ... | ./server

int main(int argc, char *argv[]) {
    message *requete;

    int c;
    ConnectionState connection = CLOSE;

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
            int version;
            semanticStartLine(root, &method, &version);

            /*On recupere la request-target (il n'y en a qu'une normalement)*/
            r = searchTree(root, "request-target");
            int l;
            char *s;
            s = getElementValue(r->node, &l);
            printf("%.*s\n", l, s);

            if (checkPath(s, l)) {
                printf("C'est OK\n");
            } else {
                printf("alert\n");
            }

            r2 = searchTree(root, "host");
            int l2;
            char *s2;
            s2 = getElementValue(r2->node, &l2);
            printf("%.*s\n", l2, s2);

            checkExistenceWithHost(s, l, s2, l2);

            _Token *r3 = searchTree(root, "Connection-header");
            if (r3 != NULL) {
                bool temp = semanticConnection(root, &connection, version);
            } else {
                if (version == 1) {
                    connection = KEEPALIVE;
                } else if (version == 0) {
                    connection = CLOSE;
                }
            }

            purgeElement(&r);
            purgeElement(&r2);
            purgeTree(root);
        } else {
            printf("Impossible de parser la requête\n");
        }


        writeDirectClient(requete->clientId, REPONSE1, strlen(REPONSE1));
        writeDirectClient(requete->clientId, REPONSE3, strlen(REPONSE3));
        writeDirectClient(requete->clientId, REPONSE2, strlen(REPONSE2));
        /*writeDirectClient(requete->clientId, "\r\n", strlen("\r\n"));*/
        /*while ((c = getchar()) != EOF) {*/
            /*char temp[2]; */
            /*temp[0] = (char) c;*/
            /*temp[1] = '\0';*/
            /*writeDirectClient(requete->clientId, temp, 1);*/
        /*}*/
        endWriteDirectClient(requete->clientId);
        if (connection == CLOSE) {
            requestShutdownSocket(requete->clientId);
        }

        freeRequest(requete);
    }

    return 1;
}
