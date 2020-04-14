Instructions for Setting up Centeye Sensors on Teensy

1. Download and Install Arduino IDE

https://www.arduino.cc/en/guide/linux

2.a. Install udev Rules

https://www.pjrc.com/teensy/49-teensy.rules

2.b. Download and Install Teensyduino

chmod +x TeensyduinoInstall.linux64cd
./TeensyduinoInstall.linux64
choose ~/arduino-1.8.12 folder

https://www.pjrc.com/teensy/td_download.html

3. Configure rosserial_arduino

3.a Install rosserial_arduino

http://wiki.ros.org/rosserial_arduino/Tutorials/Arduino%20IDE%20Setup

3.b. Install ros_lib into the Arduino Environment

cd ~/Arduino/libraries
rosrun rosserial_arduino make_libraries.py .

To confirm installation, open the Arduino IDE, go to File --> Examples, and at the bottom should be "ros_lib"

4. Increase rosserial Buffer Size

vim ~/Arduino/libraries/ros_lib/ros.h

Change...

...From: 

#else

  //typedef NodeHandle_<ArduinoHardware> NodeHandle; // default 25, 25, 512, 512
  typedef NodeHandle_<ArduinoHardware, 15, 15, 512, 1024> NodeHandle;

...To:

#else

  //typedef NodeHandle_<ArduinoHardware> NodeHandle; // default 25, 25, 512, 512
  typedef NodeHandle_<ArduinoHardware, 25, 25, 2048, 2048> NodeHandle;

5. Increase Teensy Serial Buffer Size

vim/arduino-1.8.12/hardware/teensy/avr/cores/teensy3/serial1.c
vim/arduino-1.8.12/hardware/teensy/avr/cores/teensy3/serial2.c
vim/arduino-1.8.12/hardware/teensy/avr/cores/teensy3/serial3.c
vim/arduino-1.8.12/hardware/teensy/avr/cores/teensy3/serial4.c

Change...

...From: 

#define SERIAL1_RX_BUFFER_SIZE     64 // number of incoming bytes to buffer

...To:

#define SERIAL1_RX_BUFFER_SIZE     2048 // number of incoming bytes to buffer

6. Select the proper board in the Arduino IDE

Go to Tools--> Boards-->Teensy 3.6





#define HWSERIAL Serial2 // Front
#define SESERIAL Serial4 // Right
#define THSERIAL Serial1 // Left
#define FOSERIAL Serial3 // Back