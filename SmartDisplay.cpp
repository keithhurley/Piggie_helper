// SmartDisplay - Library for writing to lcd display for piggie helper

#include "SmartDisplay.h"
#include "application.h"
#include "Adafruit_LiquidCrystal.h"


#define ON HIGH
#define OFF LOW



SmartDisplay::SmartDisplay() {
    // TO connect via I2C. Data pin is D0, Clock is D1
    // By default, no jumpers are soldered, giving an address of 0.
    LiquidCrystal myDisplay(0);
    myDisplay.begin(20, 4);
    printThis(0,0,"Initializing...");
    myDisplay.setBacklight(ON);
  }
