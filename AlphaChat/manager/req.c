#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

#include "set.h"
#include "user.h"
#include "room.h"
#include "utils.h"

void handle_request(int sock, int uid) 
{
    char buf[MAX_REQ_BUF_SIZE];
    char req[MAX_REQ_BUF_SIZE];
    char req_code_raw[4];
    int req_code;
    long len;

    memset(buf, 0, MAX_REQ_BUF_SIZE);
    memset(req, 0, MAX_REQ_BUF_SIZE);
    memset(req_code_raw, 0, 4);

    len = read(sock, buf, MAX_REQ_BUF_SIZE);
    if(len <= 0)
    {
        printf("[*] sock closed of user %d, fd %d\n", sock, uid);
        close(sock);
        FD_CLR(sock, &fdset);
        users[uid].status = USER_STATUS_OFFLINE;
        return;
    }
    
    memcpy(req_code_raw, buf, 4); 
    req_code = atoi(req_code_raw);
    printf("[*] New request from %d : code %d\n", sock, req_code);

    if(req_code == REQ_ROOM_CREATE_CODE)
    {
        room_create(buf, uid);
    }
    else if(req_code == REQ_ROOM_DELETE_CODE)
    {
        room_delete(buf, uid);
    }
    else if(req_code == REQ_ROOM_CONNECT_CODE)
    {
        room_connect(buf, uid);
    }
    else if(req_code == REQ_SEND_CHAT_CODE)
    {
        send_chat(buf, uid); 
    }
    else if(req_code == REQ_ROOM_INVITE_CODE)
    {
        room_invite(buf, uid);
    }
    else if(req_code == REQ_REGISTER_CODE)
    {
        user_register(buf, uid);
    }
    else
    {
        printf("[!] User %d invalid request code %d\n", uid, req_code);
    }
}

void user_register(char* buf, int uid)
{
    int offset = 4;
    char user_name[USER_NAME_MAX_LEN];

    memset(user_name, 0, USER_NAME_MAX_LEN);
    
    memcpy(user_name, buf+offset, USER_NAME_MAX_LEN);

    memcpy(users[uid].name, user_name, strlen(user_name));
    
    printf("[*] user_register() user id %d name %s\n", uid, user_name);

    // send user id to user
}

void room_create(char* buf, int uid)
{
    int offset = 4;
    int room_id;
    int user_id;
    char room_name[ROOM_NAME_MAX_LEN];

    room_id = new_room_id();
    user_id = atoi_size(buf, offset, 4);
    offset += 4;
    memcpy(room_name, buf+offset, ROOM_NAME_MAX_LEN);

    memset(&rooms[room_id], 0, sizeof(struct ROOM));

    rooms[room_id].status = ROOM_STATUS_ON;
    rooms[room_id].id = room_id;
    rooms[room_id].super_user_id = user_id;
    memcpy(rooms[room_id].name, room_name, ROOM_NAME_MAX_LEN);
    rooms[room_id].user_cnt++;
    rooms[room_id].users[0] = user_id; 

    printf("[*] room_create() room id %d room name %s user id %d\n", room_id, room_name, user_id);

    // send user success signal
}

void room_delete(char* buf, int uid)
{
    int offset = 4;
    int room_id;
    int user_id;

    user_id = atoi_size(buf, offset, 4);
    offset += 4;
    room_id = atoi_size(buf, offset, 4);
    offset += 4;

    if(user_id == rooms[room_id].super_user_id)
    {
        rooms[room_id].status = ROOM_STATUS_OFF;
        printf("[*] room_delete success room id %d user id %d\n", room_id, user_id);
        // send user success signal
    }
    else 
    {
        printf("[!] room_delete() invalid access : non-super-user tried to delete room\n");
        // send user invalid access
    }
}

void room_connect(char* buf, int uid)
{
    int offset = 4;
    int user_id;
    int room_id;

    user_id = atoi_size(buf, offset, 4);
    offset += 4;
    room_id = atoi_size(buf, offset, 4);
    offset += 4;

    if(room_contains_user(room_id, user_id) == 1)
    {
        // send user chat history
    }
    else
    {
        // send user invalid access
    }
}

void room_invite(char* buf, int uid)
{
    int offset = 4; 
}

void send_chat(char* buf, int uid)
{

}

void user_list(char* buf, int uid);
void room_list(char* buf, int uid);