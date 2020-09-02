#include <iostream>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>

#include <cmath>
extern "C"
{
#include <linux/i2c-dev.h>
#include <i2c/smbus.h>
}
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
#define WHO_AM_I 0x75

using namespace std;

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
        int ans = i2c_smbus_read_block_data(file, AK8963_ASAX, buf);
        magXc = (buf[0] - 128) / 256.0 + 1.0;
        magYc = (buf[1] - 128) / 256.0 + 1.0;
        magZc = (buf[2] - 128) / 256.0 + 1.0;
        cout << "Coefficients:"
             << "\t" << magXc << "\t" << magYc << "\t" << magZc << "\n";
        i2c_write(file, AK8963_CNTL1, 0x00);
        usleep(100000);
        i2c_write(file, AK8963_CNTL1, 0x12);
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
    uint16_t dataConv(uint8_t data1, uint8_t data2)
    {
        uint16_t value = data1 | (data2 << 8);
        if (value & (1 << 16 - 1))
        {
            value -= (1 << 16);
        }
        return value;
    }
    void read_raw()
    {
        magX = 0;
        magY = 0;
        magY = 0;
        int patata = i2c_read(file, AK8963_ST1);
        cout << "Status: " << patata << "\n";
        if (patata == 1 || patata == 3)
        {
            uint8_t buf[7];
            buf[0] = i2c_read(file, 0x03);
            buf[1] = i2c_read(file, 0x04);
            buf[2] = i2c_read(file, 0x05);
            buf[3] = i2c_read(file, 0x06);
            buf[4] = i2c_read(file, 0x07);
            buf[5] = i2c_read(file, 0x08);
            buf[6] = i2c_read(file, 0x09);

            magX = dataConv(buf[0], buf[1]);
            magY = dataConv(buf[2], buf[3]);
            magZ = dataConv(buf[4], buf[5]);

            magX = magX * mres * magXc;
            magY = magY * mres * magYc;
            magZ = magZ * mres * magZc;
            cout << "MagX"
                 << "\t"
                 << "MagY"
                 << "\t"
                 << "MagZ"
                 << "\n";
            cout << magX << "\t" << magY << "\t" << magZ << "\n";
            cout << "------------------------- \n";
        }
        uint8_t buf2[2];
        buf2[0] = i2c_read(mpu_file, 0x43);
        buf2[1] = i2c_read(mpu_file, 0x44);
        int accX = dataConv(buf2[0], buf2[1]);
        cout << "Acc X:" << accX / 16384.00 << "\n";
    }
};

int main()
{
    printf("Bon dia\n");
    mag mag1(0);
    mag1.init();
    cout << "Done"
         << "\n";
    while (true)
    {
        mag1.read_raw();
        usleep(100000);
    }
    return 0;
}