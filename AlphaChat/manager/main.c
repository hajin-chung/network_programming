/*

서버

1. mcast에 서버의 정보를 보냄 (IP, PORT)
2. udp를 이용하여 클라이언트가 보내온 하트비트를 수신하여 클라이언트의 정보를 저장한다.
3. 클라이언트 별로 tcp client socket과 tcp server socket을 만든다
4. 클라이언트에서 tcp client socket 으로 자신이 원하는 리퀘스트를 전송한다.
5. 서버는 그 리퀘스를 전송받아 처리를 한다. 
6. 서버는 치리 결과를 클라이언트에 접속되어있는 tcp client socket으로 보낸다.


*/

#include "main.h"
#include "utils.h"
#include "set.h"
#include "user.h"
#include "room.h"

int main()
{
    int mcast_sock; // socket for mcasting
    int heartbeat_sock;
    int serv_sock; // socket for recieving client commands
    struct sockaddr_in mcast_addr;
    int fd_num;
    int i;

    printf("[*] Server starting\n");
    printf("----------------[INFO]----------------\n");
    printf("    MULTICAST : %s:%d\n", MULTICAST_IP, MULTICAST_PORT);
    printf("    SERVER    : %s:%d\n", SERVER_IP, TCP_PORT);
    printf("    HEARDBEAT : %d\n", UDP_PORT);
    printf("--------------------------------------\n");

    // init rooms, users
    memset(users, sizeof(users), 0);
    memset(rooms, sizeof(rooms), 0);

    // init heartbeat_sock (udp)
    make_mcast_socket(&mcast_sock, &mcast_addr, MULTICAST_IP, MULTICAST_PORT);
    make_udp_socket(&heartbeat_sock, UDP_PORT);
    make_tcp_socket(&serv_sock, TCP_PORT);

    // init fd_set
    // heartbeat_socket, serv_socket
    FD_ZERO(&fdset);
    FD_SET(heartbeat_sock, &fdset);
    FD_SET(serv_sock, &fdset);
    fd_cnt = serv_sock;

    while(1)
    {
        backup_set = fdset;

        tv.tv_sec = TIME_VAL_SECONDS;
        tv.tv_usec = 0;

        fd_num = select(fd_cnt+1, &backup_set, 0, 0, &tv);
        if(fd_num == -1)
        {
            printf("[!] Server Error on select\n");
        }
        else if(fd_num == 0) // multicast server info every TIME_VAL_SECONDS seconds;
        {
            multicast_server_info(mcast_sock, mcast_addr);
            print_room_info();
            print_user_info();
        }
        else if(FD_ISSET(heartbeat_sock, &backup_set)) 
        {
            handle_heartbeat(heartbeat_sock);
        }
        else if(FD_ISSET(serv_sock, &backup_set))
        {
            handle_new_user(serv_sock);
        }

        for(i=0 ; i<users_cnt ; i++)
        {
            if(FD_ISSET(users[i].sock, &backup_set))
            {
                handle_request(users[i].sock, i);
            }
        }
        
    }
    return 0;
}

// 서버의 아이피와 포트번호가 들어있는 패킷을 만든다.
// char[30] [0 ~ 15] : ip / [16 ~ 30] : port
void multicast_server_info(int sock, struct sockaddr_in addr)
{
    char buf[MULTICAST_BUF_SIZE];
    char port[15];
    char ip[15];

    // array initialize to zero
    memset(buf, 0, MULTICAST_BUF_SIZE);
    memset(ip, 0, 15);
    memset(port, 0, 15);

    // SERVER_IP -> ip, TCP_PORT -> port (형변환)
    memcpy(ip, SERVER_IP, sizeof(SERVER_IP));
    itoa(TCP_PORT, port);

    memset(buf, 0, MULTICAST_BUF_SIZE);
    memcpy(&buf[0], ip, strlen(ip));
    memcpy(&buf[16], port, strlen(port));

    printf("[*] multicast server info %s %s\n", ip, port);
	sendto(sock, buf, MULTICAST_BUF_SIZE, 0,(struct sockaddr *)&addr, sizeof(addr));    
}

