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
//GGA
string utc;
float lat = 0;
string lat_dir;
float longi = 0;
string longi_dir;
int gps_qual_ind = 0;
float altitude = 0;
int fix_sats = 0;
int vis_sats = 0;
float speed = 0;

string bufferToString(char *buffer, int bufflen)
{
    string ret(buffer, bufflen);
    return ret;
}

void tokenize(string const &str, const char delim,
              vector<string> &out)
{
    size_t start;
    size_t end = 0;

    while ((start = str.find_first_not_of(delim, end)) != string::npos)
    {
        end = str.find(delim, start);
        out.push_back(str.substr(start, end - start));
    }
}
int main()
{
    const char delim = ',';
    string::size_type sz;

    int serial = open("/dev/ttyS0", O_RDWR | O_NOCTTY | O_SYNC);
    struct termios config;
    tcgetattr(serial, &config);
    cfsetispeed(&config, B9600);
    tcsetattr(serial, TCSANOW, &config);
    while (true)
    {
        char buffer[0];
        string final;
        while (true)
        {
            int n = read(serial, buffer, 1);
            string data = bufferToString(buffer, 1);
            final += data;
            if (data == "\n")
            {
                break;
            }
        }
        vector<string> out;
        tokenize(final, delim, out);
        string id = out[0];
        if (id == "$GNTXT")
        {
            /* MOLESTA */
        }
        else if (id == "\n")
        {
            /* MOLESTA */
        }
        else if (id == "$GNGGA")
        {
            /* https://www.trimble.com/OEM_ReceiverHelp/V4.44/en/NMEA-0183messages_GGA.html */
            utc = out[1];

            lat = atof(out[2].c_str());

            lat_dir = out[3];

            longi = atof(out[4].c_str());
            longi_dir = out[5];
            altitude = atof(out[9].c_str());
            gps_qual_ind = atof(out[6].c_str());
            fix_sats = atof(out[7].c_str());
        }
        else if (id == "$GNGSV")
        {
            /* https://www.trimble.com/OEM_ReceiverHelp/V4.44/en/NMEA-0183messages_GSV.html */
        }
        else if (id == "$GLGSV")
        {
            /* https://www.trimble.com/OEM_ReceiverHelp/V4.44/en/NMEA-0183messages_GSV.html */
        }
        else if (id == "$GPGSV")
        {
            /* https://www.trimble.com/OEM_ReceiverHelp/V4.44/en/NMEA-0183messages_GSV.html */
        }
        else if (id == "$GNRMC")
        {
            speed = atof(out[7].c_str()) * 1.852 / 3.6;
            /* https://www.trimble.com/OEM_ReceiverHelp/V4.44/en/NMEA-0183messages_RMC.html
             */
        }
        else if (id == "$GNGSA")
        {
            /* https://www.trimble.com/OEM_ReceiverHelp/V4.44/en/NMEA-0183messages_GSA.html */
        }
        else if (id == "$GNGLL")
        {
            /* 
            TENIR EN COMPTE!!
            https://www.trimble.com/OEM_ReceiverHelp/V4.44/en/NMEA-0183messages_GLL.html */
        }
        else if (id == "$GNVTG")
        {
            /* https://www.trimble.com/OEM_ReceiverHelp/V4.44/en/NMEA-0183messages_VTG.html */
        }
        cout
            << "GPS State: " << std::setprecision(10) << gps_qual_ind << '\t'
            << "Latitude: " << std::setprecision(10) << lat << '\t'
            << "Longitude: " << std::setprecision(10) << longi << '\t'
            << "Altitude: " << std::setprecision(10) << altitude << '\t'
            << "Speed: " << std::setprecision(10) << speed << '\t'
            << "Fixed Sats: " << std::setprecision(10) << fix_sats << '\t'
            << "Visible Sats: " << std::setprecision(10) << vis_sats << '\t'
            << "\n";
    }
    return 0;
}