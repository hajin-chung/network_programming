#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE    100

void error_handling(char* message); int main() { int sock; struct sockaddr_in serv_addr; char buf[BUF_SIZE]; int port = 5000; char ip[20]="127.0.0.1"; char fname[20] = "p2.txt"; int opt = 1;  // for socket reuse. I dont know why... 
    FILE *fp = NULL;
    int nbyte; sock=socket(PF_INET, SOCK_STREAM, 0); if(sock == -1) error_handling("socket() error"); memset(&serv_addr, 0, sizeof(serv_addr)); serv_addr.sin_family=AF_INET; serv_addr.sin_addr.s_addr=inet_addr(ip); serv_addr.sin_port=htons(port); if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) error_handling("setsockopt() error!"); if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))==-1) error_handling("connect() error!"); fp = fopen("./proto.c", "rb"); if ( fp == NULL ) { printf("File open error;\n"); exit(0); } printf("File open success\n"); while(1) { memset(buf, 0, BUF_SIZE); nbyte=read(sock, buf, BUF_SIZE); if( nbyte <= 0 ) { error_handling("read() error!"); break; } printf("recieved %d bytes\n", nbyte); fwrite(buf, 1, nbyte, fp); } close(sock); fclose(fp); return 0; } void error_handling(char *message) { fputs(message, stderr); fputc('\n', stderr); exit(1); }
