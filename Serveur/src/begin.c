#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "header/request.h"
#include "header/api.h"
#include "header/elisa.h"
#include "header/pm.h"
#include "header/ismail.h"
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

FCGI_Header *beginRequest(int *l){
    FCGI_BeginRequestBody *body = beginRequestBody();
    FCGI_Header *request = BeginRequestHeader(body, int *l);
    free(body);
    return request;
}

FCGI_Header *stdinRequest(int *l){
    FCGI_Header *request = stdinHeader(int *l);
    return request;
}

FCGI_BeginRequestBody *beginRequestBody(){
    FCGI_BeginRequestBody *begin = malloc(sizeof(FCGI_BeginRequestBody));
    begin->role = FCGI_RESPONDER;
    begin->flags = 0;
    for (int i=0; i<5; i++){
        begin->unused[i] = 0;
    }
    return begin;
}

FCGI_Header *BeginRequestHeader(FCGI_BeginRequestBody* begin, int *l){
    *l=8+FCGI_HEADER_SIZE;
    FCGI_Header *firstRequest = malloc(sizeof(FCGI_Header));
    firstRequest->version = FCGI_VERSION_1;
    firstRequest->type = FCGI_BEGIN_REQUEST;
    firstRequest->requestId=htons(1);
    firstRequest->contentLength=htons(8);
    firstRequest->paddingLength=0;
    firstRequest->reserved=0;
    firstRequest->contentData[0]=0;
    firstRequest->contentData[1]=begin->role;
    for (int i=2; i<8; i++){
        firstRequest->contentData[i]=* (char *) (begin+i);
    }
    return firstRequest;
}

FCGI_Header *stdinHeader(int *l){
    *l=FCGI_HEADER_SIZE;
    FCGI_Header *firstRequest = malloc(sizeof(FCGI_Header));
    firstRequest->version = FCGI_VERSION_1;
    firstRequest->type = FCGI_STDIN;
    firstRequest->requestId=htons(1);
    firstRequest->contentLength=htons(0);
    firstRequest->paddingLength=0;
    firstRequest->reserved=0;
    return firstRequest;
}