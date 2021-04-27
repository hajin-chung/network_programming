#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define BUF_SIZE 1024
void error_handling(char *msg);
typedef struct{
  int id;
  char n[30];
  int ccc;
} RH;

void itoa(int i, char *st) {
  sprintf(st, "%d", i);
  return;
}
int main(){
  int sock;
  struct sockaddr_in serv_addr;
  char msg[BUF_SIZE];
  int str_len, opt;
  char ip[20] = "192.168.100.3";
  int port = 9100;

  RH fir;
  int c = 0;
  char id[5] = {0, };
  char ccc[5] = {0, };
  char fir_msg[BUF_SIZE];

  RH ftr;
  char ftr_msg[BUF_SIZE];
  
  char buf[BUF_SIZE] = {0, };
  FILE *fp = NULL;

  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = inet_addr(ip);
  serv_addr.sin_port = htons(port);

  fir.id = 3631;
  strcpy(fir.n, "C.hj");
  fir.ccc = 1030;
  c = 0;
  memset(&fir_msg, 0, BUF_SIZE);
  
  itoa(fir.id, id);
  memcpy(&fir_msg[c], &id, sizeof(fir.id));
  
  c += sizeof(int);
  memcpy(&fir_msg[c], &fir.n,  sizeof(fir.n));
  
  c += sizeof(fir.n);
  itoa(fir.ccc, ccc);
  memcpy(&fir_msg[c], &ccc, sizeof(fir.ccc));

  ftr.id = fir.id;
  strcpy(ftr.n, fir.n);
  ftr.ccc = 2030;
  c = 0;
  memset(&ftr_msg, 0, BUF_SIZE);

  itoa(ftr.id, id);
  memcpy(&ftr_msg[c], &id, sizeof(ftr.id));

  c += sizeof(int);
  memcpy(&ftr_msg[c], &ftr.n, sizeof(ftr.n));
  
  c += sizeof(ftr.n);
  itoa(ftr.ccc, ccc);
  memcpy(&ftr_msg[c], &ccc, sizeof(ftr.ccc));
  
  sock = socket(PF_INET, SOCK_STREAM, 0);

  if (sock == -1)
    error_handling("socket() error");

  if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
    error_handling("connect() error!");

  if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) 
      error_handling("setsockopt() error!");

  str_len = read(sock, msg, sizeof(msg));
  if (str_len == -1)
    error_handling("read() error!");

  printf("Message from server: %s \n", msg);
  printf("==> file info request: %d %s %d \n", fir.id,  fir.n, fir.ccc);
  write(sock, fir_msg, sizeof(fir_msg));

  memset(&msg, 0, BUF_SIZE);
  str_len = read(sock, msg, sizeof(msg));
  printf("<== file info response: %s %s \n\n", msg,  &msg[4]);
  
  char file_name[30];
  strcpy(file_name, &msg[4]);
  printf("==> file request: %d %s %d \n", ftr.id,  ftr.n, ftr.ccc);
  write(sock, ftr_msg,  sizeof(ftr_msg));
  memset(&msg, 0, BUF_SIZE);

  str_len = read(sock, msg, sizeof(msg));
  printf("<== file request response: %s %s\n", msg, &msg[4]);
  memset(&msg, 0, BUF_SIZE);
  
  fp = fopen(file_name, "wb");
  while (1){
    memset(&buf, 0, BUF_SIZE);
    str_len = read(sock, buf, sizeof(buf));
    if (str_len <= 0) break;
    else fwrite(buf, 1, str_len, fp);
  }

  close(sock);
  return 0;
}

void error_handling(char *msg){
  fputs(msg, stderr);
  fputc('\n', stderr);
  exit(1);
}