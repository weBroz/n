#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main() {
    int s, len;
    char msg[500], result[1000]="";
    struct sockaddr_in x;

    s = socket(AF_INET, SOCK_DGRAM, 0);

    x.sin_family = AF_INET;
    x.sin_port = htons(8080);
    x.sin_addr.s_addr = INADDR_ANY;

    bind(s, (struct sockaddr*)&x, sizeof(x));

    printf("Waiting for client...\n");

    len = sizeof(x);
    int n = recvfrom(s, msg, 499, 0,(struct sockaddr*)&x, &len);
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

    sendto(s, result, strlen(result)+1, 0,(struct sockaddr*)&x, len);

    close(s);
    return 0;
}