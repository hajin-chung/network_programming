#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int main()
{
    int     fd, fd1;
    char    buf[255];
    int     state;

    struct    timeval tv;
    fd_set    readfds;
    fd = fileno(stdin);


    FD_ZERO(&readfds);
    while(1) 
    {
        FD_SET(fd, &readfds);

        tv.tv_sec = 5;
        tv.tv_usec = 0;

        printf("\n Input in 2sec :  ");
        fflush(0);
        state = select(fd + 1, &readfds, (fd_set *)0, (fd_set *)0, &tv);
        printf("%d\n", state);
        switch(state)
        {
            case -1:
                perror("select error : ");
                exit(0);
                break;
            case 0:
                printf("Time over (5sec)\n");
                close(fd);
                exit(0);
                break;
            default:
                if(FD_ISSET(fd, &readfds)) {
                    fgets(buf, 255, stdin);
                    printf("%s", buf);
                }
                break;
        }
    }

}


