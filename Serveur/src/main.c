#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "header/request.h"
#include "header/api.h"
#include "header/pm.h"

int main(int argc, char *argv[]) {
    // Au lancement du serveur, on inspecte la structure de celui-ci
    system("ls racine > liste.txt");

    /*char req[] = "GET /index.html HTTP/1.1\r\nHost: 127.0.0.1\r\n\r\n";*/
    char req[] = "GET /index.html HTTP/1.1\r\nHost: www.wichopool.com\r\nConnection: keep-alive\r\n\r\n";

    printf("%s", req);
    printf("===========\n");

    if (parseur(req, strlen(req)) != 0) {
        _Token *r, *r2;
        void *root;

        root = getRootTree();

        // On vérifie la sémantique de la start-line et on en profite pour récupérer les informations
        Method method;
        int version; // contient le numéro de la version mineure
        if (semanticStartLine(root, &method, &version)) {
            printf("Sémantique de la start-line validée\n");
        } else {
            printf("Erreur lors de la vérification de la sémantique\n");
        }

        /*On recupere la request-target (il n'y en a qu'une normalement)*/
        r = searchTree(root, "request-target");
        int l;
        char *s;
        s = getElementValue(r->node, &l);
        /*printf("%.*s\n", l, s);*/

        if (checkPath(s, l)) {
            printf("C'est OK\n");
        } else {
            printf("alert\n");
        }

        r2 = searchTree(root, "host");
        int l2;
        char *s2;
        s2 = getElementValue(r2->node, &l2);
        /*printf("%.*s\n", l2, s2);*/

        /*FILE *file = checkExistenceWithHost(s, l, s2, l2);*/
        /*FILE *file = checkExistence(s, l);*/
        FILE *file= defaultPath(s2, l2);

        if (file != NULL) {
            printf("La ressource à été identifiée\n");
            fclose(file);
        } else {
            printf("Impossible d'identifier la ressource\n");
        }

        purgeElement(&r);
        purgeElement(&r2);
        purgeTree(root);
    } else {
        printf("Impossible de parser la requête\n");
    }

    return 0;
}
