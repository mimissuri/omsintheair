from datetime import datetime
import socketio
import cv2

now = datetime.now().strftime("%d.%m.%Y_%H.%M.%S")
import os

dir_path = os.path.dirname(os.path.realpath(__file__))


def initialize():
    global debug, datetime, path, connected, phase, sip, sio, ndata, ports, lorap, camera, stream_video
    debug = True
    datetime = now
    path = dir_path
    connected = False
    phase = 0
    sip = "http://192.168.1.200:3000"
    ndata = False
    sio = socketio.Client()
    ports = False
    lorap = False
    camera = cv2.VideoCapture(0)
    stream_video = False

