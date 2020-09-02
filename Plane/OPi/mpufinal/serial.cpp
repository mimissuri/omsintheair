#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

int main()
{
    char byte;
    int fd = open("/dev/ttyS0", O_RDWR);
    struct termios config;
    tcgetattr(fd, &config);
    cfsetispeed(&config, B115200);
    tcsetattr(fd, TCSANOW, &config);
    write(fd, "X", 1);
    return 0;
}