#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char *fmtname(char *path) {
    static char buf[DIRSIZ + 1];
    char *p;

    for (p = path + strlen(path); p >= path && *p != '/'; p--)
        ;
    p++;

    if (strlen(p) >= DIRSIZ)
        return p;
    memmove(buf, p, strlen(p));
    memset(buf + strlen(p), ' ', DIRSIZ - strlen(p));
    return buf;
}

void find(char *path, char *target) {
    int fd;
    struct stat st;
    char buf[512], *p;
    struct dirent de;

    // 无法打开文件/目录
    if ((fd = open(path, 0)) < 0) {
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }
    // 无法读取文件描述符的状态
    if (fstat(fd, &st) < 0) {
        fprintf(2, "find: cannot stat %s\n", path);
    }

    switch (st.type) {
    case T_FILE:
        fprintf(2, "find: cannot open %s that is a file\n", path);
        break;
    case T_DIR:
        // 目录名称过长
        if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf) {
            printf("find: path too long\n");
            exit(1);
        }
        strcpy(buf, path);
        p = buf + strlen(buf);
        *p++ = '/';
        while (read(fd, &de, sizeof(de)) == sizeof(de)) {
            if (strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
                continue;
            if (de.inum == 0)
                continue;
            memmove(p, de.name, DIRSIZ);
            p[DIRSIZ] = 0;
            if (stat(buf, &st) < 0) {
                printf("find: cannot stat %s\n", buf);
                continue;
            }
            if (strcmp(fmtname(buf), target) == 32)
                printf("%s\n", buf);
            if (st.type == T_DIR) {
                find(buf, target);
            }
        }
    }
    close(fd);
}

int main(int argc, char *argv[]) {
    // 参数处理
    if (argc < 3) {
        printf("find: the number of arguments is incorrect.");
        exit(1);
    }
    // find主逻辑
    find(argv[1], argv[2]);
    exit(0);
}