#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    char req1[] = "GET / HTTP/1.0\r\n\r\n";
    char req2[] = "GET /index%2ehtml HTTP/1.1\r\nHost: www.hilopt.com\r\n\r\n";
    char req3[] = "GET /images/logo.png HTTP/1.2\r\nHost: www.paphypu.fr\r\nConnection: close\r\n\r\n";
    char req4[] = "GET /test1 HTTP/1.0\r\nConnection: keep-alive\r\n\r\n";
    char req5[] = "GET / HTTP/1.1\r\nHost: www.hilopt.com\r\nAccept: text/html;q=0.0\r\n\r\n";
    char req6[] = "HEAD /images/logo.png HTTP/1.0\r\nAccept: text/html, image/png\r\nAccept-Encoding: gzip, identity;q=0\r\n\r\n";
    char req7[] = "GET / HTTP/1.1\r\nHost: 127.0.0.1\r\nTransfer-Encoding: gzip, chunked\r\n\r\n";
    char req8[] = "HEAD / HTTP/1.1\r\nHost: www.wichopool.com\r\nAccept-Encoding: gzip;q=0.8, identity;q=0.0, deflate;q=0.9\r\n\r\n";

    FILE *file = fopen("test1", "w");
    fputs(req1, file);
    fclose(file);

    file = fopen("test2", "w");
    fputs(req2, file);
    fclose(file);

    file = fopen("test3", "w");
    fputs(req3, file);
    fclose(file);

    file = fopen("test4", "w");
    fputs(req4, file);
    fclose(file);

    file = fopen("test5", "w");
    fputs(req5, file);
    fclose(file);

    file = fopen("test6", "w");
    fputs(req6, file);
    fclose(file);

    file = fopen("test7", "w");
    fputs(req7, file);
    fclose(file);

    file = fopen("test8", "w");
    fputs(req8, file);
    fclose(file);

    return 0;
}
