/*
Verify code after upload OFF
Show verbose output on
Push reset for re-run
 
1023 
    off 1022
1015 
    1 LED on 963 (972 990)
945
    2 LEDs on 915 (939)
*/

const bool simulateError = 0;
const bool printADC = 0;

const bool wait_on_serial_monitor = 1; 
const int pinDelay_ms = 40;
const byte adcPin = d[0];
const int adcMin=945, adcMax=1015; 

#include <Streaming.h>
#include <avdweb_HeartBeat.h> 
#include "avdweb_AnalogReadFast.h"

unsigned inline adcMean(const byte adcPin, const int samples) 
{ unsigned long adcVal = 0;
  for(int i=0; i<samples; i++) adcVal += analogReadFast(adcPin);
  adcVal /= samples;
  return adcVal;
}

void setAllLedsOff()
{ for(int i=1; i<pinCount; i++) pinMode(d[i], INPUT);
}

void runningLight()
{ int errorCnt = 0;
  setAllLedsOff();
  for(int i=1; i<pinCount; i++) 
  { int adcOff = adcMean(adcPin, 100); // Check LEDs off
    pinMode(d[i], OUTPUT);
    digitalWrite(d[i], 0); // put Led on
    if(simulateError)
    { if(i==2) pinMode(d[i], INPUT); // Led off instead of on   
      if(i==5) 
      { pinMode(d[4], OUTPUT); // Led 4 and 5 on
        digitalWrite(d[4], 0); // Led on
      }
    }   
    int adcOn = adcMean(adcPin, 100); // Check LEDs on
    delay(pinDelay_ms);
    setAllLedsOff();
    if(printADC) Serial << "\nd" << i << " adcOn " << adcOn << " adcOff " << adcOff;
    if(adcOn < adcMin || adcOn > adcMax) 
    { Serial << "\n" << "Error on d" << i << ", adcOn = " << adcOn;
      errorCnt++;
    }
    if(adcOff < adcMax) 
    { Serial << "\n" << "Error on d" << i << ", adcOff = " << adcOff;
      errorCnt++;
    }
  }
  if(errorCnt==0) Serial << "\nPCB OK ";
  else Serial << "\nPCB NOT OK ";
  static bool b; b = !b; Serial.print(b? "/" : "\\");
}

void setup() 
{ Serial.begin(9600);
  if(wait_on_serial_monitor) while(!Serial); 
  runningLight(); 
}

void loop()
{ if(!printADC) runningLight();
}







