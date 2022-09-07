#include <BluetoothSerial.h>
#include "SparkFunLSM6DSO.h"
#include <SerialCommand.h>
#include "Wire.h"
#include <iostream>
#include <vector>
//#include "SPI.h"

LSM6DSO myIMU; // Default constructor is I2C, addr 0x6B
std::vector<float> accellx = {};
std::vector<float> accelly = {};
std::vector<float> accellz = {};

std::vector<float> gyrox = {};
std::vector<float> gyroy = {};
std::vector<float> gyroz = {};

std::vector<float> tempc = {};

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
 for (float i: gyrox)
    std::cout << i << ',';
std:: cout << ']';

std:: cout << '[';  
 for (float i: gyroy)
    std::cout << i << ',';
std:: cout << ']';

std:: cout << '[';  
 for (float i: gyroz)
    std::cout << i << ',';
std:: cout << ']';

std:: cout << '[';  
 for (float i: tempc)
    std::cout << i << ',';
std:: cout << ']';
}

void setup()
{

  Serial.begin(115200);
  delay(500);

  Wire.begin();
  delay(10);
  if (myIMU.begin())
    Serial.println("Ready.");
  else
  {
    Serial.println("Could not connect to IMU.");
    Serial.println("Freezing");
  }

  if (myIMU.initialize(BASIC_SETTINGS))
    Serial.println("Loaded Settings.");

  sCmd.addCommand("Rocket.Read?", Read_Data);
  sCmd.setDefaultHandler(UNRECOGNIZED_sCmd_default_handler);
}

void loop()
{
  int counter = 0;
  while ()
  {
    
  }
  
  while (myIMU.readFloatAccelX() != 0 && myIMU.readFloatAccelY() != 0 && myIMU.readFloatAccelZ() != 0 || counter > 65530)
  {
    counter++;
    // Get all parameters
    accellx.push_back(myIMU.readFloatAccelX());
    accelly.push_back(myIMU.readFloatAccelY());
    accellz.push_back(myIMU.readFloatAccelZ());

    gyrox.push_back(myIMU.readFloatGyroX());
    gyroy.push_back(myIMU.readFloatGyroY());
    gyroz.push_back(myIMU.readFloatGyroZ());

    tempc.push_back(myIMU.readTempC());
    delayMicroseconds(160);
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