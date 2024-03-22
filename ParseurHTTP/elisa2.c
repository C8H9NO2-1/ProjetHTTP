#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "structure.h"
#include "elisa2.h"
#include "affichage.h"

#define VERIFICATION() if (*i >= longueur) { \
if (noeud != NULL) \
free(noeud); \
return false; } \

int main(int argc, char *argv[]) {
    
    // char transferEncoding[] = "traNsfer-EnCodINg:	  ,, ,	cHUnKed";
	// char transferEncoding[] = "TrAnSfEr-encoDInG:,	,ComPRESs 	, chuNKED, 	";
	// char transferEncoding[] = "TraNsfER-ENcOdiNg:    	dEflaTE	 ,  P4lVdK%%we_#X!m";
	// char transferEncoding[] = "TrANsfEr-eNcOdinG:				,GZiP ,		Fp#SAbMJPw8wTz- ;Z#QT~h4p#u|PPK5= nKi|kxaOW$uKHke 	; z-^6'FzCNW_WJ3w =\"\"";
	// char transferEncoding[] = "TRaNSFEr-enCODiNG:  ,20Kmy%d_Yg8t.MS; 	eh6owY1JpoLr99n=\"k_*	lx:H~ rV\\-\\R\\	!Pa,v\"";
	// char transferEncoding[] = "TraNsfER-ENcOdiNg:    	dEflaTE	 ,  ,  	,  P4lVdK%%we_#X!m";
    // char transferEncoding[] = "TraNsfER-ENcOdiNg:    	dEflaTE	 ,  ,  	,  ";
    // char transferEncoding[] = "TraNsfER-ENcOdiNg:    	dEflaTE	 ,,  	,P4lVdK%%we_#X!m";
    char transferEncoding[] = "TrAnSfEr-encoDInG:,	,ComPRESs 	, chuNKED, 	Z#QT~h4p#u|PPK5= nKi|kxaOW$uKHke";

    Noeud *test = malloc(sizeof(Noeud));
    int i = 0;

    if (!checkTransferEncodingHeader(transferEncoding, &i, strlen(transferEncoding), test)) {
        printf("Hello world\n");
        test = NULL;
    }

    if (test != NULL) {
        printArbre(test, 0);
    }
    
    return 0;
}

bool checkTransferEncodingHeader(char transferEncoding[], int *i, int longueur, Noeud *noeud){//Transfer-Encoding-header = "Transfer-Encoding" ":" OWS Transfer-Encoding OWS

    VERIFICATION()

    int nombreFils = 5;
    const int indice = *i;
    Noeud *filsTransferEncodingMot = malloc(sizeof(Noeud));
    if (!checkTransferEncodingMot(transferEncoding, filsTransferEncodingMot)) {
        free(noeud);
        *i = indice;
        return false;
    }
    *i=17;
    if (transferEncoding[*i] != 58) { //? ":" = 58
        free(noeud);
        *i = indice;
        return false;
    }
    (*i)++;

    Noeud *filsOWS1 = malloc(sizeof(Noeud));
    checkOWS(transferEncoding, i, longueur, filsOWS1);

    Noeud *filsTransferEncoding = malloc(sizeof(Noeud));
    if (!checkTransferEncoding(transferEncoding, i, longueur, filsTransferEncoding)){
        free(noeud);
        *i = indice;
        return false;
    }

    Noeud *filsOWS2 = malloc(sizeof(Noeud));
    checkOWS(transferEncoding, i, longueur, filsOWS2);

    noeud->fils = malloc(nombreFils * sizeof(Noeud));
    noeud->valeur = transferEncoding + indice;
    noeud->longueur = *i - indice;
    noeud->nombreFils = nombreFils;
    noeud->tag = "Transfer-Encoding-header";

    *i = indice; // On réinitialise i
    
    noeud->fils[0] = *filsTransferEncodingMot;
    (*i) += noeud->fils[0].longueur;
    
    createFilsSimple("case_insensitive_string", transferEncoding + *i, 1, &noeud->fils[1]);
    (*i)++;
    
    noeud->fils[2] = *filsOWS1;
    (*i) += noeud->fils[2].longueur;

    noeud->fils[3] = *filsTransferEncoding;
    (*i) += noeud->fils[3].longueur;

    noeud->fils[4] = *filsOWS2;
    (*i) += noeud->fils[4].longueur;

    return true;

}

