// This #include statement was automatically added by the Spark IDE.
#include "PID.h"
#include "SmartProbe.h"
#include "SmartBbq.h"
#include "SimpleButton.h"
#include "ThermistorProbe.h"
#include "SimpleLed.h"
#include "math.h"
#include "Adafruit_LiquidCrystal.h"


#define PROBE1 A1
#define PROBE2 A5
#define PROBE3 A4

#define PROBEINDICATOR1 D7
#define PROBEINDICATOR2 D6
#define PROBEINDICATOR3 D5

#define BUTTON_LEFT A2
#define BUTTON_CENTER D3
#define BUTTON_RIGHT D2

#define FAN1 A0

#define LEDCYCLE 300U
#define ALARMSETDELAY 800U

#define DEBUG FALSE

// API variables
double probeTemp1 = 0.0;
double probeTemp2 = 0.0;
double probeTemp3 = 0.0;

double probeAlarm1 = 0.0;
double probeAlarm2 = 0.0;
double probeAlarm3 = 0.0;

bool probeAlarmE1 = FALSE;
bool probeAlarmE2 = FALSE;
bool probeAlarmE3 = FALSE;

// display timer
unsigned long ledLastMillis = 0;

// alarm set timer
unsigned long alarmFirstMillis = 0;

// Define display
//SmartDisplay myLCD;
LiquidCrystal myDisplay(0);


// Define buttons
SimpleButton buttonLeft(BUTTON_LEFT);
SimpleButton buttonCenter(BUTTON_CENTER);
SimpleButton buttonRight(BUTTON_RIGHT);

// Define SmartBbq
SmartBbq bbq(PROBE1,PROBE2,PROBE3,PROBEINDICATOR1,PROBEINDICATOR2,PROBEINDICATOR3,FAN1);

//Define PID Variables we'll be connecting to
double Setpoint, Input, Output;

//Specify the links and initial tuning parameters
PID myPID(&Input, &Output, &Setpoint,2,5,1, DIRECT);

void setup() {
    // init serial
    Serial.begin(9600);

    //setup LCD
    myDisplay.begin(20, 4);
    printThis(0,0,"Initializing...");
    myDisplay.setBacklight(ON);

    // Fan test
    pinMode(FAN1, OUTPUT);
    digitalWrite(FAN1, LOW);

    //initialize PID variables we're linked to
    Input = bbq.getProbeTemp(3);
    Setpoint = bbq.getProbeAlarm(3);

    //turn the PID on
    myPID.SetMode(AUTOMATIC);

    // API - setup
    Spark.variable("probeTemp1", &probeTemp1, DOUBLE);
    Spark.variable("probeTemp2", &probeTemp2, DOUBLE);
    Spark.variable("probeTemp3", &probeTemp3, DOUBLE);

    Spark.variable("probeAlarm1", &probeAlarm1, DOUBLE);
    Spark.variable("probeAlarm2", &probeAlarm2, DOUBLE);
    Spark.variable("probeAlarm3", &probeAlarm3, DOUBLE);

    Spark.variable("probeAlarmE1", &probeAlarmE1, BOOLEAN);
    Spark.variable("probeAlarmE2", &probeAlarmE2, BOOLEAN);
    Spark.variable("probeAlarmE3", &probeAlarmE3, BOOLEAN);

    Spark.variable("fanOutput", &Output, DOUBLE);

    myDisplay.clear();
    printDisplayLabels();
    printDisplayProbeMarker();

    // Debug delay
    if(DEBUG) {
        delay(3000);
    }
}

