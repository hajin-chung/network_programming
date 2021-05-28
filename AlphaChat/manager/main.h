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

#include "utils.h"
#include "room.h"
#include "user.h"

#ifndef _CHAT_H
#define _CHAT_H

#include "chat.h"

#endif

#define TIME_VAL_SECONDS    5
#define MULTICAST_IP "239.0.1.100"
#define MULTICAST_PORT 5000
#define SERVER_IP "172.16.1.159"
#define TCP_PORT 3000
#define MULTICAST_TTL 5
#define MULTICAST_BUF_SIZE 30 // 0~15 : server ip, 16 ~ 30 : server port
#define UDP_PORT 4000

struct timeval tv; 

void multicast_server_info();
void make_udp_socket(int* sock, int port);
void make_tcp_socket(int* sock, int port);