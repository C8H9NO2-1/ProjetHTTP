#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "header/request.h"
#include "header/api.h"
#include "header/checkRessource.h"

int main(int argc, char *argv[]) {
    /*char req[] = "GET /index.html HTTP/1.1\r\nHost: 127.0.0.1\r\n\r\n";*/
    char req[] = "GET /test2 HTTP/1.1\r\nHost: www.wichopool.com\r\n\r\n";

    printf("%s", req);
    printf("===========\n");

    if (parseur(req, strlen(req)) != 0) {
        _Token *r, *r2;
        void *root;

        root = getRootTree();

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

        /*bool b = checkExistenceWithHost(s, l, s2, l2);*/
        /*bool b = checkExistence(s, l);*/
        bool b = defaultPath(s2, l2);

        if (b) {
            printf("La ressource à été identifiée\n");
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
