#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "header/fastcgi.h"
#include "header/FCGI_param.h"
#include "header/api.h"


void freeFCGI_NameValuePair11(FCGI_NameValuePair11 * AH){
    free(AH->nameData);
    free(AH->valueData);
}

int create_FCGI_NameValuePair11(FCGI_NameValuePair11  * OH, char * Name, char * Value){
    OH->nameLengthB0= strlen(Name) ;
    //printf("Voici name len : %d \n", OH->nameLengthB0);
    OH->valueLengthB0= strlen(Value) ;
    //printf("Voici value len : %d \n", OH->valueLengthB0);
    OH->nameData= malloc(OH->nameLengthB0*sizeof(char));
    OH->valueData= malloc(OH->valueLengthB0*sizeof(char));
    strcpy(OH->nameData , Name);
    //printf("Voici le nom de la data : %s \n", OH->nameData);
    strcpy(OH->valueData, Value);
    //printf("Voici la valeur de la data : %s \n ", OH->valueData);
    return 1;

}

int add_CDATA(FCGI_NameValuePair11 * UH, char * su, int i){
    su[i]=UH->nameLengthB0;
    //printf("Voici name len dans add : %d \n", su[i]);
    su[i+1]=UH->valueLengthB0;
    i=i+2;
    int k=0;

    while (k<UH->nameLengthB0){

        //printf("Voici la valeur de k : %d \n", k);
        su[i+k]=UH->nameData[k];
        k++;

    }

    i=i+k;
    k=0;

    while (k<UH->valueLengthB0){

        su[i+k]=UH->valueData[k];
        k++;

    }
    
    i=i+k;
    return i;
}


int param(char * PHP, char * path, int desc_ecriture, void * root, char * connection){

    FCGI_Header Head;
    Head.version=FCGI_VERSION_1;
    Head.requestId=htons(FCGI_REQUEST_ID);
    Head.type=FCGI_PARAMS;
    Head.paddingLength=0;
    Head.reserved=0;

    if (PHP==NULL){
        //printf("hello\n");
        Head.contentLength=0;
        write(desc_ecriture, &Head, FCGI_HEADER_SIZE);
        return 0;
    }


    /*Début des problèmes/Headers HTTP*/

    FCGI_NameValuePair11 Host;
    create_FCGI_NameValuePair11(&Host,"HTTP_HOST", "127.0.0.1" );

    FCGI_NameValuePair11 User_Agent;
    _Token * user;
    user = searchTree(root, "User-Agent");

    if(user!=NULL){

        int l;
        char *s=getElementValue(user->node,&l);

        if (s[11]==' '){
            char d[l-12];
            int p=12;

            while(p<l){
                d[p-12]= s[p];
                p++;
            }

            create_FCGI_NameValuePair11(&User_Agent, "HTTP_USER_AGENT", d);
            
        }
        else{

            char d[l-11];
            int p=11;
            
            while(p<l){
                d[p-11]= s[p];
                p++;
            }

            create_FCGI_NameValuePair11(&User_Agent, "HTTP_USER_AGENT", d);

        }

    }

    FCGI_NameValuePair11 Connection;
    create_FCGI_NameValuePair11(&Connection,"HTTP_CONNECTION", connection );
    FCGI_NameValuePair11 Path;
    create_FCGI_NameValuePair11(&Path,"PATH", getenv("PATH") );
    FCGI_NameValuePair11 Signature;
    create_FCGI_NameValuePair11(&Signature,"SERVER_SIGNATURE", "<address>IRC_LES_GOATS Server at 127.0.0.1 Port 8080</address>");

    //Continuer les headers HTTP


    FCGI_NameValuePair11 Software;
    create_FCGI_NameValuePair11(&Software, "SERVER_SOFTWARE", "IRC_LES_GOATS");
    FCGI_NameValuePair11 Name;
    create_FCGI_NameValuePair11(&Name, "SERVER_NAME", "127.0.0.1");
    FCGI_NameValuePair11 ADDR;
    create_FCGI_NameValuePair11(&ADDR, "SERVER_ADDR", "127.0.0.1");
    FCGI_NameValuePair11 PORT;
    create_FCGI_NameValuePair11(&PORT, "SERVER_PORT", "8080");
    FCGI_NameValuePair11 RADDR;
    create_FCGI_NameValuePair11(&RADDR, "REMOTE_ADDR", "127.0.0.1");

    int u=0;
    u=add_CDATA(&Host, Head.contentData ,u);
    if(user!=NULL){
        u=add_CDATA(&User_Agent, Head.contentData, u);
    }
    u=add_CDATA(&Connection, Head.contentData ,u);
    u=add_CDATA(&Path, Head.contentData ,u);
    u=add_CDATA(&Signature, Head.contentData ,u);
    u=add_CDATA(&Software, Head.contentData ,u);
    u=add_CDATA(&Name, Head.contentData ,u);
    u=add_CDATA(&ADDR, Head.contentData ,u);
    u=add_CDATA(&PORT, Head.contentData ,u);
    u=add_CDATA(&RADDR, Head.contentData ,u);
    Head.contentLength=htons(u);
    //printf("Ceci est égal à u: %d \n", u);


    write(desc_ecriture, &Head, u + FCGI_HEADER_SIZE);


    freeFCGI_NameValuePair11(&Host);
    if(user!=NULL){
        freeFCGI_NameValuePair11(&User_Agent);
    }
    freeFCGI_NameValuePair11(&Connection);
    freeFCGI_NameValuePair11(&Path);
    freeFCGI_NameValuePair11(&Signature);
    freeFCGI_NameValuePair11(&Software);
    freeFCGI_NameValuePair11(&Name);
    freeFCGI_NameValuePair11(&ADDR);
    freeFCGI_NameValuePair11(&PORT);
    freeFCGI_NameValuePair11(&RADDR);
    return 1;
}
