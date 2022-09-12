#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void process(int pleft[]);

int main(int argc, char *argv[]) {
  int pleft[2], i = 2;
  pipe(pleft);
  while (i <= 128) {
    write(pleft[1], &i, sizeof i);
    i++;
  }
  i = 1;
  write(pleft[1], &i, sizeof i);
  close(pleft[1]);
  process(pleft);
  exit(0);
}

void process(int pleft[]) {
  int prime;
  read(pleft[0], &prime, sizeof prime);
  if (prime == 1) { // 如果是1，则代表所有数字处理完毕，退出程序
    exit(0);
  }
  printf("prime %d\n", prime);

  int pright[2];
  pipe(pright);
  if (fork() != 0) {
    close(pright[0]); // 父进程不需要读，只需要写
    int buf;
    while (buf != 1 && read(pleft[0], &buf, sizeof buf)) {
      if (buf % prime != 0)
        write(pright[1], &buf, sizeof buf); // 如果是1，写在末尾
    }
    wait(0); // 等待所有子进程结束
  } else {
    close(pright[1]); // 子进程只需要读上一级的输出
    close(pleft[0]);
    process(pright); // 递归素数筛
  }
}