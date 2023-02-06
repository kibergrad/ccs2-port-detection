#include <detection_msgs/BoundingBoxes.h>
#include <detection_msgs/BoundingBox.h>
#include <plug_test/pixel_to_coords.h>
#include <plug_test/Points.h>
#include <geometry_msgs/Point.h>
#include <ros/ros.h>

#include<utility>

#include "get_orientation.h"

using namespace detection_msgs;
using namespace plug_test;

int xmin = -1;
int ymin = -1;
int xmax = -1;
int ymax = -1;

ros::ServiceClient get_point_cli;

// coords would contain the coordinates in the order: top_left, top_right, bottom_right, botton_left
plug_test::Points coords = plug_test::Points();

geometry_msgs::Point get_coords(int x, int y){
	plug_test::pixel_to_coords get_point_srv;
	get_point_srv.request.x = x;
	get_point_srv.request.y = y;

	if (get_point_cli.call(get_point_srv)){
		return(get_point_srv.response.point);
	} else {
		ROS_ERROR("Failed to call pixel to coords service.");
	}
}

void boxesCallback(const detection_msgs::BoundingBoxes::ConstPtr& msg){
	if (msg->bounding_boxes.size() > 0){
		detection_msgs::BoundingBox bounding_box = msg->bounding_boxes.at(0);
		xmin = bounding_box.xmin;
		ymin = bounding_box.ymin;
		xmax = bounding_box.xmax;
		ymax = bounding_box.ymax;

		geometry_msgs::Point tl_coords = get_coords(xmin, ymin);
		geometry_msgs::Point tr_coords = get_coords(xmax, ymin);
		geometry_msgs::Point br_coords = get_coords(xmax, ymax);
		geometry_msgs::Point bl_coords = get_coords(xmin, ymax);
		
		// printf("(%f, %f, %f) ", tl_coords.x, tl_coords.y, tl_coords.z);
		// printf("(%f, %f, %f) ", tr_coords.x, tr_coords.y, tr_coords.z);
		// printf("(%f, %f, %f) ", br_coords.x, br_coords.y, br_coords.z);
		// printf("(%f, %f, %f) ", bl_coords.x, bl_coords.y, bl_coords.z);
		// printf("\n");
		pair <float, float> orientation = get_orientation(tl_coords.x, tl_coords.y, tl_coords.z,
						tr_coords.x, tr_coords.y, tr_coords.z,
						br_coords.x, br_coords.y, br_coords.z,
						bl_coords.x, bl_coords.y, bl_coords.z);
		printf("(%f, %f)\n", orientation.first, orientation.second);
	}
}

int main(int argc, char **argv){
	ros::init(argc,argv, "get_orient");
	ros::NodeHandle nh;

	get_point_cli = nh.serviceClient<plug_test::pixel_to_coords>("get_point_from_pixel");
	ros::Subscriber boundBox = nh.subscribe<detection_msgs::BoundingBoxes>("/yolov5/detections", 10, boxesCallback);
	// ros::Publisher coords_pub = nh.advertise<plug_test::Points>("/bb_coords", 1000);

	// ros::Rate loop_rate(10);

	ros::spin();
	return 0;
}