import cv2
import numpy as np

img = cv2.pyrDown(cv2.imread('yy.png', cv2.IMREAD_UNCHANGED))

ret, thtresh = cv2.threshold(cv2.cvtColor(img.copy(), cv2.COLOR_BGR2GRAY), 127, 255, cv2.THRESH_BINARY)
im, contours,hier = cv2.findContours(thtresh, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)


for c in contours:
    print 'c:', c.size
    if c.size < 200:
        continue

#    #find bounding box coordinates
    x,y,w,h = cv2.boundingRect(c)
    cv2.rectangle(img, (x,y), (x+w, y+h),(0,255,0), 2)

#    #find minmum area
    rect = cv2.minAreaRect(c)
    box = cv2.boxPoints(rect)
    box = np.int0(box)
    cv2.drawContours(img, [box], 0, (0,0,255),3)#red color
    #print c
    #print rect
    #print box

    (x, y), radius = cv2.minEnclosingCircle(c)
    center = (int(x), int(y))
    radius = int(radius)
    img =cv2.circle(img, center,radius, (0,255,0),2)

cv2.drawContours(img, contours, -1, (255,0,0),1)
cv2.imshow('xx', img)
cv2.waitKey();
