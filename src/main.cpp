
//#include <SD.h>
// #include <SPI.h>
#include "Wire.h"
// #include <centeye.h>

// https://www.pjrc.com/teensy/td_uart.html
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

const byte numChars = 4;
int RxMsg[numChars];

boolean newData = false;
static int ndx = 0;

void getData();
void printData();

void setup() 
{
  Serial.begin(115200);
  HWSERIAL.begin(115200, SERIAL_8N1);
}

void loop() 
{
  getData();
  printData();
}

void getData()
{
    // FOR DEVICE ID RETRIEVAL
  byte TxMsg[6] = {255, 8, 36, 0, 255, 254}; // Retrieve Device ID

  // FOR DATA RETRIEVAL  
  // byte TxMsg[6] = {255, 8, 36, 9, 255, 254}; // Retrieve Sensor Data

  HWSERIAL.write(TxMsg, 6);

  while (HWSERIAL.available() > 0 && newData == false)
  {
    if (ndx < numChars)
    {
      RxMsg[ndx] = HWSERIAL.read();
      ndx++;
    }
    else
    {
      ndx = 0;
      newData = true;
    }
  }
}

void printData()
{
  if (newData == true)
  {
    // Serial.print("This just in ... ");
    // for
    // Serial.println(RxMsg);
  
    for(int n=0; n<1; n++)
    {
      Serial.println(RxMsg[n]);
    }

    newData = false;
    Serial.flush();
    HWSERIAL.flush();

  }
}