void createFilsSimple(char nom[], char *i, int longueur, Noeud *noeud) {
    noeud->tag = nom;
    noeud->fils = NULL;
    noeud->valeur = i;
    noeud->longueur = longueur;
    noeud->nombreFils = 0;
}

bool checkTransferEncodingMot(char transferEncoding[], Noeud *noeud) {
    noeud->valeur = transferEncoding;
    noeud->longueur = 17;
    noeud->tag = "case_insensitive_string";
    noeud->fils = NULL;
    noeud->nombreFils = 0;
    char transferEncodingMinuscule[18];
    sousChaineMinuscule(transferEncoding, transferEncodingMinuscule, 0, 17);
    int a=strcmp("transfer-encoding", transferEncodingMinuscule);
    if (a!=0){
        free(noeud);
        return false ;
    }
    return true;
}

bool checkOWS(char transferEncoding[], int *i, int longueur, Noeud *noeud) { //OWS = *( SP / HTAB )

    //VERIFICATION()

    int compteur = 0;
    const int indice = *i;
    
    while ((*i<longueur)&&(transferEncoding[*i]==32 || transferEncoding[*i]==9)){
        (*i)++;
        compteur++;
    }

    if (noeud!=NULL) {
        noeud->valeur = transferEncoding + indice;
        noeud->longueur = *i - indice;
        noeud->tag = "OWS";
        if (compteur > 0) {
            noeud->fils = malloc(compteur * sizeof(Noeud));
        }
        noeud->nombreFils = compteur;

        // On réinitialise l'indice i pour la suite de la fonction
        *i = indice;
        // On remplit le tableau des fils du noeud OWS
        for (int j = 0; j < compteur; j++) {
            if (transferEncoding[*i]==32) {
                createFilsSimple("SP", transferEncoding + *i, 1, &noeud->fils[j]);
            }
            else {
                createFilsSimple("HTAB", transferEncoding + *i, 1, &noeud->fils[j]);
            }
            (*i)++;
        }
    }
    return true;
}

bool checkTransferEncoding(char transferEncoding[], int *i, int longueur, Noeud *noeud) {
    //Transfer-Encoding = *( "," OWS ) transfer-coding *( OWS "," [ OWS transfer-coding ] )

    VERIFICATION()

    int compteur = 0;
    int const indice = *i;
    bool reachCoding = false; // Indique si on a atteint le premier transfer-coding pour savoir si on inverse OWS et ,
    bool owsChecked = false;

    while ((*i)<longueur && transferEncoding[*i] == 44) {
        (*i)++;
        compteur++;
        checkOWS(transferEncoding, i, longueur, NULL);
        compteur++;
    }

    if (!checkTransferCoding(transferEncoding, i, longueur, NULL)) {
            free(noeud);
            *i = indice;
            return false;
    }
    compteur++;

    // (*i)++;
    checkOWS(transferEncoding, i, longueur, NULL);
    while (*i<longueur && transferEncoding[*i]==44) {
        compteur+=2;
        (*i)++;

        // int iTemp=(*i);
        checkOWS(transferEncoding, i, longueur, NULL);

        if(checkTransferCoding(transferEncoding, i, longueur, NULL)) {
            if (*i>=longueur) {
                compteur+=2;
                break;
            } else {
                // (*i)++;
                compteur+=2;
                checkOWS(transferEncoding, i, longueur, NULL);
            }
        }
    }

    // printf("compteur = %d\n", compteur);

    noeud->valeur = transferEncoding + indice;
    noeud->longueur = *i - indice;
    noeud->tag = "Transfer-Encoding";
    noeud->fils = malloc(compteur * sizeof(Noeud));
    noeud->nombreFils =compteur;

    // On réinitialise l'indice i pour la suite de la fonction
    *i = indice;

    // On remplit le tableau des fils du noeud method
    for (int j = 0; j < compteur; j++) {

        int iTemp=*i;
        if(transferEncoding[*i]==44) {
            if (!owsChecked && reachCoding) {
                checkOWS(transferEncoding, i, longueur, &noeud->fils[j]);
                j++;
            }
            createFilsSimple("case_insensitive_string", transferEncoding + *i, 1, &noeud->fils[j]);
            (*i)++;
            if (!reachCoding) {
                j++;
                checkOWS(transferEncoding, i, longueur, &noeud->fils[j]);
            }
            owsChecked = false;
        }
        else if (checkTransferCoding(transferEncoding, i, longueur, NULL)){
            *i=iTemp;
            if (!owsChecked && reachCoding) {
                checkOWS(transferEncoding, i, longueur, &noeud->fils[j]);
                j++;
            }
            checkTransferCoding(transferEncoding, i, longueur, &noeud->fils[j]);
            reachCoding = true;
            owsChecked = false;
        }
        else {
            checkOWS(transferEncoding, i, longueur, &noeud->fils[j]);
            owsChecked = true;
        }
    }
    return true;  
}

