#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUFFER_SIZE 1024

void error_handling(char *message);

int main()
{
    int sock;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE+1];
    int str_len;
    int port = 9100;
    char ip[30];
    scanf("%s", ip);
    char yes = 'N';
    
    int opt = 1;   

    sock=socket(PF_INET, SOCK_STREAM, 0);
    if(sock == -1)
        error_handling("socket() error");
    
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family=AF_INET;
    serv_addr.sin_addr.s_addr=inet_addr(ip);
    serv_addr.sin_port=htons(port);

    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
        error_handling("setsockopt() error!");
    }

    if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))==-1) 
        error_handling("connect() error!");

    memset(buffer, '\0', sizeof(buffer)); 
    read(sock, buffer, sizeof(buffer));
    printf("%s\n", buffer);
    
    while(1) {
        printf("Message to send: ");
        scanf("%s", buffer);
        if(strcmp(buffer, "q") == 0 || strcmp(buffer, "Q") == 0) break;
        write(sock, buffer, strlen(buffer));

        memset(buffer, '\0', sizeof(buffer)); 
        str_len=read(sock, buffer, sizeof(buffer));
        if(str_len==-1) {
            error_handling("read() error!");
            break;
        } 

        printf("Message from server: %s\n", buffer);  
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
