#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "header/fastcgi.h"
#include "header/phpResponse.h"
#include "header/colours.h"

int readPHPResponse(int fd) {
    green();
    printf("Lecture de la réponse du processus PHP\n");
    reset();

    //? On lit les données envoyées par le processus PHP
    char received[FCGI_HEADER_SIZE + FASTCGILENGTH];

    int size = read(fd, received, FCGI_HEADER_SIZE + FASTCGILENGTH);

    //!==============================================================
    //! Cette partie est uniquement nécessaire pour des tests
    /*received[0] = (char) 1;*/
    /*received[1] = (char) FCGI_STDOUT;*/
    /*received[2] = (char) 3;*/
    /*received[3] = (char) 7;*/
    /*received[4] = (char) 0;*/
    /*received[5] = (char) 8;*/
    /*received[6] = (char) 0;*/
    /*received[7] = (char) 0;*/
    /**/
    /*received[8] = 8;*/
    /*received[9] = 9;*/
    /*received[10] = 10;*/
    /*received[11] = 11;*/
    /*received[12] = 12;*/
    /*received[13] = 13;*/
    /*received[14] = 14;*/
    /*received[15] = 15;*/

    //! Fin de la partie Test
    //!==============================================================

    //? On écrit dans la structure ce qu'on a reçu pour un traitement
    //? plus simple des informations
    //! On fait bien attention aux champs short et non char
    //? Tant que l'on a pas lu tout le contenu du packet TCP, on doit
    //? continuer à lire
    int j = 0;
    while (j < size) {
        printf("j => %d\n", j);
        FCGI_Header answer;
        answer.version = received[j + 0];
        answer.type = received[j + 1];
        answer.requestId = (received[j + 2] << 7) + received[j + 3];
        answer.contentLength = (received[j + 4] << 7) + received[j + 5];
        answer.paddingLength = received[j + 6];
        answer.reserved = received[j + 7];
        for (int i = 0; i < answer.contentLength; i++) {
            answer.contentData[j + i] = received[j + i + 8];
        }
        printf("%.*s\n", answer.contentLength, answer.contentData);

        j += FCGI_HEADER_SIZE + answer.contentLength + answer.paddingLength;
    }

    return 0;
}
