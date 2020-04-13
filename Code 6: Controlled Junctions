/* this program uses two light sensors to detect two trains approaching a junction.
  The program halts the second train detected for a fixed period (interval) before
  restarting that train.

  Change INTERVAL to increase duration of crossing
*/

//analog pins used for light sensors
const byte RED_TRAIN_SENSOR = 8; //Red train
const byte GREEN_TRAIN_SENSOR = 0; //Green train

//PWM pins for train motors
const int IN1 = 28; //train motor pin 1
const int IN2 = 26; //train motor pin 2
const int ENA = 8; //train motor PWM
const int IN3 = 24; //points motor pin 1
const int IN4 = 22; //points motor pin 2
const int ENB = 9; //points motor PWM

//possible states of the state machine
const byte TRACK_SECTION_CLEAR = 0;  //both light sensors off, power to both trains
const byte RED_TRAIN_DETECTED = 1;  //Red train detected, power to both trains
const byte GREEN_TRAIN_DETECTED = 2; //Green train detected, power to both trains
const byte RED_TRAIN_OFF = 3;  //Both trains detected (Green first), turn off Red train
const byte GREEN_TRAIN_OFF = 4;  //Both trains detected (Red first), turn off Green train

byte state = TRACK_SECTION_CLEAR;

//variables
unsigned long previousMillis = 0;
int RedTrainSensorValue = 0;
int GreenTrainSensorValue = 0;
int light_sensitivity = 500;
int interval = 3000;

void setup() {

  //enable output on the PWM pins
  pinMode(ENA, OUTPUT);  //red train motor enable
  pinMode(IN1, OUTPUT);  //red train motor PWM 1
  pinMode(IN2, OUTPUT);  //red train motor PWM 2
  pinMode(ENB, OUTPUT);  //green train motor enable
  pinMode(IN3, OUTPUT);  //green motor PWM 1
  pinMode(IN4, OUTPUT);  //green motor PWM 2

  //set train speed
  analogWrite(ENA, 130 ); //red motor speed
  analogWrite(ENB, 130); //green motor speed

  //activate train motors
  digitalWrite(IN1, LOW); //rotate red train forwards
  digitalWrite(IN3, LOW); //rotate green train forwards

  Serial.begin(9600); //configure serial communication
}

void loop() {

  switch (state)
  {
    case TRACK_SECTION_CLEAR:  //both light sensors off, power to both trains
      {
        RedTrainSensorValue = analogRead(RED_TRAIN_SENSOR);
        GreenTrainSensorValue = analogRead(GREEN_TRAIN_SENSOR);
        digitalWrite(IN2, HIGH);
        digitalWrite(IN4, HIGH);

        if (GreenTrainSensorValue > light_sensitivity) {
          previousMillis = millis();    //save the time
          state = GREEN_TRAIN_DETECTED;
        }

        if (RedTrainSensorValue > light_sensitivity) {
          previousMillis = millis();    //save the time
          state = RED_TRAIN_DETECTED;
        }
      }
      break;

    case RED_TRAIN_DETECTED:  //Red train detected, power to both trains
      {
        GreenTrainSensorValue = analogRead(GREEN_TRAIN_SENSOR);

        if (GreenTrainSensorValue > light_sensitivity) {
          state = GREEN_TRAIN_OFF;
        }

        else if (millis() - previousMillis > interval) {
          state = TRACK_SECTION_CLEAR;
        }
      }
      break;

    case GREEN_TRAIN_DETECTED: //Green train detected, power to both trains
      {
        RedTrainSensorValue = analogRead(RED_TRAIN_SENSOR);

        if (RedTrainSensorValue > light_sensitivity) {
          state = RED_TRAIN_OFF;
        }

        else if (millis() - previousMillis > interval) {
          state = TRACK_SECTION_CLEAR;
        }
      }
      break;

    case RED_TRAIN_OFF:  //Both trains detected (Green first), turn off Red train
      {

        if (millis() - previousMillis < interval) {
          digitalWrite(IN2, LOW);
        }
        else {
          state = TRACK_SECTION_CLEAR;
        }
      }
      break;

    case GREEN_TRAIN_OFF:  //Both trains detected (Red first), turn off Green train
      {

        if (millis() - previousMillis < interval) {
          digitalWrite(IN4, LOW);
        }
        else {
          state = TRACK_SECTION_CLEAR;
        }
      }
      break;
  }
}
