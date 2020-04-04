#include<ros/ros.h>
#include<iostream>
#include<algorithm>
#include<std_msgs/Int32MultiArray.h>
#include<std_msgs/Float32MultiArray.h>
#include<std_msgs/Header.h>
#include <vector>
#include <iterator>
#include<std_msgs/MultiArrayDimension.h>
#include <std_msgs/Int32.h>
#include<message_filters/subscriber.h>
#include<message_filters/time_synchronizer.h>
#include <message_filters/sync_policies/approximate_time.h>
#include<my_msgs/FloatArray_of.h>
//#include <boost/bind.hpp>
//#include <boost/function.hpp>
//#include <boost/thread.hpp>
#include <sstream>
#include <bits/stdc++.h>

//Refer : http://aravindkrishnan.com/example_of_message_filters/

void callback(const my_msgs::FloatArray_of::ConstPtr &tfront, const my_msgs::FloatArray_of::ConstPtr &tright,const my_msgs::FloatArray_of::ConstPtr &tleft,const my_msgs::FloatArray_of::ConstPtr &tback);

using namespace std;
using namespace message_filters;

my_msgs::FloatArray_of tan_all;
my_msgs::FloatArray_of horizontal_angle;
//std_msgs::Float32MultiArray dat;
my_msgs::FloatArray_of dat;
my_msgs::FloatArray_of row1;
my_msgs::FloatArray_of row2;
my_msgs::FloatArray_of row3;
my_msgs::FloatArray_of row4;

ros::Publisher all_toflow;
ros::Publisher horiz_ang;
ros::Publisher allof;
ros::Publisher rows1;
ros::Publisher rows2;
ros::Publisher rows3;
ros::Publisher rows4;


float HA[96] = {43.1632,
39.3771,
35.6105,
31.8484,
28.0795,
24.2986,
20.501,
16.6862,
12.8549,
9.00862,
5.15181,
1.28841,
-2.57665,
-6.43833,
-10.292,
-14.1337,
-17.9599,
-21.769,
-25.5608,
-29.3372,
-33.1026,
-36.8653,
-40.6359,
-44.4334,
43.1964,
39.4459,
35.7021,
31.9533,
28.1901,
24.4074,
20.6024,
16.7751,
12.9271,
9.06133,
5.18269,
1.2962,
-2.59223,
-6.47671,
-10.3516,
-14.2116,
-18.0533,
-21.8732,
-25.6708,
-29.4466,
-33.2041,
-36.95,
-40.6943,
-44.4512,
43.2045,
39.4579,
35.717,
31.9693,
28.2067,
24.4235,
20.6173,
16.7877,
12.9374,
9.06878,
5.1871,
1.29735,
-2.59447,
-6.48244,
-10.3602,
-14.2231,
-18.0671,
-21.8887,
-25.6868,
-29.4632,
-33.2201,
-36.9644,
-40.7052,
-44.4569,
43.1701,
39.3989,
35.642,
31.8857,
28.1196,
24.3381,
20.5382,
16.7189,
12.8812,
9.0281,
5.16327,
1.29133,
-2.58244,
-6.45265,
-10.3144,
-14.1624,
-17.9943,
-21.8073,
-25.6003,
-29.3761,
-33.1382,
-36.8933,
-40.6531,
-44.4334};



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

/*float front_x[4][24];
float front_y[4][24];
float right_x[4][24];
float right_y[4][24];
float ha[4][24];

float left_x[4][24];
float left_y[4][24];
float back_x[4][24];
float back_y[4][24];

float sum_right_y[24];
float sum_left_y[24];
float sum_back_y[24];
float sum_front_y[24];

float sum_right_x[24];
float sum_left_x[24];
float sum_back_x[24];
float sum_front_x[24];

float sum_ha[24];*/

