#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
    int req = open("req", O_WRONLY);
    int res = open("res", O_RDONLY);
    char buf[100];

    int c = 0;
    while (1)
    {
        printf("Enter 1 for requesting, 2 for returning book: ");
        scanf("%d", &c);

        char mes[10];
        if (c == 1)
            strcpy(mes, "request");
        else
            strcpy(mes, "return");

        printf("Enter book name: ");
        char book[50];
        scanf("%s", book);
        strcat(mes, " ");
        strcat(mes, book);

        write(req, mes, sizeof(mes));
        read(res, buf, sizeof(buf));
        printf("%s\n", buf);
    }
}