#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{

    if (fork() == 0)
    {
        int errfile = open("error.txt", O_RDWR | O_CREAT | O_TRUNC, 0644);

        dup2(errfile, STDERR_FILENO);

        execlp("gcc", "gcc", argv[1], "-o", "pg", NULL);
    }
    wait(NULL);

    if (fork() == 0)
    {
        execlp("./pg", "pg", NULL);
    }
    wait(NULL);

    return 0;
}