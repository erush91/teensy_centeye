#include <Arduino.h>
#include <ros.h> // PlatformIO Library: rosserial_arduino by Open Agriculture Initiative
#include <std_msgs/Int32.h>
#include <std_msgs/Int32MultiArray.h>

//#include <SD.h>
// #include <SPI.h>

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
#define HWSERIAL Serial1

//these objects are used to set up a publisher/subscriber ros node
// std_msgs::Float32 temp_msg;
// ros::Publisher pub("temperature", &temp_msg);

std_msgs::Int32MultiArray optic_flow_msg;
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
  // byte TxMsg[6] = {255, 8, 36, 9, 255, 254}; // Retrieve Sensor Data // Dataset 11 (for V1 sensor - Classic OF Sensor)
  byte TxMsg[6] = {255, 8, 36, 11, 255, 254}; // Retrieve Sensor Data // Dataset 11 (for V2 sensor - HT Sensor)
  // byte TxMsg[6] = {255, 8, 36, 3, 255, 254}; // Retrieve Sensor Data // Dataset 3
  
  // 255, 1,36, CMD, Arg 0, Arg 1, ..., Arg N-1, 255, 254 // Send Generic Command
  // 255, 1,36, 46, Arg 0, Arg 1, Arg 2, Arg 3, 255, 254 // Change HT Command
  // Arg 0 - 3 are four bytes that comprise of a single parameter, wait 1/10s between changinh parameters

  int RxMsg;
  unsigned int bytecount = 0;
  unsigned char Data[246];

  HWSERIAL.write(TxMsg, 6);

  // Serial.println();
  // Serial.print("NEW DATA");
  // Serial.println();

  // Buffer of UART probably small, this makes sure buffer has some bytes
  delay(1); // Need 3ms delay before reading Device ID (otherwise first byte = 252)

  while(HWSERIAL.available() && bytecount < 246)
  {
    RxMsg = HWSERIAL.read();
    // Serial.print(RxMsg);
    // Serial.print(", ");
    Data[bytecount] = RxMsg;

    bytecount++;
    
    // Delay must be short to prevent buffer from overflowing, but long enough for a new byte to come in.
    delayMicroseconds(90);
  }

  // Enhance readability in serial monitor
  delay(10);

  // Serial.println();

  // Serial.clear(); 
  // If Serial.clear() is NOT COMMENTED OUT, CAUSES ROS RUNTIME ERROR:
  // Unable to sync with device; possible link problem or link software version mismatch such as hydro rosserial_python with groovy Arduino
  
  HWSERIAL.clear();

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

  signed char CapQX[121], CapQY[121];

  int frame_counter = p_int[0];

  // Window 0 = Global
  // Window 1 - 24 = Classic V1
  // Window 25 - 120 = Small
  for(int i = 0; i < 121; i++)
  {
    CapQX[i] = Data[4+2*i];
    CapQY[i] = Data[5+2*i];
  }

  // // global_x_of (4-Byte Long Integer)
  // int global_x_of = p_int[0];

  // // global_y_of (4-Byte Long Integer)
  // int global_y_of = p_int[1];

  // // x and y window optical flow odometries
  // int x_of[25],y_of[25];

  // // stereo distance and confidence flags for windows
  // short s_dist[25],s_conf[25];

  // // Note: window 0 does not have any information
  // for (int w = 0; w < 25; ++w) 
  // { 
  //   int *p_w_int = (int*)(Data + w * 20 + 44);
  //   short *p_w_short = (short*)(Data + w * 20 + 44);
  //   x_of[w] = p_w_int[0];
  //   y_of[w] = p_w_int[1];
  //   s_dist[w] = p_w_short[4];
  //   s_conf[w] = p_w_short[5];
  // }

  // int optic_flow_x[25];
  // int optic_flow_x_acc[25];
  // int optic_flow_x_acc_last[25];
  // int optic_flow_y[25];
  // int optic_flow_y_acc[25];
  // int optic_flow_y_acc_last[25];

  // for( int i = 0; i < sizeof(x_of)/sizeof(x_of[0]); i++)
  // {
  //   optic_flow_x_acc[i] = x_of[i];
  //   optic_flow_x[i] = optic_flow_x_acc[i] - optic_flow_x_acc_last[i];
  //   optic_flow_x_acc_last[i] = optic_flow_x_acc[i]; 
  // }

  // for( int i = 0; i < sizeof(y_of)/sizeof(y_of[0]); i++)
  // {
  //   optic_flow_y_acc[i] = y_of[i];
  //   optic_flow_y[i] = optic_flow_y_acc[i] - optic_flow_y_acc_last[i];
  //   optic_flow_y_acc_last[i] = optic_flow_y_acc[i];
  // }

  // Serial.println();
  // Serial.println();

  // Serial.print("optic_flow_x: ");
  // for (int w = 0; w < 25; ++w) 
  // {
  //   Serial.print(optic_flow_x[w]);
  //   Serial.print(", ");

  // }
  
  // Serial.println();
  // Serial.println();

  // Serial.print("optic_flow_y: ");
  // for (int w = 0; w < 25; ++w) 
  // {
  //   Serial.print(optic_flow_y[w]);
  //   Serial.print(", ");

  // }


  // Serial.println();
  // Serial.println();
  // Serial.print("global_x_of: ");
  // Serial.print(global_x_of);

  // Serial.println();
  // Serial.println();
  // Serial.print("global_y_of: ");
  // Serial.print(global_y_of);

  // Serial.println();
  // Serial.println();
  // Serial.print("x_of: ");
  // for (int w = 0; w < 25; ++w) 
  // {
  //   Serial.print(x_of[w]);
  //   Serial.print(", ");
  // }

  // Serial.println();
  // Serial.println();
  // Serial.print("y_of: ");
  // for (int w = 0; w < 25; ++w) 
  // { 
  //   Serial.print(y_of[w]);
  //   Serial.print(", ");
  // }

  // Serial.println();
  // Serial.println();
  // Serial.print("s_dist: ");
  // for (int w = 0; w < 25; ++w) 
  // { 
  //   Serial.print(s_dist[w]);
  //   Serial.print(", ");
  // }
  
  // Serial.println();
  // Serial.println();
  // Serial.print("s_conf: ");
  // for (int w = 0; w < 25; ++w) 
  // { 
  //   Serial.print(s_conf[w]);
  //   Serial.print(", ");
  // }
  
  // Serial.println();
  // Serial.println();

  optic_flow_msg.layout.dim = (std_msgs::MultiArrayDimension *)
  malloc(sizeof(std_msgs::MultiArrayDimension) * 2);
  optic_flow_msg.layout.dim[0].label = "length";
  optic_flow_msg.layout.dim[0].size = 50;
  optic_flow_msg.layout.dim[0].stride = 50;
  optic_flow_msg.layout.data_offset = 0;
  optic_flow_msg.data_length = 50; // THIS LINE IS CRUCIAL! 
                                  // https://answers.ros.org/question/37185/how-to-initialize-a-uint8multiarray-message/
                                  // https://answers.ros.org/question/10988/use-multiarray-in-rosserial/
  optic_flow_msg.data = (long int *)malloc(sizeof(int)*50); 

  for (int i = 0; i < 25; i++) 
  {
    optic_flow_msg.data[i] = optic_flow_x[i];
  }
  for (int i = 0; i < 25; i++) 
  {
    optic_flow_msg.data[25+i] = optic_flow_y[i];
  }

  pub_optic_flow.publish(&optic_flow_msg);

  free(optic_flow_msg.data);
  free(optic_flow_msg.layout.dim);

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












