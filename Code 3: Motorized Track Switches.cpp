/*This sketch pauses the train when it covers the green light sensor
  Every second time, it switches the points left or right.
*/

//Pins
const int IN3 = 26; //points motor pin 1
const int IN4 = 28; //points motor pin 2
const int ENB = 8; //points motor PWM
const int IN1 = 22; //train motor pin 1
const int IN2 = 24; //train motor pin 2
const int ENA = 9; //train motor PWM
const int LDR1 = 0;  //grey(green) LDR pin
const int LDR2 = 8;  //white(red) LDR pin

//Variables
int LDR2state = 0;
int LDR1Value = 0;
int LDR2Value = 0;
unsigned long previousMillis = 0;
int LightSensorCounter = 0;
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

  //Set fixed train direction
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  //Set points motor speed
  analogWrite(ENB, 255);
}

void loop() {
  Station_Sensor();
  Track_Switcher();
}

void Station_Sensor() {
  int ldrValNow;

  ldrValNow = analogRead(LDR1);
  if (ldrValNow > lightSensitivity) {
    if (LDR1Value <= lightSensitivity) {       analogWrite(ENA, 0);       previousMillis = millis();     }     else if (millis() - previousMillis > interval) {
      analogWrite(ENA, 120);
    }
  }
  else {
    analogWrite(ENA, 120);
  }
  LDR1Value = ldrValNow;

}

void Track_Switcher() {
  LDR2Value = analogRead(LDR2);  //reads the red LDR's value
    if (LDR2Value > lightSensitivity) { //if the sensor is triggered
      LDR2state = 1;   //save the sensor as ON
    }
  else   {
    LDR2state = 0;  //otherwise, sensor is OFF
  }

  if (LDR2state != previousLDR2state && previousLDR2state == 0) {  //if sensor just switched from OFF to ON
    LightSensorCounter++;    //add 1 to the light counter

    if (LightSensorCounter % 2 == 0) {     //if the light counter is an EVEN number
      digitalWrite(IN3, HIGH); // switch left
      digitalWrite(IN4, LOW);
      delay(200);
      digitalWrite(IN3, LOW);
    }
    else {                                //if the light counter is an ODD number
      digitalWrite(IN3, LOW); // switch right
      digitalWrite(IN4, HIGH);
      delay(200);
      digitalWrite(IN4, LOW);
    }

  }
  previousLDR2state = LDR2state;   //save light sensor state
}
