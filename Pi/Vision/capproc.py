import time
import picamera
import io
from ImageProcessor import ImageProcessor


topl_crop = [0.05, 0.01]
botr_crop = [0.96, 0.98]
box_ratio = [0.2, 0.2]
screen_mm = [181, 268]


max_corners = 1
quality_level = 0.1
min_distance = 100


num_blobs = 1

# Change thresholds
minThreshold = 200
maxThreshold = 255

# Filter by Area.
filterByArea = False
minArea = 50
maxArea = 100

# Filter by Circularity
filterByCircularity = False
minCircularity = 0.1
maxCircularity = 1

# Filter by Convexity
filterByConvexity = False
minConvexity = 0.0
maxConvexity = 1

# Filter by Inertia (ratio of widest to thinnest point)
filterByInertia = True
minInertiaRatio = 0
maxInertiaRatio = 1

#Filter by Color
filterByColor = False

IP = ImageProcessor()
IP.set_crop(topl_crop, botr_crop)
IP.set_box(box_ratio)
IP.set_screen_mm(screen_mm)
IP.set_num_blobs(num_blobs)
IP.set_corner_params(max_corners, quality_level, min_distance)

IP.set_blob_params(minThreshold, maxThreshold, filterByArea, minArea, maxArea, filterByCircularity,
minCircularity, maxCircularity, filterByConvexity, minConvexity, maxConvexity, filterByInertia, minInertiaRatio,
maxInertiaRatio, filterByColor)

with picamera.PiCamera() as camera:
    camera.resolution = (1640, 1232)
    for i in range(10):
	    IP.captureImage(camera)
	    print(IP.getBlobCoord_n_Color(image))
	    time.sleep(2)