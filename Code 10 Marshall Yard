//This full sketch controls two locomotives swapping three wagons.
//The first locomotive takes a wagon from the siding to the decoupler. As it passes
//the IR sensor, the wagon is detected, then uncoupled. The train jerks forward, 
//the points switch, then the locomotive collects the wagon on the other siding. 
//When it reaches the siding, power shuts down, sidings points switch, and 
//the arduino waits for an interrupt.
//When the interrupt is triggered, the other locomotive switches from the main loop 
//to the station, and repeats the uncouple-collect process.

#include <LiquidCrystal595.h>// include the library
LiquidCrystal595 lcd(49,53,51);     // datapin, latchpin, clockpin

int siding_track_power_IN1=27; //siding track power
int siding_track_power_IN2=29;
int siding_track_power_PWM=5;
int siding_points_IN1=22; //points from station to marshalling yard
int siding_points_IN2=24;
int siding_points_PWM=10;
int main_points_left_IN1=23; //points from loop to station
int main_points_left_IN2=25;
int main_points_left_PWM=4;
int main_points_right_IN1=44; //points from station to loop
int main_points_right_IN2=42;
int main_points_right_PWM=6;
int station_points_left_IN1=32; //station points in
int station_points_left_IN2=30;
int station_points_left_PWM=12;
int station_points_right_IN1=35; //station points out
int station_points_right_IN2=37;
int station_points_right_PWM=2;
int loop_track_power_IN1=33; //power to the main loop track
int loop_track_power_IN2=31;
int loop_track_power_PWM=3;

int track_power_IN1; //variable power settings for station power (either siding or loop power)
int track_power_IN2;
int track_power_PWM;

int decoupler_IN1 = 48; //power settings for station decoupler
int decoupler_IN2 = 46;
int decoupler_PWM = 8;
int IR_station=0; //global variable for each IR sensor pin (10 or 11)

int trigPin = 41; //pins for siding distance sensor
int echoPin = 39;

int light_sensitivity = 150;
int LDRValue = 0;
int previousLDRValue=255;
int previousLDRValue2=0;
int track_counter=0;
int loop_counter = 0;
long distance=10;
unsigned long previousMillis = 0;

//possible states of the state machine
enum States {
  goToCoupler,  //both light sensors off, power to both trains
  wagonDetected,  //light sensor counter goes from 0 to 1
  raiseUncoupler, //light sensor counter goes from 1 to 2
  uncouple, //light sensor counter goes from 1 to 2
  switchLoopPoints, //switch the loop points to the station
  switchStationPoints, //switch between station tracks
  switchSidingPoints, //switch the siding points
  pickUpWagon, //collect wagon from left siding
  waiting, //shut down siding train
  reverse, //loop train collects wagon
};

States state = pickUpWagon;
byte previousState = pickUpWagon;

void setup() {
  {
    lcd.begin(16,2);             // 16 characters, 2 rows
    lcd.clear();
    attachInterrupt(3, interrupted, RISING);  //if the sensor on interrupt 3 detects a train, run void interrupted
    pinMode(siding_track_power_PWM, OUTPUT);
    pinMode(siding_track_power_IN1,OUTPUT);
    pinMode(siding_track_power_IN2,OUTPUT);
    pinMode(main_points_left_IN1,OUTPUT);
    pinMode(main_points_left_IN2,OUTPUT);
    pinMode(main_points_right_IN1,OUTPUT);
    pinMode(main_points_right_IN2,OUTPUT);
    pinMode(station_points_left_IN1,OUTPUT);
    pinMode(station_points_left_IN2,OUTPUT);
    pinMode(station_points_right_IN1,OUTPUT);
    pinMode(station_points_right_IN2,OUTPUT);
    pinMode(siding_points_IN1,OUTPUT);
    pinMode(siding_points_IN2,OUTPUT);
    pinMode(loop_track_power_IN1,OUTPUT);
    pinMode(loop_track_power_IN2,OUTPUT);
    pinMode(track_power_IN1,OUTPUT);
    pinMode(track_power_IN2,OUTPUT);
   
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
    
    pinMode(IR_station,INPUT);    //check if this needs to be inside each case
    
    pinMode(decoupler_IN1, OUTPUT);
    pinMode(decoupler_IN2, OUTPUT);
    analogWrite(decoupler_PWM,80);
    analogWrite(main_points_left_PWM,255);
    analogWrite(main_points_right_PWM,255);
    analogWrite(station_points_left_PWM,255);
    analogWrite(station_points_right_PWM,255);
    analogWrite(siding_points_PWM,255);
    decouplerTrackPower();
    loopTrackPower();
    resetPoints();
  }
}

