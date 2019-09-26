#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(int argc, char** argv)
{
  int my_counter = 100;

  printf("%d: Testing Fork\n", getpid());
  fflush(stdout);

  int child_pid = fork();

  if (0 == child_pid){
    // Inside child process
    printf("%d: Inside child\n", getpid());
    fflush(stdout);

    // lets modify the my counter
    // and print it in both the child and the parent process
    my_counter = 222;

    sleep(1);

    printf("%d: Child value for mycounter is %d \n", getpid(), my_counter);
    fflush(stdout);

    exit(0);

  } else if (child_pid > 0) {

    // Inside parent process
    printf("%d: Inside parent that launched %d \n", getpid(), child_pid);
    fflush(stdout);

    int status;
    int ret = 0;

    do {
      // wait for child process to finish
      ret = waitpid(child_pid, &status, WUNTRACED | WCONTINUED);
      if (-1 == ret) {
        perror("waitpid failed");
        exit(1);
      }

      printf("%d, waiting for %d to finish\n", getpid(), child_pid);
      fflush(stdout);

    } while (!WIFEXITED(status) && !WIFSIGNALED(status));

    // Print value for mycounter in the parent process
    printf("%d: Parent value for mycounter is %d \n", getpid(), my_counter);

  } else {

    perror("Fork failed");
    exit(1);

  }
  return 0;
}
