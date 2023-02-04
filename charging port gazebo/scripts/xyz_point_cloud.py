#!/usr/bin/env python3
from sensor_msgs.msg import PointCloud2
from sensor_msgs import point_cloud2
from geometry_msgs import Point 
from scipy.integrate import quad
import rospy
import time
import numpy as np

def callback_pointcloud(data):
    xpoint = []
    ypoint = []
    zpoint = []
    assert isinstance(data, PointCloud2)
    gen = point_cloud2.read_points(data, field_names=("x", "y", "z"), skip_nans=True) 

    print(type(gen))
    for p in gen:
        xpoint.append(p[0])
        ypoint.append(p[1])
        zpoint.append(p[2]) 

    print("xpoint:", xpoint[80000])
    print("ypoint:", ypoint[80000])
    print("zpoint:", zpoint[80000])

def main():
    rospy.init_node('pcl_listener', anonymous=True)
    rospy.Subscriber('/camera/depth/points', PointCloud2, callback_pointcloud)

    rospy.spin()

if __name__ == "__main__":
    main()
