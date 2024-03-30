#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include<ctype.h>

#include "structure.h"
#include "affichage.h"
#include "ismail2.h"
#include"pm.h"

bool checkCRLF(char requete[],int *i,const int length){
    if ((*i +1) >length ){
        return false;
    }
    if (requete[*i]==13 && requete[*i +1]==10){
        return true;
    }
    return false;
}
void freeArbre(Noeud *racine){
    for (int i=0;i<(racine->nombreFils) !=0;i++){
        if (racine->fils[i].nombreFils!=0){
            freeArbre(&racine->fils[i]);
        }
        
    }
    free(racine->fils);
}
int main(int argc ,char *argv[]){

    // char requete[]="Content-Type: 		application/json ; token=aaaaa \r\n"; //\r\n
    char requete[]="Content-Type: 		application/json \r\n"; //\r\n
    // char requete[]="Content-Type: application/json ; a=\"k_*	lx:H~ rV\\-\\R\\	!Pa,v\"  \r\n"; //\r\n
    
    // Content-Type-header = "Content-Type" ":" OWS Content-Type OWS 
    // Content-Type = media-type
    // media-type = type "/" subtype *( OWS ";" OWS parameter )
    //parameter = token "=" ( token / quoted-string )

	// printf("main\n");
    Noeud *test=malloc(sizeof(Noeud));
    int i=0;
	printf("main2\n");
	
    if(!checkContentTypeHeader(requete,&i,strlen(requete),test)){
        printf("Error\n");
        test=NULL;
    }
    if (test!=NULL){
        printArbre(test,0);
        freeArbre(test);
    }
    free(test);
    return 0;
}

