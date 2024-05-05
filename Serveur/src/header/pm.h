#ifndef PM_H
#define PM_H

typedef enum Method Method;
enum Method {
    GET, HEAD
};

bool semanticStartLine(void *root, Method *method, int *version);
bool checkPath(char *path, int len);
FILE* checkExistenceWithHost(char *path, int lenPath, char *host, int lenHost);
FILE* checkExistence(char *path, int len);
FILE* defaultPath(char *host, int len);
char *copy(char *str, int len);

#endif
