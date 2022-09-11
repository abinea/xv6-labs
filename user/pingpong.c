#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
  int pf2s[2], ps2f[2];
  pipe(pf2s); // 父->子
  pipe(ps2f); // 子->父

  if (fork() != 0) {
    write(pf2s[1], "!", 1); // 1. 父在 父->子 管道的写端 写入一个字节
    char buf;
    read(ps2f[0], &buf, 1); // 4. 父在 子->父 管道的读端 读取一个字节
    printf("%d: received pong\n", getpid());
    wait(0); // 5. 等待子程序退出
  } else {
    char buf;
    read(pf2s[0], &buf, 1); // 2. 子在 父->子 管道的读端 读取一个字节
    printf("%d: received ping\n", getpid());
    write(ps2f[1], "?", 1); // 3. 子在 子->父 管道的写端 写入一个字节
    exit(0);
  }
  exit(0);
}