void loop() {
 
switch(state)
 {
  
 case pickUpWagon:
 {
long duration;
    lcd.setCursor(0,0);
    lcd.println("collecting wagon");
    decouplerTrackPower();
    analogWrite(track_power_PWM,80);
    
    digitalWrite(trigPin, LOW);  // Turn the trigger off
    delayMicroseconds(2); 
    digitalWrite(trigPin, HIGH);  // Turn the trigger on
    delayMicroseconds(10); 
    digitalWrite(trigPin, LOW);  // Turn the trigger off
          
    duration = pulseIn(echoPin, HIGH); //pulse the echo pin to detect
    distance = (duration/2) / 29.1;  //define the distance in centimetres
    
    if (distance < 9) {  // Once the train is less than 9cm from the sensor
       digitalWrite(track_power_IN2,LOW); //stop the train motor
       if (previousState == switchStationPoints) {
         previousState = pickUpWagon;
         state = switchSidingPoints;
       }
       else {
         previousState = pickUpWagon;
         state = goToCoupler;
       }
     }
    else {
      Serial.print(distance);
      Serial.println(" cm");
      digitalWrite(track_power_IN1,LOW); //reverse train
      digitalWrite(track_power_IN2,HIGH);
    }
 }
  break;

   case goToCoupler:  //both light sensors off, power to both trains
     { 
      lcd.setCursor(0,0);
      lcd.println("go to uncoupler ");

      decouplerTrackPower();
      digitalWrite(track_power_IN2,LOW);
      digitalWrite(track_power_IN1,HIGH); 
      analogWrite(track_power_PWM, 90);
      decouplerSensor();

      LDRValue = map(analogRead(IR_station), 0, 1023, 0, 255);  //reads IR sensor on decoupler
      lcd.setCursor(0,1);
      lcd.println(LDRValue); //prints the LDR values to serial monitor
   
      if (LDRValue < light_sensitivity) {          if (previousLDRValue > light_sensitivity) {
            state = wagonDetected;
          }
      }  
      previousLDRValue = LDRValue;   //save light sensor state 
  }

break;

  case wagonDetected: 
{
   lcd.setCursor(0,0);
   lcd.println("wagon detected");
   decouplerTrackPower();
   analogWrite(track_power_PWM, 90);
   decouplerSensor();
   
   LDRValue = map(analogRead(IR_station), 0, 1023, 0, 255);  //reads  IR sensor on decoupler
   lcd.setCursor(0,1);
   lcd.println(LDRValue); //prints the LDR values to serial monitor
   
   if (LDRValue > light_sensitivity) {
      if (previousLDRValue2 < light_sensitivity) {
        state = raiseUncoupler;
      }
   }   
   previousLDRValue2 = LDRValue;   //save light sensor state 
}

break;

  case raiseUncoupler:
{   
      lcd.setCursor(0,0);
      lcd.println("raise uncoupler");
      analogWrite(track_power_PWM, 0);  //turn off engine

        if (track_counter % 2 == 0) {     //if the track counter is an EVEN number
                digitalWrite(decoupler_IN1,HIGH);   // raise right siding decoupler
                digitalWrite(decoupler_IN2,LOW);
                delay(150);
                digitalWrite(decoupler_IN1,LOW);
                state = uncouple;
                }
         else {                           //if the track counter is an ODD number
               digitalWrite(decoupler_IN1,LOW);     // raise left siding decoupler
               digitalWrite(decoupler_IN2,HIGH);
               delay(150);
               digitalWrite(decoupler_IN2,LOW);
               state = uncouple;
              }
     }
break;

 case uncouple:
  {
    lcd.setCursor(0,0);
    lcd.println("uncoupling"); 
    digitalWrite(track_power_IN1,HIGH);  //high-speed pulse engine forward
    digitalWrite(track_power_IN2,LOW);
    analogWrite(track_power_PWM,255);
    delay(280);
    analogWrite(track_power_PWM,0); //turn off engine
    track_counter++;
    state = switchStationPoints;
  }
break;

 case switchStationPoints:  //Switch the tracks between the stations
     { 
       lcd.setCursor(0,0);
       lcd.println("switch station");     
       digitalWrite(track_power_IN1, LOW);
       digitalWrite(track_power_IN2, LOW);
         if (track_counter % 2 == 0) {     //if the track counter is an EVEN number
                digitalWrite(station_points_left_IN1,LOW);// switch to straight
                digitalWrite(station_points_left_IN2,HIGH);
                digitalWrite(station_points_right_IN1,LOW);// switch to straight
                digitalWrite(station_points_right_IN2,HIGH);
                delay(250);
                digitalWrite(station_points_left_IN2,LOW);
                digitalWrite(station_points_right_IN2,LOW);
                if (previousState == pickUpWagon) {
                  previousState = switchStationPoints;
                  state = pickUpWagon;
                }
                else {
                  previousState = switchStationPoints;
                  state = reverse;
                }
         }
         else {                            //if the track counter is an ODD number
               digitalWrite(station_points_left_IN2,LOW);
               digitalWrite(station_points_left_IN1,HIGH);// switch to station
               digitalWrite(station_points_right_IN2,LOW);
               digitalWrite(station_points_right_IN1,HIGH);// switch to station
               delay(250);
               digitalWrite(station_points_left_IN1,LOW);
               digitalWrite(station_points_right_IN1,LOW);
               if (previousState == pickUpWagon) {
                  previousState = switchStationPoints;
                  state = pickUpWagon;
                }
                else {
                  previousState = switchStationPoints;
                  state = reverse;
                }
              }
     }
  break;

  case reverse:
    {
      lcd.setCursor(0,0);
      lcd.println("reversing");
      digitalWrite(track_power_IN1,LOW);
      digitalWrite(track_power_IN2,HIGH);
      analogWrite(track_power_PWM,90);
      delay(3000);
      digitalWrite(track_power_IN2,LOW);
      delay(2000);
      decouplerTrackPower();
      loopTrackPower();
      delay(5000);
      loop_counter++;
      state = switchLoopPoints;
      
    }
  break;

 case switchLoopPoints:  //Switch the tracks between the stations
     { 
       lcd.setCursor(0,0);
       lcd.println("switch loop");     
         if (loop_counter % 2 == 0) {     //if the track counter is an EVEN number
                digitalWrite(main_points_left_IN1,LOW);// switch to straight
                digitalWrite(main_points_left_IN2,HIGH);
                digitalWrite(main_points_right_IN2,LOW);// switch to straight
                digitalWrite(main_points_right_IN1,HIGH);
                delay(250);
                digitalWrite(main_points_left_IN2,LOW);
                digitalWrite(main_points_right_IN1,LOW);
                if (previousState == switchStationPoints) {
                  previousState = switchLoopPoints;
                  state = switchSidingPoints;
                }
                else {
                  previousState = switchLoopPoints;
                  state = goToCoupler;
                }
                }
         else {                            //if the track counter is an ODD number
               digitalWrite(main_points_left_IN2,LOW);
               digitalWrite(main_points_left_IN1,HIGH);// switch to station
               digitalWrite(main_points_right_IN1,LOW);
               digitalWrite(main_points_right_IN2,HIGH);// switch to station
               delay(250);
               digitalWrite(main_points_left_IN1,LOW);
               digitalWrite(main_points_right_IN2,LOW);
               if (previousState == switchStationPoints) {
                  previousState = switchLoopPoints;
                  state = switchSidingPoints;
                }
                else {
                  previousState = switchLoopPoints;
                  state = goToCoupler;
                }
              }
     }
  break;

 case switchSidingPoints:  //Switch the tracks between the stations
     { 
       lcd.setCursor(0,0);
       lcd.println("switch siding");     
       digitalWrite(track_power_IN1, LOW);
       digitalWrite(track_power_IN2, LOW);
         if (previousState == switchLoopPoints) {     
                digitalWrite(siding_points_IN1,LOW);// siding power to station enabled
                digitalWrite(siding_points_IN2,HIGH);
                delay(250);
                digitalWrite(siding_points_IN2,LOW);
                state = pickUpWagon;
         }
         else {                            
               digitalWrite(siding_points_IN2,LOW);
               digitalWrite(siding_points_IN1,HIGH);// siding power to station disabled
               delay(250);
               digitalWrite(siding_points_IN1,LOW);
               
               loop_counter++;
               state = waiting;
         }
     }
  break;

  case waiting:
   {
     lcd.setCursor(0,0);
      lcd.println("waiting");
    analogWrite(siding_track_power_PWM,0);
    
   }
   break;
  }
}

