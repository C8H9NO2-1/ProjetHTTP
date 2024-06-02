#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "header/api.h"
#include "header/ismail.h"

// bool Transfer_Encoding =false ;  // in order to know if a header exist or not :how can i know ??
                                // see with elisa if she can give the info 
                                // or we can define a struct with all name of header of bool type ...
bool thereIsTransfer_Encoding(void *root){
    _Token *r= searchTree(root,"transfer-coding");
    if (r != NULL){
        return true;
        printf("il existe un transfer encoding");
    }
    return false;
}
bool semanticContentLength(void *root){
    //A sender MUST NOT send a Content-Length header field in any message
    //  that contains a Transfer-Encoding header field.
    // bool Transfer_Encoding=;
    if (thereIsTransfer_Encoding(root)){
        return false;
    }
    
    /*If a message is received that has multiple Content-Length header
   fields with field-values consisting of the same decimal value, or a
   single Content-Length header field with a field value containing a
   list of identical decimal values (e.g., "Content-Length: 42, 42"),
   indicating that duplicate Content-Length header fields have been
   generated or combined by an upstream message processor, then the
   recipient MUST either reject the message as invalid or replace the
   duplicated field-values with a single valid Content-Length field
   containing that decimal value prior to determining the message body
   length or forwarding the message.*/

   // i choose to reject the message 
    _Token *r,*tok;
    r=searchTree(root, "Content-Length");
    tok=r;
    int i=0;
    while(tok !=NULL){
        i++;
        printf("%d\n",i);
        if (i==2){
            return false;
        }
        tok =tok->next;      
    }
    return true;
}

// bool semanticContentType(void * root){
    
// }

// les contents types qu'on traite  