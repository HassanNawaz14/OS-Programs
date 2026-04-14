#include <sys/types.h>
#include <sys/stat.h>

int main()
{
    mkfifo("req", 0666);
    mkfifo("res", 0666);
    return 0;
}