#include "screen.h"

int main() {
    BUFFER2* buffer;
    int i, j;

    clearScreen();

    while(1) {
        buffer = Buffer2(getScreenWidth(), getScreenHeight());
        putcBuffer2(buffer, 1, 1, 'c');
        printScreen(buffer);
    }
}

