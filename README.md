# README

## How to Configure the Teensy

### Download and install Arduino IDE 1.8.12

### Add Teensy udev rule

### Download and install Teensyduino

- Need to make the file executable
- chmod +x TeensyduinoInstall.linux64
- ./TeensyduinoInstall.linux64

### Open Arduino Program and sekect board

Select the appropriate board
- Tools --> Board --> Teensy 3.6
- Tools --> Board --> Teensy 4.0

### Install rosserial_arduino

git clone https://github.com/ros-drivers/rosserial.git 
- git checkout melodic-devel
- git pull origin melodic-devel
- catkin_make

### Modify the Arduino library file to include the Teensy 4.0

- https://github.com/ros-drivers/rosserial/issues/455
- Added defined(__IMXRT1062__) in ArduinoHardware.h for teensy 4.0
- Do this in ~/Arduino/libraries/ros_lib/ArduinoHardware.h
- Do this in ~/rosserial/rosserial_arduino/src/ros_lib/ArduinoHardware.h, and then rebuild ros package

... from ...

#if defined(__MK20DX128__) || defined(__MK20DX256__) || defined(__MK64FX512__) || defined(__MK66FX1M0__) || defined(__MKL26Z64__)

... to ...

#if defined(__MK20DX128__) || defined(__MK20DX256__) || defined(__MK64FX512__) || defined(__MK66FX1M0__) || defined(__MKL26Z64__) || defined(__IMXRT1062__)

### Make ros_lib libraries

- cd /home/$USER/Arduino/libraries
- rosrun rosserial_arduino make_libraries.py /home/$USER/Arduino/libraries
http://wiki.ros.org/rosserial_arduino/Tutorials/Arduino%20IDE%20Setup

### Modify ros.h

- Do this in ~/Arduino/libraries/ros_lib/ros.h
- might also want to do here /home/$USER/microbrain_ws/src/rosserial/rosserial_arduino/src/ros_lib/ros.h
... from ...

#if defined(__AVR_ATmega8__) or defined(__AVR_ATmega168__)
  /* downsize our buffers */
  typedef NodeHandle_<ArduinoHardware, 6, 6, 150, 150> NodeHandle;
#elif defined(__AVR_ATmega328P__)
  typedef NodeHandle_<ArduinoHardware, 25, 25, 280, 280> NodeHandle;
#elif defined(SPARK)
  typedef NodeHandle_<ArduinoHardware, 10, 10, 2048, 2048> NodeHandle;
#else
  typedef NodeHandle_<ArduinoHardware> NodeHandle; // default 25, 25, 512, 512

... to ...

#if defined(__AVR_ATmega8__) or defined(__AVR_ATmega168__)
  /* downsize our buffers */
  typedef NodeHandle_<ArduinoHardware, 15, 15, 512, 1024> NodeHandle;
#elif defined(__AVR_ATmega328P__)
  typedef NodeHandle_<ArduinoHardware, 15, 15, 512, 1024> NodeHandle;
#elif defined(SPARK)
  typedef NodeHandle_<ArduinoHardware, 15, 15, 512, 1024> NodeHandle;
#else
  typedef NodeHandle_<ArduinoHardware, 25, 25, 2048, 2048> NodeHandle;

Note: make sure /home/$USER/Arduino/libraries/ros_lib/ArduinoHardware.h the correction:
 
#if defined(__MK20DX128__) || defined(__MK20DX256__) || defined(__MK64FX512__) || defined(__MK66FX1M0__) || defined(__MKL26Z64__) || defined(__IMXRT1062__)

### Make sure specified port for Teensy is correct.

- Do this in tensy_centeye.launch

<node name="rosserial_arduino" type="serial_node.py" pkg="rosserial_arduino">
       <param name="port" type="string" value="/dev/ttyACM2"/>
       <param name="baud" type="int" value="115200"/>
</node>

### Very Important: Modify the RX and TX buffer size in HardwareSerialX.cpp 

- /home/$USER/arduino-1.8.13-linux64/arduino-1.8.13/hardware/teensy/avr/cores/teensy4/HardwareSerial1.cpp
- /home/$USER/arduino-1.8.13-linux64/arduino-1.8.13/hardware/teensy/avr/cores/teensy4/HardwareSerial2.cpp
- /home/$USER/arduino-1.8.13-linux64/arduino-1.8.13/hardware/teensy/avr/cores/teensy4/HardwareSerial3.cpp
- /home/$USER/arduino-1.8.13-linux64/arduino-1.8.13/hardware/teensy/avr/cores/teensy4/HardwareSerial4.cpp

- see reference here: https://forum.pjrc.com/threads/43699-teensy-3-6-serial-TX-buffer-size
- see reference here: https://forum.pjrc.com/threads/45379-Serial-buffer-size-increase-Teensy-3-2

#ifndef SERIAL5_TX_BUFFER_SIZE
#define SERIAL5_TX_BUFFER_SIZE     40 // number of outgoing bytes to buffer
#endif
#ifndef SERIAL5_RX_BUFFER_SIZE
#define SERIAL5_RX_BUFFER_SIZE     64 // number of incoming bytes to buffer
#endif

#ifndef SERIAL5_TX_BUFFER_SIZE
#define SERIAL5_TX_BUFFER_SIZE     2048 // number of outgoing bytes to buffer
#endif
#ifndef SERIAL5_RX_BUFFER_SIZE
#define SERIAL5_RX_BUFFER_SIZE     2048 // number of incoming bytes to buffer
#endif

### Note

Most recently installed with arduino-1.8.13-linux64 and Teensyduino 1.53 Linux Installer (X86 64 bit)
