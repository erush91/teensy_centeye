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
#define HWSERIAL Serial4
#define SESERIAL Serial2
#define THSERIAL Serial3
#define FOSERIAL Serial1

//these objects are used to set up a publisher/subscriber ros node
// std_msgs::Float32 temp_msg;
// ros::Publisher pub("temperature", &temp_msg);

std_msgs::Int32MultiArray optic_flow_msg;
ros::Publisher pub_optic_flow("optic_flow_front", &optic_flow_msg);

std_msgs::Int32MultiArray optic_flow_msg1;
ros::Publisher pub_optic_flow1("optic_flow_right", &optic_flow_msg1);

std_msgs::Int32MultiArray optic_flow_msg2;
ros::Publisher pub_optic_flow2("optic_flow_left", &optic_flow_msg2);

std_msgs::Int32MultiArray optic_flow_msg3;
ros::Publisher pub_optic_flow3("optic_flow_back", &optic_flow_msg3);

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
    SESERIAL.begin(115200, SERIAL_8N1);
    THSERIAL.begin(115200, SERIAL_8N1);
    FOSERIAL.begin(115200, SERIAL_8N1);

    
    //start the ROS node
    nh.getHardware()->setBaud(115200);
    nh.initNode();
    // nh.loginfo("Program info");
    nh.advertise(pub_optic_flow);
    nh.advertise(pub_optic_flow1);
    nh.advertise(pub_optic_flow2);
    nh.advertise(pub_optic_flow3);
}