void handle_heartbeat(int sock)
{
    char buf[4];
    struct sockaddr_in clnt_addr;
    int len;
    struct HeartBeat hb;
    struct USER user;
    socklen_t clnt_addr_sz;

    memset(&hb, 0, sizeof(hb));

    memset(buf, 0, 4);
    clnt_addr_sz = sizeof(clnt_addr);
    
    len = recvfrom(sock, buf, 4, 0, (struct sockaddr*)&clnt_addr, &clnt_addr_sz);
    // error handling

    hb.user_id = atoi(buf);

    printf("[*] Received [%d] heart beat\n", hb.user_id);
    users[hb.user_id].status = USER_STATUS_ONLINE;
}

void handle_new_user(int sock) 
{
    int clnt_sock;
    int clnt_len;
    struct sockaddr_in clnt_addr;
    int uid;

    clnt_len = sizeof(clnt_addr);
    clnt_sock = accept(sock, (struct sockaddr *)&clnt_addr, &clnt_len); 
    printf("[*] new user connected\n");

    uid = new_user_id();
    users[uid].status = USER_STATUS_ONLINE;
    users[uid].sock = clnt_sock;
    users[uid].id = uid;

    FD_SET(clnt_sock, &fdset);
    if(clnt_sock > fd_cnt) fd_cnt = clnt_sock;
    if(uid+1 > users_cnt) users_cnt = uid + 1;

    printf("[*] connection from (%s , %d)\n", 
        inet_ntoa(clnt_addr.sin_addr), ntohs(clnt_addr.sin_port));
    printf("    user id : %d, clnt sock : %d\n", uid, clnt_sock);
}

void make_mcast_socket(int* sock, struct sockaddr_in* addr, char* ip, int port) 
{
    printf("[*] make multicast socket\n");
    int ttl = MULTICAST_TTL;

    *sock = socket(PF_INET, SOCK_DGRAM, 0);

    memset(addr, 0, sizeof(addr));
    (*addr).sin_family = AF_INET;
    (*addr).sin_addr.s_addr=inet_addr(ip);
	(*addr).sin_port=htons(port);

	setsockopt(*sock, IPPROTO_IP, IP_MULTICAST_TTL, (void*)&ttl, sizeof(ttl));
}

void make_udp_socket(int* sock, int port)
{
    struct sockaddr_in addr;

    printf("[*] make udp socket\n");
    *sock=socket(PF_INET, SOCK_DGRAM, 0);
	if(*sock==-1)
		printf("[!] UDP socket creation error\n");
	
	memset(&addr, 0, sizeof(addr));
	addr.sin_family=AF_INET;
	addr.sin_addr.s_addr=htonl(INADDR_ANY);
	addr.sin_port=htons(port);

	if(bind(*sock, (struct sockaddr*)&addr, sizeof(addr))==-1)
		printf("[!] bind() error\n");
}

void make_tcp_socket(int* sock, int port)
{
    struct sockaddr_in addr;
    int on=1;
    
    printf("[*] make tcp socket\n");
	*sock=socket(PF_INET, SOCK_STREAM, 0);
	if(*sock==-1)
		printf("[!] TCP socket creation error\n");

	memset(&addr, 0, sizeof(addr));
	addr.sin_family=AF_INET;
	addr.sin_addr.s_addr=htonl(INADDR_ANY);
	addr.sin_port=htons(port);

	setsockopt(*sock, SOL_SOCKET, SO_REUSEADDR, (const char *)&on, sizeof(on));
	
	if(bind(*sock, (struct sockaddr*) &addr, sizeof(addr))==-1)
	{
		printf("[!] bind() error"); exit(0);
	}
	if(listen(*sock, 10)==-1)
	{
		printf("[!] listen() error"); exit(0);
	}
}