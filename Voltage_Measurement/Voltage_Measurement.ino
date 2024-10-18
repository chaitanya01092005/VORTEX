#include "EmonLib.h"             // Include Emon Library

#define VOLT_CAL 592

EnergyMonitor emon1;             // Create an instance

void setup()
{  
  Serial.begin(9600);
  
  emon1.voltage(1, VOLT_CAL, 1.7);  // Voltage: input pin, calibration, phase_shift
}

void loop()
{
  emon1.calcVI(25,1000);         // Calculate all. No.of half wavelengths (crossings), time-out
  
  float supplyVoltage   = emon1.Vrms;             //extract Vrms into Variable
  if(supplyVoltage >= 100)
  {
 Serial.print("Voltage : ");  
 Serial.println(supplyVoltage);
  }
  else
 {
   Serial.println("Power OFF");
  }
 }
