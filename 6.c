#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/ip.h>

int main() {
    int s, n;
    char buffer[65536];
    struct iphdr *ip;

    s = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);

    if(s < 0) {
        printf("Socket error\n");
        return 1;
    }

    printf("Capturing Packets...\n");

    while(1) {
        n = recv(s, buffer, sizeof(buffer), 0);

        ip = (struct iphdr *)buffer;

        printf("\nSource IP: %s",
               inet_ntoa(*(struct in_addr *)&ip->saddr));

        printf("\nDestination IP: %s",
               inet_ntoa(*(struct in_addr *)&ip->daddr));

        printf("\nProtocol: %d\n", ip->protocol);
    }

    close(s);
    return 0;
}
