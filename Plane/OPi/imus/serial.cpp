#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

int main()
{
    char byte;
    int serial = open("/dev/ttyS0", O_RDWR | O_NOCTTY | O_SYNC);
    struct termios config;
    tcgetattr(fd, &config);
    cfsetispeed(&config, B460800);
    tcsetattr(fd, TCSANOW, &config);
    write(fd, "X", 1);
    return 0;
}