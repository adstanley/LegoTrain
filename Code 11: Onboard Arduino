/*
This code flips between two ultrasonic sensors
*/
#include <legopowerfunctions.h>
LEGOPowerFunctions lego(6); //PWM pin

#define trigPinFront 5
#define echoPinFront 4
#define trigPinBack 3
#define echoPinBack 2

byte trigPin = 5; //variable to hold which trigger pin is used - default front
byte echoPin = 4; //variable to hold which echo pin is used - default front

int counter = 0;

void setup() {
 Serial.begin (9600); 
 pinMode(trigPinFront, OUTPUT);
 pinMode(echoPinFront, INPUT);
 pinMode(trigPinBack, OUTPUT);
 pinMode(echoPinBack, INPUT);
}

void loop() {
 long duration, distance;
 digitalWrite(trigPin, LOW); // Turn trigger off
 delayMicroseconds(2); // wait a fraction of a second
 digitalWrite(trigPin, HIGH); //send out ultrasonic pulse
 delayMicroseconds(10); // pulse duration
 digitalWrite(trigPin, LOW); //turn off ultrasonic pulse
 duration = pulseIn(echoPin, HIGH); //receive input signal
 distance = (duration/2) / 29.1; //calculate distance in centimetres
 if (distance < 16) { // detection range
  counter++; //increase the couter by one
  changeDirection();
  }
  if (distance >= 200 || distance <= 0) {
 Serial.println("Out of range");
 }
 else {
 Serial.print(distance);
 Serial.println(" cm");
 Serial.print("counter=");
 Serial.print(counter);
 Serial.println();
 }
 if (counter % 2 ==0) {
 trigPin = trigPinFront;
 echoPin = echoPinFront;
 }
 else {
 trigPin = trigPinBack;
 echoPin = echoPinBack;
 }
 
 delay(100);
}

void changeDirection() {
 lego.SingleOutput(0, PWM_FLT, BLUE, CH1); //stop the train
 delay(1000);
 if (counter % 2 ==0) {
 lego.SingleOutput(0, PWM_FWD4, BLUE, CH1); //set direction forward
 }
 else {
 lego.SingleOutput(0, PWM_REV4, BLUE, CH1); //set direction backward
 }
}
