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
#include <curl/curl.h>

#include "i2c.cpp"
#include "imu/MahonyAHRS.cpp"

#include "imu/imu.cpp"
#include "pca/pca.cpp"
#include "serial/serial.cpp"
#include "comms/coms.cpp"

using namespace std;
using namespace chrono;
int servo0_deg = 0;
int servo1_deg = 0;
int servo2_deg = 0;
int servo3_deg = 0;
int servo4_deg = 0;
int servo5_deg = 0;
int servo6_deg = 0;
int motor1_deg = 0;
int motor2_deg = 0;
int phase = 0;

int loop_id = 0;
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
    pca servo3(3, 70, 500, 180);
    pca servo4(4, 70, 500, 180);
    pca servo5(5, 70, 500, 180);
    pca servo6(6, 70, 500, 180);
    pca motor1(6, 70, 500, 180);
    pca motor2(6, 70, 500, 180);

    //Initialize GPS
    serial gps("/dev/ttyS1", B19200);
    usleep(1000000);
    while (true)
    {
        for (int x = 0; x < 5; x++)
        {
            loop_id++;
            clock_t c_start = clock();
            auto t_start = high_resolution_clock::now();
            if (x == 0)
            {

                string gps_data = gps.read_until("\n");
                cout << gps_data << endl;
            }
            cout << "we good" << endl;
            mpu1.read_raw();
            servo0_deg++;
            servo0.rotate_deg(servo0_deg);
            servo1.rotate_deg(servo1_deg);
            servo2.rotate_deg(servo2_deg);
            servo3.rotate_deg(servo3_deg);
            servo4.rotate_deg(servo4_deg);
            servo5.rotate_deg(servo5_deg);
            servo6.rotate_deg(servo6_deg);
            motor1.rotate_deg(motor1_deg);
            motor2.rotate_deg(motor2_deg);
            string data = "?m1=" + to_string(motor1_deg) + "&m2=" + to_string(motor2_deg) + "&s1=" + to_string(servo0_deg) + "&s2=" + to_string(servo1_deg) + "&s3=" + to_string(servo2_deg) + "&s4=" + to_string(servo3_deg) + "&s5=" + to_string(servo4_deg) + "&s6=" + to_string(servo5_deg) + "&s7=" + to_string(servo6_deg) + "&lat=42&long=2&alt=200&packet_id=" + to_string(loop_id);
            communicate(data);
            cout << data << endl;
            clock_t c_end = clock();
            auto t_end = high_resolution_clock::now();
            double time = duration<double, micro>(t_end - t_start).count();
            if (time < 25000)
            {
                usleep(25000 - time);
            }
            else if (time > 25000)
            {
                cout << "Overpassed time: " << time << endl;
            }
        }
    }
}