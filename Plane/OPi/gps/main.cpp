#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <string>
#include <cstring>
#include <algorithm>
#include <iterator>
#include <vector>
#include <sstream>
#include <iomanip>

using namespace std;

string bufferToString(char *buffer, int bufflen)
{
    string ret(buffer, bufflen);
    return ret;
};

class serial
{

public:
    int ser;
    char *filename;
    speed_t baudrate;

    serial(char *a, speed_t br) : filename(a), baudrate(br)
    {
        cout << "Opened: " << filename << "\n";
        ser = open(filename, O_RDWR | O_NOCTTY | O_SYNC);
        struct termios config;
        tcgetattr(ser, &config);
        cfsetispeed(&config, baudrate);
        tcsetattr(ser, TCSANOW, &config);
    }

    string read_until(string delim)
    {
        string final_data;
        while (true)
        {
            char buffer[1];
            int buff_char = read(ser, buffer, 1);
            string data_ns = bufferToString(buffer, 1);
            if (buff_char > 0)
            {
                if (data_ns == delim)
                {
                    break;
                }
                else
                {
                    final_data += data_ns;
                }
            }
        }
        return final_data;
    }
    void ser_write(auto data, int length)
    {
        int ans = write(ser, data, length);
    }
};
int main()
{
    serial gps("/dev/ttyS0", B115200);
    usleep(1000);
    while (true)
    {
        string data = gps.read_until("\n");
        cout << data << "\n";
    }
}