///////////////////////
// RUNS CONTINUOUSLY //
///////////////////////
void loop()
{

  // byte TxMsg[6] = {255, 8, 36, 0, 255, 254}; // Retrieve Device ID
  // byte TxMsg[6] = {255, 8, 36, 9, 255, 254}; // Retrieve Sensor Data // Dataset 9 (for V1 sensor - Classic OF Sensor)
  byte TxMsg[6] = {255, 8, 36, 11, 255, 254}; // Retrieve Sensor Data // Dataset 11 (for V2 sensor - HT Sensor)
  // byte TxMsg[6] = {255, 8, 36, 3, 255, 254}; // Retrieve Sensor Data // Dataset 3
  
  // 255, 1,36, CMD, Arg 0, Arg 1, ..., Arg N-1, 255, 254 // Send Generic Command
  // 255, 1,36, 46, Arg 0, Arg 1, Arg 2, Arg 3, 255, 254 // Change HT Command
  // Arg 0 - 3 are four bytes that comprise of a single parameter, wait 1/10s between changinh parameters

  int RxMsg;
  unsigned int bytecount = 0;
  unsigned char Data[246];

  int RxMsg1;
  unsigned int bytecount1 = 0;
  unsigned char Data1[246];

  int RxMsg2;
  unsigned int bytecount2 = 0;
  unsigned char Data2[246];

  int RxMsg3;
  unsigned int bytecount3 = 0;
  unsigned char Data3[246];


  HWSERIAL.write(TxMsg, 6);
  SESERIAL.write(TxMsg, 6);
  THSERIAL.write(TxMsg, 6);
  FOSERIAL.write(TxMsg, 6);
 
  
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

 while(SESERIAL.available() && bytecount1 < 246)
  {

    RxMsg1 = SESERIAL.read();
    // Serial.print(RxMsg);
    // Serial.print(", ");
    Data1[bytecount1] = RxMsg1;

    bytecount1++;
    
    // Delay must be short to prevent buffer from overflowing, but long enough for a new byte to come in.
    delayMicroseconds(90);
  
  }

  while(THSERIAL.available() && bytecount2 < 246)
  {
    RxMsg2 = THSERIAL.read();
    // Serial.print(RxMsg);
    // Serial.print(", ");
    Data2[bytecount2] = RxMsg2;

    bytecount2++;
    
    // Delay must be short to prevent buffer from overflowing, but long enough for a new byte to come in.
    delayMicroseconds(90);
  }

   while(FOSERIAL.available() && bytecount3 < 246)
  {
    RxMsg3 = FOSERIAL.read();
    // Serial.print(RxMsg);
    // Serial.print(", ");
    Data3[bytecount3] = RxMsg3;

    bytecount3++;
    
    // Delay must be short to prevent buffer from overflowing, but long enough for a new byte to come in.
    delayMicroseconds(90);
  }



  // Enhance readability in serial monitor
  delay(50);

  // Serial.println();

  // Serial.clear(); 
  // If Serial.clear() is NOT COMMENTED OUT, CAUSES ROS RUNTIME ERROR:
  // Unable to sync with device; possible link problem or link software version mismatch such as hydro rosserial_python with groovy Arduino
  
  HWSERIAL.clear();
  SESERIAL.clear();
  THSERIAL.clear();
  FOSERIAL.clear();
 
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
   signed char CapQX1[121], CapQY1[121];
   signed char CapQX2[121], CapQY2[121];
    signed char CapQX3[121], CapQY3[121];
   

  // Window 0 = Global
  // Window 1 - 24 = Classic V1
  // Window 25 - 120 = Small
  for(int i = 0; i < 121; i++)
  {
    CapQX[i] = Data[4+2*i];
    CapQY[i] = Data[5+2*i];
   
  }


  for(int i = 0; i < 121; i++)
  {
    CapQX1[i] = Data1[4+2*i];
    CapQY1[i] = Data1[5+2*i];
  }

  for(int i = 0; i < 121; i++)
  {
    CapQX2[i] = Data2[4+2*i];
    CapQY2[i] = Data2[5+2*i];
  }

   for(int i = 0; i < 121; i++)
  {
    CapQX3[i] = Data3[4+2*i];
    CapQY3[i] = Data3[5+2*i];
  }



  optic_flow_msg.layout.dim = (std_msgs::MultiArrayDimension *)
  malloc(sizeof(std_msgs::MultiArrayDimension) * 2);
  optic_flow_msg.layout.dim[0].label = "length";
  optic_flow_msg.layout.dim[0].size = 242;
  optic_flow_msg.layout.dim[0].stride = 242;
  optic_flow_msg.layout.data_offset = 0;
  optic_flow_msg.data_length = 242; // THIS LINE IS CRUCIAL! 
                                  // https://answers.ros.org/question/37185/how-to-initialize-a-uint8multiarray-message/
                                  // https://answers.ros.org/question/10988/use-multiarray-in-rosserial/
  optic_flow_msg.data = (long int *)malloc(sizeof(int)*242); 

  for (int i = 0; i < 121; i++) 
  {
    optic_flow_msg.data[i] = CapQX[i];
  }
  for (int i = 0; i < 121; i++) 
  {
    optic_flow_msg.data[121+i] = CapQY[i];
  }

  pub_optic_flow.publish(&optic_flow_msg);

  free(optic_flow_msg.data);
  free(optic_flow_msg.layout.dim);


  optic_flow_msg1.layout.dim = (std_msgs::MultiArrayDimension *)
  malloc(sizeof(std_msgs::MultiArrayDimension) * 2);
  optic_flow_msg1.layout.dim[0].label = "length1";
  optic_flow_msg1.layout.dim[0].size = 242;
  optic_flow_msg1.layout.dim[0].stride = 242;
  optic_flow_msg1.layout.data_offset = 0;
  optic_flow_msg1.data_length = 242; // THIS LINE IS CRUCIAL! 
                                  // https://answers.ros.org/question/37185/how-to-initialize-a-uint8multiarray-message/
                                  // https://answers.ros.org/question/10988/use-multiarray-in-rosserial/
  optic_flow_msg1.data = (long int *)malloc(sizeof(int)*242); 

  for (int i = 0; i < 121; i++) 
  {
    optic_flow_msg1.data[i] = CapQX1[i];
  }
  for (int i = 0; i < 121; i++) 
  {
    optic_flow_msg1.data[121+i] = CapQY1[i];
  }

  pub_optic_flow1.publish(&optic_flow_msg1);

  free(optic_flow_msg1.data);
  free(optic_flow_msg1.layout.dim);


 optic_flow_msg2.layout.dim = (std_msgs::MultiArrayDimension *)
  malloc(sizeof(std_msgs::MultiArrayDimension) * 2);
  optic_flow_msg2.layout.dim[0].label = "length2";
  optic_flow_msg2.layout.dim[0].size = 242;
  optic_flow_msg2.layout.dim[0].stride = 242;
  optic_flow_msg2.layout.data_offset = 0;
  optic_flow_msg2.data_length = 242; // THIS LINE IS CRUCIAL! 
                                  // https://answers.ros.org/question/37185/how-to-initialize-a-uint8multiarray-message/
                                  // https://answers.ros.org/question/10988/use-multiarray-in-rosserial/
  optic_flow_msg2.data = (long int *)malloc(sizeof(int)*242); 

  for (int i = 0; i < 121; i++) 
  {
    optic_flow_msg2.data[i] = CapQX2[i];
  }
  for (int i = 0; i < 121; i++) 
  {
    optic_flow_msg2.data[121+i] = CapQY2[i];
  }

  pub_optic_flow2.publish(&optic_flow_msg2);

  free(optic_flow_msg2.data);
  free(optic_flow_msg2.layout.dim);

  optic_flow_msg3.layout.dim = (std_msgs::MultiArrayDimension *)
  malloc(sizeof(std_msgs::MultiArrayDimension) * 2);
  optic_flow_msg3.layout.dim[0].label = "length2";
  optic_flow_msg3.layout.dim[0].size = 242;
  optic_flow_msg3.layout.dim[0].stride = 242;
  optic_flow_msg3.layout.data_offset = 0;
  optic_flow_msg3.data_length = 242; // THIS LINE IS CRUCIAL! 
                                  // https://answers.ros.org/question/37185/how-to-initialize-a-uint8multiarray-message/
                                  // https://answers.ros.org/question/10988/use-multiarray-in-rosserial/
  optic_flow_msg3.data = (long int *)malloc(sizeof(int)*242); 

  for (int i = 0; i < 121; i++) 
  {
    optic_flow_msg3.data[i] = CapQX3[i];
  }
  for (int i = 0; i < 121; i++) 
  {
    optic_flow_msg3.data[121+i] = CapQY3[i];
  }

  pub_optic_flow3.publish(&optic_flow_msg3);

  free(optic_flow_msg3.data);
  free(optic_flow_msg3.layout.dim);


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
