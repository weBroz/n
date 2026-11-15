#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main() {
    int s, n, a[10][10];
    char result[50];
    struct sockaddr_in addr;

    s = socket(AF_INET, SOCK_STREAM, 0);

    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(s, (struct sockaddr*)&addr, sizeof(addr));

    printf("Enter N: ");
    scanf("%d", &n);
    printf("Matrix:\n");

    for(int i=0; i<n; i++) {
        for(int j=0; j<n; j++) {
            a[i][j] = rand()%50+1;
            printf("%d ", a[i][j]);
        }
        printf("\n");
    }

    write(s, &n, sizeof(n));
    write(s, a, sizeof(a));
    read(s, result, sizeof(result));

    printf("Matrix Type: %s\n", result);
    close(s);
    return 0;
}
