#include <iostream>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <cmath>
#include <cstring>
#include <string>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <termios.h>

#include "i2c.cpp"
#include "imu/MahonyAHRS.cpp"

#include "imu/imu.cpp"
#include "pca/pca.cpp"
#include "serial/serial.cpp"

using namespace std;
int servo0_deg = 0;
int servo1_deg = 0;
int servo2_deg = 0;
int phase = 0;
int main()
{
    cout << "Starting"
         << "\n";
    // Initialize AHRS
    mpu mpu1(1);
    mpu1.init();

    // Initialize PCA
    pca_init();
    pca servo0(0, 70, 500, 180);
    pca servo1(1, 70, 500, 180);
    pca servo2(2, 70, 500, 180);

    //Initialize GPS
    serial gps("/dev/ttyS3", B115200);
    usleep(1000000);
    while (true)
    {
        std::clock_t c_start = std::clock();
        auto t_start = std::chrono::high_resolution_clock::now();
        mpu1.read_raw();
        servo0_deg++;
        servo0.rotate_deg(servo0_deg);
        servo1.rotate_deg(servo1_deg);
        servo2.rotate_deg(servo2_deg);
        string data = gps.read_until("\n");
        std::clock_t c_end = std::clock();
        auto t_end = std::chrono::high_resolution_clock::now();

        std::cout << std::fixed << std::setprecision(2) << "CPU time used: "
                  << 1000000.0 * (c_end - c_start) / CLOCKS_PER_SEC << " ms\t"
                  << "Wall clock time passed: "
                  << std::chrono::duration<double, std::micro>(t_end - t_start).count()
                  << " ms\n";
    }
}