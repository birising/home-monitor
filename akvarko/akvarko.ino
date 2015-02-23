/*

Example of BH1750 library usage.

This example initalises the BH1750 object using the default
high resolution mode and then makes a light level reading every second.

Connection:
 VCC-5v
 GND-GND
 SCL-SCL(analog pin 5)
 SDA-SDA(analog pin 4)
 ADD-NC or GND

*/

#include <Wire.h>
#include <BH1750.h>
#define RELAY1  8                       
#define RELAY2  9
#define BUTTON  5

BH1750 lightMeter;
bool lightOn;


void setup(){
  Serial.begin(9600);
  while (!Serial) ;
   pinMode(RELAY1, OUTPUT);       
  pinMode(RELAY2, OUTPUT);
  pinMode(BUTTON, INPUT);
   Serial.println("Running...");
  lightMeter.begin();
  lightOn = false;
 
 
 
}
void turnLightOn(){
  digitalWrite(RELAY1,HIGH);
  lightOn = true;
}

void turnLightOff(){
  digitalWrite(RELAY1,LOW);
    lightOn = false;
}


void loop() {
  uint16_t lux = lightMeter.readLightLevel();
  
  if(lux){
     turnLightOn();
  }else{
      turnLightOff();
  }
  
  Serial.print("Light: ");
  Serial.print(lux);
  Serial.println(" lx");
  delay(1000);
}


