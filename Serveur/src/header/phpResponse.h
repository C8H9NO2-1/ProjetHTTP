#ifndef PHPRESPONSE_H
#define PHPRESPONSE_H

struct ListAnswers {
    FCGI_Header answer;
    struct ListAnswers *next;
};
typedef struct ListAnswers ListAnswers;

int readPHPResponse(int fd);

#endif
