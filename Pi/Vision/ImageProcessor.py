import cv2 as cv
import numpy as np
import picamera
import io

class CornerParams:
	# def __init__(self, max_corners, quality_level, min_distance):
	def __init__(self):
			self.max_corners = None
			self.quality_level = None
			self.min_distance = None


class ImageProcessor:
	# def __init__(self, topl_crop, botr_crop, box_ratio, screen_w_mm, screen_h_mm, num_blobs):
	def __init__(self):
		# self.topl_crop = topl_crop
		# self.botr_crop = botr_crop
		# self.box_ratio = box_ratio
		# self.screen_w_mm = screen_w_mm
		# self.screen_h_mm = screen_h_mm
		# self.num_blobs = num_blobs
		# self.image = None
		# self.gray = None
		# self.corners = None
		# self.warped = None

		self.topl_crop = None
		self.botr_crop = None
		self.box_ratio = None
		self.screen_mm = None
		self.num_blobs = None
		self.image = None
		self.gray = None
		self.corners = None
		self.warped = None
		self.gray_warped = None
		self.blob_params = cv.SimpleBlobDetector_Params()
		self.corner_params = CornerParams()
		self.keypoints = None
		self.detector = None
		self.stream = io.BytesIO()

	def set_crop(self,topl_crop, botr_crop):
		self.topl_crop = topl_crop
		self.botr_crop = botr_crop

	def set_box(self, box_ratio):
		self.box_ratio = box_ratio

	def set_screen_mm(self, screen_mm):
		self.screen_mm = screen_mm

	def set_num_blobs(self, num_blobs):
		self.num_blobs = num_blobs

	def set_corner_params(self, max_corners, quality_level, min_distance):
		self.corner_params.max_corners = max_corners
		self.corner_params.quality_level = quality_level
		self.corner_params.min_distance = min_distance 

	def set_blob_params(self, minThreshold, maxThreshold, filterByArea, minArea, maxArea, filterByCircularity,
		minCircularity, maxCircularity, filterByConvexity, minConvexity, maxConvexity, filterByInertia, minInertiaRatio,
		maxInertiaRatio, filterbyColor):

		(self.blob_params).minThreshold = minThreshold
		(self.blob_params).maxThreshold = maxThreshold
		(self.blob_params).filterByArea = filterByArea
		(self.blob_params).minArea = minArea
		(self.blob_params).maxArea = maxArea
		(self.blob_params).filterByCircularity = filterByCircularity
		(self.blob_params).minCircularity = minCircularity
		(self.blob_params).maxCircularity = maxCircularity
		(self.blob_params).filterByConvexity = filterByConvexity
		(self.blob_params).minConvexity = minConvexity
		(self.blob_params).maxConvexity = maxConvexity
		(self.blob_params).filterByInertia = filterByInertia
		(self.blob_params).minInertiaRatio = minInertiaRatio
		(self.blob_params).maxInertiaRatio = maxInertiaRatio
		(self.blob_params).filterByColor = filterbyColor
		self.detector = cv.SimpleBlobDetector_create(self.blob_params)


	def captureImage(self, camera):
		self.stream.seek(0)
		# capture into stream
		camera.capture(self.stream, format='jpeg', use_video_port=True)
		# convert image into numpy array
		data = np.frombuffer(self.stream.getvalue(), dtype=np.uint8)
		# turn the array into a cv2 image
		img = cv.imdecode(data, 1)
		self.updateImage(img)

	def updateImage(self, image):
		h_pix, w_pix, _ = image.shape
		topl = [int(h_pix*self.topl_crop[0]), int(w_pix*self.topl_crop[1])]
		botr = [int(h_pix*self.botr_crop[0]),int(w_pix*self.botr_crop[1])]
		self.image = image[topl[0]:botr[0], topl[1]:botr[1]]
		self.gray = cv.cvtColor(self.image,cv.COLOR_BGR2GRAY)

	def detectCorners(self):
		h_pix, w_pix, _ = self.image.shape
		box_dim = [int(h_pix*self.box_ratio[0]), int(w_pix*self.box_ratio[1])]
		topl_box = self.gray[:box_dim[0], :box_dim[1]]
		topr_box = self.gray[:box_dim[0], -1*box_dim[1]:]
		botr_box = self.gray[-1*box_dim[0]:, -1*box_dim[1]:]
		botl_box = self.gray[-1*box_dim[0]:, :box_dim[1]]
		# cv.imwrite("topl.jpeg", topl_box)
		# cv.imwrite("topr.jpeg", topr_box)
		# cv.imwrite("botr.jpeg", botr_box)
		# cv.imwrite("botl.jpeg", botl_box)
		a = self.corner_params.max_corners
		b = self.corner_params.quality_level
		c = self.corner_params.min_distance

		corners = [cv.goodFeaturesToTrack(topl_box,a,b,c)[0], cv.goodFeaturesToTrack(topr_box,a,b,c)[0],
					cv.goodFeaturesToTrack(botr_box,a,b,c)[0], cv.goodFeaturesToTrack(botl_box,a,b,c)[0]]
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

			coord.append([x,y])

			# 

		self.corners = np.array(coord,dtype="float32")

	def dstPoints(self):
		top_width = np.linalg.norm(self.corners[0] - self.corners[1])
		bottom_width = np.linalg.norm(self.corners[2] - self.corners[3])
		left_height = np.linalg.norm(self.corners[0] - self.corners[3])
		right_height = np.linalg.norm(self.corners[1] - self.corners[2])

		max_height =int(round(max(left_height, right_height)));
		max_width = int(round(max(top_width, bottom_width)));
		dst = np.array([[0,0], [max_width, 0], [max_width,max_height],[0, max_height]], dtype="float32")
		return dst

	def warp(self):
		self.detectCorners()
		dst = self.dstPoints()
		M = cv.getPerspectiveTransform(self.corners, dst)
		warped_w = dst[2][0]
		warped_h = dst[2][1]
		self.warped = cv.warpPerspective(self.image, M, (warped_w, warped_h))
		self.gray_warped = cv.cvtColor(self.warped,cv.COLOR_BGR2GRAY)

	def saveImages(self):
		h_pix, w_pix, _ = self.image.shape
		box_dim = [int(h_pix*self.box_ratio[0]), int(w_pix*self.box_ratio[1])]
		topl_box = self.gray[:box_dim[0], :box_dim[1]]
		topr_box = self.gray[:box_dim[0], -1*box_dim[1]:]
		botr_box = self.gray[-1*box_dim[0]:, -1*box_dim[1]:]
		botl_box = self.gray[-1*box_dim[0]:, :box_dim[1]]
		im_with_keypoints = cv.drawKeypoints(self.warped, self.keypoints, np.array([]), (0,0,255), cv.DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS)
		for i in self.corners:
			cv.circle(self.image,(i[0],i[1]),3,255,-1)


		#pointer = self.keypoints[0]
		# for i in range(0,100):
		# 	im_with_keypoints[int(pointer.pt[1])+i][int(pointer.pt[0])] = np.array([255,0,0])
		# im_with_keypoints[int(pointer.pt[1]):int(pointer.pt[1]+20)][int(pointer.pt[0])] = np.array([255,0,0])
		cv.imwrite("ProcImages/cropped.jpeg", self.image)
		cv.imwrite("ProcImages/topl.jpeg", topl_box)
		cv.imwrite("ProcImages/topr.jpeg", topr_box)
		cv.imwrite("ProcImages/botr.jpeg", botr_box)
		cv.imwrite("ProcImages/botl.jpeg", botl_box)
		cv.imwrite("ProcImages/corners.jpeg", self.image)
		cv.imwrite("ProcImages/warped.jpeg", self.warped)
		cv.imwrite("ProcImages/im_w_kp.jpeg", im_with_keypoints)


	def blobDetect(self):
		self.warp()
		self.keypoints = self.detector.detect(self.gray_warped)
		

	def getBlobCoord_n_Color(self, image=None):
		if image != None:
			self.updateImage(image)
		
		self.blobDetect()

		h_max, w_max, _ = self.warped.shape

		out = []
		d = np.array([20,20], dtype=int)

		for i in self.keypoints:
			blob_x = i.pt[0]
			blob_y = i.pt[1]
			# p = np.array([int(blob_x), int(blob_y)], dtype=int)
			#blob_x/w_max = mm_y/screen_w_mm
			#blob_y/h_max = mm_x/screen_h_mm

			mm_y = (blob_x/w_max)*self.screen_mm[1]
			mm_x = (blob_y/h_max)*self.screen_mm[0]
			# color = np.sum(self.warped[int(blob_x):int(blob_x)+20][int(blob_y)])/20
			# color = self.warped[int(blob_x)+20][int(blob_y)+20]
			srcB = self.warped[int(blob_y):int(blob_y)+20, int(blob_x),0]
			srcG = self.warped[int(blob_y):int(blob_y)+20, int(blob_x),1]
			srcR = self.warped[int(blob_y):int(blob_y)+20, int(blob_x),2]
			# print(src[0])
			# meanB, _ = cv.meanStdDev(srcB)
			# meanG, _ = cv.meanStdDev(srcG)
			# meanR, _ = cv.meanStdDev(srcR)
			meanB = np.sum(srcB)/len(srcB)
			meanG = np.sum(srcG)/len(srcG)
			meanR = np.sum(srcR)/len(srcR)
			# print(meanB)
			# max_mean = max(meanB, meanG, meanR)
			color = np.argmax([meanB,meanG,meanR])
			# if max_mean == meanB:
			# print (color)
			out.append([mm_x,mm_y,color])
		return out



		# src = self.warped[int(blob_y):int(blob_y)+20, int(blob_x), 0]
