## 打开摄像头并且显示图像
import cv2

clicked = False
def onMouse(evt, x, y, flags, param):
    global clicked
    if evt == cv2.EVENT_LBUTTONUP:
        clicked = True

cameraCapture = cv2.VideoCapture(0)
cv2.namedWindow('voide')
cv2.setMouseCallback('voide', onMouse)

print 'show camera feed, Click window or pass q key to stop.'
success, frame = cameraCapture.read()
while success and cv2.waitKey(50) != ord('q') and not clicked:
    cv2.imshow('voide', frame)
    success, frame = cameraCapture.read()

cv2.destroyWindow('voide')
cameraCapture.release();
