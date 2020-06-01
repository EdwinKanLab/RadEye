import cv2 as cv
import numpy as np
from matplotlib import pyplot as plt

#Image Read
image = cv.imread('Test_Images/V2/img2.jpeg')

h_pix, w_pix, _ = image.shape
print (w_pix, h_pix)
screen_h_mm, screen_w_mm = 181, 268



#Image Crop

#Tune these parameters
ratio_topl = [0.05, 0.01] #Ratio for the top-left coordinate
ratio_botr = [0.95, 0.95] #Ratio for the bottom-right coordinate

topl = [int(h_pix*ratio_topl[0]), int(w_pix*ratio_topl[1])]

botr = [int(h_pix*ratio_botr[0]),int(w_pix*ratio_botr[1])]

cropped_image = image[topl[0]:botr[0], topl[1]:botr[1]]

#Convert to Gray Scale
gray = cv.cvtColor(cropped_image,cv.COLOR_BGR2GRAY)

#Corner Detection
h_pix, w_pix, _ = cropped_image.shape
h_ratio = 0.20
w_ratio = 0.10
box_dim = [int(h_pix*h_ratio), int(w_pix*w_ratio)]

topl_box = gray[:box_dim[0], :box_dim[1]]
topr_box = gray[:box_dim[0], -1*box_dim[1]:]
botr_box = gray[-1*box_dim[0]:, -1*box_dim[1]:]
botl_box = gray[-1*box_dim[0]:, :box_dim[1]]

corners = [cv.goodFeaturesToTrack(topl_box,1,0.1,100)[0], cv.goodFeaturesToTrack(topr_box,1,0.1,100)[0],
cv.goodFeaturesToTrack(botr_box,1,0.1,100)[0], cv.goodFeaturesToTrack(botl_box,1,0.1,100)[0]]

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
def getNewDim(src):
	widthA = np.sqrt((src[0][0] - src[1][0])**2 + (src[0][1]-src[1][1])**2)
	widthB = np.sqrt((src[2][0] - src[3][0])**2 + (src[2][1]-src[3][1])**2)
	width = max(int(widthA), int(widthB))

	heightA = np.sqrt((src[0][0] - src[3][0])**2 + (src[0][1]-src[3][1])**2)
	heightB = np.sqrt((src[1][0] - src[2][0])**2 + (src[1][1]-src[2][1])**2)
	height = max(int(heightA), int(heightB))


	return (width,height)

w_max, h_max = getNewDim(src)
dst = np.array([(0,0), (w_max, 0), (w_max,h_max),(0, h_max)], dtype="float32")
M = cv.getPerspectiveTransform(src, dst)

warped = cv.warpPerspective(cropped_image, M, (w_max,h_max))
warped_gray = cv.cvtColor(warped,cv.COLOR_BGR2GRAY)


# cv.imshow("warped",warped_gray)

#Blob Detector
#Tune blob_thresh_min and blob_thresh_max
blob_thresh_min = 250
blob_thresh_max = 255
ret,thresh1 = cv.threshold(warped_gray, blob_thresh_min, blob_thresh_max, cv.THRESH_BINARY)
# cv.imshow("Blob Threshold", thresh1)

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


keypoints = detector.detect(warped)
pointer = keypoints[0]


# # Draw detected blobs as red circles.
im_with_keypoints = cv.drawKeypoints(warped, keypoints, np.array([]), (0,0,255), cv.DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS)

for i in range(0,100):
	im_with_keypoints[int(pointer.pt[1])+i][int(pointer.pt[0])] = np.array([0,0,0])
cv.imshow("Keypoints", im_with_keypoints)


blob_x = pointer.pt[0]
blob_y = pointer.pt[1]

#blob_x/w_max = mm_y/screen_w_mm
#blob_y/h_max = mm_x/screen_h_mm

mm_y = (blob_x/w_max)*screen_w_mm
mm_x = (blob_y/h_max)*screen_h_mm
print (mm_x,mm_y)

if cv.waitKey(0) & 0xff == 27:
    cv.destroyAllWindows()