void interrupted()
{
  if (state == waiting) {   //set state number here
    previousState = waiting;
    state = switchLoopPoints;
  }
}

void decouplerSensor() //determines which IR sensor to use for the decoupler
{
  if (track_counter % 2 ==0){
     IR_station = 11;  //use white track IR sensor
   }
   else {
     IR_station = 10; //use gray track IR sensor
   }
}

void decouplerTrackPower()
{
  if (previousState == switchLoopPoints) {
      track_power_IN1 = loop_track_power_IN1; //use main loop track power on station loop
      track_power_IN2 = loop_track_power_IN2;
      track_power_PWM = loop_track_power_PWM;
      }
   else {
      track_power_IN1 = siding_track_power_IN1; //use siding track power on station loop
      track_power_IN2 = siding_track_power_IN2;
      track_power_PWM = siding_track_power_PWM;
   }
}

void loopTrackPower() {
  digitalWrite(loop_track_power_IN1,HIGH);
  digitalWrite(loop_track_power_IN2,LOW);
  analogWrite(loop_track_power_PWM,110);
}

void resetPoints() {
   digitalWrite(siding_points_IN1,LOW);// siding power to station enabled
   digitalWrite(siding_points_IN2,HIGH);
   digitalWrite(main_points_left_IN1,LOW);// switch to loop
   digitalWrite(main_points_left_IN2,HIGH);
   digitalWrite(main_points_right_IN2,LOW);
   digitalWrite(main_points_right_IN1,HIGH);
   digitalWrite(station_points_left_IN1,LOW);// switch to top station
   digitalWrite(station_points_left_IN2,HIGH);
   digitalWrite(station_points_right_IN1,LOW);
   digitalWrite(station_points_right_IN2,HIGH);
   delay(250);
   digitalWrite(siding_points_IN2,LOW);
   digitalWrite(main_points_left_IN2,LOW);
   digitalWrite(main_points_right_IN1,LOW);
   digitalWrite(station_points_left_IN2,LOW);
   digitalWrite(station_points_right_IN2,LOW);
   digitalWrite(decoupler_IN1,LOW);   // raise right siding decoupler
   digitalWrite(decoupler_IN2,HIGH);
   delay(150);
   digitalWrite(decoupler_IN2,LOW);
}
