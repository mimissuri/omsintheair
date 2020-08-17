import sys
import os

import globals as g

g.initialize()
import logs as log

import sockets as sc

import getdata as gd


def main():
    while True:
        # NOTE
        # Phase 0: Sleeping
        # Phase 1: Waiting, sending data at the same time via sockets.
        # Phase 2: Controlled by base
        # Phase 3: Doing his own ting
        if g.phase == 0:
            pass
        elif g.phase == 1:
            # Phase 1: Waiting, sending data at the same time via sockets.
            if g.connected:
                log.log("Running phase 0 while connected")
                data = gd.get()
                sc.emit("data", data)
            else:
                try:
                    sc.connect_server()
                    g.connected = True
                except:
                    g.connected = False

        elif g.phase == 2:
            # Phase 2: Controlled by base
            pass
        elif g.phase == 3:
            # Phase 3: Doing his own ting
            g.sio.disconnect()
            g.connected = False
        elif g.phase == 4:
            pass
        elif g.phase == 5:
            pass
        elif g.phase == 6:
            pass
    os.system("reboot now")


if __name__ == "__main__":
    log.log("Starting")
    try:
        sc.connect_server()
        g.connected = True
    except:
        g.connected = False
    main()
    # Initialize el q es necesiti
