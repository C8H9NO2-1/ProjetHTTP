typedef struct {
    char nameLengthB0;  /* nameLengthB0  >> 7 == 0 */
    char valueLengthB0; /* valueLengthB0 >> 7 == 0 */
    char * nameData;
    char * valueData;
} FCGI_NameValuePair11;


/*Le int dec_ecriture est un descripteur en écriture qui sera utilisé pour envoyer le FCGI PARAM,
Le char * path contient le chemin du dossier dans lequel est contenu le document php : Exemple : header/dossier/info.php
Le char * connection contient ce qu'il faut mettre dans le Champ HTTP_CONNECTION
La method correspond à la method demandé par le client (GET/HEAD/POST)
Le char * content_type contient le type du content, mettre NULL si  vous ne voulez pas mettre le header
Le int content_type_len contient la taille du content, ignoré si content_type == NULL
IDEM pour content_len et content_len_len
*/
int param(char * path, int desc_ecriture, char * connection, Method method, char* content_type, int content_type_len, char * content_len, int content_len_len );


/*Nous sert à free les FCGI_NameValuePair11*/
void freeFCGI_NameValuePair11(FCGI_NameValuePair11 * AH);

/*Nous sert à initilialiser correctement tous les FCGI_.....*/
int create_FCGI_NameValuePair11(FCGI_NameValuePair11 * OH, char * Name, char * Value);

/*Nous sert à ajouter ce qu'il y a dans nos structures à la chaine content data*/
int add_CDATA(FCGI_NameValuePair11 * UH, char * su, int i);