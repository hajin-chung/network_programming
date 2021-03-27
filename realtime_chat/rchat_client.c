#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define MSG_SIZE 101

void error_handling(char *message);

int main()
{
    int sock;
    struct sockaddr_in serv_addr;
    char in_msg[MSG_SIZE + 1], out_msg[MSG_SIZE+1];
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

    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) 
        error_handling("setsockopt() error!");

    if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))==-1) 
        error_handling("connect() error!");

    while(1) {
        memset(out_msg, 0, sizeof(out_msg));
        memset(in_msg, 0, sizeof(in_msg));

        str_len=read(sock, in_msg, sizeof(in_msg));

        if(str_len==-1) {
            error_handling("read() error!");
            break;
        } 

        printf(" server msg: %s", in_msg);  
          
        printf(" client msg: ");
        // scanf("%s", out_msg);

        fgets(out_msg, MSG_SIZE, stdin);

        write(sock, out_msg, strlen(out_msg));
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
