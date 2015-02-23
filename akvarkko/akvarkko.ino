#define RELAY1  8                       
#define RELAY2  9
#define BUTTON  5
#include <Wire.h>
int BH1750_address = 0x23; // i2c Addresse
byte buff[2];
int lastButtonState = 0; 
int buttonState = 0;
void setup()
{    
// Initialise the Arduino data pins for OUTPUT
  pinMode(RELAY1, OUTPUT);       
  pinMode(RELAY2, OUTPUT);
  pinMode(BUTTON, INPUT);

}
 
 void loop()
{
  
  Wire.begin();
  BH1750_Init(BH1750_address);
  
  buttonState = digitalRead(BUTTON);
  
  if (buttonState != lastButtonState) {
    // if the state has changed, increment the counter
    if (buttonState == HIGH) { 
      digitalWrite(RELAY1,HIGH);
    }else{
      digitalWrite(RELAY1,LOW);
    }
  }  
  lastButtonState = buttonState;
 }
 


int getLightValue(){
   float valf=0;
  if(BH1750_Read(BH1750_address)==2){    
      valf=((buff[0]<<8)|buff[1])/1.2;
     return (int)valf;   
  }
}


 
void BH1750_Init(int address){
  
  Wire.beginTransmission(address);
  Wire.write(0x10); // 1 [lux] aufloesung
  Wire.endTransmission();
}



byte BH1750_Read(int address){
  
  byte i=0;
  Wire.beginTransmission(address);
  Wire.requestFrom(address, 2);
  while(Wire.available()){
    buff[i] = Wire.read(); 
    i++;
  }
  Wire.endTransmission();  
  return i;
}
