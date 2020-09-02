
#include <iostream>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>

#include <linux/i2c-dev.h>
#include <i2c/smbus.h>
#include "MadgwickAHRS.h"
#include <cmath>

using namespace std;

class pca
{

public:
    int addr;
    int bus;
    bool ready;
    int file;
    char *filename = "/dev/i2c-0";
    imu(int a) : addr(a)
    {
    }

    int init()
    {
        // Opening port
        file = open(filename, O_RDWR); //Setting all to 0
        if (ioctl(file, I2C_SLAVE, addr) < 0)
        {
            exit(1);
        }
        // Config
        i2c_write(REG_PWR_MGMT_1, 0x00);
        i2c_write(REG_GYRO_CONFIG, 0x01);
        i2c_write(REG_ACCEL_CONFIG, 0x01);
        ready = true;
    }

    void i2c_write(__u8 reg_address, __u8 val)
    {
        char buf[2];

        buf[0] = reg_address;
        buf[1] = val;

        if (write(file, buf, 2) != 2)
        {
            printf("Error, unable to write to i2c device\n");
            exit(1);
        }
    }

    char i2c_read(uint8_t reg_address)
    {
        char buf[1];
        buf[0] = reg_address;

        if (write(file, buf, 1) != 1)
        {
            printf("Error, unable to write to i2c device\n");
            exit(1);
        }

        if (read(file, buf, 1) != 1)
        {
            printf("Error, unable to read from i2c device\n");
            exit(1);
        }

        return buf[0];
    }

    int16_t i2c_readword(uint8_t MSB, uint8_t LSB)
    {

        uint8_t msb = i2c_read(MSB);

        uint8_t lsb = i2c_read(LSB);

        return ((int16_t)msb << 8) + lsb;
    }
    void process_offseat()
    {
        float roll2 = 0;
        float pitch2 = 0;
        float yaw2 = 0;
        for (int i = 0; i != 100; i++)
        {
            read_process();
            roll2 += roll;
            pitch2 += pitch;
            yaw2 += yaw;
        }
        off_pitch = pitch2 / 100;
        off_yaw = yaw2 / 100;
        off_roll = roll2 / 100;
    }
    void read_process()
    {
        read_accel();
        read_gyro();
        pitch = 180.00 * atan(accX / sqrt(accY * accY + accZ * accZ)) / M_PI - off_pitch;
        roll = 180.00 * atan(accY / sqrt(accX * accX + accZ * accZ)) / M_PI - off_roll;
        yaw = 180.00 * atan(accZ / sqrt(accX * accX + accZ * accZ)) / M_PI - off_yaw;
    }
    void read_accel()
    {
        float accel_Z = (int16_t)i2c_readword(ACCEL_ZOUT_H, ACCEL_ZOUT_L) / 8192.00;
        float accel_Y = (int16_t)i2c_readword(ACCEL_YOUT_H, ACCEL_YOUT_L) / 8192.00;
        float accel_X = (int16_t)i2c_readword(ACCEL_XOUT_H, ACCEL_XOUT_L) / 8192.00;
        accZ = accel_Z;
        accY = accel_Y;
        accX = accel_X;
    }
    void read_gyro()
    {
        float gyro_Z = (int16_t)i2c_readword(GYRO_ZOUT_H, GYRO_ZOUT_L) / 65.50;
        float gyro_Y = (int16_t)i2c_readword(GYRO_YOUT_H, GYRO_YOUT_L) / 65.50;
        float gyro_X = (int16_t)i2c_readword(GYRO_XOUT_H, GYRO_XOUT_L) / 65.50;
        gyroZ = gyro_Z;
        gyroY = gyro_Y;
        gyroX = gyro_X;
    }
};

int main()
{
    printf("Bon dia\n");
    pca pca(0x68);
    return 0;
}