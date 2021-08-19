#include <ros/ros.h>
#include <visualization_msgs/Marker.h>
#include <nav_msgs/Odometry.h>
#include <cmath>

double pickUpPos[2]  = {-1.0, -3.0};
double dropOffPos[2] = {3.0, -3.0};

double pose[2] = {0, 0};  // current pose

bool item_picked_up = false;
bool item_dropped_off = false;

void get_current_pose(const nav_msgs::Odometry::ConstPtr &msg)
{
  pose[0] = msg->pose.pose.position.x;
  pose[1] = msg->pose.pose.position.y;

  double pickup_distance = sqrt(pow(pose[0] - pickUpPos[0], 2) + pow(pose[1] - pickUpPos[1], 2));
  double drop_off_distance = sqrt(pow(pose[0] - dropOffPos[0], 2) + pow(pose[1] - dropOffPos[1], 2));

  if (pickup_distance < 0.3) {
    item_picked_up = true;
  }

  if (drop_off_distance < 0.3) {
    item_dropped_off = true;
  }
}


int main( int argc, char** argv )
{
  ros::init(argc, argv, "add_markers");
  ros::NodeHandle n;
  ros::Rate r(1);
  ros::Publisher marker_pub = n.advertise<visualization_msgs::Marker>("visualization_marker", 1);
  ros::Subscriber pose_sub = n.subscribe("odom", 10, get_current_pose);

  
  
  uint32_t shape = visualization_msgs::Marker::CUBE;


 ROS_INFO("Starting Pick Up ... ");
 while (ros::ok())
 {
    visualization_msgs::Marker marker;
    
    // Set the frame ID and timestamp.  See the TF tutorials for information on these.
    marker.header.frame_id = "map";
    marker.header.stamp = ros::Time::now();

    marker.ns = "basic_shapes";
    marker.id = 0;

    marker.type = shape;
   
    marker.action = visualization_msgs::Marker::ADD;    

    marker.pose.position.x = 0;
    marker.pose.position.y = 0;
    marker.pose.position.z = 1.0;
    marker.pose.orientation.x = 0.0;
    marker.pose.orientation.y = 0.0;
    marker.pose.orientation.z = 0.0;
    marker.pose.orientation.w = 1.0;

    marker.scale.x = 0.2;
    marker.scale.y = 0.2;
    marker.scale.z = 0.2;

    marker.color.r = 0.3f;
    marker.color.g = 0.5f;
    marker.color.b = 0.7f;
    marker.color.a = 1.0;
 
    marker.lifetime = ros::Duration();

    ros::spinOnce();

      if (!item_picked_up) 
	{
	     marker.action = visualization_msgs::Marker::ADD;
	     marker.pose.position.x = pickUpPos[0];
             marker.pose.position.y = pickUpPos[1];
	     marker_pub.publish(marker);
    	} 
      else if (!item_dropped_off) 
	{
      	     marker.action = visualization_msgs::Marker::DELETE;
             marker.pose.position.x = dropOffPos[0];
             marker.pose.position.y = dropOffPos[1];
             marker_pub.publish(marker);
        } 
      else 
	{
      	     marker.action = visualization_msgs::Marker::ADD;
	     marker.pose.position.x = dropOffPos[0];
             marker.pose.position.y = dropOffPos[1];
	     marker_pub.publish(marker);
    	}
     }
}
