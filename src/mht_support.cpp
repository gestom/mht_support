#include <ros/ros.h>
#include <tf/tf.h>
#include <sensor_msgs/CameraInfo.h>
#include <sensor_msgs/PointCloud2.h>
#include <string.h>

ros::Subscriber pointCloudSub;
ros::Publisher cameraInfoPub; 

void ptCallback(const sensor_msgs::PointCloud2::ConstPtr& msg)
{
	printf("PT received\n");
	sensor_msgs::CameraInfo ci;
	ci.header.stamp = msg->header.stamp;
	ci.header.frame_id = "/head_xtion_rgb_optical_frame";
	ci.height=480;
	ci.width=640;
	ci.distortion_model="plumb_bob";
	double d[] = {0.0, 0.0, 0.0, 0.0, 0.0};
	double k[] = {525.0, 0.0, 319.5, 0.0, 525.0, 239.5, 0.0, 0.0, 1.0};
	double r[] = {1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0};
	double p[] = {525.0, 0.0, 319.5, 0.0, 0.0, 525.0, 239.5, 0.0, 0.0, 0.0, 1.0, 0.0};
	for (int i = 0;i<5;i++) ci.D.push_back(d[i]);
	for (int i = 0;i<9;i++){
	       	ci.K[i] = k[i];
	       	ci.R[i] = r[i];
	       	ci.P[i] = p[i];
	}
	cameraInfoPub.publish(ci);
}
     
int main(int argc, char** argv)
{
	ros::init(argc, argv, "mht_support");
	ros::NodeHandle n;

	cameraInfoPub = n.advertise<sensor_msgs::CameraInfo>("/head_xtion/camera_info", 1);
	pointCloudSub = n.subscribe("/head_xtion/depth/points", 1, ptCallback);

	ros::spin();
}
