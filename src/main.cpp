
//#include <SD.h>
// #include <SPI.h>
#include "Wire.h"

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

void setup() 
{
  Serial.begin(115200);
  HWSERIAL.begin(115200, SERIAL_8N1);
}

void loop() 
{
  // byte TxMsg[6] = {255, 8, 36, 0, 255, 254}; // Retrieve Device ID
  byte TxMsg[6] = {255, 8, 36, 9, 255, 254}; // Retrieve Sensor Data
  int RxMsg[16];

  HWSERIAL.write(TxMsg, 6);

  // Wait until have entire data packet
	while(HWSERIAL.available() < 16)
  {}

  if(HWSERIAL.available() < 16)
  {
    // the data didn't come in - handle that problem here
    Serial.println("?");
  }
  else
  {
    Serial.println("data");
    for(int n=0; n<16; n++)
      {
      RxMsg[n] = HWSERIAL.read(); // Then: Get them.
      Serial.println(RxMsg[n]);
    }
    Serial.clear();
    HWSERIAL.clear();

  }
}