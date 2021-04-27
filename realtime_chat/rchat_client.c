#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>

#define SUC_CODE "200"
#define ERR_CODE "400"
#define SET_NAME "SET_NAME"
#define SEND_MSG "SEND_MESSAGE"
#define GET_USERS "GET_USERS"

#define PORT 5000
#define BUFFER_SIZE 1024

struct Client {
    char name[30];
} user;

void error_handling(char *message);
void set_name(int sock);
void get_users(int sock);
void *send_message(void *sock);
void *read_message(void *sock);

int main()
{
    int sock;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE + 1];
    int str_len;
    char ip[20]="127.0.0.1";

    int opt = 1;  
    
    pthread_t send_thread;
    pthread_t read_thread;


    sock=socket(PF_INET, SOCK_STREAM, 0);
    if(sock == -1)
        error_handling("socket() error");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family=AF_INET;
    serv_addr.sin_addr.s_addr=inet_addr(ip);
    serv_addr.sin_port=htons(PORT);

    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) 
        error_handling("setsockopt() error!");

    if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))==-1) 
        error_handling("connect() error!");

    set_name(sock);
    get_users(sock);
    sleep(1);

    pthread_create(&send_thread, NULL, send_message, (void *)&sock);
    pthread_create(&read_thread, NULL, read_message, (void *)&sock);

    close(sock);

    pthread_exit(NULL);

    return 0;
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

void set_name(int sock) {
    char buffer[BUFFER_SIZE+1];
    memset(buffer, '\0', sizeof(buffer));
    
    printf("User name : ");
    scanf("%s", user.name);
    
    sprintf(buffer, "%s %s", SET_NAME, user.name);
    write(sock, buffer, strlen(buffer));
    memset(buffer, '\0', sizeof(buffer));
    read(sock, buffer, sizeof(buffer));
}

void get_users(int sock) {
    char buffer[BUFFER_SIZE+1];
    memset(buffer, '\0', sizeof(buffer));

    write(sock, GET_USERS, sizeof(GET_USERS));
    read(sock, buffer, sizeof(buffer));
    printf("User list\n");
    printf("%s\n", buffer);
}

void *send_message(void *sock) {
    int *socket = (int *)sock;
    char buffer[BUFFER_SIZE+1];
    char message[BUFFER_SIZE+1];
    char dest[BUFFER_SIZE+1];

    printf("Send to : ");
    scanf("%s", dest);
    printf("Message : ");
    fgets(message, BUFFER_SIZE, stdin);

    write(*socket, message, strlen(message)); 
}

void *read_message(void *sock) {
    int *socket = (int *)sock;
    int valread;
    char buffer[BUFFER_SIZE+1];
    
    while(1) {
        memset(buffer, '\0', sizeof(buffer));
        valread=read(*socket, buffer, sizeof(buffer));

        printf("server: %s\n", buffer);
        if(valread<= 0) {
            error_handling("read() error!");
            break;
        } 
    } 
}
