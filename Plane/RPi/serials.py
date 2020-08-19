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
    if g.lorap.in_waiting > 0:
        data = g.lorap.read_until(b";")
        return data
    else:
        return False


def write_lora(data):
    g.lorap.write(data.encode("UTF-8"))
