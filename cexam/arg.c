#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE    1024

void error_handling(char *message);

int main(int argc, char *argv[]) {
    int port;
    char ip[20];
	int sock;
	struct sockaddr_in serv_addr;
	char message[BUF_SIZE];
	int str_len, adr_sz;
	

    if (argc != 3) {
        printf(" error \n");
        exit(0);
    }

    strcpy(ip, argv[1]);
    port = atoi(argv[2]);
    printf(" OK ip: %s port: %d \n", ip, port);

	sock=socket(PF_INET, SOCK_DGRAM, 0);
	if(sock == -1)
		error_handling("socket() error");

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=inet_addr(ip);
	serv_addr.sin_port=htons(port);


    while(1) {
        fputs("Insert message(q to quit): ", stdout);
        fgets(message, sizeof(message), stdin);

        if(!strcmp(message, "Q\n") || !strcmp(message, "q\n"))
            break; 

        sendto(sock, message, strlen(message), 0, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
        adr_sz = sizeof(serv_addr);
        str_len = recvfrom(sock, message, BUF_SIZE, 0, (struct sockaddr*)&serv_addr, &adr_sz);
        message[str_len] = 0;
        printf("Message from server : %s", message);
    }
        

    return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