if __name__ == '__main__':
	# image = cv.imread('Test_Images/May5_2020/img1.jpeg')
	# topl_crop = [0.18, 0.1]
	# botr_crop = [0.96, 0.98]
	# box_ratio = [0.1, 0.1]
	# screen_mm = [181, 268]

	# image = cv.imread('Test_Images/V2/img1.jpeg')
	#topl_crop = [0.05, 0.01]
	#botr_crop = [0.96, 0.98]
	#box_ratio = [0.2, 0.2]
	screen_mm = [181, 268]
	
	topl_crop = [0.17, 0.12]
	botr_crop = [0.98, 1]
	box_ratio = [0.15, 0.15]

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


	#IP.blobDetect()
	# print(IP.getBlobCoord_n_Color(image))
	with picamera.PiCamera() as camera:
		camera.resolution = (1640, 1232)
		IP.captureImage(camera)
	IP.blobDetect()
	IP.saveImages()

	# IP.detectCorners()
	# IP.warp()
	# cv.imshow("Corners", IP.image)
	# cv.imwrite("corners.jpeg", IP.image)
	# cv.imwrite("warped.jpeg", IP.warped)
	# IP.saveImages()

	# if cv.waitKey(0) & 0xff == 27:
	# 	cv.destroyAllWindows()




