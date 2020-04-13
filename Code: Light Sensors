 //This sketch pauses the train when it covers the light sensor.

//Pins
const int In1 = 8; //train motor pin 1
const int In2 = 9; //train motor pin 2
const int EnA = 10; //train motor PWM
const int LDR = 0;  //LDR pin

//Vars
int LDRValue = 0;            //save the LDR val of the last pass
unsigned long previousMillis = 0;     //will store last time light sensor updated

//Settings
int interval = 3000;            //pause period
int lightSensitivity = 500;  //set switching point for light sensor (LDR)

void setup()
{
  Serial.begin(9600);
  pinMode(EnA, OUTPUT);
  pinMode(In1, OUTPUT);
  pinMode(In2, OUTPUT);

  //Placed it here because it doesn't change (or I give it a default direction)
  digitalWrite(In1, LOW); // rotate forward
  digitalWrite(In2, HIGH);
}
void loop()
{
  int ldrValNow;

  Serial.println(ldrValNow);   //prints the LDR values to seial monitor
  delay(50);                  //speed at which LDR sends value to Arduino


  ldrValNow = analogRead(LDR);  //reads the LDR's value
  if (ldrValNow > lightSensitivity) {    //train in front of sensor
    if (LDRValue <= lightSensitivity) {    //but last time the train wasn't in front (so this is the transition between in front and not in front)
      analogWrite(EnA, 0);  //only then power off train
      previousMillis = millis();    //and save the time
    }

    else if (millis() - previousMillis > interval) { //Time to start the train again?
      analogWrite(EnA, 120);
    }
  }
  else {    //Not in front, also drive!
    analogWrite(EnA, 120);
  }

  LDRValue = ldrValNow;    //save for the next time so we can detect a transition
}
