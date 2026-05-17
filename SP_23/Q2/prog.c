#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>

int m1[10][10];
int m2[10][10];

int rowres[10][10];
int mat[10][10];

void matinit()
{
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            m1[i][j] = (rand() % 10) + 1;
            m2[i][j] = (rand() % 10) + 1;
        }
    }

    printf("\nm1:\n");
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            printf("%d ", m1[i][j]);
        }
        printf("\n");
    }
    printf("\nm2:\n");
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            printf("%d ", m2[i][j]);
        }
        printf("\n");
    }
}

void *mult(void *arg)
{
    int *r = (int *)arg;

    for (int i = 0; i < 10; i++)
    {
        int sum = 0;
        for (int j = 0; j < 10; j++)
        {
            sum += m1[*r][j] * m2[j][i];
        }
        rowres[*r][i] = sum;
    }
}

int main()
{
    matinit();
    pthread_t tid[10];

    for (int i = 0; i < 10; i++)
    {
        int *arg = malloc(sizeof(*arg));
        *arg = i;
        pthread_create(&tid[i], NULL, mult, (void *)arg);
    }

    for (int i = 0; i < 10; i++)
    {
        pthread_join(tid[i], NULL);
    }

    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            mat[i][j] = rowres[i][j];
        }
    }

    printf("\nresult\n:");
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            printf("%d ", mat[i][j]);
        }
        printf("\n");
    }
}