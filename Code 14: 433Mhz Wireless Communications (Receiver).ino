//receiver sketch for Digispark (ATtiny85)

#include <VirtualWire.h>
int A_IA = 4; //motor controller pin 1 (PWM)
int A_IB = 1; //motor controller pin 2 (PWM)

// inputs
const int receive_pin = 2; //433mhz receiver pin - Nano

// Variables
char x;
double sensorArray[1] = {};
boolean newData = false;
int PWMvalue = 0; //PWM command sent to the motor controller
int PWMprevvalue = 0; //PWM storage for power on/off button
bool forward = true; //set Motor direction

void setup() {
  Serial.begin(9600);
  vw_set_rx_pin(receive_pin);
  vw_set_ptt_inverted(true); //VirtualWire, Initialise the IO and ISR. Required for DR3100
  vw_setup(2000); //Bits per sec
  vw_rx_start(); //Start the receiver PLL running
  pinMode(A_IA, OUTPUT);
  pinMode(A_IB, OUTPUT);
  pinMode(0, OUTPUT);
  pinMode(3, OUTPUT);
  digitalWrite(A_IA, LOW); //motor pin 1 off
  digitalWrite(A_IB, LOW); //motor pin 2 off
  digitalWrite(0, HIGH); //spare pin for 5V
  digitalWrite(3, LOW); //spare pin for GND
}

void loop() {
  recvOneChar();
  controlMotor();
}

void recvOneChar() {

  uint8_t buf[VW_MAX_MESSAGE_LEN];
  uint8_t buflen = VW_MAX_MESSAGE_LEN;

  // Non-blocking
  if (vw_get_message(buf, &buflen))   {
    newData = true;
    memcpy(sensorArray, buf, buflen);
    x = sensorArray[0]; // == x;
  }
}

void controlMotor() {
  if (newData == true) {

    switch (x) {

      case '1': {  //Stop/Resume function
          if (PWMvalue > 0) {  //Motor is in motion
            PWMprevvalue = PWMvalue;  //save current ENA value
            PWMvalue = 0;   //set speed to zero
          }
          else                // current speed is zero
            PWMvalue = PWMprevvalue;  //restore last known speed
        }
        setMotorSpeed();
        break;

      case '2': { //change direction
          analogWrite(A_IA, 0);
          analogWrite(A_IB, 0);
          delay(500);

          if (forward == true) {
            forward = false;
          }
          else
            forward = true;
        }
        setMotorSpeed();
        break;

      case '3': {  // decrease motor speed
          PWMvalue -= 10;
          if (PWMvalue < 60) {
            PWMvalue = 0;   // min motor speed
          }
        }
        setMotorSpeed();

        break;

      case '4': { //increase motor speed
          PWMvalue += 10;
          if (PWMvalue < 60) {
            PWMvalue = 60;
          }
          if (PWMvalue > 255) {
            PWMvalue = 255; //max motor speed
          }
        }
        setMotorSpeed();
        break;
    }
    Serial.println(x);
    Serial.println(PWMvalue);
    newData = false;
  }
}

void setMotorSpeed()
{
  if (forward == true) {
    analogWrite(A_IA, PWMvalue);
    analogWrite(A_IB, 0); //sets Motor speed forward
  }
  else {
    analogWrite(A_IA, 0);
    analogWrite(A_IB, PWMvalue); //sets Motor speed reverse
  }
}
