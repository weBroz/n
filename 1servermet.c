#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main() {
    int s, c, n, a[10][10];
    int u=1, l=1, d=1, type;
    struct sockaddr_in addr;

    s = socket(AF_INET, SOCK_STREAM, 0);

    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);
    addr.sin_addr.s_addr = INADDR_ANY;

    bind(s, (struct sockaddr*)&addr, sizeof(addr));
    listen(s, 1);

    printf("Waiting...\n");
    c = accept(s, NULL, NULL);

    read(c, &n, sizeof(n));
    read(c, a, sizeof(a));

    for(int i=0; i<n; i++) {
        for(int j=0; j<n; j++) {
            if(i>j && a[i][j]!=0) u=0;
            if(i<j && a[i][j]!=0) l=0;
            if(i!=j && a[i][j]!=0) d=0;
        }
    }

    if(d) type=3;
    else if(u) type=1;
    else if(l) type=2;
    else type=0;

switch(type) {
    case 1: write(c, "Upper Triangular", 17); break;
    case 2: write(c, "Lower Triangular", 17); break;
    case 3: write(c, "Diagonal Matrix", 16); break;
    default: write(c, "Neither", 8);
}

    close(c);
    close(s);
    return 0;
}
