#ifndef RESPONSE_H
#define RESPONSE_H

//! Liste des codes à renvoyer dans les réponses
// À priori on ne va pas renvoyer de code de la forme 3xx
/* - 100: si la requête contient un header Expect (Dans ce cas il faut attendre le reste de la requête après avoir envoyé le 100)
 * - 200: OK, c'est celui que l'on va envoyer lorsque tout va bien
 * - 400: Bad Request => Il faut envoyer une explication et dire si c'est permanent ou non (c'est similaire pour tous les 4xx sauf quand on répond à une requête HEAD)
 * - 403: Forbidden => La requête est comprise mais le serveur (nous) refuse de répondre (pas certain si ce code sera utile)
 * - 404: Not Found => On ne présente plus ce code d'erreur
 * - 406: Not Acceptable => La ressource n'est disponible dans aucune des représentations demandées
 * - 411: Length Required => Le serveur refuse de traiter la requête si il n'y a pas de header Content-Length
 * - 417: Expectation Failed => En réponse à une requête contenant un header Expect si il y a une erreur
 * - 500: Internal Server Error => Si le serveur à eu un problème lors du traitement de la requête (cette erreur vient du serveur et non du client) 
 * - 501: Not Implemented => On renvoie cela lorsque la méthode nous est inconnue
 * - 505: HTTP Version Not Supported => Si la version majeure de HTTP est trop grande (il faut dire quelle version nous supportons à priori)
 */


    /*Code : code d'erreur/de succès, version:version de http, ctype: contenu de Content-type, clenght:contenu de Content-length,
     close: Contenu de Connection, filenamne: Nom du fichier à mettre dans la reponse, mettre NULL si pas de fichier, clientid: Id du client à contacter*/
    bool reponse(int code, int version, char * ctype , int clenght, char * filename, char * close , unsigned clientid  );

    /*Code : code d'erreur/de succès , version:version de http, close: Contenu de Connection,clientid: Id du client à contacter  */
    bool error(int code, int version, char * close, unsigned clientid,bool Get);

bool reponse2(int code, int version, char * ctype , int clenght, FILE *file, char * close , unsigned clientid  );


#endif
