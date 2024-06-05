#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <math.h>

#include "header/pm.h"

#include "header/request.h"
#include "header/FCGI_param.h"
#include "header/fastcgi.h"
#include "header/begin.h"


#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "header/phpServer.h"
#include "header/phpResponse.h"
#include "header/response.h"

static int createSocket(char *ip, int port) {
    int fd;
    struct sockaddr_in serv_addr;
    int enable = 1;

    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket creation failed\n");
        return (-1);
    }

    bzero(&serv_addr, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    inet_aton(ip, (struct in_addr *)&serv_addr.sin_addr.s_addr);
    /*inet_aton(ip,(struct sockaddr *)&serv_addr.sin_addr.s_addr);*/
    serv_addr.sin_port = htons(port);

    if (connect(fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("connect failed\n");
        return (-1);
    }

    return fd;
}

void phpServerResponse(char *path, int version, char* connection, Method method , unsigned clientid ,char *chaine ){
    // chain  pour passer quelque chose dans stdin
    // le debut est similaire a reponse()..
    // apres on envoi au processus php // send to server
    
    int test= createSocket("127.0.0.1",9000);


    int longueurBegin=0;
    int longueurStdin=0;
    // char *chaine = "bonjour";
    FCGI_Header *begin =  beginRequest(&longueurBegin);
    write(test, begin, longueurBegin);
    free(begin);

    param(path,test,connection,method);
    
    FCGI_Header *stdin = stdinRequest(&longueurStdin, chaine);
    write(test, stdin, longueurStdin);
    free(stdin);

    if (chaine != NULL) {
        stdin = stdinRequest(&longueurStdin, NULL);
        write(test, stdin, longueurStdin);
        free(stdin);
    }
  

    PHPResponse reponse=getPHPResponse(test);
    
    if (reponse.error ==false){
        if (method == POST) {
            reponse3(302,1,reponse.contentLength,reponse.content,reponse.length,connection,clientid);
        } else {
            reponse3(200,1,reponse.contentLength,reponse.content,reponse.length,connection,clientid);
        }
    }else{
        // int taille=reponse.length+1;
        // char erreur[taille];
        // erreur =reponse.content;
        
        char version[8]="TTP/1.1";
        for (int i=0; i<7;i++ ){
            reponse.content[i]=version[i];
        }
        writeDirectClient(clientid,"H",1);
        writeDirectClient(clientid,reponse.content,reponse.contentLength);
        endWriteDirectClient(clientid);
    }

    
    //sendToClient

}
