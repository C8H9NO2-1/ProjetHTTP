#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

/*#include "../1.3.1/include/zlib.h"*/

#include "header/request.h"
#include "header/api.h"
#include "header/elisa.h"
#include "header/pm.h"
#include "header/ismail.h"
#include "header/fastcgi.h"
#include "header/begin.h"
#include "header/FCGI_param.h"

#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "header/phpResponse.h"

static int createSocket(char *ip, int port) {
    int fd;
    struct sockaddr_in serv_addr;
    int enable = 1;

    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket creation failed\n");
        return (-1);
    }

    bzero(&serv_addr, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    inet_aton(ip, (struct in_addr *)&serv_addr.sin_addr.s_addr);
    /*inet_aton(ip,(struct sockaddr *)&serv_addr.sin_addr.s_addr);*/
    serv_addr.sin_port = htons(port);

    if (connect(fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("connect failed\n");
        return (-1);
    }

    return fd;
}

int main(int argc, char *argv[]) {

    int test = createSocket("127.0.0.1", 9000);

    printf("%d\n", test);

    int longueurBegin=0;
    int longueurStdin=0;
    FCGI_Header *begin =  beginRequest(&longueurBegin);
    FCGI_Header *stdin = stdinRequest(&longueurStdin, NULL);
    write(test, begin, longueurBegin);
    write(test, stdin, longueurStdin);
    free(begin);

    readPHPResponse(test);

    return 0;

    /*// Au lancement du serveur, on inspecte la structure de celui-ci*/
    /*system("ls racine > liste.txt");*/
    /**/
    /*/*char req[] = "GET /index.html HTTP/1.1\r\nHost: 127.0.0.1\r\n\r\n";*/
    /*char req[] = "GET /../inDex%2ehtmL HTTP/1.1\r\nHost: www.hilopt.com\r\nAccept: text/plain; q=0.5, text/html, text/x-dvi; q=0.8, text/x-c\r\n\*/
    /*                    Connection: close\r\nContent-length:	 555 \r\nContent-length:	 55\r\n\r\n";*/
    /**/
    /*/*char req[] = "GET /test/../test1 HTTP/1.1\r\nHost: www.wichopool.com\r\nConnection: keep-alive\r\n\r\n";*/
    /*/*char req[] = "GET /../ParseurHTTP/main.c HTTP/1.1\r\nHost: www.wichopool.com\r\n\r\n";*/
    /**/
    /*printf("%s", req);*/
    /*printf("===========\n");*/
    /**/
    /*if (parseur(req, strlen(req)) != 0) {*/
    /*    _Token *r, *r2, *r3;*/
    /*    void *root;*/
    /**/
    /*    root = getRootTree();*/
    /**/
    /*    // On vérifie la sémantique de la start-line et on en profite pour récupérer les informations*/
    /*    Method method;*/
    /*    int version; // contient le numéro de la version mineure*/
    /*    if (semanticStartLine(root, &method, &version)) {*/
    /*        printf("Sémantique de la start-line validée\n");*/
    /*    } else {*/
    /*        printf("Erreur lors de la vérification de la sémantique\n");*/
    /*    }*/
    /**/
    /*    if (semanticCookie(root)){*/
    /*        printf("Sémantique du cookie validée\n");*/
    /*    }*/
    /*    else {*/
    /*        printf("Erreur lors de la vérification de la sémantique cookie\n");*/
    /*    }*/
    /*    listeEncodage *listeCodeAFaire = malloc(sizeof(listeEncodage));*/
    /*    if (semanticTransferCodings(root, listeCodeAFaire, version)){*/
    /*        printf("Sémantique du transfer codings validée\n");*/
    /*    }*/
    /*    else {*/
    /*        printf("Erreur lors de la vérification de la sémantique transfer-encoding\n");*/
    /*    }*/
    /*    if (semanticContentLength(root)){*/
    /*        printf("semantique du content length validee\n");*/
    /*    }*/
    /*    else {*/
    /*        printf("Erreur lors de la vérification de la sémantique Content length\n");*/
    /*    }*/
    /**/
    /**/
    /*    /*On recupere la request-target (il n'y en a qu'une normalement)*/
    /*    /*De plus, nous sommes certains qu'il y a une start-line grace au parseur*/
    /*    r = searchTree(root, "request-target");*/
    /*    int l;*/
    /*    char *s;*/
    /*    s = getElementValue(r->node, &l);*/
    /*    /*printf("%.*s\n", l, s);*/
    /**/
    /*    /*Il faudra penser à vérifier qu'il y a un champ Host, car ce n'est pas certain*/
    /*    r2 = searchTree(root, "host");*/
    /*    int l2;*/
    /*    char *s2;*/
    /*    s2 = getElementValue(r2->node, &l2);*/
    /**/
    /*    // On vérifie si il y a un champ de connexion*/
    /*    // Et on implémente le début de la gestion de la connexion*/
    /*    // Le reste sera fait lors de l'envoi de la réponse*/
    /*    ConnectionState connection;*/
    /*    r3 = searchTree(root, "Connection-header");*/
    /*    if (r3 != NULL) {*/
    /*        bool temp = semanticConnection(root, &connection, version);*/
    /*        if (temp) {*/
    /*            printf("Option de connexion identifiée\n");*/
    /*        } else {*/
    /*            printf("Option de connexion non identifiée\n");*/
    /*        }*/
    /*    } else {*/
    /*        if (version == 1) {*/
    /*            connection = KEEPALIVE;*/
    /*        } else if (version == 0) {*/
    /*            connection = CLOSE;*/
    /*        }*/
    /*    }*/
    /**/
    /*    ContentType ressourceType = typeFromPath(s, l);*/
    /*    if (acceptHeaderVerification(root, ressourceType)) {*/
    /*        printf("Le client accepte notre réprésentation\n");*/
    /*    } else {*/
    /*        printf("Le client ne veut pas de notre réprésentation\n");*/
    /*    }*/
    /**/
    /*    EncodingState coding;*/
    /*    if (acceptEncodingHeaderVerification(root, &coding)) {*/
    /*        printf("Le client accepte l'encoding de notre représentation\n");*/
    /*    } else {*/
    /*        printf("Le client ne veut pas de l'encoding de notre représentation\n");*/
    /*        printf("Mais il accepte l'encoding suivant: ");*/
    /*        switch (coding) {*/
    /*            case GZIP:*/
    /*                printf("gzip");*/
    /*                break;*/
    /*            case DEFLATE:*/
    /*                printf("deflate");*/
    /*                break;*/
    /*            case COMPRESS:*/
    /*                printf("compress");*/
    /*                break;*/
    /*            default:*/
    /*                printf("Grosse erreur dans la vérification du l'encoding\n");*/
    /*        }*/
    /*        printf("\n");*/
    /*    }*/
    /**/
    /*    FILE *file = checkExistenceWithHost(s, l, s2, l2);*/
    /*    /*FILE *file = checkExistence(s, l);*/
    /*    /*FILE *file= defaultPath(s2, l2);*/
    /**/
    /*    if (file != NULL) {*/
    /*        printf("La ressource à été identifiée\n");*/
    /*        fclose(file);*/
    /*    } else {*/
    /*        printf("Impossible d'identifier la ressource\n");*/
    /*    }*/
    /**/
    /*    purgeElement(&r);*/
    /*    purgeElement(&r2);*/
    /*    purgeElement(&r3);*/
    /*    purgeTree(root);*/
    /*} else {*/
    /*    printf("Impossible de parser la requête\n");*/
    /*}*/
}
