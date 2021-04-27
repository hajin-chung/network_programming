#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE    100

void error_handling(char *message);

int main() {
    int serv_sock;
    int clnt_sock;
    int port = 5000;

    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size;

    char buf[BUF_SIZE];
    int opt = 1; // for socket reuse

    int nbyte;
    FILE *fp = NULL;
    char fname[20] = "data.txt";

    serv_sock=socket(PF_INET, SOCK_STREAM, 0);
    if(serv_sock == -1)
        error_handling("socket() error");
    
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family=AF_INET;
    serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
    serv_addr.sin_port=htons(port);

    if (setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
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

    fp = fopen(fname, "rb");
    if ( fp == NULL ) {
        printf("File open error;\n");
        exit(0);
    }
    printf("File open success\n");
   
    while(1) {
        memset(buf, 0, BUF_SIZE);

        nbyte = fread(buf, 1, BUF_SIZE, fp);
        if ( nbyte <= 0 ) {
            printf("file data read error \n");
            break;
        }
        write(clnt_sock, buf, nbyte);
    } 

    close(clnt_sock);    
    close(serv_sock);
    fclose(fp);

    return 0;
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
