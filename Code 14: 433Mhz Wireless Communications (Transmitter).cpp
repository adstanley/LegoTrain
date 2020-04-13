#include <VirtualWire.h>
#include <SoftwareSerial.h>// import the serial library
SoftwareSerial bluetooth(51, 53); //RX, TX for bluetooth - MEGA pins

const int transmit_pin = 43; //transmitter pin
int A_IA = 46; //motor controller pin 1
int A_IB = 48; //motor controller pin 2
int ENA = 8;


//to hold the caculated values
char *bluetoothData; // the data given from mobile app
boolean newData = false;  //flag to check if bluetooth data is received
boolean track_counter = false;  //flag to determine track point direction

void setup() {
  bluetooth.begin(9600);
  vw_set_tx_pin(transmit_pin);

  // VirtualWire setup
  vw_setup(2000);     // Bits per sec

  Serial.begin(9600);
  pinMode(A_IA, OUTPUT);
  pinMode(A_IB, OUTPUT);
  digitalWrite(A_IA, LOW);
  digitalWrite(A_IB, LOW);
  analogWrite(ENA, 120);// motor speed  
}

void loop() {

  readBluetooth();
  sendData();
}

void readBluetooth() {

  if (bluetooth.available()) {
    bluetoothData = bluetooth.read();
    newData = true;
  }
}

void sendData() {
  if (newData == true) {
    
    int x = bluetoothData;

    if (bluetoothData == '5') {
      switchPoints();
    }

    else {
      double sensorArray[] = {x};

      vw_send((uint8_t *)sensorArray, sizeof(sensorArray));
      vw_wait_tx(); // Wait until the whole message is gone
      delay(50);
    }
    //printing values
    Serial.print("X: ");
    Serial.println(x);
    newData = false;
  }
}

void switchPoints() {
  if (track_counter == false) {     //if the track counter is an EVEN number
    digitalWrite(A_IA, LOW); // rotate forward
    digitalWrite(A_IB, HIGH);
    delay(250);
    digitalWrite(A_IB, LOW);
    track_counter = true;
  }
  else {                            //if the track counter is an ODD number
    digitalWrite(A_IA, HIGH); // rotate reverse
    digitalWrite(A_IB, LOW);
    delay(300);
    digitalWrite(A_IA, LOW); // stop
    track_counter = false;
  }
}
