#include <SPI.h> // SPI Library
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(7, 8); // Begin an RF24 transceiver instance called "radio",
 // where the CE and CSN pins on the nRF24L01 board
// are connected to pins 7 and 8 on the Arduino.

const byte rxAddr[6] = "00001"; // transceiver "pipe" address
const byte txAddr[6] = "00011"; // transceiver "pipe" address
char myChar = 'a';

enum state {
  initialize,
  listening,
  incrementCharacter,
  transmitCharacter
};

int increment_btn = 5;
int transmit_btn = 6;

state statevar = initialize;

void setup() {
  // put your setup code here, to run once:
  radio.begin(); // begin transceiver communication
  radio.openReadingPipe(1, rxAddr); // pipe 1 opened for listening
  radio.startListening();

  // setup for radio out
  radio.setPALevel(RF24_PA_LOW); 
  radio.setRetries(15, 15);
  radio.openWritingPipe(txAddr);
  statevar = listening;

  Serial.begin(9600);
  // setup for buttons
  pinMode(increment_btn, INPUT_PULLUP);
  pinMode(transmit_btn, INPUT_PULLUP);
}

void incrCharacter(){ // increment state
  myChar += 1;
  if (myChar >= 107) {
    myChar = 'a';
  }
  delay(500);
  statevar = listening;
}

void transCharacter(){ // transmit state
  radio.stopListening();
  radio.write(&myChar, sizeof(myChar));
  radio.startListening();
  statevar = listening;
}

void displayMsg() {
  char c = 0;
  radio.read(&c, sizeof(c)); // read character
  Serial.println(c); // print char to serial monitor
}

void loop() {
  // listening state
  switch(statevar){
    case transmitCharacter: // is transmit_btn pressed?
      //Serial.println("transmit");
      transCharacter();
      break;
    case listening: // is there data available?
      //Serial.println("listening");
      if (radio.available()) { // if data received...
        displayMsg();
      }
      // if button 1 pressed
      if (digitalRead(increment_btn) == LOW){
        //Serial.println("increment");
        statevar = incrementCharacter;
      }
      // if button 2 pressed
      if (digitalRead(transmit_btn) == LOW){
        //Serial.println("transmit");
        statevar = transmitCharacter;
      }
      break;
    case incrementCharacter: // is increment_btn pressed?
      //Serial.println("increment case");
      incrCharacter();
      Serial.println("increment myChar to: " + String(myChar));
      break;
    case initialize:
      //Serial.println("initialize");
      char myChar = 'a';
      break;
  }
}
