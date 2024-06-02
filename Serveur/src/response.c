#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <math.h>

#include "header/request.h"
#include "header/api.h"
#include "header/response.h"

bool error(int code, int version, char * close, unsigned clientid){
    char first[50];

    if(version==1){
        strcpy(first,"HTTP/1.1 ");

    }
    else if(version==0){
        strcpy(first,"HTTP/1.0 ");

    }
    else{
        printf("Mauvaise version d'HTTP fourni \n");
        return false;
    }

    char second[100];

    switch ( code )
    { 
        case 400:
            strcpy(second,"400 Bad Request\r\n");
            break;

        case 403:
            strcpy(second,"403 Forbidden\r\n");
            break;
        case 404:
            strcpy(second,"404 Not Found\r\n");
            break;
        case 406:
            strcpy(second,"406 Not Acceptable\r\n");
            break;
        case 411:
            strcpy(second,"411 Length Required\r\n");
            break;
        case 417:
            strcpy(second,"417: Expectation Failed\r\n");
            break;
        case 500:
            strcpy(second,"500 Internal Server Error\r\n");
            break;
        case 501:
            strcpy(second,"501 Not Implemented\r\n");
            break;
        case 505:
            strcpy(second,"505 HTTP Version Not Supported\r\n");
            break;
        default:
        printf("Code d'erreur inexistant \n");
        return false;

    }


    writeDirectClient(clientid, first, strlen(first));
    writeDirectClient(clientid, second, strlen(second));
    writeDirectClient(clientid, "Connection: ", strlen("Connection: "));
    writeDirectClient(clientid, close, strlen(close));
    writeDirectClient(clientid, "\r\n", 4);

    endWriteDirectClient(clientid);

    return true;

}

bool reponse(int code, int version, char * ctype , int clenght, char * filename, char * close , unsigned clientid  ){

    bool content=false;
    if (filename != NULL){

        FILE *test = fopen(filename, "r");

        if (test == NULL){
            printf("Le fichier ne veut pas s'ouvrir");
            return false;
        }

        fclose(test);

    }

    char first[100];
    if (code==100){
        if(version==1){
            strcpy(first,"HTTP/1.1 100 Continue\r\n");

        }
        else if(version==0){
            strcpy(first,"HTTP/1.0 100 Continue\r\n");

        }
        else{
            printf("Mauvaise version d'HTTP fourni \n");
            return false;
        }

    }
    else if(code==200){
        if(version==1){
            strcpy(first,"HTTP/1.1 200 OK\r\n");

        }
        else if(version==0){
            strcpy(first,"HTTP/1.0 200 OK\r\n");

        }
        else{
            printf("Mauvaise version d'HTTP fourni \n");
            return false;
        }

    }
    else{
        printf("Mauvais code de réponse fourni \n");
        return false;
    }

    char *buff= malloc((log10(clenght)+2)*sizeof(char));
    sprintf(buff, "%d", clenght);
    buff[(int)log10(clenght)+1]='\0';

    writeDirectClient(clientid, first, strlen(first));
    writeDirectClient(clientid, "Content-Type: ", strlen("Content-Type: "));
    writeDirectClient(clientid, ctype, strlen(ctype));
    writeDirectClient(clientid, "\r\n", 4);
    writeDirectClient(clientid, "Content-Length: ", strlen("Content-Length: "));
    writeDirectClient(clientid, buff, strlen(buff));
    writeDirectClient(clientid, "\r\n", 4);
    writeDirectClient(clientid, "Connection: ", strlen("Connection: "));
    writeDirectClient(clientid, close, strlen(close));
    writeDirectClient(clientid, "\r\n", 4);
    writeDirectClient(clientid, "\r\n", 4);


    if (content){
        FILE *file = fopen(filename, "r");

        char c[1];

        if (file == NULL){
            printf("Le fichier ne veut pas s'ouvrir");
            return false;
        }

        while ((c[0] = (char) fgetc(file)) != EOF){
            writeDirectClient(clientid, c, 1);
        }

    }

    endWriteDirectClient(clientid);
    free(buff);

    return true;

}


int main(){
    printf("Ceci est un test");
}