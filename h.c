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


#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main() {
    int s, len;
    char msg[500], result[1000];
    struct sockaddr_in addr;

    s = socket(AF_INET, SOCK_DGRAM, 0);

    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    printf("Enter sentence: ");
    fgets(msg, 500, stdin);

    sendto(s, msg, strlen(msg)+1, 0,(struct sockaddr*)&addr, sizeof(addr));

    len = sizeof(addr);

    recvfrom(s, result, 1000, 0,(struct sockaddr*)&addr, &len);

    printf("Translated sentence: %s\n", result);

    close(s);
    return 0;
}

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main() {
    int s, len;
    char msg[500], result[1000]="";
    struct sockaddr_in addr;

    s = socket(AF_INET, SOCK_DGRAM, 0);

    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);
    addr.sin_addr.s_addr = INADDR_ANY;

    bind(s, (struct sockaddr*)&addr, sizeof(addr));

    printf("Waiting for client...\n");

    len = sizeof(addr);
    int n = recvfrom(s, msg, 499, 0,(struct sockaddr*)&addr, &len);
    msg[n] = '\0';

    char *word = strtok(msg, " ,.!?;");

    while(word != NULL) {

        if(strcmp(word,"tbh")==0)
            strcat(result,"to be honest ");
        else if(strcmp(word,"ig")==0)
            strcat(result,"I guess ");
        else if(strcmp(word,"btw")==0)
            strcat(result,"by the way ");
        else if(strcmp(word,"atm")==0)
            strcat(result,"at the moment ");
        else if(strcmp(word,"irl")==0)
            strcat(result,"in real life ");
        else if(strcmp(word,"lol")==0)
            strcat(result,"laughing out loud ");
        else if(strcmp(word,"asap")==0)
            strcat(result,"as soon as possible ");
        else if(strcmp(word,"omg")==0)
            strcat(result,"oh my god ");
        else if(strcmp(word,"ttyl")==0)
            strcat(result,"talk to you later ");
        else if(strcmp(word,"idk")==0)
            strcat(result,"I don't know ");
        else if(strcmp(word,"nvm")==0)
            strcat(result,"never mind ");
        else {
            strcat(result,word);
            strcat(result," ");
        }

        word = strtok(NULL, " ,.!?;");
    }

    sendto(s, result, strlen(result)+1, 0,(struct sockaddr*)&addr, len);

    close(s);
    return 0;
}






#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <sys/wait.h>

int main() {
    int s, n;
    char name[50], msg[1024];
    struct sockaddr_in x;

    s = socket(AF_INET, SOCK_STREAM, 0);

    x.sin_family = AF_INET;
    x.sin_port = htons(8080);
    x.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(s, (struct sockaddr*)&x, sizeof(x));

    printf("Enter name: ");
    fgets(name, 50, stdin);
    name[strcspn(name, "\n")] = '\0';

    send(s, name, strlen(name), 0);

    if(fork() == 0) {
        while((n = recv(s, msg, 1023, 0)) > 0) {
            msg[n] = '\0';
            printf("\n%s", msg);
        }
        exit(0);
    }
    else {
        while(1) {
            printf("Enter message: ");
            fgets(msg, 1024, stdin);

            send(s, msg, strlen(msg), 0);

            if(strcmp(msg, "exit\n") == 0)
                break;
        }
        shutdown(s, SHUT_RDWR);
        close(s);
        wait(NULL);
    }
    return 0;
}







#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/select.h>

int main() {
    int s, c, i, j, n, max;
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

    printf("Chat Server Started\n");

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

        // Accept new client
        if(FD_ISSET(s, &fds)) {
            c = accept(s, NULL, NULL);

            for(i=0; i<10; i++)
                if(clients[i] == 0) break;

            if(i < 10) {
                n = recv(c, names[i], 49, 0);

                if(n > 0) {
                    names[i][n] = '\0';
                    clients[i] = c;

                    sprintf(out, "%s joined the chat\n", names[i]);

                    for(j=0; j<10; j++)
                        if(clients[j] > 0)
                            send(clients[j], out, strlen(out), 0);
                }
                else close(c);
            }
            else close(c);
        }

        // Receive and broadcast messages
        for(i=0; i<10; i++) {
            c = clients[i];

            if(c > 0 && FD_ISSET(c, &fds)) {
                n = recv(c, msg, 1023, 0);

                if(n <= 0 || (n == 5 && memcmp(msg, "exit\n", 5) == 0)) {
                    sprintf(out, "%s left the chat\n", names[i]);

                    close(c);
                    clients[i] = 0;

                    for(j=0; j<10; j++)
                        if(clients[j] > 0)
                            send(clients[j], out, strlen(out), 0);
                }
                else {
                    msg[n] = '\0';

                    sprintf(out, "%s: %s", names[i], msg);

                    for(j=0; j<10; j++)
                        if(clients[j] > 0)
                            send(clients[j], out, strlen(out), 0);
                }
            }
        }
    }
    return 0;
}






#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
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


#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main()
{
    int s, n;
    char msg[100];
    time_t t;
    struct sockaddr_in x;
    socklen_t len = sizeof(x);

    s = socket(AF_INET, SOCK_DGRAM, 0);

    x.sin_family = AF_INET;
    x.sin_port = htons(8080);
    x.sin_addr.s_addr = INADDR_ANY;

    bind(s, (struct sockaddr*)&x, sizeof(x));

    signal(SIGCHLD, SIG_IGN);

    while(1)
    {
        recvfrom(s, msg, 100, 0,(struct sockaddr*)&x, &len);
        if(fork() == 0)
        {
            time(&t);
            strcpy(msg, ctime(&t));
            sendto(s, msg, strlen(msg)+1, 0,(struct sockaddr*)&x, len);
            close(s);
            return 0;
        }
    }
    return 0;
}





#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main()
{
    int s;
    char file[100], msg[1000];
    struct sockaddr_in x;

    s = socket(AF_INET, SOCK_STREAM, 0);

    x.sin_family = AF_INET;
    x.sin_port = htons(8080);
    x.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(s, (struct sockaddr*)&x, sizeof(x));

    printf("Enter filename: ");
    scanf("%99s", file);

    send(s, file, strlen(file)+1, 0);

    recv(s, msg, 1000, 0);

    printf("\n%s\n", msg);

    close(s);
    return 0;
}






#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main()
{
    int s, c, n;
    char file[100], msg[1000];
    FILE *fp;
    struct sockaddr_in x;

    s = socket(AF_INET, SOCK_STREAM, 0);

    x.sin_family = AF_INET;
    x.sin_port = htons(8080);
    x.sin_addr.s_addr = INADDR_ANY;

    bind(s, (struct sockaddr*)&x, sizeof(x));
    listen(s, 5);

    while(1)
    {
        c = accept(s, NULL, NULL);

        if(fork() == 0)
        {
            close(s);

            recv(c, file, 100, 0);
            file[strcspn(file, "\n")] = '\0';

            fp = fopen(file, "r");

            if(fp == NULL)
                sprintf(msg, "PID: %d\nFile not found", getpid());
            else
            {
                sprintf(msg, "PID: %d\n", getpid());

                while(fgets(msg + strlen(msg),
                            sizeof(msg) - strlen(msg), fp));

                fclose(fp);
            }

            send(c, msg, strlen(msg)+1, 0);

            close(c);
            return 0;
        }

        close(c);
    }

    return 0;
}


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
