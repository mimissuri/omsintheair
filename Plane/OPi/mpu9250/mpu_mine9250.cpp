
#include <iostream>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>

#include <linux/kernel.h>
#include <linux/module.h>
#include <cmath>
extern "C"
{
#include <linux/i2c-dev.h>
#include <i2c/smbus.h>
}
#define SLAVE_ADDRESS 0x68
#define AK8963_SLAVE_ADDRESS 0x0C
#define DEVICE_ID 0x71
#define SMPLRT_DIV 0x19
#define CONFIG 0x1A
#define GYRO_CONFIG 0x1B
#define ACCEL_CONFIG 0x1C
#define ACCEL_CONFIG_2 0x1D
#define LP_ACCEL_ODR 0x1E WOM_THR 0x1F
#define FIFO_EN 0x23
#define I2C_MST_CTRL 0x24
#define I2C_MST_STATUS 0x36
#define INT_PIN_CFG 0x37
#define INT_ENABLE 0x38
#define INT_STATUS 0x3A
#define ACCEL_OUT 0x3B
#define TEMP_OUT 0x41
#define GYRO_OUT 0x43
#define I2C_MST_DELAY_CTRL 0x67
#define SIGNAL_PATH_RESET 0x68
#define MOT_DETECT_CTRL 0x69
#define USER_CTRL 0x6A
#define PWR_MGMT_1 0x6B
#define CLOCK_SEL_PLL 0x01
#define PWR_MGMT_2 0x6C
#define FIFO_R_W 0x74
#define WHO_AM_I 0x75
#define GFS_250 0x00
#define AFS_2G 0x00
#define AK8963_ST1 0x02
#define AK8963_MAGNET_OUT 0x03
#define AK8963_CNTL1 0x0A
#define AK8963_CNTL2 0x0B
#define AK8963_ASAX 0x10
#define AK8963_ASAY 0x11
#define AK8963_ASAZ 0x12
#define AK8963_MODE_DOWN 0x00
#define AK8963_MODE_ONE 0x01
#define AK8963_MODE_C8HZ 0x02
#define AK8963_MODE_C100HZ 0x06
#define AK8963_BIT_14 0x00
#define AK8963_BIT_16 0x01
#define REG_ACCEL_XOUT_H 0x3B
#define REG_ACCEL_XOUT_L 0x3C
#define REG_ACCEL_YOUT_H 0x3D
#define REG_ACCEL_YOUT_L 0x3E
#define REG_ACCEL_ZOUT_H 0x3F
#define REG_ACCEL_ZOUT_L 0x40
#define REG_TEMP_OUT_H 0x41
#define REG_TEMP_OUT_L 0x42
#define REG_GYRO_XOUT_H 0x43
#define REG_GYRO_XOUT_L 0x44
#define REG_GYRO_YOUT_H 0x45
#define REG_GYRO_YOUT_L 0x46
#define REG_GYRO_ZOUT_H 0x47
#define REG_GYRO_ZOUT_L 0x48

using namespace std;

class imu
{

public:
    int addr;
    bool ready;
    int mpu_file;
    int mag_file;
    char *mpu_filename = "/dev/i2c-0";
    char mag_filename[250];
    float gyroX;
    float gyroY;
    float gyroZ;
    float accX;
    float accY;
    float accZ;
    float pitch = 0.00f;
    float roll = 0.00f;
    float yaw = 0.00f;
    float magXc;
    float magYc;
    int adapter_nr = 2;
    float magZc;

    imu(int a) : addr(a)
    {
    }

    int init()
    {
        /*snprintf(mpu_filename, 250, "/dev/i2c-0");*/
        mpu_file = open(mpu_filename, O_RDWR);
        if (ioctl(mpu_file, I2C_SLAVE, addr) < 0)
        {
            exit(1);
        }
        /*
        i2c_write(mpu_file, PWR_MGMT_1, 0x00);*/
        i2c_write(mpu_file, GYRO_CONFIG, 0x00);
        i2c_write(mpu_file, ACCEL_CONFIG, 0x00);
        usleep(1000000);
        ready = true;
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
    void read_process()
    {
        read_raw();
    }
    void read_raw()
    {
        uint8_t data[6];
        __u8 xd = 0x3B;
        int vull_morir = i2c_smbus_read_block_data(mpu_file, 0x43, data);
        size_t n = sizeof(data) / sizeof(data[0]);
        float accel_Z = (int16_t)i2c_readword(mpu_file, REG_ACCEL_ZOUT_H, REG_ACCEL_ZOUT_L) / 16384.00;
        float accel_Y = (int16_t)i2c_readword(mpu_file, REG_ACCEL_YOUT_H, REG_ACCEL_YOUT_L) / 16384.00;
        float accel_X = (int16_t)i2c_readword(mpu_file, REG_ACCEL_XOUT_H, REG_ACCEL_XOUT_L) / 16384.00;
        accZ = accel_Z;
        accY = accel_Y;
        accX = accel_X;
        float gyro_Z = (int16_t)i2c_readword(mpu_file, REG_GYRO_ZOUT_H, REG_GYRO_ZOUT_L) / 65.500;
        float gyro_Y = (int16_t)i2c_readword(mpu_file, REG_GYRO_YOUT_H, REG_GYRO_YOUT_L) / 65.500;
        float gyro_X = (int16_t)i2c_readword(mpu_file, REG_GYRO_XOUT_H, REG_GYRO_XOUT_L) / 65.500;
        cout << gyro_X << "\t" << gyro_Y << "\t" << gyro_Z << "\n";

        // loop through the elements of the array
        for (size_t i = 0; i < n / 2; i++)
        {
            cout << dataConv(data[i * 2], data[i * 2 + 1]) << ' ';
        }
        cout << "\n \n";
    }
    uint16_t dataConv(uint8_t data1, uint8_t data2)
    {
        uint16_t value = data1 | (data2 << 8);
        if (value & (1 << 16 - 1))
        {
            value -= (1 << 16);
        }
        return value;
    }
};

int main()
{
    printf("Bon dia\n");
    imu mpu1(0x68);
    mpu1.init();
    while (true)
    {
        mpu1.read_process();
        cout << "PITCH"
             << "\t"
             << "ROLL"
             << "\t"
             << "YAW"
             << "\n";
        cout << mpu1.accX << "\t" << mpu1.accY << "\t" << mpu1.accZ << "\n";
        usleep(100000);
    }
    return 0;
}