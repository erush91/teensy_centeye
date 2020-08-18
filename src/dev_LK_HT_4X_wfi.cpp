#include "oflow.h"

TanOflow::TanOflow(){
	front_pub = n.advertise<teensy_centeye::FloatArray_of>("tang_oflow_front",10);
	right_pub = n.advertise<teensy_centeye::FloatArray_of>("tang_oflow_right",10);
	left_pub = n.advertise<teensy_centeye::FloatArray_of>("tang_oflow_left",10);
	back_pub = n.advertise<teensy_centeye::FloatArray_of>("tang_oflow_back",10);
		
	all_toflow = n.advertise<teensy_centeye::FloatArray_of>("tang_oflow_all_averaged", 10);
	
	allof = n.advertise<teensy_centeye::FloatArray_of>("tang_oflow_all", 10);
	rows1 = n.advertise<teensy_centeye::FloatArray_of>("row1",10);
	rows2 = n.advertise<teensy_centeye::FloatArray_of>("row2",10);
	rows3 = n.advertise<teensy_centeye::FloatArray_of>("row3",10);
	rows4 = n.advertise<teensy_centeye::FloatArray_of>("row4",10);

	fourier_coeffs = n.advertise<teensy_centeye::Fourier_Coeff>("/Fourier_Coeffs",10);

	// Wait for first message to be published, determine format of incoming data
	boost::shared_ptr<std_msgs::Int8MultiArray const> init_optic_flow_in;
	std_msgs::Int8MultiArray init_optic_flow_in_pointer;
	init_optic_flow_in = ros::topic::waitForMessage<std_msgs::Int8MultiArray>("optic_flow_left");
	if(init_optic_flow_in != NULL)
	{
		//determine size of optic flow message
		std::vector<int> oflow(std::begin(init_optic_flow_in->data), std::end(init_optic_flow_in->data));
		oflow_size = oflow.size();
		std::cout << "oflow_size: " << oflow_size << std::endl;
		if(oflow_size == 48)
		{
			flag_LK = 1;
		}
		else if(oflow_size == 242)
		{
			flag_HT = 1;
		}
		else
		{
        	ROS_WARN_STREAM("Message size doesn't match neither LK(48) nor Horridge Template(242)");
		}

		init(oflow_size);
	}

	front_sub 	= n.subscribe("optic_flow_front", 10, &TanOflow::oflow_front, this);
	right_sub 	= n.subscribe("optic_flow_right", 10, &TanOflow::oflow_right, this);
	left_sub 	= n.subscribe("optic_flow_left",  10, &TanOflow::oflow_left,  this);
	back_sub 	= n.subscribe("optic_flow_back",  10, &TanOflow::oflow_back,  this);

	//steering control parameters
	n.getParam("/orientation_gain",gain_k1);
	n.getParam("/lateral_gain", gain_k2);

}


//Front Optic Flow
void TanOflow::oflow_front(const std_msgs::Int8MultiArray& ofront){

	std::cout << "begin callback_front" << std::endl;

	//Lucas Kanade
    if(flag_LK == 1)
    {   
		
        tan_front.data.clear();	
        tan_front.data.resize(oflow_size);
        for(int i=0; i<oflow_size; i++)
        {
                            
            tan_front.data[i] = ofront.data[i] * SF_LK[i];
                
        }

        tan_front.header.stamp =  ros::Time::now();
        front_pub.publish(tan_front);
    }

	//Horridge Template
    else if(flag_HT == 1)
    {
        tan_front.data.clear();	
        tan_front.data.resize(oflow_size);
        for(int i=0; i<oflow_size; i++)
        {
                            
            tan_front.data[i] = ofront.data[i] * SF_HT[i];
                
        }

        tan_front.header.stamp =  ros::Time::now();
        front_pub.publish(tan_front);
    }

    else
    {	
		//ROS_INFO("Size of %d", oflow_size);
        ROS_WARN_STREAM("Message size doesn't match neither LK(48) nor Horridge Template(242)");
    }

}

