/* This sketch controls a motor speed (ENA) based on two IR buttons:
      UP button (2080) increases speed
      DOWN button (2081) decreases speed
      POWER button is STOP/RESUME
      LEFT sets train direction anti-clockwise
      RIGHT sets train direction clockwise

   IR Code and speed results are displayed on LCD

*/

#include <IRremote.h>

#define UP 2080
#define DOWN 2081
#define POWER 2060
#define LEFT 2065
#define RIGHT 2064

int IRpin = A14;  // pin for the IR sensor
IRrecv irrecv(IRpin);
decode_results results;
int codevalue = 0;
int ENAvalue = 0; //if this is byte, we get overflow error
int ENAprevvalue = 0; //for stop/resume function


int IN1 = 33; //motor control 1
int IN2 = 31; //motor control 2
byte ENA = 2; //0-255 only

int last_command = 2081;

void setup()
{
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENA, ENAvalue);

  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver

}
void loop()
{
  if (irrecv.decode(&results)) {
    codevalue = results.value;
    if (codevalue < 2048) {   //correct low-value code readings
      codevalue = codevalue += 2048;
    }
    decode_command(codevalue);
    refresh();
    irrecv.resume();   // Receive the next value
  }
}

void decode_command(int value) {
  switch (value) {
    case UP:
      ENAvalue += 10;
      if (ENAvalue > 255) {
        ENAvalue = 255; //max motor speed
      }
      last_command = UP;
      break;

    case DOWN:
      ENAvalue -= 10;
      if (ENAvalue < 0) {
        ENAvalue = 0;   // min motor speed
      }
      last_command = DOWN;
      break;

    case POWER:
      if (ENAvalue > 0) {  //train is in motion
        ENAprevvalue = ENAvalue;  //save current ENA value
        ENAvalue = 0;   //set speed to zero
      }
      else                // current speed is zero
        ENAvalue = ENAprevvalue;  //restore last known speed
      last_command = POWER;
      break;

    case LEFT:
      digitalWrite(IN2, LOW);
      digitalWrite(IN1, HIGH);
      last_command = LEFT;
      break;

    case RIGHT:
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
      last_command = RIGHT;
      break;
  }
}
