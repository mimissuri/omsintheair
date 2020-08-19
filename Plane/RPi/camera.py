import cv2
import globals as g
import logs as log

from datetime import datetime

if not g.camera.isOpened():
    g.camera = False


def save_picture():
    ret, frame = g.camera.read()
    if ret == True:
        now = datetime.now().strftime("%d.%m.%Y_%H.%M.%S")
        cv2.imwrite(g.path + "/pictures/" + now + ".jpg", frame)
    else:
        g.camera.release()
        g.camera = cv2.VideoCapture(0)
