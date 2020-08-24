import cv2
import globals as g
import logs as log
import sockets as sc

from datetime import datetime

import base64


def save_picture():
    ret, frame = g.camera.read()
    if ret == True:
        now = datetime.now().strftime("%d.%m.%Y_%H.%M.%S")
        cv2.imwrite(g.path + "/pictures/" + now + ".jpg", frame)
    else:
        g.camera.release()
        g.camera = cv2.VideoCapture(0)


def stream_video():
    g.stream_video = True
    while g.stream_video:
        ret, frame = g.camera.read()
        if ret:
            retval, frame = cv2.imencode(".jpg", frame)
            sc.emit("video_stream", base64.b64encode(frame).decode("utf-8"))
        else:
            g.camera.release()
            g.camera = cv2.VideoCapture(0)

