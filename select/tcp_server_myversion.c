#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
// for select
#include <sys/time.h>
#include <sys/select.h>

#define BUF_SIZE 100

int main(int argc, char *argv[])
{
	int serv_sock, clnt_sock;
	struct sockaddr_in serv_adr, clnt_adr;
	int on=1;

	socklen_t adr_sz;
	char buf[BUF_SIZE];
	struct timeval timeout; // for timer
	fd_set readfds, backup_reads; // for check bit
    int maxfd; // use select 
    int status, str_len, i;

	if(argc != 2) {
		printf("Usage: %s <PORT>\n", argv[0]);
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
    if(listen(serv_sock, 5) == -1)
    {
		printf("listen() error"); exit(0);
    }
    
    FD_ZERO(&readfds);
    FD_SET(serv_sock, &readfds);
    maxfd = serv_sock;
    backup_reads = readfds;

    while(1) {
        readfds = backup_reads;
        timeout.tv_sec = 5;
        timeout.tv_uesc = 5000;

        status = select(maxfd+1, &readfds, 0, 0, &timeout);
        if(status < 0) {
            printf(" select() error\n");
            exit(1);
        }
        else if(status == 0) {
            printf(" Timeout \n");
            continue;
        }

        for(i=0 ; i<maxfd+1 ; i++) {
            if(FD_ISSET(i, &readfds))
            {
                if(i == serv_sock)
                {
                }
            }
        }
    }
}
