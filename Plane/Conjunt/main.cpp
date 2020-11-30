#include <iostream>
#include <sys/ioctl.h>
#include <fcntl.h>
//USLEEP
#include <unistd.h>
#include <cmath>
#include <cstring>
#include <string>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <termios.h>
#include <curl/curl.h>
#include <thread>

#include "i2c.cpp"
#include "imu/MahonyAHRS.cpp"

#include "imu/imu.cpp"
#include "pca/pca.cpp"
#include "serial/serial.cpp"
#include "crono.cpp"

using namespace std;
using namespace chrono;

int main()
{
    //Global Timers
    clp chrono_global;
    chrono_global.start();

    clp chrono_main;

    //Initialize IMU's
    mpu mpu_1(0x68);
    mpu_1.init();
    mpu mpu_2(0x69);
    mpu_2.init();
    mpu mpu_3(0x68);
    mpu_3.init();

    //PCA
    pca servo0(0, 70, 500, 180);
    pca servo1(1, 70, 500, 180);
    pca servo2(2, 70, 500, 180);
    pca servo3(3, 70, 500, 180);
    pca servo4(4, 70, 500, 180);
    pca servo5(5, 70, 500, 180);
    pca servo6(6, 70, 500, 180);

    //Initialize GPS
    serial gps("/dev/ttyS1", B19200);

    while (true)
    {
        for (int i = 0; i < 10; i++)
        {
            chrono_main.start();

            chrono_main.end();
        }
    }
    return 0;
}