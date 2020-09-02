#include <iostream>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <chrono>
#include <ctime>
#include <termios.h>
#include <cmath>
#include <cstring>
#include <string>
#include "MahonyAHRS.cpp"

extern "C"
{
#include <linux/i2c-dev.h>
#include <i2c/smbus.h>
}

#include <iomanip>
using namespace std;

int16_t dataConv(int16_t data1, int16_t data2)
{
    int16_t value = data1 | (data2 << 8);
    if (value & (1 << 16 - 1))
    {
        value -= (1 << 16);
    }
    return value;
}
void i2c_write(int privfile, __u8 reg_address, __u8 val)
{
    char buf[2];

    buf[0] = reg_address;
    buf[1] = val;

    if (write(privfile, buf, 2) != 2)
    {
        printf("Error, unable to write to i2c device\n");
        exit(1);
    }
}

char i2c_read(int privfile, uint8_t reg_address)
{
    char buf[1];
    buf[0] = reg_address;

    if (write(privfile, buf, 1) != 1)
    {
        printf("Error, unable to write to i2c device\n");
        exit(1);
    }
    if (read(privfile, buf, 1) != 1)
    {
        printf("Error, unable to read from i2c device\n");
        exit(1);
    }

    return buf[0];
}

int16_t i2c_readword(int privfile, uint8_t MSB, uint8_t LSB)
{

    uint8_t msb = i2c_read(privfile, MSB);

    uint8_t lsb = i2c_read(privfile, LSB);

    return ((int16_t)msb << 8) + lsb;
}

string Convert(float number)
{
    ostringstream buff;
    buff << number;
    return buff.str();
}

/*


		MPU9250

	
*/
#define PWR_MGMT_1 0x6B
#define USER_CTRL 0x6A
#define AK8963_ST1 0x02
#define SMPLRT_DIV 0x19
#define CONFIG 0x1A
#define GYRO_CONFIG 0x1B
#define ACCEL_CONFIG 0x1C
#define ACCEL_CONFIG_2 0x1D
#define INT_PIN_CFG 0x37
#define AK8963_CNTL1 0x0A
#define AK8963_ASAX 0x10
class mag
{

public:
    int bus;
    bool ready;
    int file;
    int mpu_file;
    char *filename = "/dev/i2c-0";
    char *mpu_filename = "/dev/i2c-0";
    float mres = 4912.0 / 32760.0;
    float magXc;
    float magYc;
    float magZc;
    float magX;
    float magY;
    float magZ;
    float gyroX;
    float gyroY;
    float gyroZ;
    float gyroXoff;
    float gyroYoff;
    float gyroZoff;
    int st1;
    int st2;

    mag(int a) : bus(a)
    {
    }

    int init()
    {
        file = open(filename, O_RDWR);
        mpu_file = open(mpu_filename, O_RDWR);
        if (ioctl(file, I2C_SLAVE, 0x0c) < 0)
        {
            exit(1);
        }
        if (ioctl(mpu_file, I2C_SLAVE, 0x68) < 0)
        {
            exit(1);
        }
        //MPU CONIG
        i2c_write(mpu_file, PWR_MGMT_1, 0x00);
        usleep(100000);
        i2c_write(mpu_file, PWR_MGMT_1, 0x01);
        usleep(100000);
        i2c_write(mpu_file, CONFIG, 0x00);
        i2c_write(mpu_file, SMPLRT_DIV, 0x00);
        i2c_write(mpu_file, GYRO_CONFIG, 0x00);
        i2c_write(mpu_file, ACCEL_CONFIG, 0x00);
        i2c_write(mpu_file, ACCEL_CONFIG_2, 0x00);
        i2c_write(mpu_file, INT_PIN_CFG, 0x02);
        usleep(100000);
        i2c_write(mpu_file, USER_CTRL, 0x00);
        usleep(100000);

        //AK CONFIG
        i2c_write(file, AK8963_CNTL1, 0x00);
        usleep(100000);
        i2c_write(file, AK8963_CNTL1, 0x0F);
        usleep(100000);
        __u8 buf[3];
        buf[0] = i2c_read(file, AK8963_ASAX);
        buf[1] = i2c_read(file, AK8963_ASAX + 1);
        buf[2] = i2c_read(file, AK8963_ASAX + 2);
        magXc = (buf[0] - 128) / 256.0 + 1.0;
        magYc = (buf[1] - 128) / 256.0 + 1.0;
        magZc = (buf[2] - 128) / 256.0 + 1.0;
        i2c_write(file, AK8963_CNTL1, 0x00);
        usleep(100000);
        i2c_write(file, AK8963_CNTL1, 0x16);
    }

    void read_raw()
    {
        st1 = i2c_read(file, AK8963_ST1);
        if (st1 & 0x01)
        {
            int16_t buf[7];
            buf[0] = i2c_read(file, 0x03);
            buf[1] = i2c_read(file, 0x04);
            buf[2] = i2c_read(file, 0x05);
            buf[3] = i2c_read(file, 0x06);
            buf[4] = i2c_read(file, 0x07);
            buf[5] = i2c_read(file, 0x08);
            buf[6] = i2c_read(file, 0x09);
            st2 = buf[6];
            if (buf[6] == 16)
            {
                magX = dataConv(buf[0], buf[1]);
                magY = dataConv(buf[2], buf[3]);
                magZ = dataConv(buf[4], buf[5]);

                magX = magX * mres * magXc;
                magY = magY * mres * magYc;
                magZ = magZ * mres * magZc;
            }
        }
    }
};
/*


		MPU6050


*/

