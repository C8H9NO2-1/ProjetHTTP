#ifndef BEGIN_H
#define BEGIN_H

FCGI_Header *beginRequest();
FCGI_Header *stdinRequest();
FCGI_BeginRequestBody *beginRequestBody();
FCGI_Header *BeginRequestHeader(FCGI_BeginRequestBody* begin);
FCGI_Header *stdinHeader();

#endif