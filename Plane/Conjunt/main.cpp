#include <iostream>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <cmath>
#include <cstring>
#include <string>
#include <iomanip>
#include <termios.h>

#include "i2c.cpp"
#include "imu/MahonyAHRS.cpp"

#include "imu.cpp"
#include "pca.cpp"
#include "serial.cpp"
#include "socket.cpp"
using namespace std;
int servo0_deg = 0;
int servo1_deg = 0;
int servo2_deg = 0;
int phase = 0;
int main()
{
    cout << "Starting"
         << "\n";

    // Connect to socket
    connect();
    usleep(1000000);
    send_msg();
    // Initialize AHRS
    mpu mpu1(1);
    mpu1.init();

    // Initialize PCA
    pca_init();
    pca servo0(0, 70, 500, 180);
    pca servo1(1, 70, 500, 180);
    pca servo2(2, 70, 500, 180);

    //Initialize GPS
    serial gps("/dev/ttyS3", B9600);
    usleep(1000000);
    while (true)
    {
        mpu1.read_raw();
        servo0.rotate_deg(servo0_deg);
        servo1.rotate_deg(servo1_deg);
        servo2.rotate_deg(servo2_deg);
        string data = gps.read_until("\n");
        /*
        cout
            << '\r'
            << std::setw(10) << std::setfill(' ') << mpu1.accX << '\t'
            << std::setw(10) << std::setfill(' ') << mpu1.accY << '\t'
            << std::setw(10) << std::setfill(' ') << mpu1.accZ << '\t'
            << std::setw(10) << std::setfill(' ') << mpu1.gyroX << '\t'
            << std::setw(10) << std::setfill(' ') << mpu1.gyroY << '\t'
            << std::setw(10) << std::setfill(' ') << mpu1.gyroZ << '\t'
            << std::setw(10) << std::setfill(' ') << mpu1.magX << '\t'
            << std::setw(10) << std::setfill(' ') << mpu1.magY << '\t'
            << std::setw(10) << std::setfill(' ') << mpu1.magZ << '\t'
            << std::setw(10) << std::setfill(' ') << mpu1.st1 << '\t'
            << std::setw(10) << std::setfill(' ') << mpu1.st2 << '\t'
            << std::setw(30) << std::setfill(' ') << data << '\t'
            << std::flush;*/
    }
}