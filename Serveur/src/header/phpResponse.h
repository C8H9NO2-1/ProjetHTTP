#ifndef PHPRESPONSE_H
#define PHPRESPONSE_H

struct ListAnswers {
    FCGI_Header answer;
    struct ListAnswers *next;
};
typedef struct ListAnswers ListAnswers;

struct PHPResponse {
    bool error;
    ContentType type;
    char *content;
};
typedef struct PHPResponse PHPResponse;

ListAnswers* readPHPResponse(int fd);

#endif