#define REG_PWR_MGMT_1 0x6B
#define REG_GYRO_CONFIG 0x1B
#define REG_ACCEL_CONFIG 0x1C
#define REG_ACCEL_READINGS 0x3B
#define ACCEL_XOUT_H 0x3B
#define ACCEL_XOUT_L 0x3C
#define ACCEL_YOUT_H 0x3D
#define ACCEL_YOUT_L 0x3E
#define ACCEL_ZOUT_H 0x3F
#define ACCEL_ZOUT_L 0x40
#define GYRO_XOUT_H 0x43
#define GYRO_XOUT_L 0x44
#define GYRO_YOUT_H 0x45
#define GYRO_YOUT_L 0x46
#define GYRO_ZOUT_H 0x47
#define GYRO_ZOUT_L 0x48
class imu
{

public:
    int addr;
    bool ready;
    int file;
    char *filename = "/dev/i2c-0";
    float gyroX;
    float gyroY;
    float gyroZ;
    float gyroXoff;
    float gyroYoff;
    float gyroZoff;
    float accXoff;
    float accYoff;
    float accZoff;
    float accX;
    float accY;
    float accZ;
    double pitch = 0.00f;
    double roll = 0.00f;
    double yaw = 0.00f;
    double off_pitch = 0.00f;
    double off_roll = 0.00f;
    double off_yaw = 0.00f;
    int calib_points = 100;
    imu(int a) : addr(a)
    {
    }

    int init()
    {
        file = open(filename, O_RDWR); //Setting all to 0
        if (ioctl(file, I2C_SLAVE, addr) < 0)
        {
            exit(1);
        }

        i2c_write(file, REG_PWR_MGMT_1, 0x00);
        i2c_write(file, REG_GYRO_CONFIG, 0x00);
        i2c_write(file, REG_ACCEL_CONFIG, 0x00);
        usleep(100000);
        ready = true;
    }
    void process_offseat()
    {
        float roll2 = 0;
        float pitch2 = 0;
        float yaw2 = 0;
        for (int i = 0; i != 100; i++)
        {
            read_raw();
            roll2 += roll;
            pitch2 += pitch;
            yaw2 += yaw;
        }
        off_pitch = pitch2 / 100;
        off_yaw = yaw2 / 100;
        off_roll = roll2 / 100;
    }
    void calibrate()
    {
        gyroXoff = 0;
        gyroYoff = 0;
        gyroZoff = 0;
        for (int i = 0; i < calib_points; i++)
        {
            read_raw();
            gyroXoff += gyroX;
            gyroYoff += gyroY;
            gyroZoff += gyroZ;
            accXoff += accX;
            accYoff += accY;
            accZoff += accZ;
        }
        gyroXoff /= calib_points;
        gyroYoff /= calib_points;
        gyroZoff /= calib_points;
        accXoff /= calib_points;
        accYoff /= calib_points;
        accZoff /= calib_points;
    }
    void read_calib()
    {
        read_accel();
        read_gyro();
    }
    void read_raw()
    {
        read_accel();
        read_gyro();
    }
    void read_accel()
    {
        accX = (int16_t)i2c_readword(file, ACCEL_XOUT_H, ACCEL_XOUT_L) / 16384.00;
        accY = (int16_t)i2c_readword(file, ACCEL_YOUT_H, ACCEL_YOUT_L) / 16384.00;
        accZ = (int16_t)i2c_readword(file, ACCEL_ZOUT_H, ACCEL_ZOUT_L) / 16384.00;
    }
    void read_gyro()
    {
        gyroX = (int16_t)i2c_readword(file, GYRO_XOUT_H, GYRO_XOUT_L) / 131.00;
        gyroY = (int16_t)i2c_readword(file, GYRO_YOUT_H, GYRO_YOUT_L) / 131.00;
        gyroZ = (int16_t)i2c_readword(file, GYRO_ZOUT_H, GYRO_ZOUT_L) / 131.00;
    }
};
/*


        MAIN


*/
float freq = 1000000 / 100;

int main()
{
    printf("Bon dia\n");
    mag mag1(0);
    mag1.init();
    cout << "Magnetic offset: "
         << "\t" << mag1.magXc << "\t" << mag1.magYc << "\t" << mag1.magZc << "\n";
    imu mpu1(0x69);
    mpu1.init();
    Mahony filter;
    int serial = open("/dev/ttyS3", O_RDWR);
    struct termios config;
    tcgetattr(serial, &config);
    cfsetispeed(&config, B115200);
    tcsetattr(serial, TCSANOW, &config);
    cout << "Started"
         << "\n";
    while (true)
    {
        clock_t c_start = clock();
        auto t_start = chrono::high_resolution_clock::now();
        mpu1.read_calib();
        mag1.read_raw();
        filter.update(mpu1.gyroX, mpu1.gyroY, mpu1.gyroZ,
                      mpu1.accX, mpu1.accY, mpu1.accZ,
                      mag1.magX, mag1.magY, mag1.magZ);
        int16_t buf[4];
        float pitch = filter.getPitch();
        float roll = filter.getRoll();
        float yaw = filter.getYaw();
        string ns = "Orientation:;" + Convert(yaw) + ";" + Convert(pitch) + ";" + Convert(roll) + "\n";
        const void *a = ns.c_str();
        write(serial, a, ns.length());
        clock_t c_end = clock();
        auto t_end = chrono::high_resolution_clock::now();
        double time = chrono::duration<double, micro>(t_end - t_start).count();
        if (time < freq)
        {
            usleep(10000 - freq);
        }
        else
        {
            cout << "Slow"
                 << "\n";
        }
    }
    return 0;
}