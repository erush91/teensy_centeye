#include <Arduino.h>
#include <ros.h> // PlatformIO Library: rosserial_arduino by Open Agriculture Initiative
#include <std_msgs/Float32.h>
#include <std_msgs/Float32MultiArray.h>

//IMPORTANT
//While using rosserial make sure to change the value of buffer size in the ros.h(ros_lib folder).

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
// IMPORTANT be sure  to edit the SERIAL_RX_BUFFER_SIZE to 2048 in the files Serial1.c, Serial2.c, Serial3.c ...... Serial5.c files 
// location of serial.c files arduino-1.8.8/hardware/teensy/avr/cores/teensy3 ( check for your computer)

//these objects are used to set up a publisher/subscriber ros node
// std_msgs::Float32 temp_msg;
// ros::Publisher pub("temperature", &temp_msg);
int counts=0;

std_msgs::Float32MultiArray optic_flow_msg1;
ros::Publisher pub_optic_flow1("optic_flow_left", &optic_flow_msg1);

std_msgs::Float32MultiArray optic_flow_msg2;
ros::Publisher pub_optic_flow2("optic_flow_back", &optic_flow_msg2);

std_msgs::Float32MultiArray optic_flow_msg3;
ros::Publisher pub_optic_flow3("optic_flow_front", &optic_flow_msg3);

