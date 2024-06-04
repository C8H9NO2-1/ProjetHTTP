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

    //! On fait bien attention aux champs short et non char
    //? Tant que l'on a pas lu tout le contenu du packet TCP, on doit
    //? continuer à lire
    ListAnswers *answers = NULL;
    char receivedHeader[FCGI_HEADER_SIZE];
    //? On lit les données envoyées par le processus PHP
    //? On commence par lire le contenu du header
    while (read(fd, receivedHeader, FCGI_HEADER_SIZE) != 0) {
        FCGI_Header answer;
        answer.version = receivedHeader[0];
        answer.type = receivedHeader[1];
        answer.requestId = (receivedHeader[2] << 7) + receivedHeader[3];
        answer.contentLength = (receivedHeader[4] << 7) + receivedHeader[5];
        answer.paddingLength = receivedHeader[6];
        answer.reserved = receivedHeader[7];
        printf("%d\n", answer.contentLength);

        char *receivedContent = malloc(answer.contentLength * sizeof(char));

        read(fd, receivedContent, answer.contentLength);

        for (int i = 0; i < answer.contentLength; i++) {
            answer.contentData[i] = receivedContent[i];
        }

        printf("%.*s\n", answer.contentLength, answer.contentData);
    }

    return 0;
}
