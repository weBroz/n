#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main() {
    int s, len;
    char msg[500], result[1000];
    struct sockaddr_in x;

    s = socket(AF_INET, SOCK_DGRAM, 0);

    x.sin_family = AF_INET;
    x.sin_port = htons(8080);
    x.sin_addr.s_addr = inet_addr("127.0.0.1");

    printf("Enter sentence: ");
    fgets(msg, 500, stdin);

    sendto(s, msg, strlen(msg)+1, 0,(struct sockaddr*)&x, sizeof(x));

    len = sizeof(x);

    recvfrom(s, result, 1000, 0,(struct sockaddr*)&x, &len);

    printf("Translated sentence: %s\n", result);

    close(s);
    return 0;
}
