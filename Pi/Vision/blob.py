import numpy as np
import imutils
import cv2


image = cv2.imread('Test_Images/V2/img1.jpeg')

ret,thresh1 = cv2.threshold(image,thr,255,cv2.THRESH_BINARY)
gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
gray = cv2.bilateralFilter(gray, 11, 17, 17)
gray = thresh1
cv2.imshow("gray", gray)

# Setup SimpleBlobDetector paramarization methodeters.
params = cv2.SimpleBlobDetector_Params()

# Change thresholds
params.minThreshold = 200
params.maxThreshold = 255

# Filter by Area.
params.filterByArea = False
params.minArea = 4
params.maxArea = 10000

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
detector = cv2.SimpleBlobDetector_create(params)


keypoints = detector.detect(gray)

# # Draw detected blobs as red circles.
im_with_keypoints = cv2.drawKeypoints(image, keypoints, np.array([]), (0,0,255), cv2.DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS)


cv2.imshow("Keypoints", im_with_keypoints)
if cv2.waitKey(0) & 0xff == 27:
    cv2.destroyAllWindows()