#!/usr/bin/env python3

import rospy
from sensor_msgs.msg import PointCloud2
from sensor_msgs.msg import Image, CameraInfo
from detection_msgs.msg import BoundingBox, BoundingBoxes
from image_geometry import PinholeCameraModel
from cv_bridge import CvBridge

def callback(boxes):
    global model
    global x
    global y
    global x_pixel
    global y_pixel
    if len(boxes.bounding_boxes) > 0: 
        bounding_box = boxes.bounding_boxes[0]
        x_pixel = bounding_box.xmin
        y_pixel = bounding_box.ymax
        x, y, a = model.projectPixelTo3dRay((x_pixel, y_pixel))

def camera_info_cbk(camera_info):
    global need_cam_info
    global model
    if need_cam_info:
        model.fromCameraInfo(camera_info)
        need_cam_info = False

def callback_depth(img_msg):
    global x
    global y
    global z
    global x_pixel
    global y_pixel
    bridge = CvBridge()
    cv_image = bridge.imgmsg_to_cv2(img_msg)
    if (x_pixel is None) or (y_pixel is None):
        pass
    else:
        print(x_pixel, y_pixel)
        print(cv_image[x_pixel, y_pixel])

def py_node():
    global model
    global need_cam_info
    global x
    global y
    global z
    global x_pixel
    global y_pixel

    model = PinholeCameraModel()
    need_cam_info = True
    x = None
    y = None
    z = None
    x_pixel = None
    y_pixel = None

    rospy.init_node('py_node', anonymous=True)

    rospy.Subscriber("/camera/color/camera_info", CameraInfo, camera_info_cbk)
    rospy.Subscriber("/yolov5/detections", BoundingBoxes, callback)
    rospy.Subscriber("/camera/depth/image_raw", Image, callback_depth)

    rospy.spin()

if __name__ == '__main__':
    try:
        py_node()
    except rospy.ROSInterruptException:
        pass
