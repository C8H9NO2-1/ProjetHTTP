#ifndef PM_H
#define PM_H

typedef enum Method Method;
enum Method {
    GET, HEAD, POST
};

typedef enum ConnectionState ConnectionState;
enum ConnectionState {
    CLOSE, KEEPALIVE
};

bool semanticStartLine(void *root, Method *method, int *version);
bool semanticConnection(void *root, ConnectionState *state, int version);
bool checkPath(char *path, int len);
FILE* checkExistenceWithHost(char *path, int lenPath, char *host, int lenHost);
FILE* checkExistence(char *path, int len);
FILE* defaultPath(char *host, int len);
char *URINormalization(char *path, int len);
int convertHexdig(char *str);
bool isUnreserved(int x);
char *dotRemoval(char *path, int len);

#endif
