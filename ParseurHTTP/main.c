#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "file.h"
#include "api.h"

/// Incluez ici les fichiers d'entête nécessaires pour l'execution de ce programme.
/// La fonction parseur doit être dans un autre fichier .c
/// N'ajouter aucun autre code dans ce fichier.

int main(int argc, char *argv[]) {

    // char req[]="GET /index.html HTTP/1.0\r\nHost: www.google.com\r\nTRaNSFEr-enCODiNG:  ,20Kmy%d_Yg8t.MS; 	eh6owY1JpoLr99n=\"k_*	lx:H~¥ rV\\-\\R\\	!Pa,v\"\r\nCOoKie:	'Ym!%pdVb-s~Ue6=\".vl/vjesB-<bn0q\"\r\nexPect:   100-CoNtinUE		\r\ncONneCtion:			  iuYlKmnz&ODppri	 , bdGs_7i|Ik2Ul%y	,	p~TuU2FuerMHNq*,	 \r\n\r\n\¥íÄ“±PGÒ!î‘â¦Ý";
    // char req[] = "C /V%D1/$P/%46?gZ? HTTP/3.7\r\nTransfer-Encoding: 	,compress		   		, 				  	uw7!3				   ;3~E	 		 			   =   	 	    	 \"\\´>‹Ø\" 	  		 	;	 			  	k = \"\\Ì\\ó˙\\…⁄\\U\" ,	\r\nHost: 	 			  25.222.77.231	\r\ncOntenT-TYpE:     XWsaqXcenry0vyS/Ey!g3$TdM_WdMB%; SL`Nr-LeQ!ygkKS=^ZvA4%%QuytrP0%  	\r\nConnection:,   	 ,  	 ,,	 	, 		0juQdM_,  	    	   	l2xVM&D4		,  	  	,		^mWhhr.,   ,+ ,E  ,		,,	   		#	\r\nContent-length:	 55	 \r\nCookie:4=~a; 8D=; kp3YvMrh=""; xGj=\"]XA\"; dY=\"\"; '+K=\"-\"\r\nHost:		 	[9::6:235.228.253.155]				\r\nHost:250.0.0.239  							  	\r\nl97w: 	   	‡∂	 			  	 	ê\r\n					 	 R	 \r\nj7:\r\n\r\n";
    // char req[] = "GET / HTTP/1.0\r\nCONNeCtION:, 		, ,5_p8ck0lJ|vM-lx	,\r\nCONNeCtION:, 		, ,5_p8ck0lJ|vM-lx	,\r\n";

    if (argc != 2 ) {
        printf("usage: %s <rulename>\n", argv[0]);
        return 0;
    }

    char name[100] = "testFile/test";

    for (int i = 0; i < 10; i++) {

        name[13] = '0' + i;
        name[14] = '.';
        name[15] = 't';
        name[16] = 'x';
        name[17] = 't';
        name[18] = '\0';

        char *req = lireFichier(name);

        if (parseur(req,strlen(req))) {
            _Token *r,*tok;
            void *root;
            root=getRootTree();

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
            purgeTree(root);
        } else {
            free(req);
        }
        free(req);
    }

    for (int i = 10; i < 100; i++) {

        name[13] = '0' + i / 10;
        name[14] = '0' + i % 10;
        name[15] = '.';
        name[16] = 't';
        name[17] = 'x';
        name[18] = 't';
        name[19] = '\0';

        char *req = lireFichier(name);

        if (parseur(req,strlen(req))) {
            _Token *r,*tok;
            void *root;
            root=getRootTree();

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
            purgeTree(root);
        } else {
            free(req);
        }
        free(req);
    }

    for (int i = 100; i < 1000; i++) {

        name[13] = '0' + i / 100;
        name[14] = '0' + (i % 100) / 10;
        name[15] = '0' + i % 10;
        name[16] = '.';
        name[17] = 't';
        name[18] = 'x';
        name[19] = 't';
        name[20] = '\0';

        char *req = lireFichier(name);

        if (parseur(req,strlen(req))) {
            _Token *r,*tok;
            void *root;
            root=getRootTree();

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
            purgeTree(root);
        } else {
            free(req);
        }
        free(req);
    }

    for (int i = 1000; i < 10000; i++) {

        name[13] = '0' + i / 1000;
        name[14] = '0' + (i % 1000) / 100;
        name[15] = '0' + (i % 100) / 10;
        name[16] = '0' + i % 10;
        name[17] = '.';
        name[18] = 't';
        name[19] = 'x';
        name[20] = 't';
        name[21] = '\0';

        char *req = lireFichier(name);

        if (parseur(req,strlen(req))) {
            _Token *r,*tok;
            void *root;
            root=getRootTree();

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
            purgeTree(root);
        } else {
            free(req);
        }
        free(req);
    }

    // if (parseur(req,strlen(req))) {
    //     _Token *r,*tok;
    //     void *root;
    //     root=getRootTree();

    //     // printArbre(root, 0);
    // // }

    //     r=searchTree(root,argv[1]);
    //     tok=r;
    //     while (tok) {
    //         int l;
    //         char *s;
    //         s=getElementValue(tok->node,&l);
    //         printf("FOUND [%.*s]\n",l,s);
    //         tok=tok->next;
    //     }
    //     purgeElement(&r);
    //     purgeTree(root);
    // } else {
    //     free(req);
    //     return 0;
    // }
    // free(req);
    return 1;
}
