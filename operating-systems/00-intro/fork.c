#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char const *argv[])
{
    printf("Hello World (pid: %d)\n", (int)getpid());

    int rc = fork();
    if (rc < 0)
    {
        fprintf(stderr, "fork failed\n");
        exit(1);
    }
    int c_pid = (int)getpid();
    if (rc == 0)
    {
        printf("hello, I am a child (pid: %d)\n", c_pid);
    }
    else
    {
        wait(NULL);
        waitpid(c_pid, NULL, 0); // wait(NULL) -> wait for all children
        printf("hello, I am parent of %d (pid: %d)\n", rc, c_pid);
    }
    return 0;
}
