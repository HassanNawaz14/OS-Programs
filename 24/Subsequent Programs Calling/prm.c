#include <stdio.h>
#include <unistd.h>
#include <string.h>

int checkPrime(int n)
{
    if (n <= 1)
        return 0;
    for (int i = 2; i * i <= n; i++)
    {
        if (n % i == 0)
            return 0;
    }
    return 1;
}

int main(int argc, char *argv[])
{
    printf("Prm Got String: %s\n", argv[1]);

    char st[100];
    strcpy(st, argv[1]);

    int len = 0;
    while (st[len++] != '\0')
        ;
    len--;
    printf("Length of string: %d\n", len);

    int ans = checkPrime(len);
    printf("Is the length prime? %s, My ID: %d\n", ans ? "Yes" : "No", getpid());

    return 0;
}