#include<ros/ros.h>
#include<my_msgs/FloatArray_of.h>
#include<my_msgs/Fourier_Coeff.h>
#include<iostream>
#include<math.h>
#include <opencv2/opencv.hpp>
#include <cv_bridge/cv_bridge.h>
#include<numeric>
#include<message_filters/subscriber.h>
#include<message_filters/time_synchronizer.h>
#include <message_filters/sync_policies/approximate_time.h>
#include <sstream>
#include <bits/stdc++.h>

using namespace cv_bridge;
using namespace std;
using namespace message_filters;

ros::Publisher fourier_coeffs;
my_msgs::Fourier_Coeff msg;

void horiz_fourier_coeff(const my_msgs::FloatArray_of::ConstPtr &avg_of, const my_msgs::FloatArray_of::ConstPtr &all_of,const my_msgs::FloatArray_of::ConstPtr &gamma)
{
 
	int num_horiz_of = 96; 

	int num_horiz_fourier_terms = 2;
	float horiz_scan_limit = 2 * M_PI;

	float horiz_gamma_vec[96] = {};
	float avg_off[96] = {};
	float h_a[5], h_b[5];
	float h_dg = 0;
	float h_dg_pi = 0;

	float horiz_cos_gamma_arr[ num_horiz_fourier_terms + 1][num_horiz_of];
	float horiz_sin_gamma_arr[ num_horiz_fourier_terms + 1][num_horiz_of];

	h_dg = 2 * (horiz_scan_limit/num_horiz_of);

	h_dg_pi = h_dg / M_PI;

	for(int i=0;i<96;i++)
	{
		horiz_gamma_vec[i] = gamma->data[i];
		avg_off[i] = avg_of->data[i];
	}

	cv::Mat h_cos_gamma_matcv(num_horiz_fourier_terms + 1, num_horiz_of, CV_32FC1, horiz_cos_gamma_arr);
	cv::Mat h_sin_gamma_matcv(num_horiz_fourier_terms + 1,  num_horiz_of , CV_32FC1, horiz_sin_gamma_arr);
	cv::Mat average_of(1,96,CV_32FC1, avg_off);

	
	for(int i=0; i< num_horiz_fourier_terms+1; i++)
	{
		for(int j=0; j< num_horiz_of; j++)
		{
			horiz_cos_gamma_arr[i][j] = cos( i * horiz_gamma_vec[j]);
			horiz_sin_gamma_arr[i][j] = sin( i * horiz_gamma_vec[j]);
		}	
			
		h_a[i] = average_of.dot(h_cos_gamma_matcv.row(i)) * h_dg_pi;
		h_b[i] = average_of.dot(h_sin_gamma_matcv.row(i)) * h_dg_pi;				
	
	}

	std::vector<float> h_a_vector(h_a, h_a + sizeof h_a / sizeof h_a[0]);
        std::vector<float> h_b_vector(h_b, h_b + sizeof h_b / sizeof h_b[0]);

	msg.header.stamp = ros::Time::now();
	msg.a = h_a_vector;
	msg.b = h_b_vector;
	std::cout<< "........................................................................"<<std::endl;
	fourier_coeffs.publish(msg);
}

int main( int argc, char **argv)
{

	ros::init(argc, argv, "controller");
	ros::NodeHandle nh;

	fourier_coeffs = nh.advertise<my_msgs::Fourier_Coeff>("/Fourier_Coeffs",10);

	message_filters::Subscriber<my_msgs::FloatArray_of>avgof(nh, "/tang_oflow_all_averaged",10);
	message_filters::Subscriber<my_msgs::FloatArray_of>allof(nh, "/tang_oflow_all",10);
	message_filters::Subscriber<my_msgs::FloatArray_of>h_gamma(nh, "/horizontal_angle",10);

	typedef sync_policies::ApproximateTime<my_msgs::FloatArray_of, my_msgs::FloatArray_of, my_msgs::FloatArray_of> MySyncPolicy;
	Synchronizer<MySyncPolicy> sync(MySyncPolicy(10), avgof, allof , h_gamma);
	sync.registerCallback(boost::bind(&horiz_fourier_coeff, _1,_2,_3));

	ros::spin();
	return 0;
}

