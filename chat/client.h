#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include "chat.h"

#define MAX_ROOM_CNT 50

void init_client(int sock, struct USER* user);
void send_cmd(int sock, int cmd_type, int body_size, char* body);
void recv_cmd(char* buf);
void get_room(char* body, int body_size);
void get_msg(char* body, int body_size);
void make_room(char* title);
void make_msg(int room_id, int msg_size, char* msg);
