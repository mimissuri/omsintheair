import time

import globals as g
import logs as log
import camera as cam


@g.sio.event
def connect():
    log.log("Connected to server")
    g.connected = True
    emit("login", "plane")


@g.sio.event
def connect_error(error):
    log.log("Error: " + error)


@g.sio.event
def disconnect():
    log.log("Disconnected from server")
    g.connected = False


@g.sio.on("phase")
def on_message(data):
    g.phase = int(data)
    log.log("Changed to phase: " + str(g.phase))


@g.sio.on("command")
def on_message(data):
    try:
        globals()[data]()
    except:
        if data == "save_picture":
            cam.save_picture()
        elif data == "stream_video":
            cam.stream_video()
        elif data == "stop_stream":
            g.stream_video = False
        else:
            log.log("Couldn't find: " + data)


def connect_server():
    g.sio.connect(g.sip)


def emit(rid, rarg):
    if g.connected:
        g.sio.emit(rid, rarg)
