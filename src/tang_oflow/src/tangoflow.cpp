#include <ros/ros.h>
#include <iostream>
#include<std_msgs/Int32MultiArray.h>
#include<std_msgs/Float32MultiArray.h>
#include <vector>
#include<std_msgs/MultiArrayDimension.h>
#include <std_msgs/Int32.h>
#include<my_msgs/FloatArray_of.h>
#include<stdio.h>

using namespace std;

void oflow_front(const std_msgs::Int32MultiArray &ofront);
void oflow_right(const std_msgs::Int32MultiArray &oright);
void oflow_left(const std_msgs::Int32MultiArray &oleft);
void oflow_back(const std_msgs::Int32MultiArray &oback);
//void oflow_all();


my_msgs::FloatArray_of tan_front;
my_msgs::FloatArray_of tan_right;
my_msgs::FloatArray_of tan_left;
my_msgs::FloatArray_of tan_back;
//std_msgs::Float32MultiArray tan_all;


/*double x_front[242];
double x_right[242];
double x_left[242];
double x_back[242];
double tang_all[968];

double tan_x_front[121];
double tan_x_right[121];
double tan_x_left[121];
double tan_x_back[121];

double tan_y_front[121];
double tan_y_right[121];
double tan_y_left[121];
double tan_y_back[121];*/

double SF[121] = {0,
0.81193,
0.91187,
0.95088,
0.91187,
0.81193,
0.84337,
0.95172,
0.99429,
0.95172,
0.84337,
0.84337,
0.95172,
0.99429,
0.95172,
0.84337,
0.81193,
0.91187,
0.95088,
0.91187,
0.81193,
0.90056,
0.90056,
0.90056,
0.90056,
0.75308,
0.77935,
0.80484,
0.8292,
0.85207,
0.87308,
0.89185,
0.90802,
0.92127,
0.93132,
0.93793,
0.94097,
0.94036,
0.93612,
0.92834,
0.9172,
0.90294,
0.88586,
0.86631,
0.84464,
0.82123,
0.79645,
0.77067,
0.7442,
0.78426,
0.8128,
0.84056,
0.86717,
0.89221,
0.91527,
0.93592,
0.95375,
0.96838,
0.97948,
0.9868,
0.99017,
0.98949,
0.9848,
0.97619,
0.96388,
0.94814,
0.92933,
0.90783,
0.88406,
0.85845,
0.83142,
0.80336,
0.77464,
0.78852,
0.81738,
0.84546,
0.87238,
0.89773,
0.92108,
0.94199,
0.96005,
0.97488,
0.98613,
0.99356,
0.99697,
0.99628,
0.99152,
0.9828,
0.97032,
0.95437,
0.93532,
0.91354,
0.88948,
0.86356,
0.8362,
0.80783,
0.7788,
0.76498,
0.79211,
0.81845,
0.84365,
0.86734,
0.88912,
0.90859,
0.92538,
0.93915,
0.94959,
0.95647,
0.95963,
0.959,
0.95458,
0.94649,
0.93491,
0.9201,
0.90238,
0.88209,
0.85964,
0.8354,
0.80978,
0.78314,
0.75582};


ros::Publisher front_pub;
ros::Publisher right_pub;
ros::Publisher left_pub;
ros::Publisher back_pub;
ros::Publisher alltan_pub;


