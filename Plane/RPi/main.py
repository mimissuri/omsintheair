import sys
import os

import globals as g

g.initialize()
import logs as log

import sockets as sc

import getdata as gd

import serials as s


def main():
    while True:
        # NOTE
        # Phase 0: Sleepin  g
        # Phase 1: Waiting, sending data at the same time via sockets.
        # Phase 2: Controlled by base
        # Phase 3: Waiting, sending data at the same time via LoRa.
        if g.phase == 0:
            pass
        elif g.phase == 1:
            # Phase 1: Waiting, sending data at the same time via sockets.
            if g.connected:
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
            # Phase 3: Waiting, sending data at the same time via LoRa.
            if g.connected:
                g.sio.disconnect()
                g.connected = False
                data = s.read_lora()
                if data != False:
                    pass
        elif g.phase == 4:
            pass
        elif g.phase == 5:
            pass
        elif g.phase == 6:
            pass
        elif g.phase == 99:
            break
    os.system("reboot now")


if __name__ == "__main__":
    log.log("Starting")
    try:
        sc.connect_server()
        g.connected = True
    except:
        g.connected = False
    while True:
        g.ports = s.scan()
        if len(g.ports) < 2:
            log.log("Just " + str(len(g.ports)) + " available")
            sc.emit("data", "Just " + str(len(g.ports)) + " available")
        else:
            log.log(str(len(g.ports)) + " available")
            sc.emit("data", str(len(g.ports)) + " available")
            s.setports()
            if g.lorap != False:
                log.log("LoRa at " + g.lorap)
                sc.emit("data", "LoRa at " + g.lorap)
                break
            else:
                log.log("Lora not found")
                sc.emit("data", "LoRa not found")

        if not g.connected:
            try:
                sc.connect_server()
                g.connected = True
            except:
                g.connected = False

    # Initialize el q es necesiti
    main()
