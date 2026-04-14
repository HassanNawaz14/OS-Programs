#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
    int req = open("req", O_RDONLY);
    int res = open("res", O_WRONLY);
    int lib = open("lib.txt", O_RDWR, 0666);
    char buf[100];
    while (1)
    {
        int n = read(req, buf, sizeof(buf));
        buf[n] = '\0';
        char repTyp[10], book[50];
        sscanf(buf, "%s %s", repTyp, book);

        if (strcmp(repTyp, "request") == 0)
        {
            char temp[100];
            read(lib, temp, sizeof(temp));
            char *tok = strtok(temp, "\n");
            while (tok != NULL)
            {
                long offset = tok - temp;
                char name[50];
                int copies;
                sscanf(tok, "%s %d", name, &copies);

                if (strcmp(name, book) == 0)
                {
                    if (copies == 0)
                    {
                        write(res, "Book not available", sizeof("Book not available"));
                        break;
                    }
                    else
                    {
                        lseek(lib, offset, SEEK_SET);
                        int newCopies = copies - 1;
                        write(lib, name, strlen(name));
                        write(lib, " ", 1);
                        char copiesStr[10];
                        sprintf(copiesStr, "%d", newCopies);
                        write(lib, copiesStr, strlen(copiesStr));
                        write(res, "Book issued", sizeof("Book issued"));
                        break;
                    }
                }
                tok = strtok(NULL, "\n");
            }
        }
        else
        {
            char temp[100];
            read(lib, temp, sizeof(temp));
            char *tok = strtok(temp, "\n");

            while (tok)
            {
                long offset = tok - temp;
                char name[50];
                int copies;
                sscanf(tok, "%s %d", name, &copies);

                if (strcmp(name, book) == 0)
                {
                    lseek(lib, offset, SEEK_SET);
                    int newCopies = copies + 1;
                    write(lib, name, strlen(name));
                    write(lib, " ", 1);
                    char copiesStr[10];
                    sprintf(copiesStr, "%d", newCopies);
                    write(lib, copiesStr, strlen(copiesStr));
                    write(res, "Book returned", sizeof("Book returned"));
                    break;
                }
                tok = strtok(NULL, "\n");
            }
        }
    }
}
