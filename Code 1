//This sketch makes a classic 9V or 12V train move backwards and forwards on
//the track for a fixed amount of time.

int IN1=22; // train motor pin 1
int IN2=24; // train motor pin 2
int ENA=10; // train motor PMW

void setup() //run once at startup
{
 pinMode(IN1, OUTPUT);
 pinMode(IN2, OUTPUT); 
 analogWrite(ENA, 110);  
}

void loop() // run repeatedly from top to bottom
 {
 digitalWrite(IN1,LOW); // the train will move forward
 digitalWrite(IN2,HIGH); 
 delay(250); // lock up for 0.25 seconds
 digitalWrite(IN1,LOW); // the train will stop
 digitalWrite(IN2,LOW); 
 delay(2000); // lock up for 2 seconds
 digitalWrite(IN1,HIGH);// the train will move in reverse
 digitalWrite(IN2,LOW); 
 delay(250); // Lock up for 0.25 seconds
 digitalWrite(IN1,LOW); // the train will stop
 digitalWrite(IN2,LOW);
 delay(2000); // lock up for 2 seconds
 }
