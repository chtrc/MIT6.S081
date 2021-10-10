#include "kernel/types.h"
#include "user/user.h"
void prime(int fd)
{
    int buf, p[2];
    int flag = 0;
    read(fd, &buf, 4);
    int x = buf;
    printf("prime %d\n", buf);
    while (read(fd, &buf, 4)) {
        if (!flag) {
            flag = 1;
            pipe(p);
            if (fork() == 0) {
                close(p[1]);
                prime(p[0]);
                return;
            } else {
                close(p[0]);
            }
        }
        if (buf % x != 0)
            write(p[1], &buf, 4);
    }
    close(fd);
    close(p[1]);
    wait(0);
}

int main()
{
    int p[2];

    pipe(p);

    for (int i = 2; i <= 35; ++i) {
        write(p[1], &i, 4);
    }
    close(p[1]);
    if (fork() == 0) {
        prime(p[0]);
        close(p[0]);
    }
    close(p[0]);
    wait(0);
    exit(0);
}