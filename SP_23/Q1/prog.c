#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

sem_t orderplaced[100]; // 0: not plced, 1: placed
sem_t orderprocessed[100];

// using bakery's for tokens:
int tokens[100] = {0};
int chosing[100] = {0};

int N = 20;

sem_t mutex;
sem_t orderNo;

void *placeOrders(void *arg)
{
    int *i = (int *)arg;
    // printf("...");
    sem_wait(&orderNo); // no more that 3 orders

    // assign token by bakery's algo:
    chosing[*i] = 1;
    int max = 0;
    for (int j = 0; j < N; j++)
    {
        // printf("...\n");
        if (tokens[j] > max)
            max = tokens[j];
    }
    tokens[*i] = max + 1;
    printf("Token[%d] = %d\n", *i, tokens[*i]);
    chosing[*i] = 0;

    for (int j = 0; j < N; j++)
    {
        if (j == *i)
            continue;
        while (chosing[j])
            ;
        // printf("Crossed\n");
        while (tokens[j] != 0 &&
               (tokens[j] < tokens[*i] ||
                (tokens[j] == tokens[*i] && j < *i)))
            ;
    }

    printf("Order: %d with token %d Placed!\n", *i, tokens[*i]);
    // sleep(1);
    sem_post(&orderplaced[*i]); // allow order processing
}

void *processOrders(void *arg)
{

    int *i = (int *)arg;
    sem_wait(&orderplaced[*i]);

    sem_wait(&mutex); // one processing ata time!
    // sleep(1);
    printf("Order: %d with token %d processed!\n", *i, tokens[*i]);
    sem_post(&mutex);

    sem_post(&orderprocessed[*i]);
}

void *delivOrders(void *arg)
{
    int *i = (int *)arg;

    sem_wait(&orderprocessed[*i]);

    printf("Order: %d with token %d Delivered!\n", *i, tokens[*i]);

    sem_post(&orderNo);

    tokens[*i] = 0;
}

int main()
{
    for (int i = 0; i < N; i++)
    {
        sem_init(&orderplaced[i], 0, 0);
        sem_init(&orderprocessed[i], 0, 0);
    }
    sem_init(&mutex, 0, 1);
    sem_init(&orderNo, 0, 3);

    pthread_t placeOrder[100],
        processOrder[100], delivOrder[100];

    for (int i = 0; i < N; i++)
    {
        int *arg = malloc(sizeof(*arg));
        *arg = i;
        pthread_create(&placeOrder[i], NULL, placeOrders, (void *)arg);
    }
    for (int i = 0; i < N; i++)
    {
        int *arg = malloc(sizeof(*arg));
        *arg = i;
        pthread_create(&processOrder[i], NULL, processOrders, (void *)arg);
    }
    for (int i = 0; i < N; i++)
    {
        int *arg = malloc(sizeof(*arg));
        *arg = i;
        pthread_create(&delivOrder[i], NULL, delivOrders, (void *)arg);
    }

    for (int i = 0; i < N; i++)
    {
        pthread_join(placeOrder[i], NULL);
        pthread_join(processOrder[i], NULL);
        pthread_join(delivOrder[i], NULL);
    }
}