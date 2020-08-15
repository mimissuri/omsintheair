import sys
import os

import globals as g

g.initialize()
import logs as log

import sockets as sc


def main():
    while True:
        sc.emit("login", {"foo": "bar"})
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
