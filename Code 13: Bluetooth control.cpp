#include <legopowerfunctions.h> //Power Functions library
#include <SoftwareSerial.h>// import the serial library
SoftwareSerial bluetooth(2, 3); // RX, TX
LEGOPowerFunctions lego(4); //IR pin for power functions control

char bluetoothData; // the data given from mobile app
boolean newData = false;
int PWMvalue = 0; //PWM command sent to the Power Functions receiver
int PWMprevvalue = 0; //PWM storage for power on/off button
int PWMdirvalue = 0; //PWM storage for direction button

void setup() {
 bluetooth.begin(9600);
}
void loop() {

 recvOneChar();
 controlTrain();
}

void recvOneChar() {

 if (bluetooth.available() > 0) {
 bluetoothData = bluetooth.read();
 newData = true;
 }
}

void controlTrain() {
 if (newData == true) {

 switch (bluetoothData) {

 case '0': { //because input is a char, we need single quotes around case value
 PWMvalue = 0;
 setTrainSpeed();
 }
 break;

 case '1': { //Stop/Resume function
 if (PWMvalue != 8 && PWMvalue != 0) { //train is in motion
 PWMprevvalue = PWMvalue; //save current ENA value
 PWMvalue = 0; //set speed to zero
 setTrainSpeed();
 }
 else // current speed is zero
 PWMvalue = PWMprevvalue; //restore last known speed
 setTrainSpeed();

 }
 break;

 case '2': { //change direction
 lego.SingleOutput(0, PWM_FLT, RED, CH1); //stop train
 delay(500);
 PWMdirvalue = (16 - PWMvalue);
 PWMvalue = PWMdirvalue;
 setTrainSpeed();
 }
 break;

 case '3': { // decrease motor speed
 if (PWMvalue <= 7) {
 PWMvalue--;
 if (PWMvalue < 0) {
 PWMvalue = 0;
 }

 }
 else
 PWMvalue++;
 if (PWMvalue > 15) {
 lego.SingleOutput(0, 0, RED, CH1);
 PWMvalue = 16;
 }

 setTrainSpeed();
 }
 break;

 case '4': { //increase motor speed
 if (PWMvalue >= 9) {
 PWMvalue--;
 if (PWMvalue < 9) {
 PWMvalue = 9;
 }
 }
 else
 PWMvalue++;
 if (PWMvalue == 8) {
 PWMvalue = 7;
 }
 setTrainSpeed();
 }
 break;
 }
 newData = false;
 }
}

void setTrainSpeed()
{
 lego.SingleOutput(0, PWMvalue, RED, CH1);
}
