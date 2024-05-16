/*Vérification du header host et du header expect quand on aura compris comment il marche*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#include "header/api.h"
#include "header/tom.h"

/*Vérifie si l'adresse ipv6 est  127.0.0.1, dans la taille la sentinelle n'est pas compté, elle est à l'emplacement inside[taille]*/
/* notre adresse ipv6 	0000:0000:0000:0000:0000:ffff:7f00:0001*/
bool isIPV6(char *inside, int taille){
    /*On va regarder notre adresse ipv6 par "secteur", il y en a 4, un avant :, 2 entre deux :, et un dernier après les :*/

    int compteur=0;
    int j=1; //Cette variable va nous permettre de nous balader dans la chaine, elle commence à 1
    bool not_passed=true; //Cette variable nous servira à vérifer si un secteur correspond ou non

    if (inside[j]==':'){
        if(strcmp( (inside), "[::ffff:7f00:1]" )==0){
            return true;
        }
        
        if(strcmp( (inside), "[::ffff:7f00:1:127.0.0.1]" )==0){
            return true;
        }

        return false;

    }

    else{

        //secteur 1:

        //printf("Secteur1 \n");

        while (((j)<taille) && (not_passed) && (compteur<5)){

            //printf("Voyons voir : %c ", inside[j]);
            if(inside[j]==':'){
                not_passed=false;
                j++;
            }

            else if(inside[j]!='0'){
                compteur=1000;
            }

            else{
                j++;
                compteur ++;
            }
        }

        if(not_passed){
            return false;
        }

        not_passed=true; // il est temps de passer le 2e secteur
        compteur=0;


        //secteur 2

        //printf("Secteur2 \n");

        while (((j)<taille) && (not_passed) && (compteur<5)){


            if(inside[j]==':'){
                not_passed=false;
                j++;
            }

            else if(inside[j]!='0'){
                compteur=1000;
            }

            else{
                j++;
                compteur ++;
            }
        }

        if(not_passed){
            return false;
        }

        not_passed=true; // il est temps de passer le 3e secteur
        compteur=0;

        //secteur 3

        //printf("Secteur3 \n");

        while (((j)<taille) && (not_passed) && (compteur<5)){


            if(inside[j]==':'){
                not_passed=false;
                j++;
            }

            else if(inside[j]!='0'){
                compteur=1000;
            }

            else{
                j++;
                compteur ++;
            }
        }

        if(not_passed){
            return false;
        }

        not_passed=true; // il est temps de passer le 4e secteur
        compteur=0;

        //secteur 4:

        //printf("Secteur4 \n");

        while (((j)<taille) && (not_passed) && (compteur<5)){


            if(inside[j]==':'){
                not_passed=false;
                j++;
            }

            else if(inside[j]!='0'){
                compteur=1000;
            }

            else{
                j++;
                compteur ++;
            }
        }

        if(not_passed){
            return false;
        }

        not_passed=true; // il est temps de passer le 5e secteur
        compteur=0;

        //secteur 5:

        //printf("Secteur5 \n");

        while (((j)<taille) && (not_passed) && (compteur<5)){


            if(inside[j]==':'){
                not_passed=false;
                j++;
            }

            else if(inside[j]!='0'){
                compteur=1000;
            }

            else{
                j++;
                compteur ++;
            }
        }

        if(not_passed){
            return false;
        }

        not_passed=true; // il est temps de passer le 6e secteur ou alors à une adresse ipv4
        compteur=0;

        /*On vérifie si le champ host possède notre adresse ipv4*/
        if(strcmp( (inside+j), "127.0.0.1]" )==0){
            return true;
        }

        //secteur 6

        //printf("Secteur6 \n");


        while (((j)<taille) && (not_passed) && (compteur<1)){


            if((inside[j]=='f') && (inside[j+1]=='f') && (inside[j+2]=='f') && (inside[j+3]=='f') && (inside[j+4]==':') ){
                not_passed=false;
                j=j+5;
            }
            else{
                j++;
                compteur ++;
            }
        }

        if(not_passed){
            return false;
        }

        not_passed=true; // il est temps de passer le 7e secteur ou alors à une adresse ipv4
        compteur=0;


        /*On vérifie si le champ host possède notre adresse ipv4*/
        if(strcmp( (inside+j), "127.0.0.1]" )==0){
            return true;
        }

        //secteur 7

        //printf("Secteur7 \n");

        while (((j)<taille) && (not_passed) && (compteur<1)){


            if((inside[j]=='7') && (inside[j+1]=='f') && (inside[j+2]=='0') && (inside[j+3]=='0') && (inside[j+4]==':') ){
                not_passed=false;
                j=j+5;
            }
            else{
                j++;
                compteur ++;
            }
        }

        if(not_passed){
            return false;
        }

        not_passed=true; // il est temps de passer le 8e secteur
        compteur=0;

        //secteur 8


        //printf("Secteur8 \n");

        while (((j)<taille) && (not_passed) && (compteur<4)){
            if((inside[j]=='1') && (inside[j+1]==']') ){
                not_passed=false;
                j++;
            }

            else if(inside[j]!='0'){
                compteur=1000;
            }

            else{
                j++;
                compteur ++;
            }
        }

        if(not_passed){
            return false;
        }

        return true;
    }


}


