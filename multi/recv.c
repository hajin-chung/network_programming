#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024

int main(int argc, char *argv[]) {
	int recv_sock;
	int str_len;
	char buf[BUF_SIZE];
	struct sockaddr_in addr;
	struct ip_mreq join_addr;
	int opt=1;
	
	if(argc!=3) {
		printf("Usage : %s <IP> <PORT>\n", argv[0]);
		exit(1);
	}
 
	recv_sock=socket(PF_INET, SOCK_DGRAM, 0);
 	memset(&addr, 0, sizeof(addr));

	addr.sin_family=AF_INET;
	addr.sin_addr.s_addr=htonl(INADDR_ANY);	
	addr.sin_port=htons(atoi(argv[2]));

	setsockopt(recv_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	if(bind(recv_sock, (struct sockaddr*) &addr, sizeof(addr))==-1)	{
		printf("bind() error");
		close(recv_sock);
		exit(1);	
	}

	join_addr.imr_multiaddr.s_addr=inet_addr(argv[1]);
    // inet_pton(AF_INET, argv[1], &(join_addr.imr_multiaddr.s_addr));
	join_addr.imr_interface.s_addr=htonl(INADDR_ANY);
  
	if ( (setsockopt(recv_sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (void*)&join_addr, sizeof(join_addr)))< 0 ) {
		printf(" setsockopt error \n");
		close(recv_sock);
		exit(1);
	}
  
	while(1) {
		memset(buf, 0, BUF_SIZE);
		str_len=recvfrom(recv_sock, buf, BUF_SIZE, 0, NULL, 0);
	    printf("%s", buf);	
		if(str_len<0) 
			break;
	}
	printf(" end \n");
	close(recv_sock);
	return 0;
}
