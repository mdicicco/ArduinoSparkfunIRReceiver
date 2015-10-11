/*
  IR.cpp - Library for receiving data from a particular IR remote.
  Created by Konstantin Tretyakov (http://kt.era.ee/), 2012.
  Updated by Matt DiCicco (https://github.com/mdicicco), 2015
   to support newer model of sparkfun IR Remote
  MIT license.

  NB:
    * The IR detector must be connected to PIN 2 of Arduino. Only one detector is supported.
    * The code uses PCINT2_vect, so it probably wont link with other code that wants to use this interrupt.

  Usage:
    void setup() {
      ...
      IR.setup();
      ...
    }

    void loop() {
      while (IR.available()) {
        command_t command = IR.pop(); // Read next queued IR command.
        char* cmd = IR.commandToString(command);  // Translate to string (only translates several known commands, returns NULL for others)
        if (cmd != NULL) Serial.println(cmd);     // Show
      }
    }
*/
#ifndef IR_H
#define IR_H

#ifndef NULL
#define NULL 0
#endif

typedef unsigned int command_t;

// Ring buffer for storing unsigned ints
class RingBuffer {
private:
  static const int BUF_SIZE_BITS = 7;
  static const int BUF_SIZE = (1 << BUF_SIZE_BITS);
  static const int BUF_SIZE_MASK = (BUF_SIZE - 1);
  int head;
  int tail;
  command_t buffer[BUF_SIZE];

public:
  inline void init() {
    head = tail = 0;
  }
  inline void push(command_t val) {
    buffer[head++] = val;
    head &= BUF_SIZE_MASK;
    if (head == tail) { // Overwriting tail
      tail++;
      tail &= BUF_SIZE_MASK;
    }
  }
  inline command_t pop() {  // Don't do this unless available == true
    unsigned int result = buffer[tail++];
    tail &= BUF_SIZE_MASK;
    return result;
  }
  inline int available() {
    return (head != tail);
  }
};

// --------------------------- IR Detector lib -------------------------- //
// IR Detector data
class IRDetector {
public:
  // Codes
  //  HEADER    0001000011101111 4335
  //  onoff     1101100000100111 55335
  //  A         1111100000000111 63495
  //  B         0111100010000111 30855
  //  C         0101100010100111 22695
  //  UP        1010000001011111 41055
  //  LEFT      0001000011101111 4335
  //  CENTER    0010000011011111 8415
  //  RIGHT     1000000001111111 32895
  //  DOWN      0000000011111111 255

  static const command_t HEADER = 4335;
  static const command_t ON_OFF = 55335;
  static const command_t UP = 41055;
  static const command_t DOWN = 255;
  static const command_t CENTER = 8415;
  static const command_t LEFT = 4335;
  static const command_t RIGHT = 32895;
  static const command_t A = 63495;
  static const command_t B = 30855;
  static const command_t C = 22695;

  // Simple helper for translating commands to their names
  // If unknown returns NULL
  inline char* commandToString(command_t cmd) {
    if (cmd == ON_OFF) return "ON-OFF";
    else if (cmd == UP) return "UP";
    else if (cmd == DOWN) return "DOWN";
    else if (cmd == CENTER) return "CENTER";
    else if (cmd == LEFT) return "LEFT";
    else if (cmd == RIGHT) return "RIGHT";
    else if (cmd == A) return "A";
    else if (cmd == B) return "B";
    else if (cmd == C) return "C";
    else return NULL;
  }

private:
  unsigned long lastBitTimestamp;
  unsigned long curTime;
  unsigned long elapsed;
  unsigned int numBitsAvailable;
  command_t command;
  RingBuffer buffer;

public:
  void setup();
  void interrupt();
  inline int available() { return buffer.available(); }
  inline command_t pop() { return buffer.pop(); }
};
extern IRDetector IR;

#endif
