#ifndef CHECK_RESSOURCE_H
#define CHECK_RESSOURCE_H

bool checkPath(char *path, int len);
bool checkExistenceWithHost(char *path, int lenPath, char *host, int lenHost);
bool checkExistence(char *path, int len);
bool defaultPath(char *host, int len);
char *copy(char *str, int len);

#endif
