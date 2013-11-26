#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <stdio.h>

#define NUMBER "97"

int main(void)
{
    int sock[2];
    int ret = socketpair(AF_UNIX, SOCK_STREAM, 0, sock);

    if (ret) {
        perror("socketpair");
        return 1;
    }

    int process = fork();

    // Child
    if (process == 0) {
        char buf[2], result[2];
        int first, second;
        recv(sock[0], buf, sizeof(buf), 0);
        first = buf[0] - '0';
        second = buf[1] - '0';
        sprintf(result, "%d", first + second);

        send(sock[0], result, 2, 0);
        close(sock[0]);
    }

    // Parent
    else if (process > 0) {
        char buf[2];
        send(sock[1], NUMBER, 2, 0);
        recv(sock[1], buf, sizeof(buf), 0);
        printf("Output: %s\n", buf);
        close(sock[1]);
    }

    // Error
    else {
        printf("Bad fork!");
        return 0;
    }

    return 0;
}
