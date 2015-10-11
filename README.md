ArduinoSparkfunIRReceiver
============================

Arduino library for using the SparkFun IR receiver breakout (http://www.sparkfun.com/products/8554).
With the new COM-11759 remote control (https://www.sparkfun.com/products/11759)

Usage:
  * Check the code out to your Sketchbook's libraries/ subdirectory
    (e.g. ~/Arduino/libraries $ git clone https://konstantint@github.com/konstantint/ArduinoSparkfunIRReceiver.git)
  * Restart Arduino environment.
  * Check the example.

Copyright: Konstantin Tretyakov (http://kt.era.ee)
License: MIT License.

Notes:
  * Updated from the original codebase to support the protocol on the new default sparkfun remote
  * Changes included: smaller timing window, and new codes for 32 bit header+command combo instead
    of basic 16 bit command words
  * This is a quick hack-up which was created by looking at the signal
    produced by the SparkFun remote (http://www.sparkfun.com/products/10280).
    The code recognizes those signals. There's no guarantee that it will work with anything else,
    but it should be easy to tune, if needed. Please push if you improve.
