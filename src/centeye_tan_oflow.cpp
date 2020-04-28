#include<ros/ros.h>
#include<iostream>
#include<algorithm>
#include<std_msgs/Int32MultiArray.h>
#include<std_msgs/Float32MultiArray.h>
#include<sensor_msgs/LaserScan.h>
#include<std_msgs/Header.h>
#include <vector>
#include <iterator>
#include<math.h>
#include<std_msgs/MultiArrayDimension.h>
#include <std_msgs/Int32.h>
#include<message_filters/subscriber.h>
#include<message_filters/time_synchronizer.h>
#include <message_filters/sync_policies/approximate_time.h>
#include<centeye_oflow/FloatArray_of.h>
#include<centeye_oflow/Fourier_Coeff.h>
#include <sstream>
#include <bits/stdc++.h>
#include <opencv2/opencv.hpp>
#include <cv_bridge/cv_bridge.h>
#include<numeric>


using namespace cv_bridge;
using namespace std;
using namespace message_filters;

class TanOflow
{
	
public:

	float HA_plot[96] = {0,
	3.8779,
	7.74839,
	11.60688,
	15.44868,
	19.27138,
	23.07358,
	26.85568,
	30.62088,
	34.37458,
	38.12658,
	41.89028,
	44.26298,
	48.03458,
	51.78838,
	55.54038,
	59.30088,
	63.07698,
	66.87218,
	70.68798,
	74.52398,
	78.37718,
	82.24415,
	86.12023,
	90,
	93.8779,
	97.74839,
	101.60688,
	105.44868,
	109.27138,
	113.07358,
	116.85568,
	120.62088,
	124.37458,
	128.12658,
	131.89028,
	134.26298,
	138.03458,
	141.78838,
	145.54038,
	149.30088,
	153.07698,
	156.87218,
	160.68798,
	164.52398,
	168.37718,
	172.24415,
	176.12023,
	180,
	183.8779,
	187.74839,
	191.60688,
	195.44868,
	199.27138,
	203.07358,
	206.85568,
	210.62088,
	214.37458,
	218.12658,
	221.89028,
	224.26298,
	228.03458,
	231.78838,
	235.54038,
	239.30088,
	243.07698,
	246.87218,
	250.68798,
	254.52398,
	258.37718,
	262.24415,
	266.12023,
	270,
	273.8779,
	277.74839,
	281.60688,
	285.44868,
	289.27138,
	293.07358,
	296.85568,
	300.62088,
	304.37458,
	308.12658,
	311.89028, 
	314.26298, 
	318.03458,
	321.78838,
	325.54038,
	329.30088,
	333.07698,
	336.87218,
	340.68798,
	344.52398,
	348.37718,
	352.24415,
	356.12023
	};


