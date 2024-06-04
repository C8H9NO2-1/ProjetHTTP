typedef struct {
    unsigned char nameLengthB0;  /* nameLengthB0  >> 7 == 0 */
    unsigned char valueLengthB0; /* valueLengthB0 >> 7 == 0 */
    unsigned char * nameData;
    unsigned char * valueData;
} FCGI_NameValuePair11;


/*Le int dec_ecriture est un descripteur en écriture qui sera utilisé pour envoyer le FCGI PARAM,
Le char Chaine contient ce que vous voulez envoyer*/
int param(char * Chaine, int desc_ecriture);


/*Nous sert à free les FCGI_NameValuePair11*/
void freeFCGI_NameValuePair11(FCGI_NameValuePair11 AH);

/*Nous sert à initilialiser correctement tous les FCGI_.....*/
int create_FCGI_NameValuePair11(FCGI_NameValuePair11 OH, char * Name, char * Value);


int add_CDATA(FCGI_NameValuePair11 UH, char * su, int i);