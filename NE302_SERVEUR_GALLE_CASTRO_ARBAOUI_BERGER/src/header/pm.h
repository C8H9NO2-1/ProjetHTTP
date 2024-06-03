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

typedef enum ContentType ContentType;
enum ContentType {
    HTML, CSS, JAVASCRIPT, PNG, JPEG, GIF
};

bool semanticStartLine(void *root, Method *method, int *version);
bool semanticConnection(void *root, ConnectionState *state, int version);
FILE* checkExistenceWithHost(char *path, int lenPath, char *host, int lenHost);
FILE* checkExistence(char *path, int len);
FILE* defaultPath(char *host, int len);
char *URINormalization(char *path, int len);
int convertHexdig(char *str);
bool isUnreserved(int x);
char *dotRemoval(char *path, int len);
bool compareCaseInsensitive(char *str1, char *str2, int len);
bool acceptHeaderVerification(void *root, ContentType content);
bool auxAccept(char *header, const char *str);
bool checkAcceptString(char *str);
bool priorityVerification(char *str);
bool extensionMatch(const char *name, const char *ext);
ContentType typeFromPath(char *path, int len);
bool acceptEncodingHeaderVerification(void *root, EncodingState *coding);
bool auxAcceptEncoding(char *header);
EncodingState maximumPriority(char *header);
float priorityValue(char *str);

#endif
