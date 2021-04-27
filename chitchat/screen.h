#include <sys/ioctl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
    int w;
    int h;
    char* mem;
} BUFFER2;

void putsBuffer2(BUFFER2 *buffer, int y, int x, char* str);
void putcBuffer2(BUFFER2 *buffer, int y, int x, char c);
BUFFER2* Buffer2(int w, int h); 
int getScreenWidth(); 
int getScreenHeight();
void printScreen(BUFFER2 *buffer); 
void clearScreen(); 
