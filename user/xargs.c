#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void run(char *program, char *args[]) {
  if (fork() == 0) { // 子进程执行命令
    exec(program, args);
    exit(0);
  }
  return;
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(2, "usage: xargs [order] [args]");
  }
  char *argbuf[128]; // argv 参数和 stdin 参数
  char buf[2048];    // 读取 stdin 使用的内存空间
  char *buf_p = buf, *buf_q = buf;

  char **argv_list = argbuf;
  for (int i = 1; i < argc; i++) { // 读取 argv 参数
    *argv_list = argv[i];
    argv_list++;
  }
  char **argv_end = argv_list;

  while (read(0, buf_p, 1) != 0) {
    if (*buf_p == ' ' || *buf_p == '\n') {
      *buf_p = '\0';
      *(argv_end++) = buf_q;
      buf_q = buf_p + 1;
      if (*buf_p == '\n') {
        *argv_end = '\0';
        run(argv[1], argbuf);
        argv_end = argv_list;
      }
    }
    buf_p++;
  }
  if (argv_end != argv_list) {
    *buf_p = '\0';
    *(argv_end++) = buf_q;
    *argv_end = '\0';
    run(argv[1], argbuf);
    argv_end = argv_list;
  }
  while (wait(0)!=-1){};
  // echo "1\n2" | xargs  echo line
  exit(0);
}
