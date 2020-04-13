//This full sketch couples then decouples a wagon alternating sidings.
//The locomotive collects a wagon and drives forward. It then reverses past an IR sensor.
//When the first truck is detected, the wagon is uncoupled. The train jerks forward, the points switch,
//then the locomotive collects the wagon on the other siding. As the loco collects the wagon,
//ultrasonic sensors detect the reversing train on the appropriate siding.
//A counter (track_counter) decides which track/uncoupler is used.

int LDR_Blue = 0; //sensor for the right track
int LDR_Green = 15; //sensor for the left track
int LDR_Track = 7; //track sensor
int IN1 = 22; //train motor pin 1
int IN2 = 24; //train motor pin 2
int ENA = 8; //train motor PWN pin
int IN3 = 26; //siding motor pin 1
int IN4 = 28; //siding motor pin 2
int ENB = 9; //siding motor PWN pin
int IN5 = 30; //points motor pin 1
int IN6 = 32; //points motor pin 2
int ENC = 10; //points motor PWM pin
int trigPin_Blue = 46; //trigger wire for the blue ultrasonic sensor
int echoPin_Blue = 48; //echo wire for the blue ultrasonic sensor
int trigPin_Brown = 50; //triger wire for the brown ultrasonic sensor
int echoPin_Brown = 52; //echo wire for the brown ultrasonic sensor
int light_sensitivity = 100;
int LDRValue = 0;
int previousLDRValue = 255;
int previousLDRValue2 = 0;
int track_counter = 0;
int loop_counter = 0;
long distance = 10;
unsigned long previousMillis = 0;

//possible states of the state machine

const byte REVERSING = 0;  //reverse into one of the side tracks
const byte WAGON_DETECTED = 1;  //detect a gap between loco and wagon
const byte RAISE_UNCOUPLER = 2;  //send pulse to uncoupler to start detaching wagon
const byte UNCOUPLE = 3; //break away loco from wagon and add 1 to track counter
const byte SWITCH_POINTS = 4; //switch between sidings
const byte PICK_UP_LEFT_WAGON = 5; //collect wagon from left siding
const byte PICK_UP_RIGHT_WAGON = 6; //collect wagon from siding siding
const byte PULL_AWAY = 7; //drive away with the wagon

byte state = PULL_AWAY;

void setup() {
  {
    Serial.begin(9600);
    pinMode(ENA, OUTPUT);
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);
    pinMode(IN5, OUTPUT);
    pinMode(IN6, OUTPUT);
    pinMode(trigPin_Blue, OUTPUT);
    pinMode(echoPin_Blue, INPUT);
    pinMode(trigPin_Brown, OUTPUT);
    pinMode(echoPin_Brown, INPUT);
    digitalWrite(IN1, LOW); // rotate backward
    digitalWrite(IN2, HIGH);
    analogWrite(ENB, 80);
    analogWrite(ENC, 255);
  }
}

