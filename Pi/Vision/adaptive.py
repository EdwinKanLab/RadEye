import cv2 as cv
import numpy as np
from matplotlib import pyplot as plt
img = cv.imread('Test_Images/V2/img1.jpeg',0)
# img = cv.medianBlur(img,5)
kernel = np.ones((10,10),np.float32)/100
img = cv.filter2D(img, -1, kernel)
ret,th1 = cv.threshold(img,127,255,cv.THRESH_BINARY)
th2 = cv.adaptiveThreshold(img,255,cv.ADAPTIVE_THRESH_MEAN_C, cv.THRESH_BINARY,11,2)
th3 = cv.adaptiveThreshold(img,255,cv.ADAPTIVE_THRESH_GAUSSIAN_C, cv.THRESH_BINARY,11,2)
titles = ['Original Image', 'Global Thresholding (v = 127)',
            'Adaptive Mean Thresholding', 'Adaptive Gaussian Thresholding']
images = [img, th1, th2, th3]
for i in range(4):
    plt.subplot(2,2,i+1),plt.imshow(images[i],'gray')
    plt.title(titles[i])
    plt.xticks([]),plt.yticks([])
plt.show()


# import cv2 as cv
# import numpy as np
# from matplotlib import pyplot as plt
# img = cv.imread('Test_Images/Preliminary/img1.jpeg',0)
# img = cv.medianBlur(img,5)
# # ret,th1 = cv.threshold(img,127,255,cv.THRESH_BINARY)
# # th2 = cv.adaptiveThreshold(img,255,cv.ADAPTIVE_THRESH_MEAN_C, cv.THRESH_BINARY,11,2)
# # th3 = cv.adaptiveThreshold(img,255,cv.ADAPTIVE_THRESH_GAUSSIAN_C, cv.THRESH_BINARY,11,2)
# # titles = ['Original Image', 'Global Thresholding (v = 127)']
# # images = [img, th1]
# # for i in range(2):
# #     plt.subplot(2,2,i+1),plt.imshow(images[i],'gray')
# #     plt.title(titles[i])
# #     plt.xticks([]),plt.yticks([])

# plt.subplot(1,1,1),plt.imshow(img,'gray')
# plt.show()