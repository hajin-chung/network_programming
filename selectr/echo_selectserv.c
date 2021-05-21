#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/select.h>

#define BUF_SIZE 100

int main(int argc, char *argv[])
{
	int serv_sock, clnt_sock;
	struct sockaddr_in serv_adr, clnt_adr;
	struct timeval timeout;
	fd_set reads, backup_reads; 

	socklen_t adr_sz;
	int fd_max, str_len, fd_num, sock;
	char buf[BUF_SIZE];
	int on=1;


	if(argc!=2) {
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	serv_sock=socket(PF_INET, SOCK_STREAM, 0);
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_adr.sin_port=htons(atoi(argv[1]));

	setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, (const char *)&on, sizeof(on));
	
	if(bind(serv_sock, (struct sockaddr*) &serv_adr, sizeof(serv_adr))==-1)
	{
		printf("bind() error"); exit(0);
	}
	if(listen(serv_sock, 5)==-1)
	{
		printf("listen() error"); exit(0);
	}

	FD_ZERO(&reads);
	FD_SET(serv_sock, &reads);
	fd_max=serv_sock;
	backup_reads = reads;

	while(1)
	{
		reads = backup_reads;
		timeout.tv_sec=5;
		timeout.tv_usec=5000;

		if((fd_num=select(fd_max+1, &reads, 0, 0, &timeout))==-1)
			break;
		
		if(fd_num==0) {
			printf(" Timeout \n");
			continue;
		}

		for(sock=0; sock<fd_max+1; sock++)
		{
			if(FD_ISSET(sock, &reads))
			{
				if(sock==serv_sock)     // connection request!
				{
					adr_sz=sizeof(clnt_adr);
					clnt_sock=
						accept(serv_sock, (struct sockaddr*)&clnt_adr, &adr_sz);
					FD_SET(clnt_sock, &backup_reads);
					if(fd_max<clnt_sock)
						fd_max=clnt_sock;
					printf("connected client: %d \n", clnt_sock);
				}
				else    // read message!
				{
					memset(buf, 0, BUF_SIZE);
					str_len=read(sock, buf, BUF_SIZE);
					if(str_len==0)    // close request!
					{
						FD_CLR(sock, &reads);
						close(sock);
						printf("closed client: %d \n", sock);
					}
					else
					{
						printf("client: %s \n", buf);
						write(sock, buf, str_len);    // echo!
					}
				}
			}
		}
	}
	close(serv_sock);
	return 0;
}

