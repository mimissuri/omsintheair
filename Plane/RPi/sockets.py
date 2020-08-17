import time
import socketio

import globals as g
import logs as log

g.sio = socketio.Client()


@g.sio.event
def connect():
    log.log("Connected to server")
    g.connected = True


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


def connect_server():
    g.sio.connect(g.sip)


def emit(rid, rarg):
    if g.connected:
        log.log("Emiting on " + rid + " with values: " + str(rarg))
        g.sio.emit(rid, rarg)
    else:
        return False
