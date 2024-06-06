/*Vérification du header host et du header expect quand on aura compris comment il marche*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#include "header/api.h"
#include "header/tom.h"

/*Vérifie si l'adresse ipv6 est  127.0.0.1, dans la taille la sentinelle n'est pas compté, elle est à l'emplacement inside[taille]*/
/* notre adresse ipv6 	::ffff:127.0.0.1  / ::1 / 0:0:0:0:0:0:0:1 */
bool isIPV6(char *inside){

    //A partir de là c'est du bonus, Giorgi à dis que IPV4 + nom DNS cela suffisait

    if(strcmp( inside, "[::1]" )==0){
        return true;
    }

    else if(strcmp( inside, "[0:0:0:0:0:0:0:1]" )==0){
        return true;
    }

    //Maintenant c'est le bonus du bonus, on ajoute les sufffix spéciaux

    else if(strcmp( inside, "[:64:ff9b:::127.0.0.1]" )==0){
        return true;
    }

    else if(strcmp( inside, "[::ffff:0:0:127.0.0.1]" )==0){
        return true;
    }

    else if(strcmp( inside, "[:64:ff9b:1:::127.0.0.1]" )==0){
        return true;
    }

    //Et là il reste à faire le bonus du bonus du bonus, vérifeir si cela marceh aussi si les caracteres sont en majuscules, mais j'ai la flemme

    else{
        return false;
    }
}


/* int version contient la "sous version de http", soit si la version est 1.1, il contiendra 1, si c'est 1.0, 0 etc... */

bool semanticHost(void *root, int version, int * info){

    _Token * host;
    host = searchTree(root, "host");
    /*On vérifie si il y a un champ host*/


    if (host == NULL){

        if (version==0){
            purgeElement(&host);
            *info=-1;
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
        *info=1;
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
            /*printf("Adresse dns dans le champ host \n");*/
            free(inside);
            fclose(fichier);
            purgeElement(&host);
            *info=0;
            return true;
        }
    }

    fclose(fichier);


    /*On regarde si ce qui se trouve dans le champ host n'est pas une adresse ipv6, et on fait donc appel à une fonction crée pour cela*/
    if(isIPV6(inside)){

    free(inside);

    purgeElement(&host);

    /*printf("Adresse IPV6 dans le champ host \n");*/

    *info=1;

    return true;

    }
    free(inside);

    purgeElement(&host);
    //printf("El fausseto \n");
    return false; 
}

/*int main(int argc, char *argv[]) {
char req[] = "GET /test/../inDex%2ehtmL HTTP/1.1\r\nHost: [::1]\r\nAccept-Encoding: gzip;q=0.9, deflate;q=0.345, compress;q=0, identity;q=0.0\r\nConnection: close\r\n\r\n";

//	

parseur(req, strlen(req));

void *root;

root = getRootTree();

int * k=-5;

semanticHost(root,1, k);

return 0;

}*/