	float SF[242] = {0,
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
	0.75582,
	0,
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




	TanOflow()
	{
		front_pub = n.advertise<centeye_oflow::FloatArray_of>("tang_oflow_front",10);
		right_pub = n.advertise<centeye_oflow::FloatArray_of>("tang_oflow_right",10);
		left_pub = n.advertise<centeye_oflow::FloatArray_of>("tang_oflow_left",10);
		back_pub = n.advertise<centeye_oflow::FloatArray_of>("tang_oflow_back",10);
		

		all_toflow = n.advertise<centeye_oflow::FloatArray_of>("tang_oflow_all_averaged", 10);
		horiz_ang = n.advertise<centeye_oflow::FloatArray_of>("horizontal_angle",10);
		allof = n.advertise<centeye_oflow::FloatArray_of>("tang_oflow_all", 10);
		rows1 = n.advertise<centeye_oflow::FloatArray_of>("row1",10);
		rows2 = n.advertise<centeye_oflow::FloatArray_of>("row2",10);
		rows3 = n.advertise<centeye_oflow::FloatArray_of>("row3",10);
		rows4 = n.advertise<centeye_oflow::FloatArray_of>("row4",10);

		fourier_coeffs = n.advertise<centeye_oflow::Fourier_Coeff>("/Fourier_Coeffs",10);

		front_sub = n.subscribe("optic_flow_front", 10, &TanOflow::oflow_front, this);
		right_sub = n.subscribe("optic_flow_right", 10, &TanOflow::oflow_right, this);
		left_sub = n.subscribe("optic_flow_left", 10, &TanOflow::oflow_left, this);
		back_sub = n.subscribe("optic_flow_back", 10, &TanOflow::oflow_back, this);

	}


	void oflow_front(const std_msgs::Int32MultiArray& ofront)
	{

		tan_front.data.clear();	
		tan_front.data.resize(242);
		for(int i=0; i<242; i++)
		{
						
			tan_front.data[i] = ofront.data[i] * SF[i];
			
		}

	tan_front.header.stamp =  ros::Time::now();
	front_pub.publish(tan_front);
	}


	void oflow_right(const std_msgs::Int32MultiArray& oright)
	{

		tan_right.data.clear();	
		tan_right.data.resize(242);

		for(int i=0; i<242; i++)
		{
			
			tan_right.data[i] = oright.data[i] * SF[i];
			
		}
	tan_right.header.stamp =  ros::Time::now();
	right_pub.publish(tan_right);
		
	}

	void oflow_left(const std_msgs::Int32MultiArray& oleft)
	{
		
		
		tan_left.data.clear();	
		tan_left.data.resize(242);

		for(int i=0; i<242; i++)
		{
			
						
			tan_left.data[i] = oleft.data[i] * SF[i];
			
		}

	tan_left.header.stamp =  ros::Time::now();	
	left_pub.publish(tan_left);
		
	}


	void oflow_back(const std_msgs::Int32MultiArray& oback)	{
		
		tan_back.data.clear();	
		tan_back.data.resize(242);

		for(int i=0; i<242; i++)
		{
				
			tan_back.data[i] = oback.data[i] * SF[i];
			
		}
	tan_back.header.stamp =  ros::Time::now();
	back_pub.publish(tan_back);


	average();	
	controller();
	}

	void average()
	{
		
		float front_x[4][24] = {};
		float front_y[4][24] = {};
		float right_x[4][24]= {};
		float right_y[4][24]= {};
		float ha[4][24]= {};

		float left_x[4][24]= {};
		float left_y[4][24]= {};
		float back_x[4][24]= {};
		float back_y[4][24]= {};

		float sum_right_y[24]= {};
		float sum_left_y[24]= {};
		float sum_back_y[24]= {};
		float sum_front_y[24]= {};

		float sum_right_xavg[24]= {};
		float sum_left_xavg[24]= {};
		float sum_back_xavg[24]= {};
		float sum_front_xavg[24]= {};

		float sum_right_yavg[24]= {};
		float sum_left_yavg[24]= {};
		float sum_back_yavg[24]= {};
		float sum_front_yavg[24]= {};

		float sum_right_x[24]= {};
		float sum_left_x[24]= {};
		float sum_back_x[24]= {};
		float sum_front_x[24]= {};

		float sum_ha[24]= {};	
		float sum_haavg[24]= {};

		float overall[4][96]= {};

		float sum_overall_x[96] = {}; 
		//float avg_overall_x[96] = {};

		float last[48] = {};
		float first[48]={};

		tan_allavg_x.data.clear();	
		tan_allavg_x.data.resize(96);

		row1.data.clear();	
		row1.data.resize(96);
		
		row2.data.clear();	
		row2.data.resize(96);

		row3.data.clear();	
		row3.data.resize(96);

		row4.data.clear();	
		row4.data.resize(96);
		
		horizontal_angle.data.clear();
		horizontal_angle.data.resize(96);

		dat.data.clear();
		dat.data.resize(384);

		int a = 0;

			for(int u= 25; u<37;u++)
			{
				last[a] = tan_front.data[u];
				last[a+12]= tan_front.data[u+24];
				last[a+24]= tan_front.data[u+48];
				last[a+36]= tan_front.data[u+72];
				++a;	 
			}
			
		int b = 0;

			for(int u= 37; u<49;u++)
			{
				first[b] = tan_front.data[u];
				first[b+12]= tan_front.data[u+24];
				first[b+24]= tan_front.data[u+48];
				first[b+36]= tan_front.data[u+72];
				++b;	 
			}
				


		int v=0;
		for(int i=0;i<4;i++)
		{
			for(int j= 0;j<12;j++)
			{

				overall[i][j] = first[v];
				++v;
			}	
		}




		int xy=25;
		for(int i=0; i<4;i++)
		{
			for(int j = 12; j<36;j++)
			{
				overall[i][j] = tan_right.data[xy];
				overall[i][j+24] = tan_back.data[xy];
				overall[i][j+48] = tan_left.data[xy];
				++xy;
			}

		}


		int t=0;
		for(int i=0;i<4;i++)
		{
			for(int j=84;j<96;j++)
			{

				overall[i][j] = last[t];
				++t;
			}	
		}



		    std::vector<float> vec(384, 0.0);
		    for (int i=0; i<4; i++)
			for (int j=0; j<96; j++)
			    vec[i*96 + j] = overall[i][j];
		    dat.data = vec;

		
		for(int i = 0; i<96;i++)
		{
			row1.data[i] = dat.data[i];
			row2.data[i] = dat.data[i+96];
			row3.data[i] = dat.data[i+192];
			row4.data[i]= dat.data[i+288];
		}
		

		for(int j=0; j<96;j++)
		{
			//std::cout<< "........................................................................"<<std::endl;
			for(int i = 0; i<4;i++)
			{		
				sum_overall_x[j] += overall[i][j];

			}
			
			avg_overall_x[j] = sum_overall_x[j]/4; 

		}



		for(int i=0;i<96;i++)
		{
			tan_allavg_x.data[i] = avg_overall_x[i];

		}

		horizontal_angle.header.stamp =  ros::Time::now();
		for(int i=0;i<96;i++)
		{
			horizontal_angle.data[i] = HA_plot[i];
		}
			

		tan_allavg_x.header.stamp = ros::Time::now();
		all_toflow.publish(tan_allavg_x);

		horizontal_angle.header.stamp =  ros::Time::now();
		horiz_ang.publish(horizontal_angle);

		dat.header.stamp =  ros::Time::now();
		allof.publish(dat);

		row1.header.stamp = ros::Time::now();
		rows1.publish(row1);

		row2.header.stamp = ros::Time::now();
		rows2.publish(row2);

		row3.header.stamp = ros::Time::now();
		rows3.publish(row3);

		row4.header.stamp = ros::Time::now();
		rows4.publish(row4);


	}

	void controller()
	{
		
		msg.a.clear();
		msg.a.resize(5);
		
		msg.b.clear();
		msg.b.resize(5);

		int num_horiz_of = 96; 
		int num_horiz_fourier_terms = 2;
		float horiz_scan_limit = M_PI;

	
		float h_a[5], h_b[5];
		float h_dg = 0;
		float h_dg_pi = 0;
		float horiz_cos_gamma_arr[ num_horiz_fourier_terms + 1][num_horiz_of];
		float horiz_sin_gamma_arr[ num_horiz_fourier_terms + 1][num_horiz_of];

		h_dg = (2*horiz_scan_limit)/num_horiz_of;

		//h_dg_pi = 2/num_horiz_of;


		cv::Mat h_cos_gamma_matcv(num_horiz_fourier_terms + 1, num_horiz_of, CV_32FC1, horiz_cos_gamma_arr);
		cv::Mat h_sin_gamma_matcv(num_horiz_fourier_terms + 1,  num_horiz_of , CV_32FC1, horiz_sin_gamma_arr);
		cv::Mat average_of(1,96,CV_32FC1, avg_overall_x);

		
		for(int i=0; i< num_horiz_fourier_terms+1; i++)
		{
			for(int j=0; j< num_horiz_of; j++)
			{
				horiz_cos_gamma_arr[i][j] = cos( i*HA_plot[j]/360*2*M_PI);
				horiz_sin_gamma_arr[i][j] = sin( i*HA_plot[j]/360*2*M_PI);
			}	
				
			h_a[i] = average_of.dot(h_cos_gamma_matcv.row(i)) * h_dg / M_PI;
			h_b[i] = average_of.dot(h_sin_gamma_matcv.row(i)) * h_dg / M_PI;				
		
		}

		std::vector<float> h_a_vector(h_a, h_a + sizeof h_a / sizeof h_a[0]);
		std::vector<float> h_b_vector(h_b, h_b + sizeof h_b / sizeof h_b[0]);

		

		
		msg.a = h_a_vector;
		msg.b = h_b_vector;
		
		msg.header.stamp = ros::Time::now();
		fourier_coeffs.publish(msg);


	}



private:
	
	ros::NodeHandle n;

	ros::Publisher front_pub;
	ros::Publisher right_pub;
	ros::Publisher left_pub;
	ros::Publisher back_pub;
	
	ros::Publisher all_toflow;
	ros::Publisher horiz_ang;
	ros::Publisher allof;
	ros::Publisher rows1;
	ros::Publisher rows2;
	ros::Publisher rows3;
	ros::Publisher rows4;

	ros::Publisher fourier_coeffs;	

	ros::Subscriber front_sub;
	ros::Subscriber right_sub;
	ros::Subscriber left_sub;
	ros::Subscriber back_sub;
	ros::Subscriber laser_sub;

	centeye_oflow::FloatArray_of tan_front;
	centeye_oflow::FloatArray_of tan_right;
	centeye_oflow::FloatArray_of tan_left;
	centeye_oflow::FloatArray_of tan_back;

	centeye_oflow::FloatArray_of tan_allavg_x;
	centeye_oflow::FloatArray_of horizontal_angle;
	centeye_oflow::FloatArray_of dat;
	centeye_oflow::FloatArray_of row1;
	centeye_oflow::FloatArray_of row2;
	centeye_oflow::FloatArray_of row3;
	centeye_oflow::FloatArray_of row4;
	float avg_overall_x[96] = {};
	centeye_oflow::Fourier_Coeff msg;

};


int main(int argc, char **argv)
{

	ros::init(argc, argv, "centeye_tan_oflow");

	TanOflow toflow;
	ros::spin();

	return 0;
}
