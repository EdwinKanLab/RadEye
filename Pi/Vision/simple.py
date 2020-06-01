import cv2 as cv
import numpy as np
from matplotlib import pyplot as plt
img = cv.imread('Test_Images/V2/img1.jpeg',0)
thr = 240
ret,thresh1 = cv.threshold(img,thr,255,cv.THRESH_BINARY)
ret,thresh2 = cv.threshold(img,thr,255,cv.THRESH_BINARY_INV)
ret,thresh3 = cv.threshold(img,thr,255,cv.THRESH_TRUNC)
ret,thresh4 = cv.threshold(img,thr,255,cv.THRESH_TOZERO)
ret,thresh5 = cv.threshold(img,thr,255,cv.THRESH_TOZERO_INV)
titles = ['Original Image','BINARY','BINARY_INV','TRUNC','TOZERO','TOZERO_INV']
images = [img, thresh1, thresh2, thresh3, thresh4, thresh5]

thresh = thresh1
# contours, hierarchy = cv.findContours(thresh, cv.RETR_TREE, cv.CHAIN_APPROX_SIMPLE)
# print (contours)
# cv.drawContours(thresh, contours, -1, (0,255,0), 3)
cv.imshow("Contours", thresh)
cv.waitKey(0)
# im_with_keypoints = cv.drawKeypoints(im, keypoints, np.array([]), (0,0,255), cv2.DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS)
# plt.subplot(1,1,1),plt.imshow(thresh)
# plt.show()
# for i in range(6):
#     plt.subplot(2,3,i+1),plt.imshow(images[i],'gray')
#     plt.title(titles[i])
#     plt.xticks([]),plt.yticks([])
# plt.show()