bool checkTransferCoding(char transferEncoding[], int *i, int longueur, Noeud *noeud){
    //transfer-coding = "chunked" / "compress" / "deflate" / "gzip" / transfer-extension

    VERIFICATION()

    // if (*i == longueur) {
    //     if (noeud != NULL) {
    //         free(noeud);
    //     }
    //     return false;
    // }

    const int indice=*i;

    // printf("Test = %d\n", *i);

    if (noeud!=NULL){
        noeud->valeur = transferEncoding + indice;
        noeud->tag = "transfer-coding";
        noeud->fils = malloc(sizeof(Noeud));
        noeud->nombreFils = 1;
    }

    (*i)=indice;
    char transferEncodingMinuscule[9];
    sousChaineMinuscule(transferEncoding, transferEncodingMinuscule, *i, (*i)+7);
    if (strcmp("chunked", transferEncodingMinuscule)==0){
        if (noeud!=NULL){
            noeud->longueur = 7;
            (*i)=indice;
            createFilsSimple("case_insensitive_string", transferEncoding + *i, 7, &noeud->fils[0]);
        }
        (*i)+=7;
        return true;
    }

    sousChaineMinuscule(transferEncoding, transferEncodingMinuscule, *i, (*i)+8);
    if (strcmp("compress", transferEncodingMinuscule)==0){
        if (noeud!=NULL){
            noeud->longueur = 8;
            (*i)=indice;
            createFilsSimple("case_insensitive_string", transferEncoding + *i, 8, &noeud->fils[0]);
        }
        (*i)+=8;
        return true;
    }

    sousChaineMinuscule(transferEncoding, transferEncodingMinuscule, *i, (*i)+7);
    if (strcmp("deflate", transferEncodingMinuscule)==0){
        if (noeud!=NULL){
            noeud->longueur = 7;
            (*i)=indice;
            createFilsSimple("case_insensitive_string", transferEncoding + *i, 7, &noeud->fils[0]);
        }
        (*i)+=7;
        return true;
    }

    sousChaineMinuscule(transferEncoding, transferEncodingMinuscule, *i, (*i)+4);
    if (strcmp("gzip", transferEncodingMinuscule)==0){
        if (noeud!=NULL){
            noeud->longueur = 4;
            (*i)=indice;
            createFilsSimple("case_insensitive_string", transferEncoding + *i, 4, &noeud->fils[0]);
        }
        (*i)+=4;
        return true;
    }
    if (checktransferextension(transferEncoding, i, longueur, NULL)){
        *i=indice;
        Noeud *filsTransferExtension=malloc(sizeof(Noeud));
        checktransferextension(transferEncoding, i, longueur, filsTransferExtension);
        if (noeud!=NULL){
            noeud->longueur = (*i)-indice;
            noeud->fils[0]=*filsTransferExtension;
        }
        return true;
    }

    else{
        free(noeud);
        (*i)=indice;
        return false;
    }
    return true;
}

