#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BUF_SIZE    100

int main() {
    FILE *fp = NULL;
    FILE *fp2 = NULL;
    char buf[BUF_SIZE];
    char fname[20] = "p2.txt";
    int nbyte = 0;

    // file open for read
    fp = fopen("./proto.c", "rb"); 
    if ( fp == NULL ) {
        printf("File open error;\n");
        exit(0);
    }
    printf("File open success\n");
    
    // file open for read
    // fp2 = fopen("./p.txt", "wb"); 
    fp2 = fopen(fname, "wb");
    if ( fp2 == NULL ) {
        printf("File open error;\n");
        exit(0);
    }
    printf("File open success\n");
    
    // data read from file
    while(1) {
        memset(&buf, 0, BUF_SIZE);
        nbyte = fread(buf, 1, BUF_SIZE, fp); 
        if ( nbyte <= 0 ) {
            printf("file data read error \n");
            break;
        }
        fwrite(buf, 1, nbyte, fp2);
    }

    fclose(fp);
}