//Right Optic Flow
void TanOflow::oflow_right(const std_msgs::Int8MultiArray& oright){

	std::cout << "begin callback_right" << std::endl;
    if(flag_LK == 1)
    {
        tan_right.data.clear();	
        tan_right.data.resize(oflow_size);

        for(int i=0; i<48; i++)
        {
                
            tan_right.data[i] = oright.data[i] * SF_LK[i];
                
        }
        tan_right.header.stamp =  ros::Time::now();
        right_pub.publish(tan_right);
    }

    else if(flag_HT == 1)
    {
        tan_right.data.clear();	
        tan_right.data.resize(oflow_size);

        for(int i=0; i<242; i++)
        {
                
            tan_right.data[i] = oright.data[i] * SF_HT[i];
                
        }
        tan_right.header.stamp =  ros::Time::now();
        right_pub.publish(tan_right);
    }

    else
    {
        ROS_WARN_STREAM("Message size doesn't match neither LK(48) nor Horridge Template(242)");
    }
	
    average();	
	fourier_coeffec();
	computeRobotTurnRate();
		
}

//Left Optic Flow
void TanOflow::oflow_left(const std_msgs::Int8MultiArray& oleft){

	std::cout << "begin callback_left" << std::endl;

     if(flag_LK == 1)
    {
        tan_left.data.clear();	
        tan_left.data.resize(oflow_size);

        for(int i=0; i<48; i++)
        {
                
            tan_left.data[i] = oleft.data[i] * SF_LK[i];
                
        }

        tan_left.header.stamp =  ros::Time::now();	
        left_pub.publish(tan_left);
    }

    else if(flag_HT == 1)
    {
        tan_left.data.clear();	
        tan_left.data.resize(oflow_size);

        for(int i=0; i<242; i++)
        {
                
            tan_left.data[i] = oleft.data[i] * SF_HT[i];
                
        }

        tan_left.header.stamp =  ros::Time::now();	
        left_pub.publish(tan_left);
    }

    else
    {
        ROS_WARN_STREAM("Message size doesn't match neither LK(48) nor Horridge Template(242)");
    }
		
}

//Back Optic Flow
void TanOflow::oflow_back(const std_msgs::Int8MultiArray& oback)	{

	std::cout << "begin callback_back" << std::endl;

    if(flag_LK == 1)
    {
        tan_back.data.clear();	
        tan_back.data.resize(oflow_size);

        for(int i=0; i<48; i++)
        {
                    
            tan_back.data[i] = oback.data[i] * SF_LK[i];
                
        }
        tan_back.header.stamp =  ros::Time::now();
        back_pub.publish(tan_back);
    }

    else if(flag_HT == 1)
    {
        tan_back.data.clear();	
        tan_back.data.resize(oflow_size);

        for(int i=0; i<242; i++)
        {
                    
            tan_back.data[i] = oback.data[i] * SF_HT[i];
                
        }
        tan_back.header.stamp =  ros::Time::now();
        back_pub.publish(tan_back);
    }

    else
    {
        ROS_WARN_STREAM("Message size doesn't match neither LK(48) nor Horridge Template(242)");
    }
	
}

