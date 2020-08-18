from datetime import datetime
import socketio

now = datetime.now().strftime("%d.%m.%Y_%H.%M.%S")
import os

dir_path = os.path.dirname(os.path.realpath(__file__))


def initialize():
    global debug, datetime, path, connected, phase, sip, sio, ndata
    debug = True
    datetime = now
    path = dir_path
    connected = False
    phase = 0
    sip = "http://192.168.1.200:3000"
    ndata = False
    sio = socketio.Client()