int main( int argc, char **argv)
{
	ros::init(argc,argv, "tanoverall");
	ros::NodeHandle h;

	//all_toflow = h.advertise<std_msgs::Float32MultiArray>("tang_oflow_all", 10);
        all_toflow = h.advertise<my_msgs::FloatArray_of>("tang_oflow_all_averaged", 10);
	horiz_ang = h.advertise<my_msgs::FloatArray_of>("horizontal_angle",10);
	//allof = h.advertise<std_msgs::Float32MultiArray>("all_tan_oflow",10);
	allof = h.advertise<my_msgs::FloatArray_of>("tang_oflow_all", 10);
	rows1 = h.advertise<my_msgs::FloatArray_of>("row1",10);
	rows2 = h.advertise<my_msgs::FloatArray_of>("row2",10);
	rows3 = h.advertise<my_msgs::FloatArray_of>("row3",10);
	rows4 = h.advertise<my_msgs::FloatArray_of>("row4",10);

	
	message_filters::Subscriber<my_msgs::FloatArray_of>sub_front(h, "/tang_oflow_front",10);
	message_filters::Subscriber<my_msgs::FloatArray_of>sub_right(h, "/tang_oflow_right",10);
	message_filters::Subscriber<my_msgs::FloatArray_of>sub_left(h, "/tang_oflow_left",10);
	message_filters::Subscriber<my_msgs::FloatArray_of>sub_back(h, "/tang_oflow_back",10);


	

	typedef sync_policies::ApproximateTime<my_msgs::FloatArray_of, my_msgs::FloatArray_of, my_msgs::FloatArray_of, my_msgs::FloatArray_of> MySyncPolicy;
	//TimeSynchronizer<my_msgs::FloatArray_of, my_msgs::FloatArray_of, my_msgs::FloatArray_of,my_msgs::FloatArray_of> sync(sub_front, sub_right, sub_left, sub_back, 10);
	
	Synchronizer<MySyncPolicy> sync(MySyncPolicy(10), sub_front, sub_right, sub_left, sub_back);

	sync.registerCallback(boost::bind(&callback, _1,_2,_3,_4));

	ros::spin();

	return 0;


}

