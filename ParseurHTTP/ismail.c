#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include<ctype.h>

#include "structure.h"

#include "affichage.h"
#include "ismail.h"
#include"pm.h"



void freeArbre(Noeud *racine){
    for (int i=0;i<(racine->nombreFils) !=0;i++){
        if (racine->fils[i].nombreFils!=0){
            freeArbre(&racine->fils[i]);
        }
        
    }
    free(racine->fils);
}

int main (int argc, char *argv[]){
    char requete[]="Content-length:	 55	 \r\n";
    //e.g: Content-Length: 56
    //"Content-Length-header = \"Content-Length\" \":\" OWS Content-Length OWS";

    Noeud *test = malloc(sizeof(Noeud));

    int i=0;
    if (!checkContentLenHeader(requete,&i,strlen(requete),test)){
        printf("error \n");
        // freeArbre(test);

        test= NULL;    
    }

    if (test!=NULL){
        printArbre(test,0);
        freeArbre(test);
    }
    free(test);
    return 0;
}

bool checkContentLenHeader(char requete[], int *i, int length, Noeud *noeud){
    int nombreFils=6;
     int indice = *i;
    Noeud *filsConLenH=malloc (sizeof(Noeud));

    if (!checkConLen(requete,filsConLenH)){
        free(filsConLenH);
        free(noeud);
        // free(filsConLenH);
        printf("erreur in checkConlen\n");
        *i=indice;
        return false;
    }
    *i=14;   // strlen("Content-length")
    if(requete[*i]!=58){
        free(noeud);
		free(filsConLenH);
        *i=indice;
        return false;
    }
    (*i)++;

    // Noeud *filsOWS1=malloc(sizeof(Noeud));
	checkOWS(requete,i,length,NULL);

    // if(!checkOWS(requete,i,length,filsOWS1)){
    //     free(noeud);
    //     free(filsConLenH);
    //     // free(filsOWS1);
    //     printf("OWS 1 \n");
    //     *i=indice;
    //     return false;
    // }
    Noeud *digitLength= malloc(sizeof(Noeud));
    if (!checkDigitLen(requete,i,length,digitLength)){
        free(noeud);
        free(digitLength);
        free(filsConLenH);
        // free(filsOWS1);
        printf("checkDigitlen \n");

        *i=indice;
        return false;
    }
	checkOWS(requete,i,length,NULL);

    // Noeud *filsOWS2=malloc(sizeof(Noeud));
    
    noeud->fils=malloc(nombreFils* sizeof(Noeud));
    noeud->valeur=requete +indice;
    noeud->longueur=*i-indice;
    noeud->nombreFils=nombreFils;
    noeud->tag="Content-Length-Header";

    
    *i=indice;

    noeud->fils[0]=*filsConLenH;
	free(filsConLenH);
    (*i) += noeud->fils[0].longueur;

    createFilsSimple("case-insensitive-string",requete + *i, 1, &noeud->fils[1]);
    (*i)++; 

    // noeud->fils[2] = *filsOWS1;
    // (*i) += noeud->fils[2].longueur;
	// free(filsOWS1);
	checkOWS(requete,i,length,&noeud->fils[2]);

    noeud->fils[3] = *digitLength;
    (*i) += noeud->fils[3].longueur;
	free(digitLength);

    // noeud->fils[4] = *filsOWS2;
    // (*i) += noeud->fils[4].longueur;
	// free(filsOWS2);
	checkOWS(requete,i,length,&noeud->fils[4]);

    if (!checkCRLF(requete,i,length)){
	freeArbre(noeud)
	free(noeud);
		    
        // free(digitLength);
        // free(filsConLenH);
        return false;
    }
    createFilsSimple("CRLF", requete +*i, 2, &noeud->fils[5]);
    (*i) += noeud->fils[5].longueur;
    
    return true;

}
bool checkCRLF(char requete[],int *i,const int length){
    if ((*i +1) >length ){
        return false;
    }
    if (requete[*i]==13 && requete[*i +1]==10){
        return true;
    }
    return false;
}

bool compareCaseInse(char chaine1[], char chaine2[]){
    int n =strlen(chaine1);
    int m=strlen(chaine2);
    int j=0;
    if (n!=m)
        return false;
    for (int i=0 ; i<n ;i++ ){
        if (tolower(chaine1[i])!=tolower(chaine2[j])){
            return false;
        }
        j++;
    }
    return true;
}
bool checkConLen(   char requete[] ,Noeud *noeud){ // "Content-Length"
    
   
   char try[15];    // char correcte[]="Content-Length"; pourquoi ne fonctionne pas !!!
   char *correcte="Content-Length";
   strncpy(try,requete,sizeof(try));
   try[14]='\0';               // try[sizeof(try)/sizeof(*try)-1] ='\0'
    // int i =strcmp(try,correcte); // for case-insensitive we could compare tolower of the two string
    // int  i= strcmp(tolower(*try),tolower(*correcte));
    if (compareCaseInse(try,correcte)){
        noeud->valeur=requete;
        noeud->fils=NULL;
        noeud->longueur=14;
        noeud->tag="Content-Length";
        noeud->nombreFils=0;
        
        return true;
    }
    else {
        return false;
    }
}


bool checkDigitS( char requete[],int i ,Noeud *noeud){
     if (noeud != NULL) {
        noeud->fils = NULL;
        noeud->valeur =requete+ i;
        noeud->longueur = 1;
        noeud->nombreFils = 0;
        noeud->tag = "Digit";
    }

    if (checkDigit(requete,i)){
        return true;
    }
    if (noeud !=NULL){
        free(noeud);
    }

    return false;
}


bool checkDigitLen( char requete[],int *i, int length,Noeud *noeud){ // Content-Length = 1*DIGIT
    int indice =*i;
    int compteur =0;
   
    while(*i<length && checkDigit(requete,*i) ){ //checkTChar(requete,i,NULL)&& checkDigit(requete,*i)
            (*i)++;
            compteur++;;
    }
    // printf("compteur = %d\n",compteur);
    
    if (compteur==0){
		// printf("pas de digit\n");
        // free(noeud);
        return false;
    }
    noeud->valeur=requete +indice;
    noeud->longueur=*i -indice;
    noeud->tag="Content-Length";
    noeud->fils=malloc(compteur*sizeof(Noeud));
    noeud->nombreFils=compteur;
    *i=indice;   // 

    for (int j=0;j<compteur;j++){
        checkDigitS(requete,*i,&noeud->fils[j]);
        (*i)++;
    }

    return true;
}


