from datetime import datetime

now = datetime.now().strftime("%d.%m.%Y_%H.%M.%S")
import os

dir_path = os.path.dirname(os.path.realpath(__file__))


def initialize():
    global debug, datetime, path, connected, sip, sio
    debug = True
    datetime = now
    path = dir_path
    connected = False
    s_dispatch = False
    sip = "http://192.168.1.200:3000"
