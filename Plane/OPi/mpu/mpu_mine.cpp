
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

#define REG_PWR_MGMT_1 0x6B
#define REG_GYRO_CONFIG 0x1B
#define REG_ACCEL_CONFIG 0x1C
#define REG_ACCEL_READINGS 0x3B

#define SELF_TEST_X 0x0D
#define SELF_TEST_Y 0x0E
#define SELF_TEST_Z 0x0F
#define SELF_TEST_A 0x10
#define SMPLRT_DIV 0x19
#define CONFIG 0x1A
#define GYRO_CONFIG 0x1B
#define ACCEL_CONFIG 0x1C
#define MOT_THR 0x1F
#define FIFO_EN 0x23
#define I2C_MST_CTRL 0x24
#define I2C_SLV0_ADDR 0x25
#define I2C_SLV0_REG 0x26
#define I2C_SLV0_CTRL 0x27
#define I2C_SLV1_ADDR 0x28
#define I2C_SLV1_REG 0x29
#define I2C_SLV1_CTRL 0x2A
#define I2C_SLV2_ADDR 0x2B
#define I2C_SLV2_REG 0x2C
#define I2C_SLV2_CTRL 0x2D
#define I2C_SLV3_ADDR 0x2E
#define I2C_SLV3_REG 0x2F
#define I2C_SLV3_CTRL 0x30
#define I2C_SLV4_ADDR 0x31
#define I2C_SLV4_REG 0x32
#define I2C_SLV4_DO 0x33
#define I2C_SLV4_CTRL 0x34
#define I2C_SLV4_DI 0x35
#define I2C_MST_STATUS 0x36
#define INT_PIN_CFG 0x37
#define INT_ENABLE 0x38
#define INT_STATUS 0x3A
#define ACCEL_XOUT_H 0x3B
#define ACCEL_XOUT_L 0x3C
#define ACCEL_YOUT_H 0x3D
#define ACCEL_YOUT_L 0x3E
#define ACCEL_ZOUT_H 0x3F
#define ACCEL_ZOUT_L 0x40
#define TEMP_OUT_H 0x41
#define TEMP_OUT_L 0x42
#define GYRO_XOUT_H 0x43
#define GYRO_XOUT_L 0x44
#define GYRO_YOUT_H 0x45
#define GYRO_YOUT_L 0x46
#define GYRO_ZOUT_H 0x47
#define GYRO_ZOUT_L 0x48
#define I2C_SLV0_DO 0x63
#define I2C_SLV1_DO 0x64
#define I2C_SLV2_DO 0x65
#define I2C_SLV3_DO 0x66
#define I2C_MST_DELAY_CTRL 0x67
//#define SIGNAL_PATH_RESET 0x68
#define MOT_DETECT_CTRL 0x69
#define USER_CTRL 0x6A
#define PWR_MGMT_1 0x6B
#define PWR_MGMT_2 0x6C
#define FIFO_COUNTH 0x72
#define FIFO_COUNTL 0x73
#define FIFO_R_W 0x74
#define WHO_AM_I 0x75
#define MPU6050_DEV_ADD 0x68
#define AFS_SEL 0x1C
#define FS_SEL 0x1B
#define TEMP_FIFO_EN_BIT 7
#define XG_FIFO_EN_BIT 6
#define YG_FIFO_EN_BIT 5
#define ZG_FIFO_EN_BIT 4
#define ACCEL_FIFO_EN_BIT 3
#define SLV2_FIFO_EN_BIT 2
#define SLV1_FIFO_EN_BIT 1
#define SLV0_FIFO_EN_BIT 0
#define DEV_RESET_BIT 7
#define FIFO_EN_BIT 6
#define FIFO_RESET_BIT 2

using namespace std;

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
    float accX;
    float accY;
    float accZ;
    double pitch = 0.00f;
    double roll = 0.00f;
    double yaw = 0.00f;
    double off_pitch = 0.00f;
    double off_roll = 0.00f;
    double off_yaw = 0.00f;
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
    imu mpu1(0x68);
    mpu1.init();
    mpu1.read_process();
    mpu1.process_offseat();
    while (true)
    {
        mpu1.read_process();

        cout << mpu1.accX << "\t" << mpu1.accY << "\t" << mpu1.accZ << "\n";
        cout << "PITCH"
             << "\t"
             << "ROLL"
             << "\t"
             << "YAW"
             << "\n";
        cout << mpu1.pitch << "\t" << mpu1.roll << "\t" << mpu1.yaw << "\n";
        usleep(100000);
    }

    return 0;
}