bool checktransferextension(char transferEncoding[], int *i, int longueur, Noeud *noeud) {
    //transfer-extension = token *( OWS ";" OWS transfer-parameter )

    VERIFICATION()

    int compteur = 0;
    int const indice = *i;
    bool owsChecked = false; // Booléen utilisé pour savoir si on a eu un OWS juste avant ou non
    // int point_virgule = -1;
    
    if (!checkToken(transferEncoding, i, longueur, NULL, "")) {
        free(noeud);
        (*i) = indice;
        return false;
    }
    compteur++;

    // On vérifie si on a une suite au token
    int const indice2 = *i; // On garde l'indice en mémoire

    checkOWS(transferEncoding, i, longueur, NULL);
    // if (transferEncoding[*i]==59) {
    //     point_virgule=*i;
    // }
    while (*i<longueur && transferEncoding[*i]==59) {
        compteur+=2;
        (*i)++;
        checkOWS(transferEncoding, i, longueur, NULL);
        if (!checkTransferParameter(transferEncoding, i, longueur, NULL)) {
            compteur -= 2;
            (*i) = indice2;
            break;
        }
        compteur += 2;
        checkOWS(transferEncoding, i, longueur, NULL);


        // if (checkTransferParameter(transferEncoding, i, longueur, NULL)){
        //     compteur+=2;
        //     (*i)++;
        //     checkOWS(transferEncoding, i, longueur, NULL);
        // }
    }

    if (noeud!=NULL){
        noeud->valeur = transferEncoding + indice;
        noeud->longueur = *i - indice;
        noeud->tag = "transfer-extension";
        noeud->fils = malloc(compteur * sizeof(Noeud));
        noeud->nombreFils =compteur;

        // On réinitialise l'indice i pour la suite de la fonction
        *i = indice;

        checkToken(transferEncoding, i, longueur, &noeud->fils[0], "token");

        for (int j = 1; j < compteur; j++) {
            int iTemp = *i;

            // if ((*i<point_virgule || point_virgule == -1) && checkToken(transferEncoding, i, longueur, NULL, "")) {
            //     checkToken(transferEncoding, i, longueur, &noeud->fils[j], "token");
            // }
            if(transferEncoding[*i]==59) {
                if (!owsChecked) {
                    checkOWS(transferEncoding, i, longueur, &noeud->fils[j]);
                    j++;
                }
                createFilsSimple("case_insensitive_string", transferEncoding + *i, 1, &noeud->fils[j]);
                (*i)++;
                owsChecked = false;
            }
            else if(checkTransferParameter(transferEncoding, i, longueur, NULL)) {
                *i=iTemp;
                if (!owsChecked) {
                    checkOWS(transferEncoding, i, longueur, &noeud->fils[j]);
                    j++;
                }
                checkTransferParameter(transferEncoding, i, longueur, &noeud->fils[j]);
                owsChecked = false;
            }
            else {
                *i=iTemp;
                checkOWS(transferEncoding, i, longueur, &noeud->fils[j]);
                owsChecked = true;
            }
        }
    }
    return true;
}

bool checkTransferParameter(char transferEncoding[], int *i, int longueur, Noeud *noeud){
    //transfer-parameter = token BWS "=" BWS ( token / quoted-string )

    VERIFICATION()

    //BWS=OWS
    int compteur=0;
    const int indice=*i;
    if (!checkToken(transferEncoding, i, longueur, NULL, "")) {
        free(noeud);
        (*i) = indice;
        return false;
    }
    compteur++;
    // while ((*i)<longueur && checkTChar(transferEncoding, *i, NULL)){
    //     (*i)++;
    //     compteur++;
    // }
    // if (compteur<0){
    //     free(noeud);
    //     *i=indice;
    //     return false;
    // }
    checkOWS(transferEncoding, i, longueur, NULL);
    compteur++;
    if (transferEncoding[*i]!=61){
        free(noeud);
        *i=indice;
        return false;
    }
    (*i)++;
    compteur++;
    checkOWS(transferEncoding, i, longueur, NULL);
    compteur++;
    if (checkQuotedString(transferEncoding, i, longueur, NULL)){
        compteur++;
    }
    else {
        if (!checkToken(transferEncoding, i, longueur, NULL, "")) {
            free(noeud);
            (*i) = indice;
            return false;
        }
        compteur++;
    }

    if (noeud!=NULL){
        noeud->valeur = transferEncoding + indice;
        noeud->longueur = *i - indice;
        noeud->tag = "transfer-parameter";
        noeud->fils = malloc(compteur * sizeof(Noeud));
        noeud->nombreFils =compteur;

        *i = indice;

        for (int j=0;j<compteur;j++){
            int iTemp=*i;
            if (checkToken(transferEncoding, i, longueur, NULL, "token")) {
                (*i) = iTemp;
                checkToken(transferEncoding, i, longueur, &noeud->fils[j], "token");
                // (*i)++;
            }
            // else if (transferEncoding[*i]==61){
            //     checkOWS(transferEncoding, i, longueur, &noeud->fils[j]);
            //     j++;
            //     createFilsSimple("case_insensitive_string", *i, 1, &noeud->fils[j]);
            //     printf("je suis un égal \n");
            //     (*i)++;
            //     j++;
            //     checkOWS(transferEncoding, i, longueur, &noeud->fils[j]);
            // }
            else if (checkQuotedString(transferEncoding, i, longueur, NULL)){
                *i=iTemp;
                checkQuotedString(transferEncoding, i, longueur, &noeud->fils[j]);
            }
            else {
                (*i)=iTemp;
                checkOWS(transferEncoding, i, longueur, &noeud->fils[j]);
                j++;
                createFilsSimple("case_insensitive_string", transferEncoding + *i, 1, &noeud->fils[j]);
                (*i)++;
                j++;
                checkOWS(transferEncoding, i, longueur, &noeud->fils[j]);
            }
            // (*i)++;
        }
    }
    return true;
}