void loop() {

  switch (state)
  {

    case PULL_AWAY:
      {
        Serial.println("leaving siding");
        Serial.println(loop_counter);
        digitalWrite(IN2, LOW); //send train forward
        digitalWrite(IN1, HIGH);
        analogWrite(ENA, 120);
        LDRValue = map(analogRead(LDR_Track), 0, 1023, 0, 255);  //reads blue LDR
        if (LDRValue > light_sensitivity) {
          if (previousLDRValue < light_sensitivity) {
            loop_counter++;
          }
        }

        previousLDRValue = LDRValue;   //save light sensor state

        if (loop_counter == 2) {
          analogWrite(ENA, 0);
          delay(2000);
          state = REVERSING;
        }
      }
      break;

    case REVERSING:  //both light sensors off, power to both trains
      {
        Serial.println("reversing");
        analogWrite(ENA, 110);
        digitalWrite(IN1, LOW); // rotate backward
        digitalWrite(IN2, HIGH);

        if (track_counter % 2 == 0) {
          LDRValue = map(analogRead(LDR_Blue), 0, 1023, 0, 255);  //reads blue LDR
        }
        else {
          LDRValue = map(analogRead(LDR_Green), 0, 1023, 0, 255);  //reads green LDR
        }

        Serial.println(LDRValue); //prints the LDR values to serial monitor


        if (LDRValue < light_sensitivity) {           if (previousLDRValue > light_sensitivity) {
            state = WAGON_DETECTED;
          }
        }

        previousLDRValue = LDRValue;   //save light sensor state
      }

      break;

    case WAGON_DETECTED:
      {
        Serial.println("wagon detected");
        analogWrite(ENA, 90);
        digitalWrite(IN1, LOW); // rotate backward
        digitalWrite(IN2, HIGH);

        if (track_counter % 2 == 0) {
          LDRValue = map(analogRead(LDR_Blue), 0, 1023, 0, 255);  //reads blue LDR
        }
        else {
          LDRValue = map(analogRead(LDR_Green), 0, 1023, 0, 255);  //reads green LDR
        }

        Serial.println(LDRValue); //prints the LDR values to serial monitor

        if (LDRValue > light_sensitivity) {
          if (previousLDRValue2 < light_sensitivity) {
            state = RAISE_UNCOUPLER;
          }
        }
        previousLDRValue2 = LDRValue;   //save light sensor state
      }

      break;

    case RAISE_UNCOUPLER:
      {
        Serial.println("raising uncoupler");
        analogWrite(ENA, 0);  //turn off engine

        if (track_counter % 2 == 0) {     //if the track counter is an EVEN number
          digitalWrite(IN3, HIGH);  // raise right siding decoupler
          digitalWrite(IN4, LOW);
          delay(150);
          digitalWrite(IN3, LOW);
          state = UNCOUPLE;
        }
        else {                           //if the track counter is an ODD number
          digitalWrite(IN3, LOW);    // raise left siding decoupler
          digitalWrite(IN4, HIGH);
          delay(150);
          digitalWrite(IN4, LOW);
          state = UNCOUPLE;
        }
      }
      break;

    case UNCOUPLE:
      {
        delay(2000);
        Serial.println("uncoupling");
        digitalWrite(IN1, HIGH); //high-speed pulse engine forward
        digitalWrite(IN2, LOW);
        analogWrite(ENA, 255);
        delay(400);
        analogWrite(ENA, 0); //turn off engine
        track_counter++;
        state = SWITCH_POINTS;
      }
      break;

    case SWITCH_POINTS:  //Switch the tracks between the station and straight section
      {
        Serial.println("switching points");
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, LOW);
        if (track_counter % 2 == 0) {     //if the track counter is an EVEN number
          digitalWrite(IN5, LOW); // switch to straight
          digitalWrite(IN6, HIGH);
          delay(200);
          digitalWrite(IN6, LOW);
          state = PICK_UP_RIGHT_WAGON;
        }
        else {                            //if the track counter is an ODD number
          digitalWrite(IN5, HIGH); // switch to station
          digitalWrite(IN6, LOW);
          delay(200);
          digitalWrite(IN5, LOW);
          state = PICK_UP_LEFT_WAGON;
        }
      }
      break;

    case PICK_UP_LEFT_WAGON:
      {
        long duration;
        Serial.println("collecting left wagon");
        analogWrite(ENA, 100);

        digitalWrite(trigPin_Blue, LOW);  // Turn the trigger off
        delayMicroseconds(2);
        digitalWrite(trigPin_Blue, HIGH);  // Turn the trigger on
        delayMicroseconds(10);
        digitalWrite(trigPin_Blue, LOW);  // Turn the trigger off

        duration = pulseIn(echoPin_Blue, HIGH); //pulse the echo pin to detect
        distance = (duration / 2) / 29.1; //define the distance in centimetres

        if (distance < 7) {  // Once the train is less than 7cm from the sensor
          digitalWrite(IN2, LOW); //stop the train motor
          loop_counter = 0;
          state = PULL_AWAY;
        }
        else {
          Serial.print(distance);
          Serial.println(" cm");
          digitalWrite(IN1, LOW); //reverse train
          digitalWrite(IN2, HIGH);
        }
      }
      break;

    case PICK_UP_RIGHT_WAGON:
      {
        long duration;
        Serial.println("collecting right wagon");
        analogWrite(ENA, 100);

        digitalWrite(trigPin_Brown, LOW);  // Turn the trigger off
        delayMicroseconds(2);
        digitalWrite(trigPin_Brown, HIGH);  // Turn the trigger on
        delayMicroseconds(10);
        digitalWrite(trigPin_Brown, LOW);  // Turn the trigger off

        duration = pulseIn(echoPin_Brown, HIGH); //pulse the echo pin to detect
        distance = (duration / 2) / 29.1; //define the distance in centimetres

        if (distance < 5) {  // Once the train is less than 5cm from the sensor
          digitalWrite(IN2, LOW); //stop the train motor
          loop_counter = 0;
          state = PULL_AWAY;
        }
        else {
          Serial.print(distance);
          Serial.println(" cm");
          digitalWrite(IN1, LOW); //reverse train
          digitalWrite(IN2, HIGH);
        }
      }
      break;
  }
}
