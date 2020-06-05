#include <Arduino.h>
#include <ros.h> // PlatformIO Library: rosserial_arduino by Open Agriculture Initiative
//#include <std_msgs/Int32.h>
#include <std_msgs/Float32MultiArray.h>
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
#define HWSERIAL Serial4

//these objects are used to set up a publisher/subscriber ros node
// std_msgs::Float32 temp_msg;
// ros::Publisher pub("temperature", &temp_msg);

std_msgs::Float32MultiArray optic_flow_msg;
ros::Publisher pub_optic_flow("optic_flow", &optic_flow_msg);

ros::NodeHandle nh;

///////////////
// RUNS ONCE //
///////////////
void setup()
{
    // DO NOT VIEW THE SERIAL MONITOR, if you do, this will cause a ...
    // Run loop error: Serial Port read failure: device reports readiness to read but returned no data (device disconnected or multiple access on port?

    Serial.begin(115200);
    HWSERIAL.begin(115200, SERIAL_8N1);
    
    //start the ROS node
    nh.getHardware()->setBaud(115200);
    nh.initNode();
    // nh.loginfo("Program info");
    nh.advertise(pub_optic_flow);
}

///////////////////////
// RUNS CONTINUOUSLY //
///////////////////////
void loop()
{

  // byte TxMsg[6] = {255, 8, 36, 0, 255, 254}; // Retrieve Device ID
  //byte TxMsg[6] = {255, 8, 36, 9, 255, 254}; // Retrieve Sensor Data // Dataset 9
  byte TxMsg[6] = {255, 8, 36, 3, 255, 254}; // Retrieve Sensor Data // Dataset 3

  int RxMsg;
  unsigned int bytecount = 0;
  unsigned char Data[100];
  
   unsigned long start_time;
  unsigned long elapsed_time; 

  unsigned long start_time1;
  unsigned long elapsed_time1;

  unsigned long start_time2;
  unsigned long elapsed_time2;

  start_time = micros();

  HWSERIAL.write(TxMsg, 6);

  // Buffer of UART probably small, this makes sure buffer has some bytes
  delay(1); // Need 3ms delay before reading Device ID (otherwise first byte = 252)

  while(HWSERIAL.available() && bytecount < 100)
  {

    RxMsg = HWSERIAL.read();
    // Serial.print(RxMsg);
    // Serial.print(", ");
    Data[bytecount] = RxMsg;

    bytecount++;
    
    // Delay must be short to prevent buffer from overflowing, but long enough for a new byte to come in.
    delayMicroseconds(90);
 
  }
  elapsed_time = micros() - start_time;

  

  //Serial.println();

  //Serial.clear();
  HWSERIAL.clear();

  delay(14);

  /////////////////////////////
  // ASSIGN DATA TO POINTERS //
  /////////////////////////////

  // Int = 32 Bytes
  int *p_int = (int*)Data;

  // Short Int = 16 Bytes
  short *p_short = (short*)Data;

  //////////////////////////////
  // ASSIGN DATA TO VARIABLES //
  //////////////////////////////

  //frame counter

  int frame_counter = p_int[0];
  
  start_time1 = micros();
  // x and y window optical flow odometries
  short xodo[24],yodo[24];
  int fxsign_new,fysign_new,fxsign_old,fysign_old;
  int rollover_counter_x[24]={};
  int rollover_counter_y[24]={};
  float fyodo[24],fxodo[24],fxodo_new[24],fxodo_old[24],fyodo_new[24],fyodo_old[24];
  float fxodo_final[24],fyodo_final[24];

  //int count = 2;
  
  for (int w = 0; w < 24; ++w) 
  { 
    //int *p_w_int = (int*)(Data + w * 20 + 44);
    //short *p_w_short = (short*)(Data + w * 20 + 44);
    xodo[w] = p_short[2+2*w];
    yodo[w] = p_short[3+2*w];
  }


   for(int w = 0; w < 24; ++w)
   {
     fxodo[w] = xodo[w];
     fyodo[w] = yodo[w];
   }


   for(int i=0; i<24; i++)
   {

      if(i == 0)
      {
        rollover_counter_x[i] = 0;
        fxodo_new[i] = fxodo[i];
        fxodo_old[i] = fxodo_new[i];
        fxodo_final[i] = fxodo_new[i];
      }

      else
      {
        fxodo_old[i] = fxodo[i-1];

        fxodo_new[i] = fxodo[i];

        fxsign_new = sign<float>(fxodo_new[i]);
        fxsign_old = sign<float>(fxodo_old[i]);

        if(fxsign_new * fxsign_old < 0)
        {
          if( abs(fxodo_old[i]) > 12384 && abs(fxodo_new[i]) > 12384)
          {
            if(fxodo_new[i]<0)
              rollover_counter_x[i]++;
            else
              rollover_counter_x[i]--;  
          }
        }
        fxodo_final[i] = fxodo_new[i] + 65536 * rollover_counter_x[i];
      }
   }

   for(int i=0; i<24; i++)
   {

      if(i == 0)
      {
        rollover_counter_y[i] = 0;
        fyodo_new[i] = fyodo[i];
        fyodo_old[i] = fyodo_new[i];
        fyodo_final[i] = fyodo_new[i];
      }

      else
      {
        fyodo_old[i] = fyodo[i-1];

        fyodo_new[i] = fyodo[i];

        fysign_new = sign<float>(fyodo_new[i]);
        fysign_old = sign<float>(fyodo_old[i]);

        if(fysign_new * fysign_old < 0)
        {
          if( abs(fyodo_old[i]) > 12384 || abs(fyodo_new[i]) > 12384)
          {
            if(fyodo_new[i]<0)
              rollover_counter_y[i]++;
            else
              rollover_counter_y[i]--;  
          }
        }
        fyodo_final[i] = fyodo_new[i] + 65536 * rollover_counter_y[i];
      }
   }


  float optic_flow_x_prev[24],optic_flow_y_prev[24];
  float optic_flow_x[24];
  float optic_flow_y[24];
  float optic_flow_x_curr[24],optic_flow_y_curr[24];


  for(int i=0;i< sizeof(fxodo_final)/sizeof(fxodo_final[0]);i++)
  {
    optic_flow_x_curr[i] = fxodo_final[i];
    optic_flow_x[i] = optic_flow_x_curr[i] - optic_flow_x_prev[i];
    optic_flow_x_prev[i] = optic_flow_x_curr[i];
  }

  for(int i=0;i<sizeof(fyodo_final)/sizeof(fyodo_final[0]);i++)
  {
    optic_flow_y_curr[i] = fyodo_final[i];
    optic_flow_y[i] = optic_flow_y_curr[i] - optic_flow_y_prev[i];
    optic_flow_y_prev[i] = optic_flow_y_curr[i];
  } 
  elapsed_time1 = micros() - start_time1;


  start_time2 = micros();
  optic_flow_msg.layout.dim = (std_msgs::MultiArrayDimension *)
  malloc(sizeof(std_msgs::MultiArrayDimension) * 2);
  optic_flow_msg.layout.dim[0].label = "length";
  optic_flow_msg.layout.dim[0].size = 48;
  optic_flow_msg.layout.dim[0].stride = 48;
  optic_flow_msg.layout.data_offset = 0;
  optic_flow_msg.data_length = 48; // THIS LINE IS CRUCIAL! 
                                  // https://answers.ros.org/question/37185/how-to-initialize-a-uint8multiarray-message/
                                  // https://answers.ros.org/question/10988/use-multiarray-in-rosserial/
  optic_flow_msg.data = (float *)malloc(sizeof(float)*48); 

  for (int i = 0; i < 24; i++) 
  {
    optic_flow_msg.data[i] = optic_flow_x[i];
    optic_flow_msg.data[24+i] = optic_flow_y[i];
  }

  pub_optic_flow.publish(&optic_flow_msg);

  free(optic_flow_msg.data);
  free(optic_flow_msg.layout.dim);

  // Buffer time
  
  
  elapsed_time2 = micros() - start_time2;

  char time1[10],time2[8],time3[8];
  dtostrf(elapsed_time, 8, 2, time1);
  dtostrf(elapsed_time1, 6, 2, time2);
  dtostrf(elapsed_time2, 6, 2, time3);

  nh.loginfo("time1:");
  nh.loginfo(time1);
  nh.loginfo("time2:");
  nh.loginfo(time2);
  nh.loginfo("time3:");
  nh.loginfo(time3);

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
