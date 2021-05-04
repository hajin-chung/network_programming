#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/wait.h>

#define TTL	2
#define BUF_SIZE 5120

int main(int argc, char *argv[]) {
	int sock, i;
	int time_live = TTL;
	int str_len;
	char buf[BUF_SIZE];
	struct sockaddr_in adr;
  struct ip_mreq join_adr;
	int on=1;
  FILE *fp;
	
	if(argc!=4) {
		printf("Usage : %s <IP> <PORT> <FILE_PATH>\n", argv[0]);
		exit(1);
	}
  
  fp = fopen(argv[3], "rb");

	sock=socket(PF_INET, SOCK_DGRAM, 0);
	adr.sin_family=AF_INET;
	adr.sin_addr.s_addr=inet_addr(argv[1]);
	adr.sin_port=htons(atoi(argv[2]));

	on=0;
	setsockopt(sock, IPPROTO_IP, IP_MULTICAST_TTL, (void*)&time_live, sizeof(time_live));

  while(1) {
    for(i=1 ; i<=100000 ; i++);
    fread(&buf, sizeof(char), BUF_SIZE, fp);
    sendto(sock, buf, BUF_SIZE, 0,(struct sockaddr *)&adr, sizeof(adr));
  }
	printf(" Close \n");
	close(sock);
	return 0;
}