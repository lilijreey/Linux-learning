# -*- coding: utf-8  -*-
import cv2
import numpy as np
import filters
from scipy import ndimage

im = cv2.imread('scr.png')

### Canny 边缘检测算法
cv2.imshow('canny', cv2.Canny(im, 200,300))
cv2.waitKey()

## 线性模糊 medianBlur
#imx55 = filters.strokeEdges(im,5,5)
#imx57 = filters.strokeEdges(im,5,7)
#cv2.imshow('imx55', imx55)
#cv2.imshow('imx57', imx57)
#im5 = cv2.medianBlur(im, 5)
#im9 = cv2.medianBlur(im, 9)
#cv2.imshow('im5', im5)
#cv2.imshow('im9', im9)

cv2.waitKey()



#kernel_3x3 = np.array([[-1, -1, -1], [-1,  8, -1], [-1, -1, -1]])
#kernel_5x5 = np.array([[-1, -1, -1, -1, -1],
#                       [-1,  1,  2,  1, -1],
#                       [-1,  2,  4,  2, -1],
#                       [-1,  1,  2,  1, -1],
#                       [-1, -1, -1, -1, -1]])
#kernel_7x7 = np.array([[-1, -1, -1, -1, -1,-1,-1],
#                       [-1, -1, -1,  2, -1,-1,-1],
#                       [-1,  1,  2,  4,  2,-1,-1],
#                       [-1,  2,  4,  -1,  4, 2,-1],
#                       [-1,  1,  2,  4,  2,-1,-1],
#                       [-1, -1,  1,  2, -1,-1,-1],
#                       [-1, -1, -1, -1, -1,-1,-1]])

## 模糊
#kernel_mh = np.array([[0.04, 0.04,0.04,0.04,0.04],
#                      [0.04, 0.04,0.04,0.04,0.04],
#                      [0.04, 0.04,0.04,0.04,0.04],
#                      [0.04, 0.04,0.04,0.04,0.04],
#                      [0.04, 0.04,0.04,0.04,0.04]])


#img = cv2.imread("/tmp/z.png", 0)

#k3 = ndimage.convolve(img, kernel_3x3)
#k5 = ndimage.convolve(img, kernel_5x5)
#k7 = ndimage.convolve(img, kernel_7x7)
#kmh = ndimage.convolve(img, kernel_mh)

#blurred = cv2.GaussianBlur(img, (5,5), 0)
#g_hpf = img - blurred

#cv2.imshow("org", img)
#cv2.imshow("3x3", k3)
#cv2.imshow("5x5", k5)
#cv2.imshow("7x7", k7)
#cv2.imshow("g_hpf", g_hpf)
#cv2.imshow("kmh", kmh)
#cv2.waitKey()


