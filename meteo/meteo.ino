#include <DHT.h>
#include <avr/wdt.h> //We need watch dog for this program
#include <Wire.h> //I2C needed for sensors
#include <SPI.h>
#include <Ethernet.h>

//Hardware pin definitions
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// digital I/O pins
const byte WSPEED = 3 ;
const byte RAIN = 4;
const byte STAT1 = 7;


// analog I/O pins
const byte WDIR = A0;
const byte LIGHT = A1;
const byte REFERENCE_3V3 = A3;
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#include <SPI.h> //Needed for lighting sensor



//Global Variables
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
long lastSecond; //The millis counter to see when a second rolls by
unsigned int minutesSinceLastReset; //Used to reset variables after 24 hours. Imp should tell us when it's midnight, this is backup.
byte seconds; //When it hits 60, increase the current minute
byte seconds_2m; //Keeps track of the "wind speed/dir avg" over last 2 minutes array of data
byte minutes; //Keeps track of where we are in various arrays of data
byte minutes_10m; //Keeps track of where we are in wind gust/dir over last 10 minutes array of data

long lastWindCheck = 0;
volatile long lastWindIRQ = 0;
volatile byte windClicks = 0;


//We need to keep track of the following variables:
//Wind speed/dir each update (no storage)
//Wind gust/dir over the day (no storage)
//Wind speed/dir, avg over 2 minutes (store 1 per second)
//Wind gust/dir over last 10 minutes (store 1 per minute)
//Rain over the past hour (store 1 per minute)
//Total rain over date (store one per day)

byte windspdavg[120]; //120 bytes to keep track of 2 minute average
int winddiravg[120]; //120 ints to keep track of 2 minute average
float windgust_10m[10]; //10 floats to keep track of largest gust in the last 10 minutes
int windgustdirection_10m[10]; //10 ints to keep track of 10 minute max
volatile float rainHour[60]; //60 floating numbers to keep track of 60 minutes of rain

//These are all the weather values that wunderground expects:
int winddir; // [0-360 instantaneous wind direction]
float windspeedmph; // [mph instantaneous wind speed]
float windgustmph; // [mph current wind gust, using software specific time period]
int windgustdir; // [0-360 using software specific time period]
float windspdmph_avg2m; // [mph 2 minute average wind speed mph]
int winddir_avg2m; // [0-360 2 minute average wind direction]
float windgustmph_10m; // [mph past 10 minutes wind gust mph ]
int windgustdir_10m; // [0-360 past 10 minutes wind gust direction]
float humidity; // [%]
float tempf; // [temperature F]
float rainin; // [rain inches over the past hour)] -- the accumulated rainfall in the past 60 min
volatile float dailyrainin; // [rain inches so far today in local time]
//float baromin = 30.03;// [barom in] - It's hard to calculate baromin locally, do this in the agent
float pressure;
//float dewptf; // [dewpoint F] - It's hard to calculate dewpoint locally, do this in the agent

//These are not wunderground values, they are just for us
float batt_lvl = 11.8;
float light_lvl = 0.72;

// volatiles are subject to modification by IRQs
volatile unsigned long raintime, rainlast, raininterval, rain;

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//ETHERNET 
IPAddress ip(192, 168, 88, 250);
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
// Initialize the Ethernet server library
// with the IP address and port you want to use
// (port 80 is default for HTTP):
EthernetServer server(80);

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//DHT22
#define DHTPIN 7 
#define DHTTYPE DHT22  
DHT dht(DHTPIN, DHTTYPE);

//Interrupt routines (these are called by the hardware interrupts, not by the main code)
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void rainIRQ()
// Count rain gauge bucket tips as they occur
// Activated by the magnet and reed switch in the rain gauge, attached to input D2
{
  raintime = millis(); // grab current time
  raininterval = raintime - rainlast; // calculate interval between this and last event

    if (raininterval > 10) // ignore switch-bounce glitches less than 10mS after initial edge
  {
    dailyrainin += 0.2794; //Each dump is 0.011" of water
    rainlast = raintime; // set up for next event
  }
}

void wspeedIRQ()
// Activated by the magnet in the anemometer (2 ticks per rotation), attached to input D3
{
  if (millis() - lastWindIRQ > 10) // Ignore switch-bounce glitches less than 10ms (142MPH max reading) after the reed switch closes
  {
    lastWindIRQ = millis(); //Grab the current time
    windClicks++; //There is 1.492MPH for each click per second.
  }
}

