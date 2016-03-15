// SmartDisplay - Library for 20x4 character display use in piggie helper

#ifndef SmartDisplay_h
#define SmartDisplay_h

#include "application.h"
#include "Adafruit_LiquidCrystal.h"

#define ON HIGH
#define OFF LOW


class SmartDisplay
{
    public:
        SmartDisplay();
        void printProbeMarker(uint8_t probeIndex);
        void printTemp(uint8_t probeIndex, double probeTemp);
        void printAlarm(uint8_t probeIndex, double probeAlarm);
        void printFan(double fanSpeed, double fanPercent);
        void backlightOn();
        void backlightOff();
        void backlightToggle();
        void printThis(int cursorX, int cursorY, String myText);
        void updateDisplay();
    private:
        bool _state;
        int _pin;
};

#endif