void createFilsSimple(char nom[], char *i, int longueur, Noeud *noeud) {
    noeud->tag = nom;
    noeud->fils = NULL;
    noeud->valeur = i;
    noeud->longueur = longueur;
    noeud->nombreFils = 0;
}
bool checkContentTypeHeader(char requete[],int *i,int length,Noeud *noeud){
    int nombreFils= 5;
    const int indice =*i;
    Noeud* filsContypeH=malloc(sizeof(Noeud));

    if (!checkConType(requete,filsContypeH)){
        free(filsContypeH);
        free(noeud);
        *i=indice;
        printf("Error in ContentType \n");
        return false;
    }
    
    *i=12;
    if (requete[*i]!=58){
        free(filsContypeH);
        free(noeud);
        *i=indice;
        return false;
    }
    (*i)++;
    // Noeud *filsOWS1=malloc(sizeof(Noeud));
    // checkOWS(requete,i,length,NULL);
     Noeud *filsOWS1=malloc(sizeof(Noeud));
    if(!checkOWS(requete,i,length,filsOWS1)){
        free(noeud);
        printf("OWS 1 \n");
        *i=indice;
        return false;
    }
       

    Noeud *contentType=malloc(sizeof(Noeud));     //content-Type =media-type
    if (!checkMediaType(requete,i ,length,contentType)){
        free(noeud);
        *i=indice;

        printf("erreur in media type\n");
        return false;

    } 
    
    // Noeud *filsOWS2=malloc(sizeof(Noeud));
    // checkOWS(requete,i,length,NULL);
    Noeud *filsOWS2=malloc(sizeof(Noeud));
    if(!checkOWS(requete,i,length,filsOWS2)){
        free(noeud);
        printf("OWS 2 \n");

        *i=indice;
        return false;
    }

       

    noeud->fils=malloc(nombreFils*sizeof(noeud));
    noeud->valeur=requete +indice;
    noeud->longueur=*i-indice;
    noeud->nombreFils=nombreFils;
    noeud->tag="Content-Type-Header";

    *i=indice;
     
    noeud->fils[0]=*filsContypeH;
    free(filsContypeH);
    (*i)+=noeud->fils[0].longueur;

    
    createFilsSimple("case-insensitive-string",requete + *i, 1, &noeud->fils[1]);
    (*i)++;
    printf("apres 1 create of 58\n");
    // Noeud *filsOWS1=malloc(sizeof(Noeud));
	// checkOWS(requete, i, length, filsOWS1);

    noeud->fils[2] = *filsOWS1;
    (*i) += noeud->fils[2].longueur;
    free(filsOWS1);
    
	// checkOWS(requete, i, length, &noeud->fils[2]);
    printf("after ows1\n");
    noeud->fils[3] = *contentType;


    // printf("a :%d\n",*i);
    (*i) += noeud->fils[3].longueur;

	free(contentType);


//    checkOWS(requete,i,length,&noeud->fils[4]);
    noeud->fils[4] = *filsOWS2;
	free(filsOWS2);
    
	
   
    // if (!checkCRLF(requete,i,length)){
    //     printf("erreur in CRlF\n");
    //     return false;
    // }
    
    // createFilsSimple("CRLF", requete + *i, 2, &noeud->fils[5]);
    // (*i) += noeud->fils[5].longueur;
	// printf("fin\n");

    return true;
  
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

bool checkConType( char requete[] ,Noeud *noeud){
	noeud->valeur=requete;
	noeud->fils=NULL;
	noeud->longueur=12;
	noeud->tag="case_insensitive_string";
	noeud->nombreFils=0;
	char ConTypeMinuscule[13];

	sousChaineMinuscule(requete,ConTypeMinuscule,0,12);
    int a=strcmp("content-type",ConTypeMinuscule);
	if (a!=0){
        free(noeud);
		
        return false;
    }
    else {
        return true;
    }

}

bool checkMediaType(char requete[],int *i,int length,Noeud *noeud){

    // media-type = type "/" subtype *( OWS ";" OWS parameter )
    const int indice =*i;
    int compteur=0;
    bool owsCheked=false;

    // Noeud *filsType=malloc(sizeof(Noeud));
    if (!checkToken(requete,i ,length,NULL,"")){
        free(noeud);
        // free(filsType);
        printf("erreur in type");
        *i=indice;
        return false;
    }
    
	compteur++;
    if (requete[*i]!=47){
        // free(noeud);
        *i=indice;
        printf("probleme in  '/' \n");
        return false;
    }
    (*i)++;
	compteur++;

    // Noeud * filsSubType=malloc(sizeof(Noeud));
    if(!checkToken(requete,i,length,NULL,"")){
        // free(filsSubType);
        // free(noeud);
        *i=indice;
        printf("erreur in subtype\n");
        return false;
    }

    compteur++;

    
    //  ========*( OWS ";" OWS parameter )
    int const indice2=*i;
    checkOWS(requete,i,length,NULL);

    while(*i<length && requete[*i]==59){
        compteur+=2;
        
        (*i)++;
        checkOWS(requete,i,length,NULL);
        if(!checkParameter(requete,i,length,NULL)){
            compteur -=2;
            (*i)=indice2;
            break;
        }
        compteur +=2;
        checkOWS(requete,i,length,NULL);

    }


	//remplir tout  fils en noued 
    if (noeud!=NULL){
        noeud->valeur= requete+indice;
        noeud->longueur=*i-indice;
        noeud->tag= "media-type";
        noeud->fils=malloc((compteur)*sizeof(Noeud));
        noeud->nombreFils=compteur;

        *i=indice;
        
        checkToken(requete,i,length,&noeud->fils[0],"type");

        createFilsSimple("case-insensitive-string", requete+*i, 1, &noeud->fils[1]);
        (*i)++;
        checkToken(requete,i,length,&noeud->fils[2],"subtype");
    
        
        // media-type = type "/" subtype *( OWS ";" OWS parameter )
 
        for (int j=3; j < compteur; j++){
            int iTemp=*i;
			// printf("dans l'entree de  for j= %d\n",j);
            
            if (requete[*i]==59){
                // printf("%d\n",owsCheked);
                if(!owsCheked){
                    checkOWS(requete, i, length, &noeud->fils[j]);
                    j++;
                }
                createFilsSimple("case-insensitive-string", requete + *i, 1 , &noeud->fils[j]);
                (*i)++;
                owsCheked=false;
            }

            else if(checkParameter(requete,i,length,NULL)){
                *i=iTemp;
                

                if(!owsCheked){
                    checkOWS(requete, i, length, &noeud->fils[j]);
    

                    j++;
                }
                checkParameter(requete,i,length,&noeud->fils[j]);
                owsCheked=false;
            }
            else{
                
                checkOWS(requete,i,length,&noeud->fils[j]);

                owsCheked=true;
            }

        }
    }
    return true;
}

bool checkParameter(char requete[],int *i,int length,Noeud *noeud){
        //parameter = token "=" ( token / quoted-string )
        int compteur=0;
        const int indice=*i;
        int nombreFils=3;
        bool quoted=false;
        // bool token= false;
        
        if (!checkToken(requete,i,length,NULL,"")){
            // printf("token false\n");
            free(noeud);
            (*i)=indice;
            return false;
        }
        compteur++;
		

        if (requete[*i]!=61){
            free(noeud);
			printf("=\n");
            *i=indice;
            return false;
        }
        (*i)++;
        compteur++;
        // Noeud *filsTok_quotedS=malloc(sizeof(Noeud));
        int Temp=*i;
        if (checkQuotedString(requete,i,length,NULL)){
            quoted=true;
            compteur++;

        }else{
            if (!checkToken(requete,i,length,NULL,"")){
                (*i)=indice;
                return false;
            }
            compteur ++;
        }
            
		if (noeud!=NULL){
			noeud->valeur= requete + indice;
			noeud->longueur=*i- indice;
			noeud->tag= "parameter";
			noeud->fils=malloc(compteur*sizeof(Noeud));
			noeud->nombreFils= compteur;
			
			*i=indice;

			checkToken(requete,i,length,&noeud->fils[0],"token");
			// printf("1111 %s\n",requete+ *i);
			createFilsSimple("case-insensitive-string",requete+ *i,1, &noeud->fils[1]);
			(*i)++;
			int iTemp=*i;

            if (quoted){
                checkQuotedString(requete,i,length,&noeud->fils[2]);
            }else {
                checkToken(requete,i,length,&noeud->fils[2],"token");
            }
		}
        return true;
    
    }

//====================fonctions depuis ELISA2.c=====

bool checkQuotedString(char transferEncoding[], int *i, int longueur, Noeud *noeud){
    //quoted-string = DQUOTE *( qdtext / quoted-pair ) DQUOTE

    // VERIFICATION()

    int compteur=0;
    int const indice =(*i);
    if (transferEncoding[*i]!=34){
        (*i)=indice;
        free(noeud);
        return false;
    }
    (*i)++;
    compteur++;
    while(((*i)<longueur) && (checkQdtext(transferEncoding, *i, NULL)||checkQuotedPair(transferEncoding, i, longueur, NULL))){
        compteur++;
        (*i)++;
    }
    if (transferEncoding[*i]!=34){
        (*i)=indice;
        free(noeud);
        return false;
    }
    (*i)++;
    compteur++;

    if (noeud!=NULL){
        noeud->valeur = transferEncoding + indice;
        noeud->longueur = *i - indice;
        noeud->tag = "quoted-string";
        noeud->fils = malloc(compteur * sizeof(Noeud));
        noeud->nombreFils =compteur;

        *i = indice;

        for (int j=0;j<compteur;j++){
            if (transferEncoding[*i]==34){
                createFilsSimple("dquote", transferEncoding + *i, 1, &noeud->fils[j]);
                (*i)++;
            }
            else if (checkQdtext(transferEncoding, *i, NULL)){
                checkQdtext(transferEncoding, *i, &noeud->fils[j]);
                (*i)++;
            }
            else{
                checkQuotedPair(transferEncoding, i, longueur, &noeud->fils[j]);
            }
            // (*i)++;
        }
    }
    return true;
}

bool checkQdtext(char transferEncoding[], int i, Noeud *noeud){
    // qdtext = HTAB / SP / "!" / %x23-5B ; '#'-'['
    //          / %x5D-7E ; ']'-'~'
    //         / obs-text
    // obs-text = %x80-FF

    if (transferEncoding[i]==9 || transferEncoding[i]==32 || transferEncoding[i]==33 || (transferEncoding[i]>=35 && transferEncoding[i]<=91) || (transferEncoding[i]>=93 && transferEncoding[i]<=126) || (transferEncoding[i]>=128 && transferEncoding[i]<=255)) {
        if (noeud!=NULL){
            createFilsSimple("qdtext", transferEncoding + i, 1, noeud);
        }
        return true;
    } else {
        free(noeud);
        return false;
    }
}

bool checkQuotedPair(char transferEncoding[], int *i, int longueur, Noeud *noeud){
    //quoted-pair = "\" ( HTAB / SP / VCHAR / obs-text )

    // VERIFICATION()

    int indice= *i;
    if (noeud!=NULL){
        noeud->valeur = transferEncoding + *i;
        noeud->longueur = 2;
        noeud->tag = "quoted-pair";
        noeud->fils = malloc(2*sizeof(Noeud));
        noeud->nombreFils =2;
    }
    if (transferEncoding[*i]!=92){
        free(noeud);
        return false;
    }
    if (noeud!=NULL){
        createFilsSimple("case_insensitive_string",transferEncoding + *i, 1, &noeud->fils[0]);
    }
    (*i)++;
    if (transferEncoding[*i]==9){
        if (noeud!=NULL){
            createFilsSimple("HTAB", transferEncoding + *i, 1, &noeud->fils[1]);
        }
    }
    else if (transferEncoding[*i]==32){
        if (noeud!=NULL){
            createFilsSimple("SP", transferEncoding + *i, 1, &noeud->fils[1]);
        }
    }
    else if (transferEncoding[*i]>=33 && transferEncoding[*i]<= 126){
        if (noeud!=NULL){
            createFilsSimple("VCHAR", transferEncoding + *i, 1, &noeud->fils[1]);
        }
    }
    else if (transferEncoding[*i]>=128 && transferEncoding[*i]<=255){
        if (noeud!=NULL){
            createFilsSimple("obs-text", transferEncoding + *i, 1, &noeud->fils[1]);
        }
    }
    else {
        free(noeud);
        (*i)=indice;
        return false;
    }
    (*i)++;

    if (noeud == NULL) {
        (*i)--;
    }

    return true;
}








    