//initialise the variables acccording to message size
void TanOflow::init(int of_size){

	//Lucas Kanade
	if(flag_LK == 1)
    {
		horiz_ang_lk = n.advertise<teensy_centeye::FloatArray_of>("horizontal_angle_lk",10);
		horizontal_angle_lk.data.clear();
		horizontal_angle_lk.data.resize(48);

		row_size = 4;
		col_size = 20;
		dat_size = row_size * col_size;
		dat_start = 0;

		last_size = 8;
		last_inc = 2;
		last_col_start = 0;
		last_col_end = 2;

		first_size = 12;
		first_inc = 3;
		first_col_start = 2;
		first_col_end = 5;

		data_inc = 5;

		overall_first_start = 0;
		overall_middle_start = 3;
		overall_last_start = 18;

		overall_first_end = 3;
		overall_middle_end = 8;
		overall_last_end = 20;

		num_horiz_of = 20;
		
		//LK angle(x)
		for(int i=0;i< 48;i++)
		{
			horizontal_angle_lk.data[i] = HA_LK[i];
		}

		horizontal_angle_lk.header.stamp =  ros::Time::now();
		horiz_ang_lk.publish(horizontal_angle_lk);

    }

	//Horridge Template
    else if(flag_HT == 1)
    {
		horiz_ang_ht = n.advertise<teensy_centeye::FloatArray_of>("horizontal_angle_ht",10);
		horizontal_angle_ht.data.clear();
		horizontal_angle_ht.data.resize(96);

       	row_size = 4;
		col_size = 96;
		dat_size = row_size * col_size;
		dat_start = 25;


		last_size = 48;
		last_inc = 12;
		last_col_start = 25;
		last_col_end = 37;

		first_size = 48;
		first_inc = 12;
		first_col_start = 37;
		first_col_end = 49;

		data_inc = 24;

		overall_first_start = 0;
		overall_middle_start = 12;
		overall_last_start = 84;

		overall_first_end = 12;
		overall_middle_end = 36;
		overall_last_end = 96;

		num_horiz_of = 96;

		//HT angle(x)
		for(int i=0;i< 96;i++)
		{
			horizontal_angle_ht.data[i] = HA_HT[i];
		}

		horizontal_angle_ht.header.stamp =  ros::Time::now();
		horiz_ang_ht.publish(horizontal_angle_ht);
    }

    else
    {
        ROS_WARN_STREAM("Message size doesn't match neither LK(48) nor Horridge Template(242)");
    }

}

