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
        cout << "Serial initialized"
             << "\n";
    }
    string read_until(string delim)
    {
        string final_data;
        int no_data = 0;
        while (true)
        {
            no_data = 0;
            char buffer[1];
            int buff_char = read(ser, buffer, 1);
            if (buff_char > 0)
            {
                string data_ns = bufferToString(buffer, 1);
                if (data_ns == delim)
                {
                    break;
                }
                else
                {
                    final_data += data_ns;
                }
            }
            else
            {
                return Convert(buff_char);
            }
        }
        return final_data;
    }
    void ser_write(auto data, int length)
    {
        int ans = write(ser, data, length);
    }
};