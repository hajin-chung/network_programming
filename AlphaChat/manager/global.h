#pragma once

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

#include "room.h"
#include "user.h"
#include "req.h"
#include "res.h"


#define MULTICAST_IP "239.0.1.100"
#define MULTICAST_PORT 5000
#define SERVER_IP "172.16.1.159"
#define TCP_PORT 3000
#define UDP_PORT 4000

#define MAX_REQ_BUF_SIZE 1024
#define USER_MAX 1000
#define ROOM_MAX 1000

struct USER users[USER_MAX];
struct ROOM rooms[ROOM_MAX];
int users_cnt;
int rooms_cnt;

fd_set fdset, backup_set;