void loop() {
    // update display
    updateDisplay();
    printDisplay();

    // check if a button was pressed
    checkButtons();

    // update fan
    updatePID();

    // API - update variables
    probeTemp1 = bbq.getProbeTemp(0);
    probeTemp2 = bbq.getProbeTemp(1);
    probeTemp3 = bbq.getProbeTemp(2);

    probeAlarm1 = bbq.getProbeAlarm(0);
    probeAlarm2 = bbq.getProbeAlarm(1);
    probeAlarm3 = bbq.getProbeAlarm(2);

    probeAlarmE1 = bbq.getProbeAlarmE(0);
    probeAlarmE2 = bbq.getProbeAlarmE(1);
    probeAlarmE3 = bbq.getProbeAlarmE(2);

    // delay for short period just makes things work nicely
    delay(1000);
}

void checkButtons() {
    if(buttonLeft.isPressed()) {
        bbq.down();
        alarmFirstMillis = millis();
        printAlarm();
    } else if(buttonCenter.isPressed()) {
        bbq.up();
        alarmFirstMillis = millis();
        printAlarm();
    } else if(buttonRight.isPressed()) {
        bbq.select();
        printTemp();
    }
}

void updateDisplay() {
    if(showAlarm()) {
        printAlarm();
    } else {
        if(cycleCheck(&ledLastMillis, LEDCYCLE))
        {
            printTemp();
        }
    }
}

void updatePID() {
    Setpoint = bbq.getProbeAlarm(0); // target temp
    Input = bbq.getProbeTemp(0); // current temp
    myPID.Compute();
    analogWrite(FAN1,Output);
}

void printTemp() {
    double temp = bbq.getActiveTemp();
    if(temp > 50.0) {
        //myLCD.print("");
    } else {
        //myLCD.print("Low Temp Error");
    }
}

void printAlarm() {
    double alarm = bbq.getActiveAlarm();
    //lcd.print(alarm);
    //lcd.print("");
}

boolean cycleCheck(unsigned long *lastMillis, unsigned int cycle) {
    unsigned long currentMillis = millis();

    if(currentMillis - *lastMillis >= cycle) {
        *lastMillis = currentMillis;
        return true;
    } else {
        return false;
    }
}

boolean showAlarm() {
    if(millis() - alarmFirstMillis >= ALARMSETDELAY) {
        return false;
    } else {
        return true;
    }
}

////////////////////////////////////////////////////
/////// Display functions

void printDisplayProbeMarker(){
  myDisplay.setCursor(0,bbq.getActiveProbe());
  myDisplay.print(">");
}


void printDisplayTemp(){
  myDisplay.setCursor(6,0);
  myDisplay.print(analogRead(A1));
  myDisplay.setCursor(6,1);
  myDisplay.print(analogRead(A5));
  myDisplay.setCursor(6,2);
  myDisplay.print(probeTemp3);
}

void printDisplayAlarm(){
  myDisplay.setCursor(6,0);
  myDisplay.print(probeAlarm1);
  myDisplay.setCursor(6,1);
  myDisplay.print(probeAlarm2);
  myDisplay.setCursor(6,2);
  myDisplay.print(probeAlarm3);
}

void printDisplayFan(){
  myDisplay.setCursor(6,3);
  myDisplay.print(Output);
  myDisplay.setCursor(14,3);
  myDisplay.print(Output);
}

void printThis(int cursorX, int cursorY, String myText){
  myDisplay.setCursor(cursorX, cursorY);
  myDisplay.print(myText);
}

void printDisplayLabels(){
    myDisplay.setCursor(1,0);
    myDisplay.print("Pit:");
    myDisplay.setCursor(1,1);
    myDisplay.print("M 1:");
    myDisplay.setCursor(1,2);
    myDisplay.print("M 2:");
    myDisplay.setCursor(1,3);
    myDisplay.print("Fan:");
    myDisplay.setCursor(12,0);
    myDisplay.print("/");
    myDisplay.setCursor(12,1);
    myDisplay.print("/");
    myDisplay.setCursor(12,2);
    myDisplay.print("/");
    myDisplay.setCursor(12,3);
    myDisplay.print("/");
    myDisplay.setCursor(17,3);
    myDisplay.print("%");
}

void printDisplay(){
printDisplayTemp();
printDisplayAlarm();
printDisplayFan();
}
