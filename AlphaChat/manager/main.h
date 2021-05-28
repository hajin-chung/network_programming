#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/types.h>

#define TIME_VAL_SECONDS    5
#define MULTICAST_TTL 5
#define MULTICAST_BUF_SIZE 30 // 0~15 : server ip, 16 ~ 30 : server port

#define MAX_REQ_BUF_SIZE 1024
#define MULTICAST_IP "239.0.1.100"
#define MULTICAST_PORT 5000
#define SERVER_IP "172.16.1.159"
#define TCP_PORT 3000
#define UDP_PORT 4000

struct timeval tv; 

struct HeartBeat {
    int user_id;
};


void multicast_server_info(int sock, struct sockaddr_in addr);
void make_mcast_socket(int* sock, struct sockaddr_in* addr, char* ip, int port);
void make_udp_socket(int* sock, int port);
void make_tcp_socket(int* sock, int port);

void handle_heartbeat(int sock);
void handle_request(int sock, int uid);
void handle_new_user(int sock);
