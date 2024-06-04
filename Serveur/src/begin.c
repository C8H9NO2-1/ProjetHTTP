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

FCGI_BeginRequestBody *beginRequest(){
    FCGI_BeginRequestBody *begin = malloc(sizeof(FCGI_BeginRequestBody));
    begin->role = FCGI_RESPONDER;
    begin->flags = 0;
    for (int i=0; i<5; i++){
        begin->unused[i] = 0;
    }
    return begin;
}