void setup()
{
  wdt_reset(); //Pet the dog
  wdt_disable(); //We don't want the watchdog during init
   dht.begin();
//  Serial.begin(9600);
//   while (!Serial) {
//    ; // wait for serial port to connect. Needed for Leonardo only
//  }  

  pinMode(WSPEED, INPUT_PULLUP); // input from wind meters windspeed sensor
  pinMode(RAIN, INPUT_PULLUP); // input from wind meters rain gauge sensor
  pinMode(WDIR, INPUT);
  seconds = 0;
  lastSecond = millis();

  // attach external interrupt pins to IRQ functions
  attachInterrupt(0, rainIRQ, FALLING);
  attachInterrupt(1, wspeedIRQ, FALLING);

  interrupts();

 // Serial.println("Wimp Weather Station online!");
//----------------------------
  Ethernet.begin(mac, ip);
  server.begin();
  
}

void loop()
{
 
  EthernetClient client = server.available();
  if (client) {

    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
         
          
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: application/json");
          client.println("Connection: close");  // the connection will be closed after completion of the response

          client.println();
          client.print("{\"wind\": {\"speed\": ");
          client.print(get_wind_speed());
          client.print(",\"dir\": ");
          client.print(get_wind_direction());
          client.print("},\"rain\": {\"total\":");
          client.print(get_rain());
          client.print("},\"air\": {\"pressure\": ");
          client.print(24);
          client.print(", \"humidity\": ");
          client.print(get_humidity());
          client.print(",\"temperature\": ");
          client.print(get_temperature());  
          client.println( "},\"lightning\": { \"distance\": \"\"}}");
          
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        }
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  }






  

//  //Keep track of which minute it is
//  if(millis() - lastSecond >= 1000)
//  {
//    lastSecond += 1000;
//
//    //Take a speed and direction reading every second for 2 minute average
//    if(++seconds_2m > 119) seconds_2m = 0;
//
//    //Calc the wind speed and direction every second for 120 second to get 2 minute average
//    windspeedmph = get_wind_speed();
//    winddir = get_wind_direction();
//    windspdavg[seconds_2m] = (int)windspeedmph;
//    winddiravg[seconds_2m] = winddir;
//    //if(seconds_2m % 10 == 0) displayArrays();
//
//    //Check to see if this is a gust for the minute
//    if(windspeedmph > windgust_10m[minutes_10m])
//    {
//      windgust_10m[minutes_10m] = windspeedmph;
//      windgustdirection_10m[minutes_10m] = winddir;
//    }
//
//    //Check to see if this is a gust for the day
//    //Resets at midnight each night
//    if(windspeedmph > windgustmph)
//    {
//      windgustmph = windspeedmph;
//      windgustdir = winddir;
//    }
//
//    //Blink stat LED briefly to show we are alive
//    digitalWrite(STAT1, HIGH);
//    reportWeather(); //Print the current readings. Takes 172ms.
//    delay(25);
//    digitalWrite(STAT1, LOW);
//
//    //If we roll over 60 seconds then update the arrays for rain and windgust
//    if(++seconds > 59)
//    {
//      seconds = 0;
//
//      if(++minutes > 59) minutes = 0;
//      if(++minutes_10m > 9) minutes_10m = 0;
//
//      rainHour[minutes] = 0; //Zero out this minute's rainfall amount
//      windgust_10m[minutes_10m] = 0; //Zero out this minute's gust
//
//      minutesSinceLastReset++; //It's been another minute since last night's midnight reset     
//    }
//  }
}


//When the imp tells us it's midnight, reset the total amount of rain and gusts
void midnightReset()
{
  dailyrainin = 0; //Reset daily amount of rain

  windgustmph = 0; //Zero out the windgust for the day
  windgustdir = 0; //Zero out the gust direction for the day

  minutes = 0; //Reset minute tracker
  seconds = 0;
  lastSecond = millis(); //Reset variable used to track minutes

    minutesSinceLastReset = 0; //Zero out the backup midnight reset variable
}

