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
    int serv_sock;
    int clnt_sock;
    int port = 5000;

    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size;

    char in_msg[MSG_SIZE + 1], out_msg[MSG_SIZE+1];
    int yes = 1; // for socket reuse

    serv_sock=socket(PF_INET, SOCK_STREAM, 0);
    if(serv_sock == -1)
        error_handling("socket() error");
    
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family=AF_INET;
    serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
    serv_addr.sin_port=htons(port);

    if (setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) {
        error_handling("setsockopt() error!");
    }
    
    if(bind(serv_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr))==-1 )
        error_handling("bind() error"); 
    
    if(listen(serv_sock, 5)==-1)
        error_handling("listen() error");
    
    clnt_addr_size=sizeof(clnt_addr);  
    clnt_sock=accept(serv_sock, (struct sockaddr*)&clnt_addr,&clnt_addr_size);
    if(clnt_sock==-1)
        error_handling("accept() error");  
   
    while(1) {
        memset(out_msg, 0, sizeof(out_msg));
        memset(in_msg, 0, sizeof(in_msg));

        printf(" server msg: ");
        scanf("%s", out_msg);
        write(clnt_sock, out_msg, strlen(out_msg));

        listen(clnt_sock, 5);
        read(clnt_sock, in_msg, sizeof(in_msg));
        printf(" client msg: %s \n", in_msg);
    } 
    close(clnt_sock);    
    close(serv_sock);
    return 0;
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
