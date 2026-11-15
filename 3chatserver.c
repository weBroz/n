#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main() {
    int s, c, n, i, j, max;
    int clients[10] = {0};
    char names[10][50], msg[1024], out[1100];
    fd_set fds;
    struct sockaddr_in x;

    s = socket(AF_INET, SOCK_STREAM, 0);

    x.sin_family = AF_INET;
    x.sin_port = htons(8080);
    x.sin_addr.s_addr = INADDR_ANY;

    bind(s, (struct sockaddr*)&x, sizeof(x));
    listen(s, 10);

    printf("Chat Server Started...\n");

    while(1) {
        FD_ZERO(&fds);
        FD_SET(s, &fds);
        max = s;

        for(i=0; i<10; i++) {
            if(clients[i] > 0) {
                FD_SET(clients[i], &fds);
                if(clients[i] > max)
                    max = clients[i];
            }
        }

        select(max+1, &fds, NULL, NULL, NULL);

        if(FD_ISSET(s, &fds)) {
            c = accept(s, NULL, NULL);

            for(i=0; i<10; i++)
                if(clients[i] == 0) break;

            if(i < 10) {
                n = recv(c, names[i], 49, 0);

                if(n > 0) {
                    names[i][n] = '\0';
                    clients[i] = c;

                    printf("%s joined\n", names[i]);

                    snprintf(out, sizeof(out),
                             "%s joined the chat\n", names[i]);

                    for(j=0; j<10; j++)
                        if(clients[j] > 0)
                            send(clients[j], out, strlen(out), 0);
                }
                else close(c);
            }
            else close(c);
        }

        for(i=0; i<10; i++) {
            c = clients[i];

            if(c > 0 && FD_ISSET(c, &fds)) {
                n = recv(c, msg, sizeof(msg)-1, 0);

                if(n <= 0 || (n == 5 && strcmp(msg, "exit\n")==0)) {
                    snprintf(out, sizeof(out),
                             "%s left the chat\n", names[i]);

                    close(c);
                    clients[i] = 0;

                    for(j=0; j<10; j++)
                        if(clients[j] > 0)
                            send(clients[j], out, strlen(out), 0);
                }
                else {
                    msg[n] = '\0';

                    snprintf(out, sizeof(out),
                             "%s: %s", names[i], msg);

                    for(j=0; j<10; j++)
                        if(clients[j] > 0)
                            send(clients[j], out, strlen(out), 0);
                }
            }
        }
    }

    close(s);
    return 0;
}