//Calculates each of the variables that wunderground is expecting
void calcWeather()
{
  //current winddir, current windspeed, windgustmph, and windgustdir are calculated every 100ms throughout the day

  //Calc windspdmph_avg2m
  float temp = 0;
  for(int i = 0 ; i < 120 ; i++)
    temp += windspdavg[i];
  temp /= 120.0;
  windspdmph_avg2m = temp;

  //Calc winddir_avg2m
  temp = 0; //Can't use winddir_avg2m because it's an int
  for(int i = 0 ; i < 120 ; i++)
    temp += winddiravg[i];
  temp /= 120;
  winddir_avg2m = temp;

  //Calc windgustmph_10m
  //Calc windgustdir_10m
  //Find the largest windgust in the last 10 minutes
  windgustmph_10m = 0;
  windgustdir_10m = 0;
  //Step through the 10 minutes  
  for(int i = 0; i < 10 ; i++)
  {
    if(windgust_10m[i] > windgustmph_10m)
    {
      windgustmph_10m = windgust_10m[i];
      windgustdir_10m = windgustdirection_10m[i];
    }
  }

 

  //Total rainfall for the day is calculated within the interrupt
  //Calculate amount of rainfall for the last 60 minutes
  rainin = 0;  
  for(int i = 0 ; i < 60 ; i++)
    rainin += rainHour[i];

}



//Returns the instataneous wind speed
float get_wind_speed()
{
  float deltaTime = millis() - lastWindCheck; //750ms

  deltaTime /= 1000.0; //Covert to seconds

  float windSpeed = (float)windClicks / deltaTime; //3 / 0.750s = 4

  windClicks = 0; //Reset and start watching for new wind
  lastWindCheck = millis();

  windSpeed *= 2.4; //4 * 1.492 = 5.968MPH

  /* Serial.println();
   Serial.print("Windspeed:");
   Serial.println(windSpeed);*/

  return(windSpeed);
}


float get_rain()
{
  float tempRain =  dailyrainin;
  dailyrainin = 0;
  return (tempRain);
 }

float get_temperature()
{
  return(dht.readTemperature());
}

float get_humidity()
{
  return( dht.readHumidity());
}

 
int get_wind_direction() 
// read the wind direction sensor, return heading in degrees
{
  unsigned int adc;

  adc = averageAnalogRead(WDIR); // get the current reading from the sensor

  // The following table is ADC readings for the wind direction sensor output, sorted from low to high.
  // Each threshold is the midpoint between adjacent headings. The output is degrees for that ADC reading.
  // Note that these are not in compass degree order! See Weather Meters datasheet for more information.

  if (adc < 178) return (90);
  if (adc < 322) return (135);
  if (adc < 458) return (180);
  if (adc < 645) return (45);
  if (adc < 790) return (225);
  if (adc < 899) return (0);
  if (adc < 960) return (315);
  if (adc < 993) return (270);
  
  return (-1); // error, disconnected?


}

//Reports the weather string to the Imp
void reportWeather()
{
  calcWeather(); //Go calc all the various sensors

  Serial.print("$,winddir=");
  Serial.print(winddir);
  Serial.print(",windspeedmph=");
  Serial.print(windspeedmph, 1);
  Serial.print(",windgustmph=");
  Serial.print(windgustmph, 1);
  Serial.print(",windgustdir=");
  Serial.print(windgustdir);
  // Serial.print(",windspdmph_avg2m=");
  // Serial.print(windspdmph_avg2m, 1);
  // Serial.print(",winddir_avg2m=");
  // Serial.print(winddir_avg2m);
  // Serial.print(",windgustmph_10m=");
  // Serial.print(windgustmph_10m, 1);
  // Serial.print(",windgustdir_10m=");
  // Serial.print(windgustdir_10m);
  // Serial.print(",humidity=");
  // Serial.print(humidity, 1);
  // Serial.print(",tempf=");
  // Serial.print(tempf, 1);
  Serial.print(",rainin=");
  Serial.print(rainin, 2);
  Serial.print(",dailyrainin=");
  Serial.print(dailyrainin, 2);
  Serial.print(","); //Don't print pressure= because the agent will be doing calcs on the number
  // Serial.print(pressure, 2);
  // Serial.print(",batt_lvl=");
  // Serial.print(batt_lvl, 2);
  // Serial.print(",light_lvl=");
  Serial.print(light_lvl, 2);



  Serial.print(",");
  Serial.println("#,");

  //Test string
  //Serial.println("$,winddir=270,windspeedmph=0.0,windgustmph=0.0,windgustdir=0,windspdmph_avg2m=0.0,winddir_avg2m=12,windgustmph_10m=0.0,windgustdir_10m=0,humidity=998.0,tempf=-1766.2,rainin=0.00,dailyrainin=0.00,-999.00,batt_lvl=16.11,light_lvl=3.32,#,");
}

//Takes an average of readings on a given pin
//Returns the average
int averageAnalogRead(int pinToRead)
{
  byte numberOfReadings = 8;
  unsigned int runningValue = 0; 

  for(int x = 0 ; x < numberOfReadings ; x++)
    runningValue += analogRead(pinToRead);
  runningValue /= numberOfReadings;

  return(runningValue);  
}


