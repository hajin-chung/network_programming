#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/select.h>

#define MAX_BUF	255
#define MAX_SOCK 10
#define TTL	2
#define BUF_SIZE 255

struct Client {
    int is_active;
    int fd;
    struct sockaddr_in addr;
};

void send_server_info(int sock, struct sockaddr_in adr, 
    char* server_ip, char* server_port);
void print_client_info(int max_client, struct Client clients[MAX_SOCK]);

int main(int argc, char **argv)
{
    int server_sockfd, client_sockfd, sockfd;
    int multicast_sockfd;
    int state, client_len;
    int i, max_client, maxfd;
    int on = 1;
	int time_live = TTL;
    struct Client clients[MAX_SOCK];
	struct timeval timeout;

    struct sockaddr_in clientaddr, serveraddr, multicastaddr;
    struct timeval tv;
    fd_set readfds, otherfds, allfds;

    char buf[MAX_BUF];
    int stdin_fd = fileno(stdin);
    char server_ip[20];
    char server_port[20];

    state = 0;

    if(argc!=5) {
	    printf("Usage : %s <MULTICAST IP> <MULTICAST PORT> <SERVER IP> <SERVER PORT>\n", argv[0]);
	    exit(1);
    }
    memset(server_ip, 0, 20);
    memset(server_port, 0, 20);

    strcpy(server_ip, argv[3]);
    strcpy(server_port, argv[4]);
    printf("[*] server ip: %s, server port: %s \n", server_ip, server_port);

    // multicast socket setup
	multicast_sockfd=socket(PF_INET, SOCK_DGRAM, 0);
    memset(&multicastaddr, 0, sizeof(multicastaddr));
	multicastaddr.sin_family=AF_INET;
	multicastaddr.sin_addr.s_addr=inet_addr(argv[1]);
	multicastaddr.sin_port=htons(atoi(argv[2]));
    printf("%s %s\n", argv[1], argv[2]);

	on=0;
	setsockopt(multicast_sockfd, IPPROTO_IP, 
        IP_MULTICAST_TTL, (void*)&time_live, sizeof(time_live));

    printf("[*] set server socket");
    // server socket setup
    if ((server_sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket error : ");
        exit(0);
    }

    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(atoi(argv[4]));

    state = bind(server_sockfd, (struct sockaddr *)&serveraddr,
            sizeof(serveraddr));
    if (state == -1) {
        perror("bind error : ");
        exit(0);
    }

    state = listen(server_sockfd, 5);
    if (state == -1) {
        perror("listen error : ");
        exit(0);
    }

    client_sockfd = server_sockfd;

    // init
    max_client = -1;
    maxfd = server_sockfd;

   for (i = 0; i < MAX_SOCK; i++) {
        clients[i].is_active = 0;
    }

    FD_ZERO(&readfds);
    FD_SET(server_sockfd, &readfds);
    FD_SET(stdin_fd, &readfds);
    //

    printf("\nTCP Server Starting ... \n\n\n");
    fflush(stdout);

    while(1)
    {
        printf("[*] Main while loop\n");
        allfds = readfds;
		timeout.tv_sec=5;
		timeout.tv_usec=5000;
        state = select(maxfd + 1 , &allfds, NULL, NULL, &timeout);

        if(state == 0) { // send server infos 
            send_server_info(multicast_sockfd, multicastaddr, server_ip, server_port);
        }

	    // Server Socket - accept from client
        if (FD_ISSET(server_sockfd, &allfds)) {
            client_len = sizeof(clientaddr);
            client_sockfd = accept(server_sockfd,
                    (struct sockaddr *)&clientaddr, &client_len);
            printf("\nconnection from (%s , %d)",
                    inet_ntoa(clientaddr.sin_addr),ntohs(clientaddr.sin_port));

            for (i = 0; i < MAX_SOCK; i++)
            {
                if (clients[i].is_active == 0) {
                    clients[i].fd = client_sockfd;
                    clients[i].addr = clientaddr;
                    printf("\nclientNUM=%d\nclientFD=%d\n", i+1, client_sockfd);
                    break;
                }
            }

            if (i == MAX_SOCK) {
                perror("too many clients\n");
            } else {
                printf("accept [%d]\n", client_sockfd);
                printf("===================================\n");
                FD_SET(client_sockfd,&readfds);
            }

            if (client_sockfd > maxfd)
                maxfd = client_sockfd;

            if (i > max_client)
                max_client = i;

            if (--state <= 0)
                continue;
        }

        // get commands from stdin
        if(FD_ISSET(stdin_fd, &readfds)) {
            memset(buf, 0, BUF_SIZE);
            scanf("%s", buf);
            
            if(strcmp(buf, "/list") == 0)
            {
                print_client_info(max_client, clients);
            }
        }

	    // client socket check
        for (i = 0; i <= max_client; i++)
        {
            if ((sockfd = clients[i].fd) < 0) {
                continue;
            }

            if (FD_ISSET(sockfd, &allfds))
            {
                memset(buf, 0, MAX_BUF);

                // Disconnet from Client 
                if (read(sockfd, buf, MAX_BUF) <= 0) {
                    printf("Close sockfd : %d\n",sockfd);
            	    printf("===================================\n");
                    close(sockfd);
                    FD_CLR(sockfd, &readfds);
                    clients[i].is_active = 0;
                }
                else {
                    printf("[%d]RECV: %s\n", sockfd, buf);
                    write(sockfd, buf, MAX_BUF);
                }
                if (--state <= 0)
                    break;
            }
        } // for
    } // while
}

void send_server_info(int sock, struct sockaddr_in adr, char* server_ip, char* server_port)
{
    char buf[BUF_SIZE];

    memset(buf, 0, BUF_SIZE);
    memcpy(buf, server_ip, strlen(server_ip));
    memcpy(&buf[20], server_port, strlen(server_port));

    sendto(sock, buf, BUF_SIZE, 0,(struct sockaddr *)&adr, sizeof(adr));
    printf("[*] server info sent ip: %s, port: %s\n", server_ip, server_port);
}

void print_client_info(int max_client, struct Client clients[MAX_SOCK])
{
    int i;
    printf("======= Client List =======\n");
    for(i=0 ; i<=max_client ; i++)
    {
        if(clients[i].is_active)
        {
            printf("[%d] ip : %s, port : %d\n", 
                i, inet_ntoa(clients[i].addr.sin_addr),ntohs(clients[i].addr.sin_port));
        }
    }
    printf("===========================\n\n");
}