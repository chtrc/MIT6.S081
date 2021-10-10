#include "kernel/types.h"
#include "kernel/fs.h"
#include "kernel/stat.h"
#include "user/user.h"

void find(char* path, char* name)
{
    int fd;
    char buf[512], *p;
    struct stat st;
    struct dirent de;
    if ((fd = open(path, 0)) < 0) {
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }

    if (fstat(fd, &st) < 0) {
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }
    switch (st.type) {
    case T_FILE:

        printf("bug\n");
        break;

    case T_DIR:
        if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf) {
            printf("find: path too long\n");
            break;
        }
        strcpy(buf, path);
        p = buf + strlen(buf);
        *p++ = '/';
        while (read(fd, &de, sizeof(de)) == sizeof(de)) {
            if(de.inum == 0)
                continue;
            if (strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
                continue;
            memmove(p, de.name, DIRSIZ);
            p[DIRSIZ] = 0;
            if (stat(buf, &st) < 0) {
                printf("find: cannot stat %s :%s\n", buf, de.name);
                continue;
            }
            if (st.type == T_DIR) {
                find(buf, name);
            } else if (st.type == T_FILE) {
                if (strcmp(de.name, name) == 0)
                    printf("%s\n", buf);
            }
        }
        break;
    }
    close(fd);
}
int main(int argc, char* argv[])
{
    if (argc < 2) {
        printf("usage: find filename\n");
        exit(0);
    }
    for (int i = 1; i < argc; ++i)
        find(".", argv[i]);
    exit(0);
}