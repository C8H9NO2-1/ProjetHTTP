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
    FCGI_Header answer;
    answer.version = received[0];
    answer.type = received[1];
    answer.requestId = (received[2] << 7) + received[3];
    answer.contentLength = (received[4] << 7) + received[5];
    answer.paddingLength = received[6];
    answer.reserved = received[7];
    for (int i = 8; i < size; i++) {
        answer.contentData[i - 8] = received[i];
    }

    printf("%d\n", answer.requestId);

    return 0;
}
