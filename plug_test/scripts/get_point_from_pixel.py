#!/usr/bin/env python3

import rospy

from plug_test.srv import pixel_to_coords, pixel_to_coordsResponse
from geometry_msgs.msg import Point
from sensor_msgs.msg import Image, CameraInfo

from cv_bridge import CvBridge
from image_geometry import PinholeCameraModel

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

def get_point_from_pixel():
	rospy.init_node('pixel_to_coords_service')
	ser = rospy.Service('get_point_from_pixel', pixel_to_coords, handler)
	print("pixel_to_coords_service stared. Pass x, y as integers...")
	rospy.spin()

if __name__ == "__main__":
	get_point_from_pixel()
