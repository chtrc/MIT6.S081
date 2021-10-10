#include "kernel/types.h"
#include "user/user.h"

int main()
{
    int pid, n;
    int p2c[2];
    int c2p[2];
    char buf;

    if (pipe(p2c) < 0) {
        exit(1);
    }
    if (pipe(c2p) < 0) {
        exit(1);
    }

    pid = fork();
    if (pid == 0) { //child process
        n = read(c2p[0], &buf, 1);
        printf("%d: received ping\n", getpid());
        write(p2c[1], &buf, n);
    } else {
        write(c2p[1], "a", 1);
        n = read(p2c[0], &buf, 1);
        printf("%d: received pong\n", getpid());
    }
    exit(0);
}