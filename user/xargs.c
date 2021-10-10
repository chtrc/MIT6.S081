#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char* argv[]) {
    if(argc < 2) {
        printf("usage: xargs command args...\n");
    }
    char* cmd = argv[1];
    char* buf[200];
    int cnt = 0;
    for(int i=1;i<argc;++i){
        int len = strlen(argv[i]);
        buf[cnt] = (char*)malloc(len);
        memcpy(buf[cnt], argv[i], len);
        ++cnt;
    }
    char c;
    int ii = 0;
    char cache[100];
    int flag = 2;
    int num;
    while((num = read(0, &c, 1)) || flag) {
        if(num == 0){
            
            flag--;
        }
        if(c == '\0') {
            cache[ii++] = '\0';
            buf[cnt] = (char*)malloc(ii);
            memcpy(buf[cnt], cache, ii);
            break;
        }
        if(c == ' '|| c == '\n'){
            cache[ii++] = '\0';
            buf[cnt] = (char*)malloc(ii);
            memcpy(buf[cnt], cache, ii);
            ii = 0;
            ++cnt;
            if(cnt == 200){
                printf("bug: cnt exceed 200\n");
                exit(0);
            }
            continue;
        }
        cache[ii++] = c;
        if(ii == 100) {
            printf("bug: length exceed 100\n");
            exit(0);
        }   
    }
    if(fork() == 0) {
        exec(cmd, buf);
    } else {
        wait(0);
    }
    exit(0);
}