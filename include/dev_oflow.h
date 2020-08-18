#ifndef OFLOW_H
#define OFLOW_H

#include<ros/ros.h>
#include <ros/console.h>
#include<iostream>
#include<algorithm>
#include<std_msgs/Int8MultiArray.h>
#include<std_msgs/Float32MultiArray.h>
#include<sensor_msgs/LaserScan.h>
#include<std_msgs/Header.h>
#include <vector>
#include <array>
#include <iterator>
#include<math.h>
#include<std_msgs/MultiArrayDimension.h>
#include <std_msgs/Int32.h>
#include<message_filters/subscriber.h>
#include<message_filters/time_synchronizer.h>
#include <message_filters/sync_policies/approximate_time.h>
#include<teensy_centeye/FloatArray_of.h>
#include<teensy_centeye/Fourier_Coeff.h>
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

    int oflow_size;

    float HA_HT[96] = {0,
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

	float HA_LK[20] = {0,18,36,54,72,90,108,126,144,162,180,198,216,234,252,270,288,306,324,342};


	float SF_HT[242] = {1.0,
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
	1.0,
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

	float SF_LK[48]={0.81193,
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
	0.90056
};

	int	row_size;
	int	col_size;
	int	dat_size;
	int	dat_start;

	int	last_size;
	int	last_inc;
	int	last_col_start;
	int	last_col_end;

	int	first_size;
	int	first_inc;
	int	first_col_start;
	int	first_col_end;

	int	data_inc;

	int	overall_first_start;
	int	overall_middle_start;
	int	overall_last_start;

	int	overall_first_end;
	int	overall_middle_end;
	int	overall_last_end;

	// Fourier Coefficients
	float turn_rate, gain_k2, gain_k1;
	int	num_horiz_of;
	int num_horiz_fourier_terms = 2;
	float horiz_scan_limit = M_PI;
	
	float h_a[5], h_b[5];
	float h_dg = 0;
	float h_dg_pi = 0;

	TanOflow();

	void init(int of_size);
    void oflow_front(const std_msgs::Int8MultiArray& ofront);
	void oflow_right(const std_msgs::Int8MultiArray& oright);
	void oflow_left(const std_msgs::Int8MultiArray& oleft);
	void oflow_back(const std_msgs::Int8MultiArray& oback);

    void average();
    void fourier_coeffec();
	void computeRobotTurnRate();

private:
	
	ros::NodeHandle n;

	ros::Publisher front_pub;
	ros::Publisher right_pub;
	ros::Publisher left_pub;
	ros::Publisher back_pub;
	
	ros::Publisher all_toflow;
	ros::Publisher horiz_ang_ht;
	ros::Publisher horiz_ang_lk;
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
	
    teensy_centeye::FloatArray_of tan_front;
	teensy_centeye::FloatArray_of tan_right;
	teensy_centeye::FloatArray_of tan_left;
	teensy_centeye::FloatArray_of tan_back;

	teensy_centeye::FloatArray_of tan_allavg_x;
	teensy_centeye::FloatArray_of horizontal_angle_ht;
	teensy_centeye::FloatArray_of horizontal_angle_lk;
	teensy_centeye::FloatArray_of dat;
	teensy_centeye::FloatArray_of row1;
	teensy_centeye::FloatArray_of row2;
	teensy_centeye::FloatArray_of row3;
	teensy_centeye::FloatArray_of row4;
	teensy_centeye::Fourier_Coeff msg;

	bool flag_LK;
	bool flag_HT;

};
#endif
