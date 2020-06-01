import cv2 as cv
import numpy as np
from matplotlib import pyplot as plt

#Image Read
image = cv.imread('Test_Images/V2/img2.jpeg')

h_pix, w_pix, _ = image.shape


# h_border_mm, w_border_mm = 
# screen_h_mm, screen_w_mm = 210 - (h_border_mm*2), 297 - (w_border_mm*2)

screen_h_mm, screen_w_mm = 181, 268


#Image Crop

#Tune these parameters
# ratio_topl = [0.05, 0.01]
ratio_topl = [0.00, 0.00]
ratio_botr = [0.95, 0.95]

topl = [int(h_pix*ratio_topl[0]), int(w_pix*ratio_topl[1])]

botr = [int(h_pix*ratio_botr[0]),int(w_pix*ratio_botr[1])]

cropped_image = image[topl[0]:botr[0], topl[1]:botr[1]]

cv.imshow("Cropped", cropped_image)
# cv.waitKey(0)

#Convert to Gray Scale
gray = cv.cvtColor(cropped_image,cv.COLOR_BGR2GRAY)
cv.imshow("Gray + Cropped", gray)
# cv.waitKey(0)

#Thresholding
kernel = np.ones((10,10),np.float32)/100
img = cv.filter2D(gray, -1, kernel)
# ret,th1 = cv.threshold(img,127,255,cv.THRESH_BINARY)
adaptive_th = cv.adaptiveThreshold(img,255,cv.ADAPTIVE_THRESH_MEAN_C, cv.THRESH_BINARY,11,2)
cv.imshow("Adaptive Mean Threshold", adaptive_th)


#Blob Detector
#Tune blob_thresh_min and blob_thresh_max
blob_thresh_min = 250
blob_thresh_max = 255
ret,thresh1 = cv.threshold(gray, blob_thresh_min, blob_thresh_max, cv.THRESH_BINARY)
cv.imshow("Blob Threshold", thresh1)

# cv.waitKey(0)

params = cv.SimpleBlobDetector_Params()

# Change thresholds
params.minThreshold = 200
params.maxThreshold = 255

# Filter by Area.
params.filterByArea = False
params.minArea = 50
params.maxArea = 100

# Filter by Circularity
params.filterByCircularity = False
params.minCircularity = 0.1

# Filter by Convexity
params.filterByConvexity = False
params.minConvexity = 0.0

# Filter by Inertia (ratio of widest to thinnest point)
params.filterByInertia = True
params.maxInertiaRatio = 1
params.minInertiaRatio = 0

params.filterByColor = False



# Create a detector with the parameters
detector = cv.SimpleBlobDetector_create(params)


keypoints = detector.detect(thresh1)
pointer = keypoints[0]

# # Draw detected blobs as red circles.
im_with_keypoints = cv.drawKeypoints(cropped_image, keypoints, np.array([]), (0,0,255), cv.DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS)

# print (im_with_keypoints[int(pointer.pt[0])][int(pointer.pt[1])].shape)

# print (np.array([0,0,0]).shape)

# for i in range(0,100):
# 	im_with_keypoints[int(pointer.pt[1])+i][int(pointer.pt[0])] = np.array([0,0,0])

# print (keypoint.pt)

cv.imshow("Keypoints", im_with_keypoints)

#Shi-Thomas
# blur = cv.GaussianBlur(gray,(5,5),0)
# ret3,th3 = cv.threshold(gray,0,255,cv.THRESH_BINARY+cv.THRESH_OTSU)

# h_pix, w_pix, _ = cropped_image.shape

# box_dim = [100, 100]
# topl_offset = [70,0]
# topr_offset = [0, w_pix - 200]
# botr_offset = [h_pix-240, w_pix-100]
# botl_offset = [h_pix-100, 0]

# def getBoxRange(box_dim, offset):
# 		return (slice(offset[0],box_dim[0]+offset[0]), slice(offset[1], box_dim[1]+offset[1]))

# topl_square = gray[getBoxRange(box_dim,topl_offset)]
# topr_square = gray[getBoxRange(box_dim,topr_offset)]
# botr_square = gray[getBoxRange(box_dim,botr_offset)]
# botl_square = gray[getBoxRange(box_dim,botl_offset)]
# topr_square = cropped_image[:box_dim, -1*box_w:]
# botr_square = cropped_image[-1*box_h:, -1*box_w:]
# botl_square = cropped_image[-1*box_h:, :box_w]

# cv.imshow("topl", topl_square)
# cv.imshow("topr", topr_square)
# cv.imshow("botr", botr_square)
# cv.imshow("botl", botl_square)

# corners = cv.goodFeaturesToTrack(gray,4,0.1,100)
# corners = np.int0(corners)

# corner_topl = cv.goodFeaturesToTrack(topl_square,4,0.1,100)
# corners = []
# np.int0(corner_topl)

# for i in corners:
#     x,y = i.ravel()
#     cv.circle(topl_square,(x,y),3,255,-1)

# plt.imshow(img),plt.show()
# cv.imshow("Corners", topl_square)
# corners = cv.goodFeaturesToTrack(gray,4,0.1,100)
# corners = np.int0(corners)

h_pix, w_pix, _ = cropped_image.shape
h_ratio = 0.20
w_ratio = 0.10
box_dim = [int(h_pix*h_ratio), int(w_pix*w_ratio)]

topl_box = gray[:box_dim[0], :box_dim[1]]
topr_box = gray[:box_dim[0], -1*box_dim[1]:]
botr_box = gray[-1*box_dim[0]:, -1*box_dim[1]:]
botl_box = gray[-1*box_dim[0]:, :box_dim[1]]

# corners = cv.goodFeaturesToTrack(gray,5,0.1,100)
corners = [cv.goodFeaturesToTrack(topl_box,1,0.1,100)[0], cv.goodFeaturesToTrack(topr_box,1,0.1,100)[0],
cv.goodFeaturesToTrack(botr_box,1,0.1,100)[0], cv.goodFeaturesToTrack(botl_box,1,0.1,100)[0]]

cv.imshow("topl", topl_box)
cv.imshow("topr", topr_box)
cv.imshow("botr", botr_box)
cv.imshow("botl", botl_box)

corners = np.int0(corners)
coord = []
for i in range(4):
	x,y = corners[i].ravel()
	if i == 1:
		x = w_pix - box_dim[1] + x

	if i == 2:
		x, y = w_pix - box_dim[1] + x, h_pix - box_dim[0] + y
	
	if i == 3:
		y = h_pix - box_dim[0] + y

	coord.append((x,y))

	cv.circle(cropped_image,(x,y),3,255,-1)
# print (coord)
src = np.array(coord, dtype="float32")
dst = np.array([(0,0), (screen_w_mm, 0), (screen_w_mm,screen_h_mm),(0, screen_h_mm)], dtype="float32")
M = cv.getPerspectiveTransform(src, dst)

# warped = cv.warpPerspective(cropped_image, M, (screen_w_mm*2,screen_h_mm*2))

print (M)

# for i in corners:
#     x,y = i.ravel()
#     cv.circle(cropped_image,(x,y),3,255,-1)

cv.imshow("Corners", cropped_image)
# cv.imshow("Warped", warped)
#Perspective Transform from corners



# #Estimate Blob coordinate
# b_coord = np.array([[pointer.pt[0]], [pointer.pt[1]]], dtype="float32")
# final = np.matmul(M, b_coord)
# print (final)


if cv.waitKey(0) & 0xff == 27:
    cv.destroyAllWindows()
