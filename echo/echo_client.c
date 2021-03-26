#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

void error_handling(char *message);

int main()
{
    int sock;
    struct sockaddr_in serv_addr;
    char message[30];
    int str_len;
    int port = 5000;
    char ip[20]="127.0.0.1";
    
    int yes = 1;  // for socket reuse. I dont know why... 

    sock=socket(PF_INET, SOCK_STREAM, 0);
    if(sock == -1)
        error_handling("socket() error");
    
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family=AF_INET;
    serv_addr.sin_addr.s_addr=inet_addr(ip);
    serv_addr.sin_port=htons(port);

    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) {
        error_handling("setsockopt() error!");
    }

    if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))==-1) 
        error_handling("connect() error!");
    
    while(1) {
        memset(message, 0, sizeof(message)); 
        str_len=read(sock, message, sizeof(message));

        if(str_len==-1) {
            error_handling("read() error!");
            break;
        } else if(strcmp(message, "/End") == 0 || str_len == 0) {
            printf("Connection Ended!");
            break;
        }
        printf("Message from server: %s \n", message);  
    } 
    close(sock);

    return 0;
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
