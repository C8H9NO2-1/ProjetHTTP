#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "header/fastcgi.h"
#include "header/FCGI_param.h"


void freeFCGI_NameValuePair11(FCGI_NameValuePair11 AH){
    free(AH.nameData);
    free(AH.valueData);
}

int create_FCGI_NameValuePair11(FCGI_NameValuePair11 OH, char * Name, char * Value){
    OH.nameLengthB0= strlen(Name) ;
    OH.valueLengthB0= strlen(Value) ;
    OH.nameData= malloc(OH.nameLengthB0*sizeof(char));
    OH.valueData= malloc(OH.valueLengthB0*sizeof(char));
    strcpy(OH.nameData , Name);
    strcpy(OH.valueData, Value);
    return 1;

}

int add_CDATA(FCGI_NameValuePair11 UH, char * su, int i){
    su[i]=UH.nameLengthB0;
    su[i+1]=UH.valueLengthB0;
    i=i+2;
    int k=0;
    while (k<UH.nameLengthB0){
        su[i+k]=UH.nameData[k];
        k++;
    }
    i=i+k;
    k=0;
    while (k<UH.valueLengthB0){
        su[i+k]=UH.valueData[k];
        k++;
    }
    i=i+k;
    return i;
}


int param(char * Chaine, int desc_ecriture){

    FCGI_Header Head;
    Head.version=FCGI_VERSION_1;
    Head.requestId=FCGI_REQUEST_ID;
    Head.type=FCGI_PARAMS;
    Head.paddingLength=0;
    Head.reserved=0;

    if (Chaine==NULL){
        Head.contentLength=0;
        write(desc_ecriture, Head, sizeof(Head));
        return 0;
    }


    /*Début des problèmes/Headers HTTP*/

    FCGI_NameValuePair11 Host;
    create_FCGI_NameValuePair11(Host,"HTTP_HOST =", " 127.0.0.1" );

    FCGI_NameValuePair11 User_Agent;

    /*Utiliser le parseur pour chercher User-Agent et il renverra : "User-Agent:'OWS'zmrg,G,OẐG,ẐROG.ZGpjZGO" SUPPRIMER LE DEBUT POUR NE GARDER QUE zmgzjiog*/

    FCGI_NameValuePair11 Accept;
    /*Accept*/


    /*Continuer les headers HTTP*/


    FCGI_NameValuePair11 Software;
    create_FCGI_NameValuePair11(Software, "SERVER_SOFTWARE =", " IRC_LES_GOATS");
    FCGI_NameValuePair11 Name;
    create_FCGI_NameValuePair11(Name, "SERVER_NAME =", " 127.0.0.1");
    FCGI_NameValuePair11 ADDR;
    create_FCGI_NameValuePair11(ADDR, "SERVER_ADDR =", " 127.0.0.1");
    FCGI_NameValuePair11 PORT;
    create_FCGI_NameValuePair11(PORT, "SERVER_PORT =", " 8080");
    FCGI_NameValuePair11 RADDR;
    create_FCGI_NameValuePair11(RADDR, "REMOTE_ADDR =", " 127.0.0.1");

    int u=0;
    u=add_CDATA(Host, Head.contentData ,u);
    u=add_CDATA(Software, Head.contentData ,u);
    u=add_CDATA(Name, Head.contentData ,u);
    u=add_CDATA(ADDR, Head.contentData ,u);
    u=add_CDATA(PORT, Head.contentData ,u);
    u=add_CDATA(RADDR, Head.contentData ,u);
    Head.contentLength=u;



    
    







    //Head.contentLength=


    write(desc_ecriture, Head, sizeof(Head));


    freeFCGI_NameValuePair11(Host);
    freeFCGI_NameValuePair11(User_Agent);
    freeFCGI_NameValuePair11(Accept);
    freeFCGI_NameValuePair11(Software);
    freeFCGI_NameValuePair11(Name);
    freeFCGI_NameValuePair11(ADDR);
    freeFCGI_NameValuePair11(PORT);
    freeFCGI_NameValuePair11(RADDR);
    return 1;
}
