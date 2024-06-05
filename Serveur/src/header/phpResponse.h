#ifndef PHPRESPONSE_H
#define PHPRESPONSE_H

struct ListAnswers {
    FCGI_Header answer;
    struct ListAnswers *next;
};
typedef struct ListAnswers ListAnswers;

struct PHPResponse {
    bool error;
    char *content;
    int contentLength;
    int length;
};
typedef struct PHPResponse PHPResponse;

PHPResponse getPHPResponse(int fd);

ListAnswers* readPHPResponse(int fd);

#endif
