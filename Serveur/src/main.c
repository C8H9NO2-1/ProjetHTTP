#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "header/api.h"
#include "header/checkRessource.h"

int main(int argc, char *argv[]) {
    char req[] = "GET /test/../index.html HTTP/1.1\r\nHost: www.hilopt.com\r\n\r\n";

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

        checkExistence(s, l, s2, l2);

        purgeElement(&r);
        purgeElement(&r2);
        purgeTree(root);
    }

    return 0;
}
