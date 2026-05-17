#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>

int main()
{
    sem_t *mutex = sem_open("/log_sem", O_CREAT, 0666, 1);
    if (mutex == SEM_FAILED)
    {
        perror("sem_open");
        return 1;
    }

    sem_post(mutex);

    int N;
    scanf("%d", &N);

    // int fd = open("log.txt", O_CREAT | O_WRONLY);

    for (int i = 0; i < N; i++)
    {
        if (fork() == 0)
        {
            while (1)
            {
                int n = (rand() % 5) + i;
                printf("n:%d\n", n);
                sleep(n); // process comes again after random time
                sem_wait(mutex);
                printf("...\n");
                int fd = open("log.txt", O_WRONLY | O_APPEND | O_CREAT, 0666);
                char mes[100] = "Process";
                char c[100] = "";
                sprintf(c, "%d", i);
                strcat(mes, c);
                mes[9] = '\n';
                write(fd, mes, 10);
                printf("Written: %s", mes);
                sem_post(mutex);
            }
        }
    }
    for (int i = 0; i < N; i++)
    {
        wait(NULL);
    }

    sem_unlink("/log_sem");
}