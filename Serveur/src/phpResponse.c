#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "header/pm.h"
#include "header/fastcgi.h"
#include "header/colours.h"
#include "header/phpResponse.h"

PHPResponse getPHPResponse(int fd) {
    //! Quelques notes sur cette fonction
    //? -> On pourrait si besoin vérifier l'indentifiant de la requête
    //? -> Nous ignorons complètement tout ce qui est écrit dans padding

    ListAnswers *listAnswers = readPHPResponse(fd);

    // On créé une structure de la réponse
    PHPResponse response;
    response.content = NULL;
    response.length = 0;
    response.error = false;

    // On parcourt les différentes réponses données par
    // le processus PHP
    ListAnswers *temp = listAnswers;
    if (temp == NULL) {
        // Si il y a n'y a rien dans ce cas c'est très bizarre
        // car nous n'avons rien reçu
        return response;
    }

    int i = 1;
    int tempLength = 0;
    while (temp->next != NULL) {
        //? Si on reçoit une erreur, il faut la transmettre au client
        if (temp->answer.type == FCGI_STDERR) {
            response.error = true;
        } else if (temp->answer.type == FCGI_END_REQUEST) {
            //! Code pour la fin de la requête
            //! Il faut récuperer toutes les valeurs nécessaires et peut-être les renvoyer
            //! Ou du moins vérifier que tout c'est bien déroulé
            FCGI_EndRequestBody endRequest;
            endRequest.appStatus = (temp->answer.contentData[0] << 24);
            endRequest.appStatus += (temp->answer.contentData[1] << 16);
            endRequest.appStatus += (temp->answer.contentData[2] << 8);
            endRequest.appStatus += temp->answer.contentData[3];
            printf("App Status => %d\n", endRequest.appStatus);
            endRequest.protocolStatus = temp->answer.contentData[4];
            
            if (endRequest.protocolStatus != FCGI_REQUEST_COMPLETE) {
                red();
                printf("La requête n'a pas pu être complété par le processus\n");
                reset();
                response.error = true;
            }
        } else {
            // Si c'est une réponse sans type particulier
            // On doit bien vérifier les headers
            char *toBeDeleted = response.content;
            response.content = malloc((response.length
                        + temp->answer.contentLength
                        + 1) * sizeof(char));
            response.content[response.length] = '\0';
            // On recopie ce qu'on a déjà écrit
            if (toBeDeleted != NULL) {
                strncpy(response.content, toBeDeleted, response.length);
            }

            // On écrit ensuite le reste
            strncat(response.content,
                    temp->answer.contentData, 
                    temp->answer.contentLength);

            // On recalcule ensuite la nouvelle longueur
            response.length += temp->answer.contentLength;
            if (i == 1) {
                int j = 0;
                while (j < temp->answer.contentLength) {
                    if (temp->answer.contentData[j] == 13
                            && temp->answer.contentData[j + 1] == 10
                            && temp->answer.contentData[j + 2] == 13
                            && temp->answer.contentData[j + 3] == 10) {
                        tempLength = j + 4;
                        break;
                    }
                    j++;
                }
                i++;
            }

            // On supprime le texte précédemment stockée dans la réponse
            if (toBeDeleted != NULL) {
                free(toBeDeleted);
            }
        }
        temp = temp->next;
    }

    //? On libère la mémoire
    while (listAnswers != NULL) {
        temp = listAnswers->next;
        free(listAnswers);
        listAnswers = temp;
    }

    response.contentLength = response.length - tempLength;

    return response;
}

ListAnswers* readPHPResponse(int fd) {
    green();
    printf("Lecture de la réponse du processus PHP\n");
    reset();

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
    unsigned char receivedHeader[FCGI_HEADER_SIZE];
    bool done = false; // Ce booléen sera vrai si la réponse est finie
    //? On lit les données envoyées par le processus PHP
    //? On commence par lire le contenu du header
    while (!done && read(fd, receivedHeader, FCGI_HEADER_SIZE) != 0) {
        FCGI_Header answer;
        answer.version = receivedHeader[0];
        answer.type = receivedHeader[1];
        answer.requestId = (receivedHeader[2] << 8) + receivedHeader[3];
        answer.contentLength = (receivedHeader[4] << 8) + receivedHeader[5];
        answer.paddingLength = receivedHeader[6];
        answer.reserved = receivedHeader[7];
        printf("%d\n", answer.contentLength);

        // Si on a reçu une erreur, il faut en tenir compte
        // et probablement renvoyer le contenu de l'erreur au client
        bool errorReceived = false;
        if (answer.type == FCGI_STDERR) {
            errorReceived = true;
            red();
            printf("Nous avons recu une erreur\n");
            reset();
        }

        bool endReceived = false;
        if (answer.type == FCGI_END_REQUEST) {
            endReceived = true;
            done = true;
            green();
            printf("Nous avons reçu la fin de la requete\n");
            reset();
        }

        unsigned char *receivedContent = malloc(answer.contentLength * sizeof(char));

        read(fd, receivedContent, answer.contentLength);

        for (int i = 0; i < answer.contentLength; i++) {
            answer.contentData[i] = receivedContent[i];
        }

        //! On pense à lire les paddings data
        unsigned char *receivedPadding = malloc(answer.paddingLength * sizeof(char));
        read(fd, receivedPadding, answer.paddingLength);
        free(receivedPadding);

        if (errorReceived) {
            printf("Erreur => %.*s\n", answer.contentLength, answer.contentData);
        } else if (endReceived) {
            printf("Fin de la requête\n");
        } else {
            printf("%.*s\n", answer.contentLength, answer.contentData);
        }

        ListAnswers *newAnswer = malloc(sizeof(ListAnswers));
        newAnswer->answer = answer;
        newAnswer->next = NULL;
        // Ce n'est pas plus simple ici mais c'est pour rendre plus
        // simple la lecture de cette liste par la suite, on insère la
        // nouvelle réponse à la fin de la liste:
        ListAnswers *temp = answers;
        if (temp == NULL) {
            answers = newAnswer;
        } else {
            while (temp->next != NULL) {
                temp = temp->next;
            }
            temp->next = newAnswer;
        }

        free(receivedContent);
    }

    return answers;
}
