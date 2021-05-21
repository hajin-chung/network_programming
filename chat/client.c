#include "client.h"

struct USER user;
struct ROOM rooms[MAX_ROOM_CNT];

int main(int argc, char *argv[])
{
	int sock;
	char buf[BUF_SIZE];
	int str_len;
	struct sockaddr_in serv_adr;
    
	if(argc!=4) {
		printf("Usage : %s <IP> <port> <name>\n", argv[0]);
		exit(1);
	}
	
	sock=socket(PF_INET, SOCK_STREAM, 0);   
	if(sock==-1) 
    {
		printf("socket() error"); exit(0); 
    }
	
    strcpy(user.name, argv[4]);
    user.sock = sock;

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=inet_addr(argv[1]);
	serv_adr.sin_port=htons(atoi(argv[2]));
	
	if(connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr))==-1) {
		printf("connect() error!"); exit(0); }
	else
		printf("Connected...........\n");
	
    init_client(sock, &user);

	while(1) 
	{
		memset(buf, 0, BUF_SIZE);
		str_len=read(sock, buf, BUF_SIZE);
		printf("buf from server: %s", buf);
        recv_cmd(buf);
	}
	
	close(sock);
	return 0;
}

void init_client(int sock, struct USER* user)
{
    char body[USER_NAME_LEN];

    memset(body, 0, USER_NAME_LEN);
    memcpy(body, user->name, strlen(user->name));
    send_cmd(sock, CMD_INIT_CLI, strlen(body), body); 
}

void send_cmd(int sock, int cmd_type, int body_size, char* body) 
{
    char buf[BUF_SIZE];

    memset(buf, 0, BUF_SIZE);
    itoa(buf, cmd_type);
    itoa(&buf[50], body_size);
    memcpy(&buf[100], body, body_size);

    write(sock, buf, BUF_SIZE);
}

void recv_cmd(char* buf)
{
    int cmd_type;
    int body_size;
    char* body;

    cmd_type = atoi(&buf[0]);
    body_size = atoi(&buf[50]);
    body = malloc(body_size);
    memset(body, 0, body_size);
    memcpy(body, &buf[100], body_size);

    if(cmd_type == CMD_GET_MSG)
    {
        get_msg(body, body_size);
    }
    else if(cmd_type == CMD_GET_ROOM)
    {
        get_room(body, body_size);
    }
}

void get_room(char* buf, int body_size)
{

}

void get_msg(char* body, int body_size)
{
}

void make_room(char* title)
{
}

void make_msg(int room_id, int msg_size, char* msg)
{
}
