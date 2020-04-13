/*
  3-pin Arduino interface for HD44780 LCDs via 74HC595 Shift Register
      by Rowan Simms   code@rowansimms.com
      License: Creative Commons - Attribution.
      Full Documentation and Description:  http://rowansimms.com/article.php/lcd-hookup-in-seconds

  This sketch allows Arduinos to use a shift register to control an LCD, allowing
  a reduction in pins it requires from 6 to 3 while still retaining full control
  including backlight on/off.
  This requires the use of the LiquidCrystal595 library
  available at: http://code.google.com/p/arduino-lcd-3pin/
*/

#include <LiquidCrystal595.h>    // include the library
LiquidCrystal595 lcd(53, 51, 49);   // datapin, latchpin, clockpin

//Pins
const int IN1 = 28; //train motor pin 1
const int IN2 = 26; //train motor pin 2
const int ENA = 8; //train motor PWM
const int LDR1 = 0;  //green LDR pin
const int LDR2 = 8;  //red LDR pin

//Variables
int LDR2state = 0;  //save LDR2 state
int LDR1Value = 0;            //save the LDR val of the last pass
int LDR2Value = 0;            //save the LDR val of the last pass
unsigned long previousMillis = 0;     //will store last time light sensor updated

//Settings
int interval = 3000;            //pause period
int lightSensitivity = 500;  //set switching point for light sensor (LDR)
int LightSensorCounter = 1;      //counts the number of times the red light sensor is turned off
int previousLDR2state = 0;


void setup() {
  Serial.begin(9600);
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  //Set welcome message
  lcd.begin(16, 2);            // 16 characters, 2 rows
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("Welcome to");

  //Set fixed train direction
  digitalWrite(IN1, LOW); // rotate forward
  digitalWrite(IN2, HIGH);
}

void loop() {

  Station_Sensor();
  Station_Name();
}
void Station_Sensor() {
  int ldrValNow;

  ldrValNow = analogRead(LDR1);  //reads the green LDR's value
  if (ldrValNow > lightSensitivity) {  //train in front of sensor
    if (LDR1Value <= lightSensitivity) {  //but last time the train wasn't in front (so this is the transition between in front and not in front)
      analogWrite(ENA, 0);  //only then power off train
      previousMillis = millis();    //and save the time
    }

    else if (millis() - previousMillis > interval) { //Time to start the train again?
      analogWrite(ENA, 150);
    }
  }
  else {   //Not in front, also drive!
    analogWrite(ENA, 150);
  }

  LDR1Value = ldrValNow;    //save for the next time so we can detect a transition
}


void Station_Name() {
  {
    LDR2Value = analogRead(LDR2);  //reads the red LDR's value
    if (LDR2Value > lightSensitivity) { //if the sensor is triggered
      LDR2state = 1;   //save the sensor as ON
    }
    else   {
      LDR2state = 0;  //otherwise, sensor is OFF
    }

    if (LDR2state != previousLDR2state && previousLDR2state == 0) {  //if sensor just switched from OFF to ON
      LightSensorCounter++;    //add 1 to the light counter
    }

    if (LightSensorCounter == 1) {
      lcd.setCursor(2, 1);
      lcd.print(" LEGOVILLE! ");
    }

    if (LightSensorCounter == 2) {
      lcd.setCursor(2, 1);
      lcd.print(" SPEED CITY");
    }

    if (LightSensorCounter == 3) {
      lcd.setCursor(1, 1);
      lcd.print("TEMPLE OF DOOM");
    }

    if (LightSensorCounter == 4) {
      lcd.setCursor(1, 1);
      lcd.print(" TRANSYLVANIA ");
    }

    if (LightSensorCounter == 5) {
      LightSensorCounter = 1;
    }
    previousLDR2state = LDR2state;   //save light sensor state
  }
}
