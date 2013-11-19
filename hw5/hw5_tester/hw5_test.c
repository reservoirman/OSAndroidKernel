#include <stdio.h>
#include <errno.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pwd.h>
#include <signal.h>

#define SEG 10000000

void test1() {
    printf("@@@@uid: %d\n", getuid());
    printf("@@@@pid: %d\n", getpid());
}

int main(int argc, char **argv) {
  struct passwd *pwd_struct = getpwnam(argv[1]);
  if (!pwd_struct) {
    printf("Failed to fetch passwd struct\n");
    return -1;
  }
  printf("pw->uid=%d, pw->name=%s\n", pwd_struct->pw_uid, pwd_struct->pw_name);

  int limit = atoi(argv[2]);
  pid_t *pids = (pid_t*)malloc((argc-3)*sizeof(pid_t));
  syscall(378, pwd_struct->pw_uid, limit);
  int i, mem;
  test1();
  for (i = 3; i < argc; ++i) {
     mem = atoi(argv[i]);
     pid_t p = pids[i-3] = fork();
     if (0 == p) {
        printf("child process start malloc: pid=%d, uid=%d, mem=%d\n", getpid(), getuid(), mem);
        char *m = (char*)malloc(mem);
        //memset(m, 0, mem);
        int j = 0, k = 0;
        int n = 0, rmd = 0;
        n =   mem / SEG;
        rmd = mem % SEG;
        //printf("n = %d, rmd = %d\n", n, rmd);
        for (j = 0; j < n; j++) {
            for (k = 0; k < SEG; k++) {
                m[j*SEG+k] = '\0';
            }
            sleep(1);
        }
        for (k = 0; k < rmd; k++)
            m[n*SEG+k] = '\0';
        /*for (j = 0; j < mem; j++){
            m[j] = '\0';
        }*/
        sleep(5);
        printf("child process finish malloc: pid=%d, uid=%d, mem=%d\n", getpid(), getuid(), mem);
        free(m);
        return 0;
     }
  }

  for (i = 0; i < argc-3; ++i) {
    waitpid(pids[i], NULL, 0);
  }
  free(pids);

  return 0;
}