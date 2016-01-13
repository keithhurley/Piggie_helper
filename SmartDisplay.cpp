// SmartDisplay - Library for writing to lcd display for piggie helper

#include "SmartDisplay.h"
#include "application.h"

#define ON HIGH
#define OFF LOW


SmartDisplay::SmartDisplay(uint8_t address) {
    // TO connect via I2C. Data pin is D0, Clock is D1
    // By default, no jumpers are soldered, giving an address of 0.
    LiquidCrystal myDisplay(address);
    myDisplay.begin(20, 4);
    myDisplay.setCursor(0,0);
    myDisplay.print("Initializing...");
    myDisplay.setBacklight(ON);
  }


void SmartDisplay::printProbeMarker(uint8_t probeIndex){

}


void SmartDisplay::printTemp(uint8_t probeIndex, double probeTemp){

}


void SmartDisplay::printAlarm(uint8_t probeIndex, double probeAlarm){

}


void SmartDisplay::printFan(double fanSpeed, double fanPercent){

}


void SmartDisplay::backlightOn(){

}


void SmartDisplay::backlightOff(){

}


void SmartDisplay::backlightToggle(){

}

void SmartDisplay::printThis(int cursorX, int cursory, String myText){

}

void SmartDisplay::updateDisplay(){
  
}
