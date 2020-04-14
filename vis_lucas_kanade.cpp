#include <ros/ros.h>
#include <visualization_msgs/MarkerArray.h>
#include<visualization_msgs/Marker.h>
#include <geometry_msgs/Point.h>
#include <std_msgs/Int32MultiArray.h>
#include <functional> 
#include <iterator>
#include <vector>

using namespace std;


void callback(const std_msgs::Int32MultiArray::ConstPtr & msg);
ros::Publisher optic_pub;

int main( int argc, char** argv )
{ 
  ros::init(argc, argv, "optic");
  ros::NodeHandle nh;
  optic_pub = nh.advertise<visualization_msgs::MarkerArray>("/optic_markers", 10);
  ros::Subscriber sub = nh.subscribe("/optic_flow", 10, callback);
  ros::spin();
}


int i = 1;
int j = 26;
int startx[24]={ 36, 18, 0,  -18,-36,36,18, 0,-18,-36,36,18,0,-18,-36,36,18,0,-18,-36, 12,-12,12,-12};
int starty[24]={-18,-18,-18,- 18,-18,-6,-6,-6,-6, -6, 6, 6, 6, 6,  6,18, 18,18,18, 18,-24,-24,24, 24};
//visualization_msgs::MarkerArray marrays;

void callback(const std_msgs::Int32MultiArray::ConstPtr & msg)
{
 
 
    visualization_msgs::MarkerArray marrays;
 
    for(auto & ma: marrays.markers)
    {
        ma.action = visualization_msgs::Marker::DELETE;
    }
    
  optic_pub.publish(marrays);

  marrays.markers.clear();


  geometry_msgs::Point q;
  geometry_msgs::Point p;
  p.x = 0;
  p.y = 0;
  p.z = 0;
  q.x = 0;
  q.y = 0;
  q.z = 0;
 
  int x = 0;
  
  for ( int k = 0; k<24; k++)
  {
        visualization_msgs::Marker ma;
	ma.header.frame_id = "/my_frame";
	ma.header.stamp = ros::Time::now();
	ma.ns = "Arrows";
	ma.id = 0 + k;
	ma.type = visualization_msgs::Marker::ARROW;
	ma.action = visualization_msgs::Marker::ADD;
	ma.pose.position.x = 0.0;
	ma.pose.position.y = 0.0;
	ma.pose.position.z = 0.0;
	ma.pose.orientation.x = 0.0;
	ma.pose.orientation.y = 0.0;
	ma.pose.orientation.z = 0.0;	
	ma.pose.orientation.w = 1.0;
	ma.scale.x = 0.3;
	ma.scale.y = 0.3;
	ma.scale.z = 0.3;
	ma.color.a = 1;
	ma.color.r = 1;
	ma.color.g = 0;
	ma.color.b = 0;

	
	q.x = startx[k];
	q.y = starty[k];
        q.z = x;
	ma.points.push_back(q);

	
	p.x = msg->data[i] + startx[k];
	p.y = msg->data[j] + starty[k];
	p.z = x;
	ma.points.push_back(p);

	marrays.markers.push_back(ma);
 
    
        i++;
	j++;

       if(i == 24 && j == 49)
	{
		break;
	}
	
}
	


i = 1;
j = 26;
	

optic_pub.publish(marrays);



}
  
  

