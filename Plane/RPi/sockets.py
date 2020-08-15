import asyncio
import time
import socketio

import globals as g
import logs as log

g.sio = socketio.Client()


@g.sio.event
def connect():
    log.log("Connected to server")


@g.sio.event
def connect_error(error):
    log.log("Error: " + error)


@g.sio.event
def disconnect():
    log.log("Disconnected from server")
    g.connected = False


@g.sio.on("login")
def on_message(data):
    log.log(data)


def connect_server():
    g.sio.connect(g.sip)


def emit(id, arg):
    if g.connected:
        g.sio.emit(id, arg)
    else:
        return False
