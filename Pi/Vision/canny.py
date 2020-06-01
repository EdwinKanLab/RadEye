import numpy as np
import cv2 as cv
from matplotlib import pyplot as plt
img = cv.imread('Test_Images/V2/img1.jpeg',0)
# blur = cv.GaussianBlur(img,(5,5),0)
img = cv.medianBlur(img,5)
# ret3,th3 = cv.threshold(blur,0,255,cv.THRESH_BINARY+cv.THRESH_OTSU)
th3 = cv.adaptiveThreshold(img,255,cv.ADAPTIVE_THRESH_MEAN_C, cv.THRESH_BINARY,11,2)
edges = cv.Canny(th3,0,100)

plt.subplot(121),plt.imshow(img,cmap = 'gray')
plt.title('Original Image'), plt.xticks([]), plt.yticks([])
plt.subplot(122),plt.imshow(edges,cmap = 'gray')
plt.title('Edge Image'), plt.xticks([]), plt.yticks([])
plt.show()