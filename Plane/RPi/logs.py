import globals as g
import sockets as sc


def log(data):
    if g.debug:
        print(data)
    else:
        f = open(g.path + "/log/" + g.datetime + ".txt", "a")
        f.write(data + "\n")
        f.close()
    sc.emit("log", data)
