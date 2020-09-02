from board import SCL, SDA
import busio

from adafruit_pca9685 import PCA9685

from adafruit_motor import servo

i2c = busio.I2C(SCL, SDA)

pca = PCA9685(i2c)
pca.frequency = 50
import time

minim = int(input("Minimum: "))
maxim = int(input("Maximum: "))
servos = servo.Servo(pca.channels[0], min_pulse=minim, max_pulse=maxim)

servos.angle = 0
time.sleep(2)
servos.angle = 180
time.sleep(2)
servos.angle = 0
print(servo)
pca.deinit()
