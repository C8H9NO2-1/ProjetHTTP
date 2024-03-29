#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>

#include "structure.h"
#include "affichage.h"
#include "api.h"

/// Incluez ici les fichiers d'entête nécessaires pour l'execution de ce programme. 
/// La fonction parseur doit être dans un autre fichier .c
/// N'ajouter aucun autre code dans ce fichier.

int main(int argc, char *argv[]) {

    char req[]="GET / HTTP/1.0\r\nHost: www.google.com\r\nTRaNSFEr-enCODiNG:  ,20Kmy%d_Yg8t.MS; 	eh6owY1JpoLr99n=\"k_*	lx:H~¥ rV\\-\\R\\	!Pa,v\"\r\nCOoKie:	'Ym!%pdVb-s~Ue6=\".vl/vjesB-<bn0q\"\r\nexPect:   100-CoNtinUE		\r\ncONneCtion:			  iuYlKmnz&ODppri	 , bdGs_7i|Ik2Ul%y	,	p~TuU2FuerMHNq*,	 \r\n\r\n\¥íÄ“±PGÒ!î‘â¦Ý";
    // char req[] = "GET / HTTP/1.0\r\nCONNeCtION:, 		, ,5_p8ck0lJ|vM-lx	,\r\nCONNeCtION:, 		, ,5_p8ck0lJ|vM-lx	,\r\n";
    printf("length => %lu\n", strlen(req));

    if (argc != 2 ) {
        printf("usage: %s <rulename>\n",argv[0]); 
        return 0; 
    }
    if (parseur(req,strlen(req))) {
        _Token *r,*tok;
        void *root; 
        root=getRootTree();

        // printArbre(root, 0);
    // }

        r=searchTree(root,argv[1]); 
        tok=r;
        while (tok) {
            int l;
            char *s; 
            s=getElementValue(tok->node,&l); 
            printf("FOUND [%.*s]\n",l,s); 
            tok=tok->next;
        } 
        purgeElement(&r); 
        purgeTree(root) ;
    } else {
        return 0; 
    }
    return 1; 
}