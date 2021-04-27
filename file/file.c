#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define	BUF_SIZE	1024	// buf size

typedef struct {
  int id;
  char name[30];
  int cmd_code;
} FILE_INFO_REQUEST;

typedef struct {
  int cmd_code;
  char file_name[30];
} FILE_INFO_RESPONSE;

typedef struct {
  int id;
  char name[30];
  int cmd_code;
} FILE_TRANS_REQUEST;

typedef struct {
  int cmd_code;
  char OK[2];
} FILE_TRANS_RESPONSE;

// integer to ascii
void itoa(int i, char *st)
{
	sprintf(st, "%d", i);
	return;
}

void error_handling(char *message);

int main()
{
  int sock;
  struct sockaddr_in serv_addr;
  char buffer[BUF_SIZE+1];
  int str_len;
  int port = 9100;
  char ip[30] = "192.168.100.3";
  int off, opt, nbyte;
  char id[4] = {0, }, cmd_code[4] = {0, }, filename[BUF_SIZE]={0, }, ok[2] = {0, };

  FILE_INFO_REQUEST fiq;
  FILE_INFO_RESPONSE fis;
  FILE_TRANS_REQUEST ftq;
  FILE_TRANS_RESPONSE fts;

  FILE *fp;

  // socket setting
  sock=socket(PF_INET, SOCK_STREAM, 0);
  if(sock == -1)
      error_handling("socket() error");
  
  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family=AF_INET;
  serv_addr.sin_addr.s_addr=inet_addr(ip);
  serv_addr.sin_port=htons(port);
  if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
      error_handling("setsockopt() error!");
  }
  if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))==-1) 
      error_handling("connect() error!");

  memset(&buffer, '\0', BUF_SIZE);
  read(sock, buffer, BUF_SIZE);
  printf("%s\n", buffer);
  

  fiq.cmd_code = 1030;
  strcpy(fiq.name, "3631chj");
  fiq.id = 3631;

  off = 0;
  itoa(fiq.id, id);
  memcpy(&buffer, &id, 4);
  off += 4;
  memcpy(&buffer[off], &fiq.name, 30);
  off+=30;
  itoa(fiq.cmd_code, cmd_code);
  memcpy(&buffer[off], &cmd_code, 4);
  off = 0;
  
  write(sock, buffer, BUF_SIZE);

  printf("==> file info request: %d %s %d\n", fiq.id, fiq.name, fiq.cmd_code);
  memset(buffer, '\0', BUF_SIZE);
  str_len = read(sock, buffer, BUF_SIZE);
  if(str_len==-1) {
      error_handling("read() error!");
      return 0;
  } 

  off = 0;
  memcpy(&cmd_code, &buffer, 4);
  fis.cmd_code = atoi(cmd_code);
  off += 4;
  memcpy(&filename, &buffer[off], str_len-4);
  strcpy(fis.file_name, filename);

  printf("<== file info response: %d %s\n\n", fis.cmd_code, fis.file_name);

  
  // set file transfer request
  ftq.cmd_code = 2030;
  strcpy(ftq.name, "3631chj");
  ftq.id = 3631;

  memset(buffer, '\0', BUF_SIZE);
  str_len = read(sock, buffer, BUF_SIZE);

  off = 0;
  itoa(ftq.id, id);
  memcpy(&buffer, &id, 4);
  off += 4;
  memcpy(&buffer[off], &ftq.name, 30);
  off+=30;
  itoa(ftq.cmd_code, cmd_code);
  memcpy(&buffer[off], &cmd_code, 4);
  off = 0;

  write(sock, buffer, BUF_SIZE);
  printf("==> file request: %d %s %d\n", ftq.id, ftq.name, ftq.cmd_code);
  
  // file request response
  memset(buffer, '\0', BUF_SIZE);
  str_len = read(sock, buffer, BUF_SIZE);
  if(str_len==-1) {
    error_handling("read() error!");
    return 0;
  } 

  off = 0;
  memcpy(&cmd_code, &buffer, 4);
  fts.cmd_code = atoi(cmd_code);
  off += 4;
  memcpy(&ok, &buffer[off], 2);
  strcpy(fts.OK, ok);

  printf("<== file request response: %d %s\n", fts.cmd_code, fts.OK);


  fp = fopen(fis.file_name, "wb");
  if(fp == NULL) {
    printf("fopen() error\n");
    exit(0);
  }

  while(1) {
    memset(buffer, 0, BUF_SIZE);

    nbyte = read(sock, buffer, BUF_SIZE);
    if(nbyte <= 0) break;

    fwrite(buffer, 1, nbyte, fp);
  }
  fclose(fp);

  return 0;
}


void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

