import mpu_lib
import time
import sys

mpu9250 = mpu_lib.MPU9250()

c=100
acc_x=0
acc_y=0
acc_z=0
gryo_x=0
gryo_y=0
gryo_z=0
for d in range(c):
    accel = mpu9250.readAccel()
    acc_x+=accel['x']
    acc_y+=accel['y']
        acc_z+=accel['z']
        accel = mpu9250.readGyro()
    gryo_x+=accel['x']
    gryo_y+=accel['y']
    gryo_z+=accel['z']
    time.sleep(0.02)

acc_x=acc_x/c
acc_y=acc_y/c
acc_z=acc_z/c
gryo_x=gryo_x/c
gryo_y=gryo_y/c
gryo_z=gryo_z/c

def data():
    while True:
        accel = mpu9250.readAccel()
        gyro = mpu9250.readGyro()
        mag = mpu9250.readMagnet()
        acc = [round(accel['x']-acc_x,2),round(accel['y']-acc_y,2),round(accel['z']-acc_z,2)]
        gryo = [round(gyro['x']-gryo_x,2),round(gyro['y']-gryo_y,2),round(gyro['z']-gryo_z,2)]
        mag = [mag['x'],mag['y'],mag['z']]
        return acc+gryo+mag