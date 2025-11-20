/*
* Name: övningsprojekt 
* Author: Vincent Stridh
* Date: 2025-11-20
* Description: This project uses a ds3231 to measure time and displays the time to an 1306 oled display, 
* Further, it measures temprature with ds3231 and displays a mapped value to a 9g-servo-motor.
*/

// Include Libraries
#include "RTClib.h"
#include <Wire.h>
#include "U8glib.h"
#include <Servo.h>
#include <math.h>
#include <LedControl.h>

// Init constants
const int oledLength = 128;
const int length = 10;
const int ServoPin = A0;

// Init global variables
int pos = 0;

// Construct objects
Servo myservo;
RTC_DS3231 rtc;
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NO_ACK);

// DIN, CLK, CS
LedControl lc = LedControl(12, 11, 10, 1);

void setup() {
  // init communication
  Serial.begin(9600);
  Wire.begin();

  // Init Hardware
  rtc.begin();
  myservo.attach(9);

  // Settings
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  
  lc.shutdown(0, false);    // Turn on the display
  lc.setIntensity(0, 8);    // Brightness: 0–15
  lc.clearDisplay(0);       // Clear
  lc.setDigit(0, 0, 4, false); // Digit position 0, show "4"
  lc.setDigit(0, 1, 2, false);
  lc.setDigit(0, 2, 6, false);
  lc.setDigit(0, 3, 9, false);
}

void loop() {
  DateTime now = rtc.now();
  pos = map(getTemp(), 20, 30, 0, 360);
  myservo.write(pos);
  
  showTimeOnDisplay(now);
  Serial.println(getPrint());

 // oledWrite(getTime()); //remove comment when the function is written
 // servoWrite(getTemp()); //remove comment when the function is written

  int v = now.second() * 6;
  draw(v);
  delay(1000);
}


//this function draws the circle for the 1306 oled display
void draw(int v){
  u8g.firstPage();
  do {
  u8g.drawCircle(45, 30, 30);
  u8g.drawLine(45, 30,(45+15*cos(v-90)+5), (20+15*sin(v-90)+5));
  } 
    while (u8g.nextPage());
}



//This function reads time and temp from an ds3231 module and package the time as a String
//Parameters: Void
//Returns: time in hh:mm:ss and temp as String for the print function
String getPrint() {
  DateTime now = rtc.now();
  return String(now.hour()) + ":" + String(now.minute()) + ":" + String(now.second() + 10) + "   Temp: " + String(rtc.getTemperature()) + "   Pos: " + pos;
}

//This function reads the current time and 
//Parameters: Void
//Returns: time (kommer inte ihåg vad jag skulle ha denna till)
String getTime() {
  DateTime now = rtc.now();
  char buffer[9]; // hh:mm:ss\0
  sprintf(buffer, "%02d:%02d:%02d", now.hour(), now.minute(), now.second());
  return String(buffer);
}

/*
* This function takes temprature from ds3231 and returns as a float
* Parameters: Void
* Returns: temprature as float 
*/
float getTemp() {
  return (rtc.getTemperature());
}

/*
* This function takes the current time and makes it possible for the clock to read it
* Parameters: void
* Returns: time now to the display
*/


void showTimeOnDisplay(DateTime now) {
  int hours = now.hour();
  int minutes = now.minute();
  int seconds = now.secondstime();

  static bool blink = false;
  blink = !blink;

  // Hours
  lc.setDigit(0, 7, hours / 10, false);
  lc.setDigit(0, 6, hours % 10, blink);

  // Minutes
  lc.setDigit(0, 5, minutes / 10, false);
  lc.setDigit(0, 4, minutes % 10, blink);

  // Seconds
  lc.setDigit(0, 3, seconds/10, false);
  lc.setDigit(0, 2, seconds%10, blink);
}


/*
* This function takes a string and draws it to an oled display
* Parameters: - text: String to write to display
* Returns: void
*/
void oledWrite() {
}

/*
* takes a temprature value and maps it to corresppnding degree on a servo
* Parameters: - value: temprature
* Returns: void
*/
// void servoWrite(float getTemp()) {
// }