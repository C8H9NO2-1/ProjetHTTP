#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "header/request.h"

#define REPONSE1 "HTTP/1.0 200 OK\r\n"
#define REPONSE2 "\r\n"

int main(int argc, char *argv[]) {
    message *requete;

    while (true) {
        if ((requete = getRequest(8080)) == NULL) {
            return -1;
        }

        printf("###########################################\n");
        printf("Demande reçue depuis le client %d\n", requete->clientId);
        printf("Client [%d] [%s:%d]\n", requete->clientId, inet_ntoa(requete->clientAddress->sin_addr), htons(requete->clientAddress->sin_port));
        printf("Contenu de la demande %.*s\n\n", requete->len, requete->buf);

        writeDirectClient(requete->clientId, REPONSE1, strlen(REPONSE1));
        writeDirectClient(requete->clientId, REPONSE2, strlen(REPONSE2));
        endWriteDirectClient(requete->clientId);
        requestShutdownSocket(requete->clientId);

        freeRequest(requete);
    }

    return 1;
}