std_msgs::Float32MultiArray optic_flow_msg4;
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
  // byte TxMsg[6] = {255, 8, 36, 9, 255, 254}; // Retrieve Sensor Data // Dataset 9 (for V1 sensor - Classic OF Sensor)
  byte TxMsg[6] = {255, 8, 36, 11, 255, 254}; // Retrieve Sensor Data // Dataset 11 (for V2 sensor - HT Sensor)
  // byte TxMsg[6] = {255, 8, 36, 3, 255, 254}; // Retrieve Sensor Data // Dataset 3
  
  // 255, 1,36, CMD, Arg 0, Arg 1, ..., Arg N-1, 255, 254 // Send Generic Command
  // 255, 1,36, 46, Arg 0, Arg 1, Arg 2, Arg 3, 255, 254 // Change HT Command
  // Arg 0 - 3 are four bytes that comprise of a single parameter, wait 1/10s between changinh parameters


  int RxMsg1;
  unsigned int bytecount1 = 0;
  unsigned char Data1[246];

  int RxMsg2;
  unsigned int bytecount2 = 0;
  unsigned char Data2[246];

  int RxMsg3;
  unsigned int bytecount3 = 0;
  unsigned char Data3[246];

  int RxMsg4;
  unsigned int bytecount4 = 0;
  unsigned char Data4[246];


  Serial1.write(TxMsg, 6);
  Serial2.write(TxMsg, 6);
  Serial4.write(TxMsg, 6);
  Serial3.write(TxMsg, 6);
 
  
  // Serial.println();
  // Serial.print("NEW DATA");
  // Serial.println();

  // Buffer of UART probably small, this makes sure buffer has some bytes
  delay(1); // Need 3ms delay before reading Device ID (otherwise first byte = 252)

  double begin1 = nh.now().toNsec();
  while(Serial1.available() && bytecount1 < 246)
  {
    RxMsg1 = Serial1.read();
    // Serial.print(RxMsg);
    // Serial.print(", ");
    Data1[bytecount1] = RxMsg1;

    bytecount1++;
    
    // Delay must be short to prevent buffer from overflowing, but long enough for a new byte to come in.
    delayMicroseconds(90);
  }


  while(Serial2.available() && bytecount2 < 246)
  {

    RxMsg2 = Serial2.read();
    // Serial.print(RxMsg);
    // Serial.print(", ");
    Data2[bytecount2] = RxMsg2;

    bytecount2++;
    
    // Delay must be short to prevent buffer from overflowing, but long enough for a new byte to come in.
    delayMicroseconds(90);
  
  }

  while(Serial3.available() && bytecount3 < 246)
  {
    RxMsg3 = Serial3.read();
    // Serial.print(RxMsg);
    // Serial.print(", ");
    Data3[bytecount3] = RxMsg3;

    bytecount3++;
    
    // Delay must be short to prevent buffer from overflowing, but long enough for a new byte to come in.
    delayMicroseconds(90);
  }

  while(Serial4.available() && bytecount4 < 246)
  {
    RxMsg4 = Serial4.read();
    // Serial.print(RxMsg);
    // Serial.print(", ");
    Data4[bytecount4] = RxMsg4;

    bytecount4++;
    
    // Delay must be short to prevent buffer from overflowing, but long enough for a new byte to come in.
    delayMicroseconds(90);
  }

  double end1 = nh.now().toNsec();

  

  // Enhance readability in serial monitor
  //delay(50);

  // Serial.println();

  // Serial.clear(); 
  // If Serial.clear() is NOT COMMENTED OUT, CAUSES ROS RUNTIME ERROR:
  // Unable to sync with device; possible link problem or link software version mismatch such as hydro rosserial_python with groovy Arduino
  
  Serial1.clear();
  Serial2.clear();
  Serial3.clear();
  Serial4.clear();
 
  //////////////////////////////
  // ASSIGN DATA TO VARIABLES //
  //////////////////////////////

  signed char CapQX1[121], CapQY1[121];
  signed char CapQX2[121], CapQY2[121];
  signed char CapQX3[121], CapQY3[121];
  signed char CapQX4[121], CapQY4[121];
  
  // Window 0 = Global
  // Window 1 - 24 = Classic V1
  // Window 25 - 120 = Small
  double begin2 = nh.now().toNsec();
  for(int i = 0; i < 121; i++)
  {
    CapQX1[i] =   Data1[4+2*i];
    CapQY1[i] = - Data1[5+2*i];
    CapQX2[i] =   Data2[4+2*i];
    CapQY2[i] = - Data2[5+2*i];
    CapQX3[i] =   Data3[4+2*i];
    CapQY3[i] = - Data3[5+2*i];
    CapQX4[i] =   Data4[4+2*i];
    CapQY4[i] = - Data4[5+2*i];
  }
  double end2 = nh.now().toNsec();

  double begin3 = nh.now().toNsec();
  //FRONT
  optic_flow_msg1.layout.dim = (std_msgs::MultiArrayDimension *)
  malloc(sizeof(std_msgs::MultiArrayDimension) * 2);
  optic_flow_msg1.layout.dim[0].label = "length";
  optic_flow_msg1.layout.dim[0].size = 242;
  optic_flow_msg1.layout.dim[0].stride = 242;
  optic_flow_msg1.layout.data_offset = 0;
  optic_flow_msg1.data_length = 242; // THIS LINE IS CRUCIAL! 
                                  // https://answers.ros.org/question/37185/how-to-initialize-a-uint8multiarray-message/
                                  // https://answers.ros.org/question/10988/use-multiarray-in-rosserial/
  optic_flow_msg1.data = (float *)malloc(sizeof(float)*242); 

  for (int i = 0; i < 121; i++) 
  {
    optic_flow_msg1.data[i] = CapQX1[i];
    optic_flow_msg1.data[121+i] = CapQY1[i];
  }

  pub_optic_flow1.publish(&optic_flow_msg1);

  free(optic_flow_msg1.data);
  free(optic_flow_msg1.layout.dim);


  //RIGHT
  optic_flow_msg2.layout.dim = (std_msgs::MultiArrayDimension *)
  malloc(sizeof(std_msgs::MultiArrayDimension) * 2);
  optic_flow_msg2.layout.dim[0].label = "length1";
  optic_flow_msg2.layout.dim[0].size = 242;
  optic_flow_msg2.layout.dim[0].stride = 242;
  optic_flow_msg2.layout.data_offset = 0;
  optic_flow_msg2.data_length = 242; // THIS LINE IS CRUCIAL! 
                                  // https://answers.ros.org/question/37185/how-to-initialize-a-uint8multiarray-message/
                                  // https://answers.ros.org/question/10988/use-multiarray-in-rosserial/
  optic_flow_msg2.data = (float *)malloc(sizeof(float)*242); 

  for (int i = 0; i < 121; i++) 
  {
    optic_flow_msg2.data[i] = CapQX2[i];
    optic_flow_msg2.data[121+i] = CapQY2[i];
  }

  pub_optic_flow2.publish(&optic_flow_msg2);

  free(optic_flow_msg2.data);
  free(optic_flow_msg2.layout.dim);

  

  //LEFT
  optic_flow_msg3.layout.dim = (std_msgs::MultiArrayDimension *)
  malloc(sizeof(std_msgs::MultiArrayDimension) * 2);
  optic_flow_msg3.layout.dim[0].label = "length2";
  optic_flow_msg3.layout.dim[0].size = 242;
  optic_flow_msg3.layout.dim[0].stride = 242;
  optic_flow_msg3.layout.data_offset = 0;
  optic_flow_msg3.data_length = 242; // THIS LINE IS CRUCIAL! 
                                  // https://answers.ros.org/question/37185/how-to-initialize-a-uint8multiarray-message/
                                  // https://answers.ros.org/question/10988/use-multiarray-in-rosserial/
  optic_flow_msg3.data = (float *)malloc(sizeof(float)*242); 

  for (int i = 0; i < 121; i++) 
  {
    optic_flow_msg3.data[i] = CapQX3[i];
    optic_flow_msg3.data[121+i] = CapQY3[i];
  }

  pub_optic_flow3.publish(&optic_flow_msg3);

  free(optic_flow_msg3.data);
  free(optic_flow_msg3.layout.dim);

  
  
  //BACK
  optic_flow_msg4.layout.dim = (std_msgs::MultiArrayDimension *)
  malloc(sizeof(std_msgs::MultiArrayDimension) * 2);
  optic_flow_msg4.layout.dim[0].label = "length3";
  optic_flow_msg4.layout.dim[0].size = 242;
  optic_flow_msg4.layout.dim[0].stride = 242;
  optic_flow_msg4.layout.data_offset = 0;
  optic_flow_msg4.data_length = 242; // THIS LINE IS CRUCIAL! 
                                  // https://answers.ros.org/question/37185/how-to-initialize-a-uint8multiarray-message/
                                  // https://answers.ros.org/question/10988/use-multiarray-in-rosserial/
  optic_flow_msg4.data = (float *)malloc(sizeof(float)*242); 

  for (int i = 0; i < 121; i++) 
  {
    optic_flow_msg4.data[i] = CapQX4[i];
    optic_flow_msg4.data[121+i] = CapQY4[i];
  }

  pub_optic_flow4.publish(&optic_flow_msg4);

  free(optic_flow_msg4.data);
  free(optic_flow_msg4.layout.dim);
  double end3 = nh.now().toNsec();

  
  double time1 = end1-begin1;
  double time2 = end2-begin2;
  double time3 = end3-begin3;

  char result1[10],result2[10],result3[10];

  dtostrf(time1,7,2,result1);
  dtostrf(time2,7,2,result2);
  dtostrf(time3,7,2,result3);
  nh.loginfo(result1);
  nh.loginfo(result2);
  nh.loginfo(result3);

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
