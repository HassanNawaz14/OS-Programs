#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc, void *argv[])
{
    char st[100];
    strcpy(st, argv[1]);
    printf("String: %s, My ID: %d\n", st, getpid());

    if (fork() == 0)
    {
        execlp("./prm", "prm", st, NULL);
    }

    wait(NULL);
    printf("All Done\n");

    return 0;
}