int main(int argc, char **argv) 
	{

	ros::init(argc, argv, "tangoflow");
	ros::NodeHandle n;
	
	ROS_INFO(" 1 ");

	front_pub = n.advertise<my_msgs::FloatArray_of>("tang_oflow_front",10);
	right_pub = n.advertise<my_msgs::FloatArray_of>("tang_oflow_right",10);
	left_pub = n.advertise<my_msgs::FloatArray_of>("tang_oflow_left",10);
        back_pub = n.advertise<my_msgs::FloatArray_of>("tang_oflow_back",10);
	

	

	ros::Subscriber front_sub = n.subscribe("optic_flow_front", 10, oflow_front);
	ros::Subscriber right_sub = n.subscribe("optic_flow_right", 10, oflow_right);
	ros::Subscriber left_sub = n.subscribe("optic_flow_left", 10, oflow_left);
	ros::Subscriber back_sub = n.subscribe("optic_flow_back", 10, oflow_back);
	

	/*tan_all.data.clear();	
	tan_all.layout.dim.push_back(std_msgs::MultiArrayDimension());
	tan_all.data.resize(968);
  	tan_all.layout.dim[0].label = "length0";
  	tan_all.layout.dim[0].size = 968;
  	tan_all.layout.dim[0].stride = 968;
  	tan_all.layout.data_offset = 0;


	for(int i=0; i<242;i++)
	{
		tan_all.data[i] = tan_front.data[i];
	}
	
	for(int i=242;i<484;i++)
	{
		tan_all.data[i] = tan_right.data[i];
	}

	for(int i=484;i<726;i++)
	{
		tan_all.data[i] = tan_left.data[i];
	}

	for(int i=726;i<968;i++)
	{
		tan_all.data[i] = tan_back.data[i];
	}*/

	ros::spin();

	return 0;

	
	}
	

	void oflow_front(const std_msgs::Int32MultiArray  &ofront)
	{

		tan_front.data.clear();	
		tan_front.data.resize(242);

		 printf( "sizeof( tanfront) = %d\n",  sizeof(tan_front) );

		for(int i=0; i<121; i++)
		{
			ROS_INFO(" 2 ");
			tan_front.header.stamp =  ros::Time::now();			
			tan_front.data[i] = ofront.data[i] * SF[i];
			tan_front.data[121+i] = ofront.data[121+i] * SF[i];
			
		}


		/*for(int i=0; i<121;i++)
		{
			ROS_INFO(" 2 ");
			tan_front.header.stamp =  ros::Time::now();	
			tan_front.data[121+i] = ofront.data[121+i] * SF[i];
			
		}*/


	front_pub.publish(tan_front);
	}


	void oflow_right(const std_msgs::Int32MultiArray &oright)
	{

		tan_right.data.clear();	
		tan_right.data.resize(242);

		ROS_INFO(" 3 ");
		for(int i=0; i<121; i++)
		{
			
			tan_right.header.stamp =  ros::Time::now();				
			tan_right.data[i] = oright.data[i] * SF[i];
			tan_right.data[121+i] = oright.data[121+i] * SF[i];
			
		}


		/*for(int i=0; i<121;i++)
		{
			
			tan_right.header.stamp =  ros::Time::now();				
			tan_right.data[121+i] = oright.data[121+i] * SF[i];
		
		}*/


	right_pub.publish(tan_right);
		
	}

	void oflow_left(const std_msgs::Int32MultiArray &oleft)
	{
		
		
		tan_left.data.clear();	
		tan_left.data.resize(242);

		ROS_INFO(" 4 ");
		for(int i=0; i<121; i++)
		{
			
			tan_left.header.stamp =  ros::Time::now();				
			tan_left.data[i] = oleft.data[i] * SF[i];
			tan_left.data[121+i] = oleft.data[121+i] * SF[i];
			
		}


		/*for(int i=0; i<121;i++)
		{
			tan_left.header.stamp =  ros::Time::now();		
			tan_left.data[121+i] = oleft.data[121+i] * SF[i];
		
		}*/


	left_pub.publish(tan_left);
		
	}


	void oflow_back(const std_msgs::Int32MultiArray &oback)
	{
			ROS_INFO(" 5 ");
	
		
		tan_back.data.clear();	
		tan_back.data.resize(242);

		for(int i=0; i<121; i++)
		{
			tan_back.header.stamp =  ros::Time::now();	
			tan_back.data[i] = oback.data[i] * SF[i];
			tan_back.data[121+i] = oback.data[121+i] * SF[i];
			
		}


		/*for(int i=0; i<121;i++)
		{
			tan_back.header.stamp =  ros::Time::now();	
			tan_back.data[121+i] = oback.data[121+i] * SF[i];
		
		}*/


	back_pub.publish(tan_back);
		
	}


		


		


	













