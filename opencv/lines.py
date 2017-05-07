import cv2
import numpy as np

def minLineLengthChange(p):
    print 'min change',p
    minLineLength = p
    show()

def maxLineGapChange(p):
    print 'min change',p
    maxLineGap= p
    show()

def show():
    lines = cv2.HoughLinesP(edges,1,np.pi/180,
            100,
            minLineLength,
            maxLineGap)
    for line in lines:
        for x1,y1,x2,y2 in line:
          cv2.line(img,(x1,y1),(x2,y2),(0,255,0),2)

    cv2.imshow("lines", img)

img = cv2.imread('lines3.png')
gray = cv2.cvtColor(img,cv2.COLOR_BGR2GRAY)
edges = cv2.Canny(gray,40,220)

minLineLength = 40
maxLineGap = 50


cv2.namedWindow('lines')
cv2.createTrackbar('ttmin', 'lines', minLineLength, 1000, minLineLengthChange)
cv2.createTrackbar('ttmax', 'lines', maxLineGap, 1000, maxLineGapChange)

show()

cv2.imshow("edges", edges)
cv2.waitKey()
