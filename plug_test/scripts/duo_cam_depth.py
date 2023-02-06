#!/usr/bin/env python3

import rospy

from plug_test.srv import pixel_to_coords, pixel_to_coordsResponse
from geometry_msgs.msg import Point
from sensor_msgs.msg import Image, CameraInfo

from cv_bridge import CvBridge
from image_geometry import PinholeCameraModel, StereoCameraModel

def handler(req):
	model = PinholeCameraModel()
	model.fromCameraInfo(rospy.wait_for_message("/camera/color/camera_info", CameraInfo))

	x, y, a = model.projectPixelTo3dRay((req.x, req.y))
	depth_img = rospy.wait_for_message("/camera/depth/image_raw", Image)

	bridge = CvBridge()
	cv_img = bridge.imgmsg_to_cv2(depth_img)
	z = cv_img[req.y, req.x]

	# print(f"({x}, {y}, {z})")

	point = Point()
	point.x = x
	point.y = y
	point.z = z
	coords_res = pixel_to_coordsResponse()
	coords_res.point = point

	return coords_res

def right_cam_info(cam_info):
	global need_right_cam_info
	global right_cam_info
	if need_right_cam_info:
		right_cam_info = cam_info
		need_right_cam_info = False

def left_cam_info(cam_info):
	global need_left_cam_info
	global left_cam_info
	if need_left_cam_info:
		left_cam_info = cam_info
		need_left_cam_info = False

def get_point_from_pixel():
	global model
	global need_left_cam_info
	global need_right_cam_info
	global left_cam_info
	global right_cam_info

	need_left_cam_info = True
	need_right_cam_info = True
	left_cam_info = None
	right_cam_info = None
	need_camera_info = True

	model = StereoCameraModel()

	rospy.init_node('duo_cam_depth')
	rospy.Subscriber('/duo_cam/left_cam/camera_info', CameraInfo, left_cam_info)
	rospy.Subscriber('/duo_cam/right_cam/camera_info', CameraInfo, right_cam_info)
	rospy.Publisher('/duo_cam/depth', Image, queue_size=10)
	
	rate = rospy.Rate(10)
	while not rospy.is_shutdown():
		if need_camera_info and (left_cam_info is not None) and (right_cam_info is not None):
			model.fromCameraInfo(left_cam_info, right_cam_info)
			need_camera_info = False

		if need_camera_info is False:
			

		rate.sleep()

if __name__ == "__main__":
	get_point_from_pixel()
