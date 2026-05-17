#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>
#include <sys/wait.h>
#include <pthread.h>
#include <sys/shm.h>
#include <sys/ipc.h>

typedef struct
{
    sem_t S1[10];
    int tasks[11];
} Data;

int check(Data *ptr)
{
    for (int i = 0; i < 10; i++)
    {
        if (ptr->tasks[i] != 0)
        {
            return 1;
        }
    }
    return 0;
}
int main()
{
    int id = shmget(111, 10 * sizeof(int), IPC_CREAT | 0666);
    Data *ptr = shmat(id, NULL, 0);

    char readBuff[8];
    int fd[3][2];

    for (int i = 0; i < 10; i++)
        sem_init(&ptr->S1[i], 1, 1);

    for (int i = 0; i < 10; i++)
        ptr->tasks[i] = i + 1;
    ptr->tasks[10] = 0;

    for (int i = 0; i < 3; i++)
        pipe(fd[i]);

    for (int i = 0; i < 3; i++)
    {
        if (fork() == 0)
        {
            int id = shmget(111, 10 * sizeof(int), IPC_CREAT | 0666);
            Data *ptr = shmat(id, NULL, 0);

            while (check(ptr))
            {
                close(fd[i][0]);

                // for (int i = 0; i < 11; i++)
                //     printf("%d ", ptr->tasks[i]);
                // printf("tasks[%d] = %d ", i, ptr->tasks[i]);

                int k;
                for (int k = 0; k < 10; k++)
                {
                    if (ptr->tasks[k] != 0)
                    {
                        sem_wait(&ptr->S1[k]);
                        printf("Task %d Read by Worker %d\n", ptr->tasks[k], i);
                        write(fd[i][1], "Success", 8);
                        ptr->tasks[k] = 0;
                        break;
                    }
                }
                sem_post(&ptr->S1[k]);

                close(fd[i][1]);
            }
        }
    }

    while (ptr->tasks[10] != 10)
    {
        for (int i = 0; i < 3; i++)
        {
            close(fd[i][1]);
            int n = read(fd[i][0], readBuff, 8);
            if (!n)
                continue;
            if (strcmp("Success", readBuff) == 0)
                ptr->tasks[10]++;
            close(fd[i][0]);
        }
    }

    wait(NULL);
    wait(NULL);
    wait(NULL);

    printf("None Leftee. Ending\n");

    shmdt(ptr);
    shmctl(id, IPC_RMID, NULL);
    return 0;
}