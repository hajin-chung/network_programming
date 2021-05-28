#include "req.h"
#include "global.h"

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
        room_create(buf);
    }
    else if(req_code == REQ_ROOM_DELETE_CODE)
    {
        room_delete(buf);
    }
    else if(req_code == REQ_ROOM_CONNECT_CODE)
    {
        room_connect(buf);
    }
    else if(req_code == REQ_SEND_CHAT_CODE)
    {
        send_chat(buf); 
    }
    else if(req_code == REQ_ROOM_INVITE_CODE)
    {
        room_invite(buf);
    }
    else if(req_code == REQ_REGISTER_CODE)
    {
        user_register(buf);
    }
    else
    {
        printf("[!] User %d invalid request code %d\n", uid, req_code);
    }
}

void user_register(char* buf)
{

}

void room_create(char* buf)
{

}

void room_delete(char* buf)
{

}

void room_connect(char* buf)
{

}

void room_invite(char* buf)
{
    
}

void send_chat(char* buf)
{

}