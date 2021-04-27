#include <sys/ioctl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "screen.h"

void putsBuffer2(BUFFER2 *buffer, int y, int x, char* str) {
    if(y > buffer->h || x > buffer->w || y < 0 || x < 0) {
        printf("[error] putsBuffer2 x, y, over size\n"); 
        return;
    }
    if(buffer->size < y * buffer->w + x) { 
        printf("[error] putsBuffer2 buffer size too big\n"); 
        return; 
    }
    int loc = y * buffer->w + x;

    memcpy(buffer->mem + loc, str, strlen(str));
    printScreen(buffer);
}

void putcBuffer2(BUFFER2 *buffer, int y, int x, char c) {
    if(y > buffer->h || x > buffer->w || y < 0 || x < 0) return;

    int loc = y * buffer->w + x;

    buffer->mem[loc] = c;
    printScreen(buffer);
}

BUFFER2* Buffer2(int w, int h) {
    BUFFER2* buffer;
    buffer = (BUFFER2 *) malloc(sizeof(BUFFER2));
    buffer->mem = (char *) malloc(w*h*sizeof(char));
    buffer->w = w;
    buffer->h = h;
    return buffer;
}

int getScreenWidth() {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    return (int)w.ws_col;
}

int getScreenHeight() {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    return (int)w.ws_row;
}

void printScreen(BUFFER2 *buffer) {
    int i, j;

    clearScreen();

    for(i=0 ; i<buffer->h ; i++) {
        for(j=0 ; j<buffer->w ; j++) printf("%c", *(buffer->mem + i * buffer->w + j));
        printf("\n");
    }
}

void clearScreen() {
    system("clear");
}

