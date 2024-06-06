#ifndef PHP_S
#define PHP_S

void phpServerResponse(char *path, int version, char* connection, Method method , unsigned clientid ,char *chaine, char *contentLength, int length1,
        char *contentType, int length2, char *cookie, int length3);

#endif