/* int version contient la "sous version de http", soit si la version est 1.1, il contiendra 1, si c'est 1.0, 0 etc... */

bool semanticHost(void *root, int version){

    _Token * host;
    host = searchTree(root, "host");
    /*On vérifie si il y a un champ host*/


    if (host == NULL){

        if (version==0){
            purgeElement(&host);
            return true;
        }

        else{
            printf("Erreur, Http n'est pas en version 1.0 et il n'y a pas de champ host \n");
            purgeElement(&host);
            return false;
        }

    }

    /*On vérifie si il y a d'autres headers host, si oui alors on à un problème*/

    if(host->next != NULL){
        purgeElement(&host);
        return false;
    }

    int l;
    char *s=getElementValue(host->node,&l);

    /*Inside nous sert à avoir une chaine de caractere utilisable avec la fonction strcmp cad avec une sentinelle à la fin*/
    char *inside=malloc((l+1)*sizeof (char));
    int i=0;

    while(i < l){
        inside[i]=s[i];
        i++;
    }
    inside[l]='\0';

    /*On vérifie si le champ host possède notre adresse ipv4*/
    if(strcmp( inside, "127.0.0.1" )==0){
        printf("Adresse ipv4 127.0.0.1 dans le champ host \n");
        free(inside);
        purgeElement(&host);
        return true;
    }

    /*On vérifie si le champ host possède l'adresse dns de l'un de nos serveurs*/
    char lire[256]; /*On commence par créer une chaine de caractere qui va stocker le nom de nos adresses */
   
    int k=0;
    bool activate=true;
    FILE * fichier=fopen("liste.txt", "r");


    /*On va vérifier le fichier txt ligne par ligne pour voir si la chaine présente dans le champ host correspond au nom dns*/
    while (fgets(lire, sizeof(lire), fichier)){
        activate=true;
        k=0;
        
        /*Cette boucle if sert à supprimer le \n pour pouvoir faire marcher strcmp*/
        while (lire[k]!='\0' && activate){

            if(lire[k]=='\n'){
                lire[k]='\0';
                activate=false;
            }
            k++;
        }

        //printf("Bon, là c'est ça: %s \n", lire);

        if(strcmp( inside, lire )==0){
            //printf("Adresse dns dans le champ host \n");
            free(inside);
            fclose(fichier);
            purgeElement(&host);
            return true;
        }
    }

    fclose(fichier);


    /*On regarde si ce qui se trouve dans le champ host n'est pas une adresse ipv6, et on fait donc appel à une fonction crée pour cela*/
    if(isIPV6(inside,l)){

    free(inside);

    purgeElement(&host);

    //printf("El testo \n");

    return true;

    }
    free(inside);

    purgeElement(&host);
    //printf("El fausseto \n");
    return false; 
}

/*int main(int argc, char *argv[]) {
char req[] = "GET /test/../inDex%2ehtmL HTTP/1.1\r\nHost: [::ffff:7f00:1]\r\nAccept-Encoding: gzip;q=0.9, deflate;q=0.345, compress;q=0, identity;q=0.0\r\nConnection: close\r\n\r\n";

//	::ffff:7f00:1    	0:0:0:0:0:ffff:7f00:0001    0000:0000:0000:0000:0000:ffff:7f00:0001

parseur(req, strlen(req));

void *root;

root = getRootTree();

semanticHost(root,1);

return 0;

}*/