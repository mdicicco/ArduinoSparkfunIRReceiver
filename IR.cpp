/*
  IR.cpp - Library for receiving data from a particular IR remote.
  Created by Konstantin Tretyakov (http://kt.era.ee/), 2012.
  Updated by Matt DiCicco (https://github.com/mdicicco), 2015
   to support newer model of sparkfun IR Remote
  MIT license.
*/

#include "Arduino.h"
#include "IR.h"

// We use IR detector on Arduino pin 2, which is atmega's PD2
#define PIN_ATMEGA PIND
static const int PINBIT_ATMEGA = 2;
static const int PIN_ARDUINO = 2;

void IRDetector::setup() {
  pinMode(PIN_ARDUINO, INPUT);
  PCICR  |=  4; // enable PCIE2 which services PCINT18
  PCMSK2 |=  4; // enable PCINT18 --> Pin Change Interrupt of PD2

  lastBitTimestamp = 0;
  numBitsAvailable = 0;
  command = 0;
  buffer.init();
}

// Interrupt handler
void IRDetector::interrupt() {
  if (PIN_ATMEGA & (1<<PINBIT_ATMEGA)) return;  // Only consider falling edges
  curTime = micros();
  elapsed = curTime - lastBitTimestamp;
  lastBitTimestamp = curTime;
  //buffer.push(elapsed);

  numBitsAvailable++;
  if (elapsed < 1200) { // Zero
    command <<= 1;
  }
  else if (elapsed < 2400) { // One
    command <<= 1;
    command++;
  }
  else { // Start new command
    // command = 0; // No need as we work in 16-bit chunks
    numBitsAvailable = 0;
  }

  // Command is finished on 16 bits.
  if (numBitsAvailable == 16) {

    buffer.push(command);
    numBitsAvailable = 0;
  }
}


// Instantiate a single IR detector
IRDetector IR;

// Register interrupt handler
ISR(PCINT2_vect) {
  IR.interrupt();
}
