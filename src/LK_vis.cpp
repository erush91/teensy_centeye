#include <ros/ros.h>
#include <visualization_msgs/MarkerArray.h>
#include<visualization_msgs/Marker.h>
#include <geometry_msgs/Point.h>
#include <std_msgs/Float32MultiArray.h>
#include <functional> 
#include<math.h>

using namespace std;


void callback(const std_msgs::Float32MultiArray::ConstPtr & msg);
ros::Publisher optic_pub;

int main( int argc, char** argv )
{ 
  ros::init(argc, argv, "optic");
  ros::NodeHandle nh;
  optic_pub = nh.advertise<visualization_msgs::MarkerArray>("/optic_markers", 10);
  ros::Subscriber sub = nh.subscribe("/optic_flow", 10, callback);
  ros::spin();
}


int i = 0;
int j = 24;    //1   2   3    4   5   6  7  8  9   10 11 12 13 14 15 16  17 18 19 20   21  22 23  24
int startx[24]={ 36, 18, 0,  -18,-36,36,18, 0,-18,-36,36,18,0,-18,-36,36,18,0,-18,-36, 12,-12, 12,-12};
int starty[24]={-18,-18,-18,- 18,-18,-6,-6,-6,-6, -6, 6, 6, 6, 6,  6,18, 18,18,18, 18,-24,-24,24, 24};
//visualization_msgs::MarkerArray marrays;
//36,-18  18,-18
void callback(const std_msgs::Float32MultiArray::ConstPtr & msg)
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
 
  float of_x;
  float of_y;
  float mag;
  for ( int k = 0; k<24; k++)
  {
		visualization_msgs::Marker ma;
		ma.header.frame_id = "/my_frame";
		ma.header.stamp = ros::Time::now();
		ma.ns = "Arrows";
		ma.id = 1 + k;
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
		ma.scale.y = 2;
		ma.scale.z = 1;
		ma.color.a = 1;
		ma.color.r = 1;
		ma.color.g = 0;
		ma.color.b = 0;

		of_x = msg->data[i]*10000 ;
		of_y = msg->data[j]*10000 ;
		mag = sqrt(of_x * of_x + of_y * of_y);
		cout << mag << "    ";

		// Limit magnitude to 10
		if (mag > 10)
		{
			of_x = of_x * 10 / mag;
			of_y = of_y * 10 / mag;
		}

        q.x = startx[k];
        q.y = starty[k];
        q.z = 0.0;
        p.x = startx[k] + of_x;
        p.y = starty[k] + of_y;
        p.z = 0.0;

		ma.points.push_back(q);
		ma.points.push_back(p);

		marrays.markers.push_back(ma);
	
    
        i++;
	    j++;

       if(i > 23 && j > 48)
		{
		break;
		}
	
	}
	


    i = 0;
    j = 24;
	

    optic_pub.publish(marrays);



}
  

