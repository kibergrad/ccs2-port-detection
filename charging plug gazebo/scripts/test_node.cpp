#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <sensor_msgs/Image.h>
#include <sensor_msgs/PointCloud2.h>
#include <iomanip>
#include <string>
#include <fstream>
// #include <opencv2/highgui/highgui.hpp>

namespace enc = sensor_msgs::image_encodings;
using namespace std;
using namespace cv;

int littleendian()
{
  int intval = 1;
  uchar * uval = (uchar * ) & intval;
  return uval[0] == 1;
}

int writeFilePFM(const std::string filename, const Mat & im, float scalef = 1 / 255.0)
{
  // create fstream object to write out pfm file
  // open the file in binary
  fstream file(filename.c_str(), ios::out | ios::binary);

  // init variables
  int type = im.type();
  string bands;
  int width = im.size().width, height = im.size().height; // width and height of the image
  float fvalue;   // scale factor and temp value to hold pixel value
  Vec3f vfvalue;  // temp value to hold 3-band pixel value

  // determine identifier string based on image type
  switch (type)
  {
    case CV_32FC1:
          bands = "Pf"; // grayscale
          break;
    case CV_32FC3:
          bands = "PF"; // color
          break;
    default:
          cout << "Unsupported image type, must be CV_32FC1 or CV_32FC3";
          return -1;
  }

  // sign of scalefact indicates endianness, see pfms specs
  if (littleendian())
    scalef = -scalef;

  // insert header information
  file << bands << "\n";
  file << width << " ";
  file << height << "\n";
  file << scalef << "\n";

  cout << setfill('=') << setw(19) << "=" << endl;
  cout << "Writing image to pfm file: " << filename << endl;
  cout << "Little Endian?: " << ((littleendian()) ? "true" : "false") << endl;
  cout << "width: " << width << endl;
  cout << "height: " << height << endl;
  cout << "scale: " << scalef << endl;

  // handle 1-band image
  if (bands == "Pf")
  {
    cout << "Writing grayscale image (1-band)" << endl;
    cout << "Writing into CV_32FC1 image" << endl;
    for (int i = height - 1; i >= 0; --i)
    {
      for (int j = 0; j < width; ++j)
      {
        fvalue = im.at < float > (i, j);
        file.write((char * ) & fvalue, sizeof(fvalue));

      }
    }
  }
  // handle 3-band image
  else if (bands == "PF")
  {
    cout << "writing color image (3-band)" << endl;
    cout << "writing into CV_32FC3 image" << endl;
    for (int i = height - 1; i >= 0; --i)
    {
      for (int j = 0; j < width; ++j)
      {
        vfvalue = im.at < Vec3f > (i, j);
        file.write((char * ) & vfvalue, sizeof(vfvalue));
      }
    }
  }
  else
  {
    cout << "unknown bands description";
    return -1;
  }
  cout << setfill('=') << setw(19) << "=" << endl << endl;
  return 0;
}

void chatterCallback(const sensor_msgs::Image::ConstPtr& msg) {
  cv_bridge::CvImagePtr Dest = cv_bridge::toCvCopy(msg);
  // ROS_INFO("Value: %f", Dest->image.at<float>(msg->width/2,msg->height/2));
  ROS_INFO("Encoding: %s", (Dest->encoding).c_str());
  writeFilePFM("img_depth.pfm", Dest->image, 1/255.0);
  ROS_INFO("Written depth file.");
}

void imageCallback(const sensor_msgs::ImageConstPtr& msg)
{
    cv_bridge::CvImagePtr cv_ptr;
    try
    {
      cv_ptr = cv_bridge::toCvCopy(msg, enc::TYPE_16UC1);//now cv_ptr is the matrix, do not forget "TYPE_" before "16UC1"
    }
    catch (cv_bridge::Exception& e)
    {
      ROS_ERROR("cv_bridge exception: %s", e.what());
      return;
    }

    double depth = cv_ptr->image.at<float>(cv::Point(240,320));//you can change 240,320 to your interested pixel
    ROS_INFO("Depth: %f", depth);
    // std::cout << std::setprecision(3) << depth << std::endl;
    // double distance = cv_ptr->image.at<u_int16_t>(cv::Point(240,320));
    // ROS_INFO("New Depth: %f", distance);
}

// void imageCallback2(const sensor_msgs::PointCloud2 point_cloud2_msgs)
// {
//   //sensor_msgs::msg::PointCloud2& cloud = ;
//   pcl::PointCloud<pcl::PointXYZ> point_cloud;

//   // convert point clouds2 msgs data to points
//   pcl::fromROSMsg(point_cloud2_msgs, point_cloud);

//   // resize vector every callback
//   point_cloud2_points.resize(point_cloud.points.size());

//   ROS_INFO("size = %d",point_cloud.points.size());


//   // for(unsigned long i = 0; i < point_cloud.points.size(); i++)
//   // {
//   //   // get points
//   //   point_cloud2_points.at(i).x = point_cloud.points.at(i).x;
//   //   point_cloud2_points.at(i).y = point_cloud.points.at(i).y;
//   //   point_cloud2_points.at(i).z = point_cloud.points.at(i).z;       
//   // }

// }

// void imgcb(const sensor_msgs::Image::ConstPtr& msg)
// {
//     try {
//         cv_bridge::CvImageConstPtr cv_ptr;
//         cv_ptr = cv_bridge::toCvShare(msg);

//         //get image dimension once
//         static bool runOnce = true;
//         if (runOnce){
//             std::cout << "Image dimension (Row,Col): " << cv_ptr->image.rows << " x " << cv_ptr->image.cols << std::endl;
//             runOnce = false;
//         }

//         //get global max depth value
//         double max = 0.0;
//         cv::minMaxLoc(cv_ptr->image, 0, &max, 0, 0);
//         std::cout << "Max value: " << max << std::endl;

//         //get global min depth value
//         double min = 0.0;
//         cv::minMaxLoc(cv_ptr->image, &min, &max, 0, 0);
//         std::cout << "Min value: " << min << std::endl;

//         //get depth value at a point
//         float distanceVal = cv_ptr->image.at<float>(100, 100);
//         std::cout << "Distance value: " << distanceVal << "m" << std::endl;

//     } catch (const cv_bridge::Exception& e) {
//         ROS_ERROR("cv_bridge exception: %s", e.what());
//     }
// }

void img_clb(const sensor_msgs::PointCloud2 cloud){
  // std::cout << cloud << std::endl;
  // for (int i = 0; i < cloud->width; i++){
  //   int index = cloud->row_step*ROW_NUM + cloud->point_step*i + cloud->fields[2].offset;
  //   memcpy(&depth_data[i], &cloud->data[index], sizeof(float));
  // }
}

int main(int argc, char **argv)
{
  ros::init(argc,argv,"test_node");
  ros::NodeHandle nh; 
  
  ros::Subscriber depth_find = nh.subscribe<sensor_msgs::Image>
        ("/camera/depth/image_raw", 10, chatterCallback);

  // ros::Subscriber depth_find = nh.subscribe<sensor_msgs::PointCloud2>
  //       ("/camera/depth/points", 10, img_clb);

  ros::spin();

  ROS_INFO("Chutiya banaya tumko");
  return 0;
}
