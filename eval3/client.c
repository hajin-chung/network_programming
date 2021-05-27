#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/select.h>

#define BUF_SIZE 5120 

int main(int argc, char *argv[])
{
	int multicast_sock;
	int str_len;
	char buf[BUF_SIZE];
	struct sockaddr_in multicastaddr;
	struct ip_mreq join_addr;

	char server_ip[20];
	int server_port;
	
	if(argc!=3) {
		printf("Usage : %s <GroupIP> <PORT>\n", argv[0]);
		exit(1);
	 }
 
       // Create UDP Socket	
	multicast_sock=socket(PF_INET, SOCK_DGRAM, 0);
 	memset(&multicastaddr, 0, sizeof(multicastaddr));
	multicastaddr.sin_family=AF_INET;
	multicastaddr.sin_addr.s_addr=htonl(INADDR_ANY);	
	multicastaddr.sin_port=htons(atoi(argv[2]));

	int on=1;
       setsockopt(multicast_sock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

// Bind socket	
	if(bind(multicast_sock, (struct sockaddr*) &addr, sizeof(addr))==-1)
	{
		printf("bind() error");
		close(multicast_sock);
		exit(1);	
	}

	// Specify the multicast Group	
	join_addr.imr_multiaddr.s_addr=inet_addr(argv[1]);
	// Accept multicast from any interface
	join_addr.imr_interface.s_addr=htonl(INADDR_ANY);
  
// Join Multicast Group	
	if ( (setsockopt(multicast_sock, IPPROTO_IP, 
		IP_juADD_MEMBERSHIP, (void*)&join_addr, sizeof(join_addr)))< 0 ) 
	{
		printf(" SetsockOpt Join Error \n");
		close(multicast_sock);
		exit(1);
	}
  
	while(1)
	{
		memset(buf, 0, BUF_SIZE);
		str_len=recvfrom(multicast_sock, buf, BUF_SIZE, 0, NULL, 0);
		if(str_len<0) 
			continue;
		puts(buf);
		strcpy(server_ip, buf);
		server_port = atoi(&buf[20]);
		printf("Server ip : %s, Server port : %d", server_ip, server_port);
		break;
	}
	printf(" end ======== \n");
	close(multicast_sock);



	return 0;
}