bool checkQuotedString(char transferEncoding[], int *i, int longueur, Noeud *noeud){
    //quoted-string = DQUOTE *( qdtext / quoted-pair ) DQUOTE

    VERIFICATION()

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

    VERIFICATION()

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
bool checkTChar(char requete[], int i, Noeud *noeud) {
    //! Cette fonction a deux éxécution différentes:
    //* - Une qui ne fait que vérifier syntaxiquement un caractère
    //* - Une qui vérifie syntaxiquement un caractère mais stocke aussi ce caractère

    // On stocke les données nécessaires pour le noeud courant
    if (noeud != NULL) {
        noeud->fils = NULL;
        noeud->valeur = requete + i;
        noeud->longueur = 1;
        noeud->nombreFils = 0;
        noeud->tag = "tchar";
    }

    // On vérifie que la syntaxe est correcte
    switch (requete[i]) {
        case '!':
        case '#':
        case '$':
        case '%':
        case '&':
        case 39 : //? ' = 39
        case '*':
        case '+':
        case '-':
        case '.':
        case '^':
        case '_':
        case '`':
        case '|':
        case '~':
            return true;
            break;
        
        default:
            break;
    }

    if (checkAlpha(requete, i) || checkDigit(requete, i)) {
        return true;
    }

    // Si la syntaxe n'est pas correcte, on libère la mémoire et on renvoie false
    if (noeud != NULL) {
        free(noeud);
    }

    return false;
}

bool checkAlpha(const char requete[], int i) {
    return ((requete[i] >= 'a' && requete[i] <= 'z') || (requete[i] >= 'A' && requete[i] <= 'Z'));
}

bool checkDigit(const char requete[], int i) {
    return (requete[i] >= '0' && requete[i] <= '9');
}

void sousChaineMinuscule(const char chaine1[], char chaine2[], int i, int j) {
    int diff = 'a' - 'A';
    int n = strlen(chaine1);
    for (int k = i; k < j; k++) {
        if (k < n) {
            if (chaine1[k] >= 'A' && chaine1[k] <= 'Z') {
                chaine2[k - i] = chaine1[k] + diff;
            } else {
                chaine2[k - i] = chaine1[k];
            }
        }
    }
    chaine2[j - i] = '\0';
}

bool checkToken(char requete[], int *i, int longueur, Noeud *noeud, char nom[]) {

    //VERIFICATION()

    int compteur = 0;
    const int indice = *i;

    // On compte le nombre de caractères dans le champ courant
    while (*i < longueur && checkTChar(requete, *i, NULL)) {
        (*i)++;
        compteur++;
    }

    // Si on a 0 tchar, on renvoie false
    if (compteur < 1) {
        if (noeud != NULL) {
            free(noeud);
        }
        (*i) = indice;
        return false;
    }

    if (noeud != NULL) {
        // On stocke les données nécessaires pour le noeud courant
        noeud->valeur = requete + indice;
        noeud->longueur = *i - indice;
        noeud->tag = nom;
        noeud->fils = malloc(compteur * sizeof(Noeud));
        noeud->nombreFils = compteur;

        // On réinitialise l'indice i pour la suite de la fonction
        *i = indice;

        // On remplit le tableau des fils du noeud method
        for (int j = 0; j < compteur; j++) {
            checkTChar(requete, *i, &noeud->fils[j]); // On a pas besoin de récupérer la valeur de retour cette fois-ci
            (*i)++;
        }
    }
    
    return true;
}