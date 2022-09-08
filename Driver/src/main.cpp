#include <BluetoothSerial.h>
#include <Adafruit_MPL3115A2.h>
#include <Adafruit_MMA8451.h>
#include <Adafruit_Sensor.h>
#include <SerialCommand.h>
#include "Wire.h"
#include <iostream>
#include <vector>
//#include "SPI.h"
Adafruit_MPL3115A2 mpl;
Adafruit_MMA8451 mma = Adafruit_MMA8451();
std::vector<float> accellx = {};
std::vector<float> accelly = {};
std::vector<float> accellz = {};

std::vector<float> height = {};


SerialCommand sCmd(Serial); // the SerialCommand parser object

void UNRECOGNIZED_sCmd_default_handler(SerialCommand this_sCmd)
{
  SerialCommand::CommandInfo command = this_sCmd.getCurrentCommand();
  this_sCmd.print(F("### Error: command '"));
  this_sCmd.print(command.name);
  this_sCmd.print(F("' not recognized ###\n"));
}

void Read_Data(SerialCommand this_sCmd)
{
std:: cout << '[';  
 for (float i: accellx)
    std::cout << i << ',';
std:: cout << ']';

std:: cout << '[';  
 for (float i: accelly)
    std::cout << i << ',';
std:: cout << ']';

std:: cout << '[';  
 for (float i: accellz)
    std::cout << i << ',';
std:: cout << ']';

std:: cout << '[';  
 for (float i: height)
    std::cout << i << ',';
std:: cout << ']';

}

void setup()
{
  
  Serial.begin(115200);
  delay(500);

  Wire.begin();
  delay(10);
  sCmd.addCommand("Rocket.Read?", Read_Data);
  sCmd.setDefaultHandler(UNRECOGNIZED_sCmd_default_handler);
  mpl.setMode(MPL3115A2_BAROMETER);
  mma.setRange(MMA8451_RANGE_2_G);
}

void loop()
{
  int counter = 0;

  
  while (counter < 250000)// flies 40 seconds before readout
  {
    mpl.startOneShot();
    while (!mpl.conversionComplete())
    {
    
    }
    counter++;
    mma.read();
    sensors_event_t event; 
    mma.getEvent(&event);
    // Get all parameters
    accellx.push_back(event.acceleration.x);
    accelly.push_back(event.acceleration.y);
    accellz.push_back(event.acceleration.z);

    height.push_back(mpl.getLastConversionResults(MPL3115A2_PRESSURE));

    delayMicroseconds(150);
  }
  while (true)
  {
    int num_bytes = sCmd.readSerial(); // fill the buffer
    if (num_bytes > 0)
    {
      sCmd.processCommand();
    }
    delay(10);
  }
}