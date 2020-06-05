#include <ros/ros.h>
#include <visualization_msgs/MarkerArray.h>
#include <visualization_msgs/Marker.h>
#include <geometry_msgs/Point.h>
#include <std_msgs/Float32MultiArray.h>
#include <functional> 		
#include<math.h>
#include <iostream>

using namespace std;
#define M_PI 3.14159265358979323846


/*
0.6218,
0.31377,
2.0679E-17,
-0.31377,
-0.6218,
0.62327,
0.31522,
6.93E-18,
-0.31522,
-0.62327,
0.62327,
0.31522,
6.93E-18,
-0.31522,
-0.62327,
0.6218,
0.31377,
2.0679E-17,
-0.31377,
-0.6218,
0.20146,
-0.20146,
0.20146,
-0.20146,
*/


double HA[120] = {0.6218,
0.31377,
2.0679E-17,
-0.31377,
-0.6218,
0.62327,
0.31522,
6.93E-18,
-0.31522,
-0.62327,
0.62327,
0.31522,
6.93E-18,
-0.31522,
-0.62327,
0.6218,
0.31377,
2.0679E-17,
-0.31377,
-0.6218,
0.20146,
-0.20146,
0.20146,
-0.20146,
0.75334,
0.68726,
0.62152,
0.55586,
0.49008,
0.42409,
0.35781,
0.29123,
0.22436,
0.15723,
0.089916,
0.022487,
-0.044971,
-0.11237,
-0.17963,
-0.24668,
-0.31346,
-0.37994,
-0.44612,
-0.51203,
-0.57775,
-0.64342,
-0.70923,
-0.77551,
0.75392,
0.68846,
0.62312,
0.55769,
0.49201,
0.42599,
0.35958,
0.29278,
0.22562,
0.15815,
0.090455,
0.022623,
-0.045243,
-0.11304,
-0.18067,
-0.24804,
-0.31509,
-0.38176,
-0.44804,
-0.51394,
-0.57952,
-0.6449,
-0.71025,
-0.77582,
0.75406,
0.68867,
0.62338,
0.55797,
0.4923,
0.42627,
0.35984,
0.293,
0.2258,
0.15828,
0.090532,
0.022643,
-0.045282,
-0.11314,
-0.18082,
-0.24824,
-0.31533,
-0.38203,
-0.44832,
-0.51423,
-0.5798,
-0.64515,
-0.71044,
-0.77592,
0.75346,
0.68764,
0.62207,
0.55651,
0.49078,
0.42478,
0.35846,
0.2918,
0.22482,
0.15757,
0.090116,
0.022538,
-0.045072,
-0.11262,
-0.18002,
-0.24718,
-0.31406,
-0.38061,
-0.44681,
-0.51271,
-0.57837,
-0.64391,
-0.70953,
-0.77551};


/*
-0.33311,
-0.33618,
-0.33771,
-0.33618,
-0.33311,
-0.1113,
-0.11258,
-0.11318,
-0.11258,
-0.1113,
0.1113,
0.11258,
0.11318,
0.11258,
0.1113,
0.33311,
0.33618,
0.33771,
0.33618,
0.33311,
-0.44768,
-0.44768,
0.44768,
0.44768,
*/


double VA[120]={-0.33311,
-0.33618,
-0.33771,
-0.33618,
-0.33311,
-0.1113,
-0.11258,
-0.11318,
-0.11258,
-0.1113,
0.1113,
0.11258,
0.11318,
0.11258,
0.1113,
0.33311,
0.33618,
0.33771,
0.33618,
0.33311,
-0.44768,
-0.44768,
0.44768,
0.44768,
-0.36559,
-0.3658,
-0.36625,
-0.36686,
-0.36756,
-0.36829,
-0.36899,
-0.36964,
-0.37019,
-0.37062,
-0.3709,
-0.37104,
-0.37101,
-0.37083,
-0.37049,
-0.37002,
-0.36943,
-0.36876,
-0.36805,
-0.36732,
-0.36665,
-0.36608,
-0.3657,
-0.3656,
-0.14413,
-0.14435,
-0.14465,
-0.145,
-0.14537,
-0.14573,
-0.14608,
-0.14639,
-0.14665,
-0.14685,
-0.14699,
-0.14705,
-0.14704,
-0.14695,
-0.14679,
-0.14657,
-0.14629,
-0.14597,
-0.14561,
-0.14524,
-0.14488,
-0.14455,
-0.14427,
-0.14408,
0.077623,
0.077753,
0.077923,
0.078116,
0.078321,
0.078523,
0.078714,
0.078886,
0.07903,
0.079141,
0.079215,
0.07925,
0.079243,
0.079195,
0.079108,
0.078985,
0.078831,
0.078652,
0.078456,
0.078252,
0.07805,
0.077863,
0.077704,
0.077592,
0.29917,
0.29946,
0.29993,
0.30051,
0.30116,
0.30182,
0.30245,
0.30302,
0.30351,
0.30389,
0.30414,
0.30426,
0.30423,
0.30407,
0.30378,
0.30336,
0.30284,
0.30225,
0.3016,
0.30094,
0.30031,
0.29975,
0.29933,
0.29913};

double HAdegree[120],VAdegree[120];


void callback(const std_msgs::Float32MultiArray::ConstPtr & msg);

ros::Publisher optic_pub;

int main( int argc, char** argv )
{ 

  for(int r = 0; r<120; r++)
	{
	HAdegree[r] = HA[r] * (180/M_PI);	
	VAdegree[r] = VA[r] * (180/M_PI);
	}

  ros::init(argc, argv, "optic");
  ros::NodeHandle nh;
  optic_pub = nh.advertise<visualization_msgs::MarkerArray>("/optic_markers", 10);
  ros::Subscriber sub = nh.subscribe("/optic_flow", 10, callback);
  ros::spin();
}


int i = 0;
int j = 120;    


 
//visualization_msgs::MarkerArray marrays;

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
  for ( int k = 0; k<120; k++)
  {
        visualization_msgs::Marker ma;
	ma.header.frame_id = "/centeye";
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

	of_x = msg->data[i] / 5;
	of_y = msg->data[j] / 5;
	mag = sqrt(of_x * of_x + of_y * of_y);
	
	// PRINT OPTIC FLOW MAGNITUDE (DEBUGGING)
	// cout << mag << "    ";

	// Limit magnitude to 10
	if (mag > 10)
	{
		of_x = of_x * 10 / mag;
	    of_y = of_y * 10 / mag;
	}

        q.x = - HAdegree[k];
        q.y = VAdegree[k];
        q.z = 0.0;
        p.x = - HAdegree[k] + of_x;
        p.y = VAdegree[k] + of_y;
        p.z = 0.0;

	ma.points.push_back(q);
	ma.points.push_back(p);

	marrays.markers.push_back(ma);
 
    
        i++;
	j++;

       if(i > 120 && j > 240)
	{
		break;
	}
	
}
	


i = 0;
j = 120;
	

optic_pub.publish(marrays);



}








 

 

  
