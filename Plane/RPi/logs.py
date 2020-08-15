import globals as g


def log(data):
    if g.debug:
        print(data)
    else:
        f = open(g.path + "/log/" + g.datetime + ".txt", "a")
        f.write(data + "\n")
        f.close()