void callback(const my_msgs::FloatArray_of::ConstPtr &tfront, const my_msgs::FloatArray_of::ConstPtr &tright,const my_msgs::FloatArray_of::ConstPtr &tleft,const my_msgs::FloatArray_of::ConstPtr &tback)
{
	std::cout<< "........................................................................"<<std::endl;
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

	float last[12] = {};
	float first[84]={};

	tan_all.data.clear();	
	tan_all.data.resize(192);

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

	//std::vector<float> fronts(96,0.0);
	float front[96];


	std::cout<<"last"<<std::endl;
	for(int i=0;i<12;i++)
	{
		last[i] = tfront->data[i+25];
		std::cout<<last[i]<<std::endl;
	}

	std::cout<<"first"<<std::endl;
	for(int i=0;i<84;i++)
	{
		first[i] = tfront->data[i+37];
		std::cout<<first[i]<<std::endl;
	} 

	std::copy( first, first + 84 , front);
	std::copy(last, last + 12, front + 84);
	
	std::cout<<"front"<<std::endl;
	for(int i=0;i<96;i++)
	{          
		std::cout<<front[i]<<std::endl;
	} 

	int xy=25;
	int q=0;
	for(int i=0; i<4;i++)
	{
		for(int j = 0; j<24;j++)
		{
			overall[i][j] = front[q];
			overall[i][j+24] = tright->data[xy];
			overall[i][j+48] = tback->data[xy];
			overall[i][j+72] = tleft->data[xy];
			++xy;
			++q;
		}

	}
	
	
    /*dat.data.clear();
    //dat.data.resize(384);
    dat.layout.dim.push_back(std_msgs::MultiArrayDimension());
    dat.layout.dim.push_back(std_msgs::MultiArrayDimension());
    dat.layout.dim[0].label = "height";
    dat.layout.dim[1].label = "width";
    dat.layout.dim[0].size = 4;
    dat.layout.dim[1].size = 96;
    //dat.layout.dim[0].stride = 384;
    //dat.layout.dim[1].stride = 96;
    dat.layout.data_offset = 0;
    std::vector<float> vec(384, 0);
    for (int i=0; i<4; i++)
        for (int j=0; j<96; j++)
            vec[i*96 + j] = overall[i][j];
    dat.data = vec;*/

    std::vector<float> vec(384, 0.0);
    for (int i=0; i<4; i++)
        for (int j=0; j<96; j++)
            vec[i*96 + j] = overall[i][j];
    dat.header.stamp =  ros::Time::now();
    dat.data = vec;

	row1.header.stamp = ros::Time::now();
	row2.header.stamp = ros::Time::now();
	row3.header.stamp = ros::Time::now();
	row4.header.stamp = ros::Time::now();

	for(int i = 0; i<96;i++)
	{
		row1.data[i] = dat.data[i];
		row2.data[i] = dat.data[i+96];
		row3.data[i] = dat.data[i+192];
		row4.data[i]= dat.data[i+288];
	}
	


	int fx = 25;
	int fy = 145;
	int k = 0;
					
//Storing x and y in multiarray, easier for averaging
	for(int i=0; i<4;i++)
	{
		for(int j = 0; j<24;j++)
		{
			front_x[i][j] = front[k];
			front_y[i][j] = tfront->data[fy];

			right_x[i][j] = tright->data[fx];
			right_y[i][j] = tright->data[fy];

			ha[i][j] = HA[k];
			//std::cout<<ha[i][j]<<std::endl;

			left_x[i][j] = tleft->data[fx];
			left_y[i][j] = tleft->data[fy];

			back_x[i][j] = tback->data[fx];
			back_y[i][j] = tback->data[fy];
			++fx;
			++fy;
			++k;

		}
			        
	}

	
	for(int i=0; i<4;i++)
	{	
		//std::cout<< "........................................................................"<<std::endl;
		for(int j = 0; j<96;j++)
		{
			//std::cout<<"overall" <<overall[i][j]<<std::endl;
			
		}

		
			        
	}


//Average.
	

	for(int j=0; j<24;j++)
	{
		//std::cout<< "........................................................................"<<std::endl;
		for(int i = 0; i<4;i++)
		{
			sum_front_x[j] += front_x[i][j];
			//std::cout<<"data"<< front_x[i][j]<<std::endl;
			//std::cout<< sum_front_x[j]<<std::endl;

			sum_front_y[j] += front_y[i][j];

			sum_right_x[j] += right_x[i][j];
			sum_right_y[j] += right_y[i][j];

			sum_left_x[j] += left_x[i][j];
			sum_left_y[j] += left_y[i][j];

			sum_back_x[j] += back_x[i][j];
			sum_back_y[j] += back_y[i][j];

			sum_ha[j] += ha[i][j];
			
		}
	
		sum_front_xavg[j] = sum_front_x[j]/4;	
		sum_front_yavg[j] = sum_front_y[j]/4;  

		sum_right_xavg[j] = sum_right_x[j]/4;	
		sum_right_yavg[j] = sum_right_y[j]/4;

		sum_left_xavg[j] = sum_left_x[j]/4;	
		sum_left_yavg[j] = sum_left_y[j]/4;

		sum_back_xavg[j] = sum_back_x[j]/4;	
		sum_back_yavg[j] = sum_back_y[j]/4;

		sum_haavg[j] = sum_ha[j]/4;
		

		/*std::cout<<"AVGF " <<sum_front_xavg[j]<<std::endl;
		std::cout<<"AVGR" <<sum_right_xavg[j]<<std::endl;
		std::cout<<"AVGL" <<sum_left_xavg[j]<<std::endl;
		std::cout<<"AVGB" <<sum_back_xavg[j]<<std::endl;*/
      
	}


	for(int i=0; i<24;i++)
	{
		
		tan_all.header.stamp =  ros::Time::now();		
		tan_all.data[i] = sum_front_xavg[i];
		tan_all.data[i+24] = sum_right_xavg[i];
		tan_all.data[i+48] = sum_back_xavg[i];
		tan_all.data[i+72] = sum_left_xavg[i];
		
	}
	
	for(int i=96; i<121;i++)
	{
		tan_all.header.stamp =  ros::Time::now();		
		tan_all.data[i] = sum_front_yavg[i];
		tan_all.data[i+24] = sum_right_yavg[i];
		tan_all.data[i+48] = sum_back_yavg[i];
		tan_all.data[i+72] = sum_left_yavg[i];
		
	}

	for(int i=0;i<96;i++)
	{
		horizontal_angle.header.stamp =  ros::Time::now();
		horizontal_angle.data[i] = HA_plot[i];
	}
		


	all_toflow.publish(tan_all);
	horiz_ang.publish(horizontal_angle);
	allof.publish(dat);
	rows1.publish(row1);
	rows2.publish(row2);
	rows3.publish(row3);
	rows4.publish(row4);

}




































