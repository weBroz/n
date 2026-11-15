#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main()
{
    int s;
    char msg[100];
    struct sockaddr_in x;
    socklen_t len = sizeof(x);

    s = socket(AF_INET, SOCK_DGRAM, 0);

    x.sin_family = AF_INET;
    x.sin_port = htons(8080);
    x.sin_addr.s_addr = inet_addr("127.0.0.1");

    strcpy(msg, "TIME");

    sendto(s, msg, strlen(msg)+1, 0,(struct sockaddr*)&x, sizeof(x));

    recvfrom(s, msg, 100, 0,(struct sockaddr*)&x, &len);

    printf("Server Time: %s", msg);

    close(s);
    return 0;
}