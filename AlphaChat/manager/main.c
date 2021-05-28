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

int main()
{
    int heartbeat_sock;
    int tcp_sock; // socket for recieving client commands

    int fd_num;
    int fd_cnt;
    fd_set backup_set, fdset;

    printf("[*] Server starting\n");
    printf("----------------[INFO]----------------\n");
    printf("    MULTICAST : %s:%d\n", MULTICAST_IP, MULTICAST_PORT);
    printf("    SERVER    : %s:%d\n", SERVER_IP, TCP_PORT);
    printf("    HEARDBEAT : %d\n", UDP_PORT);
    printf("--------------------------------------\n");

    // init heartbeat_sock (udp)
    make_udp_socket(&heartbeat_sock, UDP_PORT);
    make_tcp_socket(&tcp_sock, TCP_PORT);

    // init fd_set
    FD_ZERO(&fdset);
    FD_SET(heartbeat_sock, &fdset);
    FD_SET(tcp_sock, &fdset);
    fd_cnt = heartbeat_sock;
    backup_set = fdset;

    while(1)
    {
        fdset = backup_set;

        tv.tv_sec = TIME_VAL_SECONDS;
        tv.tv_usec = 0;

        fd_num = select(fd_cnt+1, &fdset, 0, 0, &tv);
        if(fd_num == -1)
        {
            printf("[!] Server Error on select\n");
        }
        else if(fd_num == 0) // multicast server info every TIME_VAL_SECONDS seconds;
        {
            multicast_server_info();
        }
        else if(fd_num == heartbeat_sock) 
        {
            // recieve heartbeat 
        }
        else if(fd_num == tcp_sock)
        {
            // recieve command
        }
    }
    return 0;
}

void multicast_server_info()
{
    int msock;
    int ttl = MULTICAST_TTL;
    struct sockaddr_in addr;
    char buf[MULTICAST_BUF_SIZE];
    char port[15];
    char ip[15];

    memset(ip, 0, 15);
    memset(port, 0, 15);

    memcpy(ip, SERVER_IP, sizeof(SERVER_IP));
    itoa(TCP_PORT, port);

    printf("[*] multicast server info %s %s\n", ip, port);

    msock = socket(PF_INET, SOCK_DGRAM, 0);
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
	addr.sin_addr.s_addr=inet_addr(MULTICAST_IP);
	addr.sin_port=htons(TCP_PORT);

	setsockopt(msock, IPPROTO_IP, IP_MULTICAST_TTL, (void*)&ttl, sizeof(ttl));

    memset(buf, 0, MULTICAST_BUF_SIZE);
    memcpy(&buf[0], ip, strlen(ip));
    memcpy(&buf[16], port, strlen(port));

	sendto(msock, buf, MULTICAST_BUF_SIZE, 0,(struct sockaddr *)&addr, sizeof(addr));    
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