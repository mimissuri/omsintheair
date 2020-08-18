import globals as g
import logs as log
import serial
from serial.tools import list_ports


def scan():
    available = list_ports.comports()
    log.log(str(available))

    return available


def setports():
    ports = scan()
    for port in ports:
        if port[1][:19] == "Silicon Labs CP210x":
            g.lorap = serial.Serial(port[0], 115200)


def read_lora():
    if g.lorap.readable():
        return g.lorap.read_until()
    else:
        return False
