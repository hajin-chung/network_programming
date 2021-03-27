#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h> 

#define MAX_CLIENTS 30
#define SUC_CODE "200"
#define ERR_CODE "400"
#define SET_NAME "SET_NAME"
#define SEND_MSG "SEND_MESSAGE"
#define GET_USERS "GET_USERS"

#define PORT 5000

struct Client {
    int socket;
    char name[30];
    int id;
    int is_connected;
};

struct Message {
    int id_from;
    int id_to;
    char message[100];
};

struct Client clients[MAX_CLIENTS];

void error_handling(char *message);
void client_handling(int idx, char *buffer);
struct Client name_to_client(char *name);
int send_message(char *buffer);
int set_name(int idx, char *buffer);
int get_users(int idx);
	
int main(int argc , char *argv[])
{
	int opt = 1;
	int master_socket;
    int new_socket;
    struct Client new_client;

    int addrlen;
    int activity;
	int max_sd, valread , sd;
	struct sockaddr_in address;
    int i;
		
	char buffer[1025];
		
	fd_set readfds;

	for (i=0 ; i<MAX_CLIENTS ; i++) memset(&clients[i], 0, sizeof(struct Client));
		
    master_socket = socket(PF_INET, SOCK_STREAM, 0);
	if(master_socket == 0) error_handling("socket() error");
	
	if(setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
        error_handling("setsockopt() error!");
	
    memset(&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);
		
	if (bind(master_socket, (struct sockaddr *)&address, sizeof(address)) == -1)
        error_handling("bind() error");
	printf("Listener on port %d \n", PORT);
		
	if (listen(master_socket, 3) == -1)
        error_handling("listen() error");
    
	addrlen = sizeof(address);
	printf("Waiting for connections ...\n");
		
	while(1)
	{
		FD_ZERO(&readfds);
	
		FD_SET(master_socket, &readfds);
		max_sd = master_socket;
			
		for(i=0 ; i<MAX_CLIENTS ; i++) {
			sd = clients[i].socket;
			if(sd > 0) FD_SET(sd, &readfds);
			if(sd > max_sd) max_sd = sd;
		}
	
		activity = select(max_sd+1, &readfds, NULL, NULL, NULL);
	
		if ((activity < 0) && (errno!=EINTR)) error_handling("select() error");
			
		if (FD_ISSET(master_socket, &readfds))
		{
            new_socket = accept(master_socket, (struct sockaddr *)&address, (socklen_t*)&addrlen);
			if(new_socket<0) error_handling("accept() error");
			
            new_client.socket = new_socket;
            new_client.is_connected = 1;

			printf("New connection | socket fd : %d | ip : %s | port : %d\n", 
                    new_client.socket, inet_ntoa(address.sin_addr), ntohs(address.sin_port));
		
			write(new_socket, SUC_CODE, strlen(SUC_CODE));
            // write error handling

			for (i=0 ; i<MAX_CLIENTS ; i++)
				if(clients[i].is_connected == 0)
				{
					clients[i] = new_client;
                    clients[i].id = i;
					printf("Added Client on index %d\n" , i);
					break;
				}
		}
			
		for(i=0 ; i<MAX_CLIENTS ; i++)
		{
			sd = clients[i].socket;
			if (FD_ISSET(sd , &readfds))
			{
                valread = read(sd, buffer, 1024);
				if (valread == 0)
				{
                    getpeername(sd, (struct sockaddr*)&address, (socklen_t*)&addrlen); 
					printf("Host disconnected | ip : %s | port %d \n", 
                            inet_ntoa(address.sin_addr), ntohs(address.sin_port));
						
					close(sd);
                    memset(&clients[i], 0, sizeof(struct Client));
				}
					
				else
				{
					buffer[valread] = '\0';
                    // send(sd , buffer , strlen(buffer) , 0 );

                    printf("%s\n", buffer);

                    client_handling(i, buffer);
				}
			}
		}
	}
		
	return 0;
}

void client_handling(int idx, char *buffer) {
    char type[30];
    
    sscanf(buffer, "%s", type);
    printf("Client index %d Command : %s\n", idx, type);
    
    if(strcmp(type, SET_NAME) == 0) {
        set_name(idx, buffer);
    } else if(strcmp(type, SEND_MSG) == 0) {
        send_message(buffer);
    } else if(strcmp(type, GET_USERS) == 0) {
        get_users(idx);
    }
}

int send_message(char *buffer) {
    char type[30];
    char to[30];
    char from[30];
    char msg[100];
    int dest_sd;

    sscanf(buffer, "%s %s %s %s", type, to, from, msg);
    printf("%s -> %s : %s |||\n\n", from, to, msg);
    dest_sd = name_to_client(to).socket;
    
    write(dest_sd, msg, strlen(msg));
    return 0;
}

int set_name(int idx, char *buffer) {
    char type[30];
    char name[30];

    sscanf(buffer, "%s %s", type, name); // param1 -> name
    strcpy(clients[idx].name, name);
    printf("Client index %d changed name to %s\n", idx, clients[idx].name);
    return 0;
}

int get_users(int idx) {
    char buffer[1025];
    int i;

    memset(buffer, 0, sizeof(buffer));
        
    for(i=0 ; i<MAX_CLIENTS ; i++) 
        if(clients[i].is_connected) {
            printf("%s\n", clients[i].name); 
            sprintf(buffer, "%s ", clients[i].name); 
        }

    write(clients[idx].socket, buffer, strlen(buffer));
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

struct Client name_to_client(char *name) {
    int i;

    for(i=0 ; i<MAX_CLIENTS ; i++) 
        if(strcmp(name, clients[i].name) == 0) {
            return clients[i]; 
        }
    return clients[0];
}