//Average the tangential optic flow column wise to get 1D Optic flow
void TanOflow::average(){
	float overall[row_size][col_size]= {};

	float sum_overall_x[col_size] = {}; 
	float avg_overall_x[col_size] = {};

	float last[last_size] = {};
	float first[first_size]={};

	tan_allavg_x.data.clear();	
	tan_allavg_x.data.resize(col_size);

	row1.data.clear();	
	row1.data.resize(col_size);
		
	row2.data.clear();	
	row2.data.resize(col_size);

	row3.data.clear();	
	row3.data.resize(col_size);

	row4.data.clear();	
	row4.data.resize(col_size);

	dat.data.clear();
	dat.data.resize(dat_size);

	//split front optic flow 
	int a = 0;

	for(int u = last_col_start; u < last_col_end;u++)
	{
	    last[a] = tan_front.data[u];
		last[a+ last_inc]= tan_front.data[u+ data_inc];
		last[a+ last_inc*2]= tan_front.data[u+ data_inc*2];
		last[a+ last_inc*3]= tan_front.data[u+ data_inc*3];
		++a;	 
	}
			
	int b = 0;

	for(int u= first_col_start; u < first_col_end; u++)
	{
		first[b] = tan_front.data[u];
		first[b+ first_inc]= tan_front.data[u+ data_inc];
		first[b+ first_inc*2]= tan_front.data[u+ data_inc*2];
		first[b+ first_inc*3]= tan_front.data[u+ data_inc*3];
		++b;	 
	}
				

	//overall optic flow structure: *front_first|right back|left|front_last*
	int v=0;
	for(int i=0;i<4;i++)
	{
		for(int j= 0;j<overall_first_end;j++)
		{

			overall[i][j] = first[v];
			++v;
		}	
	}




	int xy=dat_start; //data_start
	for(int i=0; i<4;i++)
	{
		for(int j = overall_middle_start; j< overall_middle_end;j++)
		{
			overall[i][j] = tan_right.data[xy];
			overall[i][j+data_inc] = tan_back.data[xy];
			overall[i][j+ data_inc*2] = tan_left.data[xy];
			++xy;
	    }

	}


	int t=0;
	for(int i=0;i<4;i++)
	{
		for(int j=overall_last_start;j<overall_last_end ;j++)
		{

			overall[i][j] = last[t];
			++t;
		}	
	}



	std::vector<float> vec(dat_size, 0.0);
	for (int i=0; i<row_size; i++){
	    for (int j=0; j<col_size; j++){
		    vec[i*col_size + j] = overall[i][j];
		    dat.data = vec;
        }
    }
		
	for(int i = 0; i<col_size;i++)
    {
	
        row1.data[i] = dat.data[i];
	    row2.data[i] = dat.data[i+col_size];
	    row3.data[i] = dat.data[i+ col_size*2];
	    row4.data[i]= dat.data[i+ col_size*3];
    }
		
	//average the optic flow column wise
	for(int j=0; j<col_size;j++)
	{
		//std::cout<< "........................................................................"<<std::endl;
		for(int i = 0; i<row_size;i++)
		{		
		    sum_overall_x[j] += overall[i][j];

		}
		avg_overall_x[j] = sum_overall_x[j]/row_size; 
    }



	for(int i=0;i<col_size;i++)
	{
	    tan_allavg_x.data[i] = avg_overall_x[i];
    }

	
	tan_allavg_x.header.stamp = ros::Time::now();
	all_toflow.publish(tan_allavg_x);

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

//calculate fourier coefficients
void TanOflow::fourier_coeffec(){
		
	float avg_ov_x[col_size];	
	msg.a.clear();
	msg.a.resize(5);
		
	msg.b.clear();
	msg.b.resize(5);

	float horiz_cos_gamma_arr[ num_horiz_fourier_terms + 1][num_horiz_of];
	float horiz_sin_gamma_arr[ num_horiz_fourier_terms + 1][num_horiz_of]; 
	
	for(int i=0;i<col_size;i++)
	{
		avg_ov_x[i] = tan_allavg_x.data[i];
	}

	h_dg = (2*horiz_scan_limit)/num_horiz_of;

	//h_dg_pi = 2/num_horiz_of;


	cv::Mat h_cos_gamma_matcv(num_horiz_fourier_terms + 1, num_horiz_of, CV_32FC1, horiz_cos_gamma_arr);
	cv::Mat h_sin_gamma_matcv(num_horiz_fourier_terms + 1, num_horiz_of , CV_32FC1, horiz_sin_gamma_arr);
	cv::Mat average_of(1,num_horiz_of,CV_32FC1, avg_ov_x);
	
	//a1: orienatation
	//a2:lateral dispacement
	//b1:forward speed

	if(num_horiz_of == 96)//Horridge
	{	
		for(int i=0; i< num_horiz_fourier_terms+1; i++)
		{
			for(int j=0; j< num_horiz_of; j++)
			{
				horiz_cos_gamma_arr[i][j] = cos( i*HA_HT[j]/360*2*M_PI);
				horiz_sin_gamma_arr[i][j] = sin( i*HA_HT[j]/360*2*M_PI);
			}	
					
			h_a[i] = average_of.dot(h_cos_gamma_matcv.row(i)) * h_dg / M_PI;
			h_b[i] = average_of.dot(h_sin_gamma_matcv.row(i)) * h_dg / M_PI;				
			
		}
	}

	else if(num_horiz_of == 20)//Lucas Kanade
	{	
		for(int i=0; i< num_horiz_fourier_terms+1; i++)
		{
			for(int j=0; j< num_horiz_of; j++)
			{
				horiz_cos_gamma_arr[i][j] = cos( i*HA_LK[j]/360*2*M_PI);
				horiz_sin_gamma_arr[i][j] = sin( i*HA_LK[j]/360*2*M_PI);
			}	
					
			h_a[i] = average_of.dot(h_cos_gamma_matcv.row(i)) * h_dg / M_PI;
			h_b[i] = average_of.dot(h_sin_gamma_matcv.row(i)) * h_dg / M_PI;				
			
		}
	}


	std::vector<float> h_a_vector(h_a, h_a + sizeof h_a / sizeof h_a[0]);
	std::vector<float> h_b_vector(h_b, h_b + sizeof h_b / sizeof h_b[0]);

		
	msg.a = h_a_vector;
	msg.b = h_b_vector;
		
	msg.header.stamp = ros::Time::now();
	fourier_coeffs.publish(msg);

}


void TanOflow::computeRobotTurnRate(){

	turn_rate = gain_k1 * h_a[1] + gain_k2 * h_a[2];
	ROS_INFO("Turn : %f", turn_rate);
}


int main(int argc, char **argv)
{

	ros::init(argc, argv, "teensy_centeye_oflow");

	TanOflow toflow;
	ros::spin();

	return 0;
}
