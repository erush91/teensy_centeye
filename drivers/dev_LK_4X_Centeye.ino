#include <Arduino.h>
#include <ros.h> // PlatformIO Library: rosserial_arduino by Open Agriculture Initiative
//#include <std_msgs/Int32.h>
#include <std_msgs/Int32MultiArray.h>
#include<math.h>
#include<cmath>

//#include <SD.h>
// #include <SPI.h>

template <typename T> int sign(T val) {
    return (T(0) < val) - (val < T(0));
}

// https://www.pjrc.com/teensy/td_uart.html
// https://forum.pjrc.com/threads/43085-choosing-a-serial-port-on-the-Teensy-3-6
// https://forum.pjrc.com/threads/55342-Teensy3-6-Serial-comm-error?p=199937#post199937

// Write byte array
// https://forum.arduino.cc/index.php?topic=412964.0

// Read byte array
// https://forum.arduino.cc/index.php?topic=383010.0
// https://forum.arduino.cc/index.php?topic=46652.0

// Serial: USB
// Serial1: RX1 / TX1

// set this to the hardware serial port you wish to use
//these objects are used to set up a publisher/subscriber ros node
// std_msgs::Float32 temp_msg;
// ros::Publisher pub("temperature", &temp_msg);

std_msgs::Int32MultiArray optic_flow_msg1;
ros::Publisher pub_optic_flow1("optic_flow_left", &optic_flow_msg1);

std_msgs::Int32MultiArray optic_flow_msg2;
ros::Publisher pub_optic_flow2("optic_flow_back", &optic_flow_msg2);

std_msgs::Int32MultiArray optic_flow_msg3;
ros::Publisher pub_optic_flow3("optic_flow_front", &optic_flow_msg3);

std_msgs::Int32MultiArray optic_flow_msg4;
ros::Publisher pub_optic_flow4("optic_flow_right", &optic_flow_msg4);


ros::NodeHandle nh;

///////////////
// RUNS ONCE //
///////////////
void setup()
{
    // DO NOT VIEW THE SERIAL MONITOR, if you do, this will cause a ...
    // Run loop error: Serial Port read failure: device reports readiness to read but returned no data (device disconnected or multiple access on port?

    Serial.begin(115200);
    Serial1.begin(115200, SERIAL_8N1);
    Serial2.begin(115200, SERIAL_8N1);
    Serial3.begin(115200, SERIAL_8N1);
    Serial4.begin(115200, SERIAL_8N1);
    
    
    //start the ROS node
    nh.getHardware()->setBaud(115200);
    nh.initNode();
    // nh.loginfo("Program info");
    nh.advertise(pub_optic_flow1);
    nh.advertise(pub_optic_flow2);
    nh.advertise(pub_optic_flow3);
    nh.advertise(pub_optic_flow4);
}

