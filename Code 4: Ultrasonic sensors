/* This sketch loops a train around the track once, then reverses it into a siding the second time it passes the light sensor. It pauses for 5 seconds before stating.
  NOTE: This sketch uses DELAY.
*/

//Pins
const int IN1 = 28;
const int IN2 = 26;
const int ENA = 8;
const int LDR = 8;
const int trigPin = 50;
const int echoPin = 52;

//Vars
int LDRValue = 0;
int LightSensorCounter = 1;

//Settings
int lightSensitivity = 500;

void setup()
{
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  analogWrite(ENA, 110);
}

void loop() {
  movetrainforward();
}

void movetrainforward()
{
  int ldrValNow;

  Serial.println(ldrValNow);
  delay(50);

  ldrValNow = analogRead(LDR);
  if (ldrValNow < lightSensitivity) {
    if (LDRValue <= lightSensitivity) {
      LightSensorCounter++;
    }
  }
  LDRValue =  ldrValNow;

  if (LightSensorCounter % 3 == 0) {
    gotosiding();
  }
  else {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
  }
}

void gotosiding() {
  long duration, distance,;
  digitalWrite(trigPin, LOW);  // Added this line
  delayMicroseconds(2); // Added this line
  digitalWrite(trigPin, HIGH);
  //  delayMicroseconds(1000); - Removed this line
  delayMicroseconds(10); // Added this line
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration / 2) / 29.1;

  if (distance < 27) {  // This is where the LED On/Off happens
    digitalWrite(IN1, LOW);
    delay(5000);
    LightSensorCounter++;
  }
  else {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
  }
}
