#include "utils.h"
#include "global.h"

#define TIME_VAL_SECONDS    5
#define MULTICAST_TTL 5
#define MULTICAST_BUF_SIZE 30 // 0~15 : server ip, 16 ~ 30 : server port

struct timeval tv; 

struct HeartBeat {
    int user_id;
};

int fd_cnt;

void multicast_server_info(int sock, struct sockaddr_in addr);
void make_mcast_socket(int* sock, struct sockaddr_in* addr, char* ip, int port);
void make_udp_socket(int* sock, int port);
void make_tcp_socket(int* sock, int port);

void handle_heartbeat(int sock);
void handle_request(int sock, int uid);
void handle_new_user(int sock);

int get_new_uid();