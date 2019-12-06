
//#include <SD.h>
// #include <SPI.h>
#include "Wire.h"

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

void setup() 
{
  Serial.begin(115200);
  HWSERIAL.begin(115200, SERIAL_8N1);
}

void loop() 
{
  // byte TxMsg[6] = {255, 8, 36, 0, 255, 254}; // Retrieve Device ID
  byte TxMsg[6] = {255, 8, 36, 9, 255, 254}; // Retrieve Sensor Data // Dataset 9
  // byte TxMsg[6] = {255, 8, 36, 3, 255, 254}; // Retrieve Sensor Data // Dataset 3

  int RxMsg;
  unsigned int bytecount = 0;
  unsigned char Data[544];

  HWSERIAL.write(TxMsg, 6);

  Serial.println();
  Serial.print("NEW DATA");
  Serial.println();

  // Buffer of UART probably small, this makes sure buffer has some bytes
  delay(1); // Need 3ms delay before reading Device ID (otherwise first byte = 252)

  while(HWSERIAL.available() && bytecount < 544)
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
  delay(1000);

  Serial.println();

  Serial.clear();
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

  // global_x_of (4-Byte Long Integer)
  int global_x_of = p_int[0];

  // global_y_of (4-Byte Long Integer)
  int global_y_of = p_int[1];

  // x and y window optical flow odometries
  int x_of[25],y_of[25];

  // stereo distance and confidence flags for windows
  short s_dist[25],s_conf[25];

  // Note: window 0 does not have any information
  for (int w = 0; w < 25; ++w) 
  { 
    int *p_w_int = (int*)(Data + w * 20 + 44);
    short *p_w_short = (short*)(Data + w * 20 + 44);
    x_of[w] = p_w_int[0];
    y_of[w] = p_w_int[1];
    s_dist[w] = p_w_short[4];
    s_conf[w] = p_w_short[5];
  }

  Serial.println();
  Serial.println();
  Serial.print("global_x_of: ");
  Serial.print(global_x_of);

  Serial.println();
  Serial.println();
  Serial.print("global_y_of: ");
  Serial.print(global_y_of);

  Serial.println();
  Serial.println();
  Serial.print("x_of: ");
  for (int w = 0; w < 25; ++w) 
  {
    Serial.print(x_of[w]);
    Serial.print(", ");

  }

  Serial.println();
  Serial.println();
  Serial.print("y_of: ");
  for (int w = 0; w < 25; ++w) 
  { 
    Serial.print(y_of[w]);
    Serial.print(", ");
  }

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
  
  Serial.println();
  Serial.println();



}