///////////////////////
// RUNS CONTINUOUSLY //
///////////////////////
void loop()
{

  // byte TxMsg[6] = {255, 8, 36, 0, 255, 254}; // Retrieve Device ID
  //byte TxMsg[6] = {255, 8, 36, 9, 255, 254}; // Retrieve Sensor Data // Dataset 9
  byte TxMsg[6] = {255, 8, 36, 3, 255, 254}; // Retrieve Sensor Data // Dataset 3

  int RxMsg1;
  unsigned int bytecount1 = 0;
  unsigned char Data1[100];

  int RxMsg2;
  unsigned int bytecount2 = 0;
  unsigned char Data2[100];


  int RxMsg3;
  unsigned int bytecount3 = 0;
  unsigned char Data3[100];


  int RxMsg4;
  unsigned int bytecount4 = 0;
  unsigned char Data4[100];


  Serial1.write(TxMsg, 6);
  Serial2.write(TxMsg, 6);
  Serial3.write(TxMsg, 6);
  Serial4.write(TxMsg, 6);

  // Buffer of UART probably small, this makes sure buffer has some bytes
  delay(1); // Need 3ms delay before reading Device ID (otherwise first byte = 252)

  double begin1 = nh.now().toSec();
  while(Serial1.available() && bytecount1 < 100)
  {

    RxMsg1 = Serial1.read();
    // Serial.print(RxMsg);
    // Serial.print(", ");
    Data1[bytecount1] = RxMsg1;

    bytecount1++;
    
    // Delay must be short to prevent buffer from overflowing, but long enough for a new byte to come in.
    delayMicroseconds(90);
  
  }

  while(Serial2.available() && bytecount2 < 100)
  {

    RxMsg2 = Serial2.read();
    // Serial.print(RxMsg);
    // Serial.print(", ");
    Data2[bytecount2] = RxMsg2;

    bytecount2++;
    
    // Delay must be short to prevent buffer from overflowing, but long enough for a new byte to come in.
    delayMicroseconds(90);
  
  }

  while(Serial3.available() && bytecount3 < 100)
  {

    RxMsg3 = Serial3.read();
    // Serial.print(RxMsg);
    // Serial.print(", ");
    Data3[bytecount3] = RxMsg3;

    bytecount3++;
    
    // Delay must be short to prevent buffer from overflowing, but long enough for a new byte to come in.
    delayMicroseconds(90);
  
  }

  while(Serial4.available() && bytecount4 < 100)
  {

    RxMsg4 = Serial4.read();
    // Serial.print(RxMsg);
    // Serial.print(", ");
    Data4[bytecount4] = RxMsg4;

    bytecount4++;
    
    // Delay must be short to prevent buffer from overflowing, but long enough for a new byte to come in.
    delayMicroseconds(90);
  
  }
  double end1 = nh.now().toSec();
  // Enhance readability in serial monitor
  //delay(50);

  //Serial.println();

  //Serial.clear();
  Serial1.clear();
  Serial2.clear();
  Serial3.clear();
  Serial4.clear();
  delay(14);
  

  /////////////////////////////
  // ASSIGN DATA TO POINTERS //
  /////////////////////////////
  double begin2 = nh.now().toSec();
  // Int = 32 Bytes
  int *p_int1 = (int*)Data1;
  int *p_int2 = (int*)Data2;
  int *p_int3 = (int*)Data3;
  int *p_int4 = (int*)Data4;

  // Short Int = 16 Bytes
  short *p_short1 = (short*)Data1;
  short *p_short2 = (short*)Data2;
  short *p_short3 = (short*)Data3;
  short *p_short4 = (short*)Data4;

  //////////////////////////////
  // ASSIGN DATA TO VARIABLES //
  //////////////////////////////

  //frame counter

  int frame_counter1 = p_int1[0];
  int frame_counter2 = p_int2[0];
  int frame_counter3 = p_int3[0];
  int frame_counter4 = p_int4[0];
  

  // x and y window optical flow odometries
  short xodo1[24],yodo1[24],xodo2[24],yodo2[24],xodo3[24],yodo3[24],xodo4[24],yodo4[24];
  int fxsign_new1,fysign_new1,fxsign_new2,fysign_new2,fxsign_new3,fysign_new3,fxsign_new4,fysign_new4;
  int fxsign_old1,fysign_old1,fxsign_old2,fysign_old2,fxsign_old3,fysign_old3,fxsign_old4,fysign_old4;
  int rollover_counter_x1[24]={},rollover_counter_x2[24]={},rollover_counter_x3[24]={},rollover_counter_x4[24]={};
  int rollover_counter_y1[24]={},rollover_counter_y2[24]={},rollover_counter_y3[24]={},rollover_counter_y4[24]={};
  float fyodo1[24],fxodo1[24],fxodo_new1[24],fxodo_old1[24],fyodo_new1[24],fyodo_old1[24];
  float fyodo2[24],fxodo2[24],fxodo_new2[24],fxodo_old2[24],fyodo_new2[24],fyodo_old2[24];
  float fyodo3[24],fxodo3[24],fxodo_new3[24],fxodo_old3[24],fyodo_new3[24],fyodo_old3[24];
  float fyodo4[24],fxodo4[24],fxodo_new4[24],fxodo_old4[24],fyodo_new4[24],fyodo_old4[24];
  float fxodo_final1[24],fyodo_final1[24],fxodo_final2[24],fyodo_final2[24],fxodo_final3[24],fyodo_final3[24],fxodo_final4[24],fyodo_final4[24];

 
  
  for (int w = 0; w < 24; ++w) 
  { 
    //int *p_w_int = (int*)(Data + w * 20 + 44);
    //short *p_w_short = (short*)(Data + w * 20 + 44);
    xodo1[w] = p_short1[2+2*w];
    yodo1[w] = p_short1[3+2*w];
    xodo2[w] = p_short2[2+2*w];
    yodo2[w] = p_short2[3+2*w];
    xodo3[w] = p_short3[2+2*w];
    yodo3[w] = p_short3[3+2*w];
    xodo4[w] = p_short4[2+2*w];
    yodo4[w] = p_short4[3+2*w];
  }


   for(int w = 0; w < 24; ++w)
   {
     fxodo1[w] = xodo1[w]*0.001;
     fyodo1[w] = yodo1[w]*0.001;
     fxodo2[w] = xodo2[w]*0.001;
     fyodo2[w] = yodo2[w]*0.001;
     fxodo3[w] = xodo3[w]*0.001;
     fyodo3[w] = yodo3[w]*0.001;
     fxodo4[w] = xodo4[w]*0.001;
     fyodo4[w] = yodo4[w]*0.001;
   }


   for(int i=0; i<24; i++)
   {

      if(i == 0)
      {
        rollover_counter_x1[i] = 0;
        fxodo_new1[i] = fxodo1[i];
        fxodo_old1[i] = fxodo_new1[i];
        fxodo_final1[i] = fxodo_new1[i];
      }

      else
      {
        fxodo_old1[i] = fxodo1[i-1];

        fxodo_new1[i] = fxodo1[i];

        fxsign_new1 = sign<float>(fxodo_new1[i]);
        fxsign_old1 = sign<float>(fxodo_old1[i]);

        if(fxsign_new1 * fxsign_old1 < 0)
        {
          if( abs(fxodo_old1[i]) > 12384 && abs(fxodo_new1[i]) > 12384)
          {
            if(fxodo_new1[i]<0)
              rollover_counter_x1[i]++;
            else
              rollover_counter_x1[i]--;  
          }
        }
        fxodo_final1[i] = fxodo_new1[i] + 65536 * rollover_counter_x1[i];
      }
   }

   for(int i=0; i<24; i++)
   {

      if(i == 0)
      {
        rollover_counter_y1[i] = 0;
        fyodo_new1[i] = fyodo1[i];
        fyodo_old1[i] = fyodo_new1[i];
        fyodo_final1[i] = fyodo_new1[i];
      }

      else
      {
        fyodo_old1[i] = fyodo1[i-1];

        fyodo_new1[i] = fyodo1[i];

        fysign_new1 = sign<float>(fyodo_new1[i]);
        fysign_old1 = sign<float>(fyodo_old1[i]);

        if(fysign_new1 * fysign_old1 < 0)
        {
          if( abs(fyodo_old1[i]) > 12384 || abs(fyodo_new1[i]) > 12384)
          {
            if(fyodo_new1[i]<0)
              rollover_counter_y1[i]++;
            else
              rollover_counter_y1[i]--;  
          }
        }
        fyodo_final1[i] = fyodo_new1[i] + 65536 * rollover_counter_y1[i];
      }
   }

//2
for(int i=0; i<24; i++)
   {

      if(i == 0)
      {
        rollover_counter_x2[i] = 0;
        fxodo_new2[i] = fxodo2[i];
        fxodo_old2[i] = fxodo_new2[i];
        fxodo_final2[i] = fxodo_new2[i];
      }

      else
      {
        fxodo_old2[i] = fxodo2[i-1];

        fxodo_new2[i] = fxodo2[i];

        fxsign_new2 = sign<float>(fxodo_new2[i]);
        fxsign_old2 = sign<float>(fxodo_old2[i]);

        if(fxsign_new2 * fxsign_old2 < 0)
        {
          if( abs(fxodo_old2[i]) > 12384 && abs(fxodo_new2[i]) > 12384)
          {
            if(fxodo_new2[i]<0)
              rollover_counter_x2[i]++;
            else
              rollover_counter_x2[i]--;  
          }
        }
        fxodo_final2[i] = fxodo_new2[i] + 65536 * rollover_counter_x2[i];
      }
   }

   for(int i=0; i<24; i++)
   {

      if(i == 0)
      {
        rollover_counter_y2[i] = 0;
        fyodo_new2[i] = fyodo2[i];
        fyodo_old2[i] = fyodo_new2[i];
        fyodo_final2[i] = fyodo_new2[i];
      }

      else
      {
        fyodo_old2[i] = fyodo2[i-1];

        fyodo_new2[i] = fyodo2[i];

        fysign_new2 = sign<float>(fyodo_new2[i]);
        fysign_old2 = sign<float>(fyodo_old2[i]);

        if(fysign_new2 * fysign_old2 < 0)
        {
          if( abs(fyodo_old2[i]) > 12384 || abs(fyodo_new2[i]) > 12384)
          {
            if(fyodo_new2[i]<0)
              rollover_counter_y2[i]++;
            else
              rollover_counter_y2[i]--;  
          }
        }
        fyodo_final2[i] = fyodo_new2[i] + 65536 * rollover_counter_y2[i];
      }
   }

//3
for(int i=0; i<24; i++)
   {

      if(i == 0)
      {
        rollover_counter_x3[i] = 0;
        fxodo_new3[i] = fxodo3[i];
        fxodo_old3[i] = fxodo_new3[i];
        fxodo_final3[i] = fxodo_new3[i];
      }

      else
      {
        fxodo_old3[i] = fxodo3[i-1];

        fxodo_new3[i] = fxodo3[i];

        fxsign_new3 = sign<float>(fxodo_new3[i]);
        fxsign_old3 = sign<float>(fxodo_old3[i]);

        if(fxsign_new3 * fxsign_old3 < 0)
        {
          if( abs(fxodo_old3[i]) > 12384 && abs(fxodo_new3[i]) > 12384)
          {
            if(fxodo_new3[i]<0)
              rollover_counter_x3[i]++;
            else
              rollover_counter_x3[i]--;  
          }
        }
        fxodo_final3[i] = fxodo_new3[i] + 65536 * rollover_counter_x3[i];
      }
   }

   for(int i=0; i<24; i++)
   {

      if(i == 0)
      {
        rollover_counter_y3[i] = 0;
        fyodo_new3[i] = fyodo3[i];
        fyodo_old3[i] = fyodo_new3[i];
        fyodo_final3[i] = fyodo_new3[i];
      }

      else
      {
        fyodo_old3[i] = fyodo3[i-1];

        fyodo_new3[i] = fyodo3[i];

        fysign_new3 = sign<float>(fyodo_new3[i]);
        fysign_old3 = sign<float>(fyodo_old3[i]);

        if(fysign_new3 * fysign_old3 < 0)
        {
          if( abs(fyodo_old3[i]) > 12384 || abs(fyodo_new3[i]) > 12384)
          {
            if(fyodo_new3[i]<0)
              rollover_counter_y3[i]++;
            else
              rollover_counter_y3[i]--;  
          }
        }
        fyodo_final3[i] = fyodo_new3[i] + 65536 * rollover_counter_y3[i];
      }
   }

//4

for(int i=0; i<24; i++)
   {

      if(i == 0)
      {
        rollover_counter_x4[i] = 0;
        fxodo_new4[i] = fxodo4[i];
        fxodo_old4[i] = fxodo_new4[i];
        fxodo_final4[i] = fxodo_new4[i];
      }

      else
      {
        fxodo_old4[i] = fxodo4[i-1];

        fxodo_new4[i] = fxodo4[i];

        fxsign_new4 = sign<float>(fxodo_new4[i]);
        fxsign_old4 = sign<float>(fxodo_old4[i]);

        if(fxsign_new4 * fxsign_old4 < 0)
        {
          if( abs(fxodo_old4[i]) > 12384 && abs(fxodo_new4[i]) > 12384)
          {
            if(fxodo_new4[i]<0)
              rollover_counter_x4[i]++;
            else
              rollover_counter_x4[i]--;  
          }
        }
        fxodo_final4[i] = fxodo_new4[i] + 65536 * rollover_counter_x4[i];
      }
   }

   for(int i=0; i<24; i++)
   {

      if(i == 0)
      {
        rollover_counter_y4[i] = 0;
        fyodo_new4[i] = fyodo4[i];
        fyodo_old4[i] = fyodo_new4[i];
        fyodo_final4[i] = fyodo_new4[i];
      }

      else
      {
        fyodo_old4[i] = fyodo4[i-1];

        fyodo_new4[i] = fyodo4[i];

        fysign_new4 = sign<float>(fyodo_new4[i]);
        fysign_old4 = sign<float>(fyodo_old4[i]);

        if(fysign_new4 * fysign_old4 < 0)
        {
          if( abs(fyodo_old4[i]) > 12384 || abs(fyodo_new4[i]) > 12384)
          {
            if(fyodo_new4[i]<0)
              rollover_counter_y4[i]++;
            else
              rollover_counter_y4[i]--;  
          }
        }
        fyodo_final4[i] = fyodo_new4[i] + 65536 * rollover_counter_y4[i];
      }
   }

  // double end1 = nh.now().toSec();
  float optic_flow_x_prev1[24],optic_flow_y_prev1[24], optic_flow_x_prev2[24],optic_flow_y_prev2[24], optic_flow_x_prev3[24],optic_flow_y_prev3[24], optic_flow_x_prev4[24],optic_flow_y_prev4[24];
  float optic_flow_x1[24],optic_flow_x2[24],optic_flow_x3[24],optic_flow_x4[24];
  float optic_flow_y1[24],optic_flow_y2[24],optic_flow_y3[24],optic_flow_y4[24];
  float optic_flow_x_curr1[24],optic_flow_y_curr1[24],optic_flow_x_curr2[24],optic_flow_y_curr2[24],optic_flow_x_curr3[24],optic_flow_y_curr3[24],optic_flow_x_curr4[24],optic_flow_y_curr4[24];

double begin3 = nh.now().toSec();
//1
  for(int i=0;i< sizeof(fxodo_final1)/sizeof(fxodo_final1[0]);i++)
  {
    optic_flow_x_curr1[i] = fxodo_final1[i];
    optic_flow_x1[i] = optic_flow_x_curr1[i] - optic_flow_x_prev1[i];
    optic_flow_x_prev1[i] = optic_flow_x_curr1[i];
  }

  for(int i=0;i<sizeof(fyodo_final1)/sizeof(fyodo_final1[0]);i++)
  {
    optic_flow_y_curr1[i] = fyodo_final1[i];
    optic_flow_y1[i] = optic_flow_y_curr1[i] - optic_flow_y_prev1[i];
    optic_flow_y_prev1[i] = optic_flow_y_curr1[i];
  } 


  
  optic_flow_msg1.layout.dim = (std_msgs::MultiArrayDimension *)
  malloc(sizeof(std_msgs::MultiArrayDimension) * 2);
  optic_flow_msg1.layout.dim[0].label = "length1";
  optic_flow_msg1.layout.dim[0].size = 48;
  optic_flow_msg1.layout.dim[0].stride = 48;
  optic_flow_msg1.layout.data_offset = 0;
  optic_flow_msg1.data_length = 48; // THIS LINE IS CRUCIAL! 
                                  // https://answers.ros.org/question/37185/how-to-initialize-a-uint8multiarray-message/
                                  // https://answers.ros.org/question/10988/use-multiarray-in-rosserial/
  optic_flow_msg1.data = (float *)malloc(sizeof(float)*48); 

  for (int i = 0; i < 24; i++) 
  {
    optic_flow_msg1.data[i] = -optic_flow_x1[i];
    optic_flow_msg1.data[24+i] = optic_flow_y1[i];
  }

  pub_optic_flow1.publish(&optic_flow_msg1);

  free(optic_flow_msg1.data);
  free(optic_flow_msg1.layout.dim);
  
//2
for(int i=0;i< sizeof(fxodo_final2)/sizeof(fxodo_final2[0]);i++)
  {
    optic_flow_x_curr2[i] = fxodo_final2[i];
    optic_flow_x2[i] = optic_flow_x_curr2[i] - optic_flow_x_prev2[i];
    optic_flow_x_prev2[i] = optic_flow_x_curr2[i];
  }

  for(int i=0;i<sizeof(fyodo_final2)/sizeof(fyodo_final2[0]);i++)
  {
    optic_flow_y_curr2[i] = fyodo_final2[i];
    optic_flow_y2[i] = optic_flow_y_curr2[i] - optic_flow_y_prev2[i];
    optic_flow_y_prev2[i] = optic_flow_y_curr2[i];
  } 

  optic_flow_msg2.layout.dim = (std_msgs::MultiArrayDimension *)
  malloc(sizeof(std_msgs::MultiArrayDimension) * 2);
  optic_flow_msg2.layout.dim[0].label = "length2";
  optic_flow_msg2.layout.dim[0].size = 48;
  optic_flow_msg2.layout.dim[0].stride = 48;
  optic_flow_msg2.layout.data_offset = 0;
  optic_flow_msg2.data_length = 48; // THIS LINE IS CRUCIAL! 
                                  // https://answers.ros.org/question/37185/how-to-initialize-a-uint8multiarray-message/
                                  // https://answers.ros.org/question/10988/use-multiarray-in-rosserial/
  optic_flow_msg2.data = (float *)malloc(sizeof(float)*48); 

  for (int i = 0; i < 24; i++) 
  {
    optic_flow_msg2.data[i] = -optic_flow_x2[i];
    optic_flow_msg2.data[24+i] = optic_flow_y2[i];
  }

  pub_optic_flow2.publish(&optic_flow_msg2);

  free(optic_flow_msg2.data);
  free(optic_flow_msg2.layout.dim);

//3
for(int i=0;i< sizeof(fxodo_final3)/sizeof(fxodo_final3[0]);i++)
  {
    optic_flow_x_curr3[i] = fxodo_final3[i];
    optic_flow_x3[i] = optic_flow_x_curr3[i] - optic_flow_x_prev3[i];
    optic_flow_x_prev3[i] = optic_flow_x_curr3[i];
  }

  for(int i=0;i<sizeof(fyodo_final3)/sizeof(fyodo_final3[0]);i++)
  {
    optic_flow_y_curr3[i] = fyodo_final3[i];
    optic_flow_y3[i] = optic_flow_y_curr3[i] - optic_flow_y_prev3[i];
    optic_flow_y_prev3[i] = optic_flow_y_curr3[i];
  } 



  optic_flow_msg3.layout.dim = (std_msgs::MultiArrayDimension *)
  malloc(sizeof(std_msgs::MultiArrayDimension) * 2);
  optic_flow_msg3.layout.dim[0].label = "length3";
  optic_flow_msg3.layout.dim[0].size = 48;
  optic_flow_msg3.layout.dim[0].stride = 48;
  optic_flow_msg3.layout.data_offset = 0;
  optic_flow_msg3.data_length = 48; // THIS LINE IS CRUCIAL! 
                                  // https://answers.ros.org/question/37185/how-to-initialize-a-uint8multiarray-message/
                                  // https://answers.ros.org/question/10988/use-multiarray-in-rosserial/
  optic_flow_msg3.data = (float *)malloc(sizeof(float)*48); 

  for (int i = 0; i < 24; i++) 
  {
    optic_flow_msg3.data[i] = -optic_flow_x3[i];
    optic_flow_msg3.data[24+i] = optic_flow_y3[i];
  }

  pub_optic_flow3.publish(&optic_flow_msg3);

  free(optic_flow_msg3.data);
  free(optic_flow_msg3.layout.dim);

//4

for(int i=0;i< sizeof(fxodo_final4)/sizeof(fxodo_final4[0]);i++)
  {
    optic_flow_x_curr4[i] = fxodo_final4[i];
    optic_flow_x4[i] = optic_flow_x_curr4[i] - optic_flow_x_prev4[i];
    optic_flow_x_prev4[i] = optic_flow_x_curr4[i];
  }

  for(int i=0;i<sizeof(fyodo_final4)/sizeof(fyodo_final4[0]);i++)
  {
    optic_flow_y_curr4[i] = fyodo_final4[i];
    optic_flow_y4[i] = optic_flow_y_curr4[i] - optic_flow_y_prev4[i];
    optic_flow_y_prev4[i] = optic_flow_y_curr4[i];
  } 



  optic_flow_msg4.layout.dim = (std_msgs::MultiArrayDimension *)
  malloc(sizeof(std_msgs::MultiArrayDimension) * 2);
  optic_flow_msg4.layout.dim[0].label = "length4";
  optic_flow_msg4.layout.dim[0].size = 48;
  optic_flow_msg4.layout.dim[0].stride = 48;
  optic_flow_msg4.layout.data_offset = 0;
  optic_flow_msg4.data_length = 48; // THIS LINE IS CRUCIAL! 
                                  // https://answers.ros.org/question/37185/how-to-initialize-a-uint8multiarray-message/
                                  // https://answers.ros.org/question/10988/use-multiarray-in-rosserial/
  optic_flow_msg4.data = (float *)malloc(sizeof(float)*48); 

  for (int i = 0; i < 24; i++) 
  {
    optic_flow_msg4.data[i] = -optic_flow_x4[i];
    optic_flow_msg4.data[24+i] = optic_flow_y4[i];
  }

  pub_optic_flow4.publish(&optic_flow_msg4);

  free(optic_flow_msg4.data);
  free(optic_flow_msg4.layout.dim);
 // double begin3 = nh.now().toSec();
  nh.spinOnce();

  delay(1);
  
  // INSTALL: git clone https://github.com/ros-drivers/rosserial.git
  // RUN: rosrun rosserial_arduino serial_node.py _port:=/dev/ttyACM1 _baud:=115200
  // NOTE CANNOT VIEW SERIAL MONITOR AND CONNECT TO ROSSERIAL SIMULTANEOUSLY
  // NEED delay(1) above, else RUNTIME ERROR: Unable to sync with device; possible link problem or link software version mismatch such as hydro rosserial_python with groovy Arduino

  // FIXED the data pointer issues
  // https://answers.ros.org/question/285670/rosserial-multi-array-issue/
  // https://answers.ros.org/question/10988/use-multiarray-in-rosserial/


  // STOPS PUBLISHING AFTER 44 seconds (array of 7), 23 seconds (14), 12 seconds (28), and 6 seconds (50).
  // Not sure why, but I think the buffer is overflowing or something, the time it does is inversely proportional to the length of the array
  // https://github.com/ros-drivers/rosserial/issues/271
  
  // https://answers.ros.org/question/173564/rosserial-arduino-serial-port-read-failure/
  // Commented out Serial.print statement

  // https://answers.ros.org/question/11022/rosserial_arduino-trouble-with-too-much-messaging/
  // Increasing the baud rate to 230400 in the (rosserial run parameter and the C code) did not help
  // Increasing the publish buffer size also did not help (~/.platformio/ros.h and ~/.platformio/ros/node_handle.h)

  // https://github.com/ros-drivers/rosserial/issues/271
  // Maybe should try more powerful microcontroller

  // https://answers.ros.org/question/215687/rosserial-arduino-connectivity-problem/
  // Tried commenting out logging, nh.loginfo("Program info");

  // https://answers.ros.org/question/208079/rosserial-arduino-custom-message-message-larger-than-buffer/
  // https://answers.ros.org/question/28890/using-rosserial-for-a-atmega168arduino-based-motorcontroller/
  // http://wiki.ros.org/rosserial_arduino/Tutorials/NodeHandle%20and%20ArduinoHardware
  // https://answers.ros.org/question/73627/how-to-increase-rosserial-buffer-size/
  // Tried increasing the default publisher buffer

  // Tried clearing / flushing the buffer
  // https://forum.arduino.cc/index.php?topic=234151.0
  // https://forum.arduino.cc/index.php?topic=252001.0

  // https://github.com/ros-drivers/rosserial/issues/351
  // https://github.com/chrisspen/rosserial
  // Tried different rosserial package



}
