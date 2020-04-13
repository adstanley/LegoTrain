/* This sketch controls two trains on a single track, with a station.
  When a train is detected by a light sensor the trains stops and the points switch.
  After the interval the second train moves in the opposite direction, while the corresponding
  sensor is deactivated for the interval.
  A single red and green LED are set up to indicate the STATION train.
  A light sensor on A15 activates the station LEDS on plin 53.
*/

//Pins
const int IN1 = 22; //train motor pin 1
const int IN2 = 24; //train motor pin 2
const int ENA = 3; //train motor PWM
const int IN3 = 26; //points motor pin 1
const int IN4 = 28; //points motor pin 2
const int ENB = 2; //points motor PWM
const int LDR1 = 0;  //grey(green) LDR pin
const int LDR2 = 8;  //white(red) LDR pin
const int LIGHTSENSOR = 15; //ambient light level sensor pin
const int REDLED = 25; //red LED digital pin
const int GREENLED = 23; //green LED digital pin
const int STATIONLEDS = 53; //station LEDs

//possible states of the state machine

const byte RED_TRAIN_GO = 0;
const byte GREEN_TRAIN_GO = 1;
const byte SWITCH_POINTS = 2;
const byte CHECK_AMBIENT_LIGHT = 3; //Check ambient light levels
const byte RED_TRAIN_CHECK = 4;
const byte GREEN_TRAIN_CHECK = 5;

byte state = RED_TRAIN_GO;

//Variables
int LDRValue = 0;             // save the LDR val of the ambient light sensor
int LDR1Value = 0;            //save the LDR val of the green train
int LDR2Value = 0;            //save the LDR val of the red train
unsigned long previousMillis = 0;     //will store last time light sensor updated
int LightSensorCounter = 0;      //counts the number of times either LDR was tripped (therefore track left or right position)
int LDR1state = 0;  //whether LDR1 has been triggered or not
int previousLDR1state = 0;
int LDR2state = 0;  //whether LDR2 has been triggered or not
int previousLDR2state = 0;

//Settings
int interval = 3000;            //pause period
int lightSensitivity = 500;  //set switching point for light sensor (LDR)

void setup()
{
  Serial.begin(9600);
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(REDLED, OUTPUT);
  pinMode(GREENLED, OUTPUT);
  pinMode(STATIONLEDS, OUTPUT);

  //Set fixed train direction
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  //Set points motor speed
  analogWrite(ENB, 255);
}

void loop() {

  switch (state)
  {
    case RED_TRAIN_GO:  //both light sensors off, power to both trains
      {
        if (millis() - previousMillis > interval) {
          digitalWrite(REDLED, LOW);
          digitalWrite(GREENLED, HIGH);
          digitalWrite(IN2, HIGH);
          analogWrite(ENA, 120);
          previousMillis = millis();
          state = RED_TRAIN_CHECK;
        }
      }
      break;

    case RED_TRAIN_CHECK:
      {
        if (millis() - previousMillis > interval) {
          LDR2Value = analogRead(LDR2);  //reads the red LDR's value
          if (LDR2Value > lightSensitivity) { //if the sensor is triggered
            LDR2state = 1;   //save the sensor as ON
          }
          else   {
            LDR2state = 0;  //otherwise, sensor is OFF
          }

          if (LDR2state != previousLDR2state && previousLDR2state == 0) {  //if sensor just switched from OFF to ON
            LightSensorCounter++;    //add 1 to the light counter
            digitalWrite(GREENLED, LOW);
            digitalWrite(REDLED, HIGH);
            state = CHECK_AMBIENT_LIGHT;
          }
          previousLDR2state = LDR2state;   //save light sensor state
        }
      }
      break;

    case CHECK_AMBIENT_LIGHT: //Check ambient light levels before switching points
      {
        LDRValue = analogRead(LIGHTSENSOR);
        if (LDRValue > lightSensitivity) {
          digitalWrite(STATIONLEDS, HIGH);
        }
        else {
          digitalWrite(STATIONLEDS, LOW);
        }
        state = SWITCH_POINTS;
      }
      break;

    case GREEN_TRAIN_GO:  //both light sensors off, power to both trains
      {
        if (millis() - previousMillis > interval) {
          analogWrite(ENA, 120);
          digitalWrite(IN1, HIGH);
          previousMillis = millis();
          state = GREEN_TRAIN_CHECK;
        }
      }
      break;

    case GREEN_TRAIN_CHECK:
      {
        if (millis() - previousMillis > interval) {
          LDR1Value = analogRead(LDR1);  //reads the red LDR's value
          if (LDR1Value > lightSensitivity) { //if the sensor is triggered
            LDR1state = 1;   //save the sensor as ON
          }
          else   {
            LDR1state = 0;  //otherwise, sensor is OFF
          }

          if (LDR1state != previousLDR1state && previousLDR1state == 0) {  //if sensor just switched from OFF to ON
            LightSensorCounter++;    //add 1 to the light counter
            state = CHECK_AMBIENT_LIGHT;
          }
          previousLDR1state = LDR1state;   //save light sensor state
        }
      }
      break;

    case SWITCH_POINTS:  //Switch the tracks between the station and straight section
      {
        previousMillis = millis();
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, LOW);
        if (LightSensorCounter % 2 == 0) {     //if the light counter is an EVEN number
          digitalWrite(IN3, HIGH); // switch to straight
          digitalWrite(IN4, LOW);
          delay(250);
          digitalWrite(IN3, LOW);
          state = GREEN_TRAIN_GO;
        }
        else {      //if the light counter is an ODD number
          digitalWrite(IN3, LOW); // switch to station
          digitalWrite(IN4, HIGH);
          delay(250);
          digitalWrite(IN4, LOW);
          state = RED_TRAIN_GO;
        }
      }
      break;
  }
}
