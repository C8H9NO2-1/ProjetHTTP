#ifndef BEGIN_H
#define BEGIN_H

FCGI_Header *beginRequest(int *l);
FCGI_Header *stdinRequest(int *l, char *chaine);
FCGI_BeginRequestBody *beginRequestBody();
FCGI_Header *BeginRequestHeader(FCGI_BeginRequestBody* begin, int *l);
FCGI_Header *stdinHeader(int *l, char* chaine);

#endif