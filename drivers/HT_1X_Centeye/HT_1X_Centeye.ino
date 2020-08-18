#include <Arduino.h>
#include <ros.h> // PlatformIO Library: rosserial_arduino by Open Agriculture Initiative
#include <std_msgs/Float32.h>
#include <std_msgs/Float32MultiArray.h>

#define HWSERIAL Serial1

std_msgs::Float32MultiArray optic_flow_msg;
ros::Publisher pub_optic_flow("optic_flow", &optic_flow_msg);

ros::NodeHandle nh;

void setup()
{
  
    Serial.begin(115200);
    HWSERIAL.begin(115200, SERIAL_8N1);
    
    //start the ROS node
    nh.getHardware()->setBaud(115200);
    nh.initNode();
    // nh.loginfo("Program info");
    nh.advertise(pub_optic_flow);
}


void loop()
{

  // byte TxMsg[6] = {255, 8, 36, 0, 255, 254}; // Retrieve Device ID
  // byte TxMsg[6] = {255, 8, 36, 9, 255, 254}; // Retrieve Sensor Data // Dataset 9 (for V1 sensor - Classic OF Sensor)
  byte TxMsg[6] = {255, 8, 36, 11, 255, 254}; // Retrieve Sensor Data // Dataset 11 (for V2 sensor - HT Sensor)
  // byte TxMsg[6] = {255, 8, 36, 3, 255, 254}; // Retrieve Sensor Data // Dataset 3

  unsigned long start_time;
  unsigned long elapsed_time; 

  unsigned long start_time1;
  unsigned long elapsed_time1;

  unsigned long start_time2;
  unsigned long elapsed_time2;

  int RxMsg;
  unsigned int bytecount = 0;
  unsigned char Data[246];

  start_time = micros();
  HWSERIAL.write(TxMsg, 6);

  

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
  elapsed_time = micros()- start_time;
  // Enhance readability in serial monitor
  //delay(50);

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
  delay(3);

  signed char CapQX[121], CapQY[121];

  int frame_counter = p_int[0];

  // Window 0 = Global
  // Window 1 - 24 = Classic V1
  // Window 25 - 120 = Small
  start_time1 = micros();
  for(int i = 0; i < 121; i++)
  {
    CapQX[i] = Data[4+2*i];
    CapQY[i] = Data[5+2*i];
  }
  elapsed_time1 = micros() - start_time1;

  //delay(15);
  start_time2 = micros();
  optic_flow_msg.layout.dim = (std_msgs::MultiArrayDimension *)
  malloc(sizeof(std_msgs::MultiArrayDimension) * 2);
  optic_flow_msg.layout.dim[0].label = "length";
  optic_flow_msg.layout.dim[0].size = 242;
  optic_flow_msg.layout.dim[0].stride = 242;
  optic_flow_msg.layout.data_offset = 0;
  optic_flow_msg.data_length = 242; // THIS LINE IS CRUCIAL! 
                                  // https://answers.ros.org/question/37185/how-to-initialize-a-uint8multiarray-message/
                                  // https://answers.ros.org/question/10988/use-multiarray-in-rosserial/
  optic_flow_msg.data = (float *)malloc(sizeof(float)*242); 

  for (int i = 0; i < 121; i++) 
  {
    optic_flow_msg.data[i] = CapQX[i];
    optic_flow_msg.data[121+i] = CapQY[i];
  }

  pub_optic_flow.publish(&optic_flow_msg);

  free(optic_flow_msg.data);
  free(optic_flow_msg.layout.dim);
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
}
