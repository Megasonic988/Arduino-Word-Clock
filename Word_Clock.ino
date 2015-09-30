#include <Wire.h>
#include "RTClib.h"
RTC_DS1307 RTC;

int latchPin = 8;
int clockPin = 12;
int dataPin = 11;
int OEPin = 10;

struct ShiftRegTime {
  byte SR1;
  byte SR2;
  byte SR3;
  byte SR4;
};

struct ShiftRegTime calcTime(DateTime time);
void printTime(DateTime now);
void shiftOutTime(ShiftRegTime time);

void setup() {
  //set pins to output so you can control the shift register
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(OEPin, OUTPUT);
  //set minute and AM/PM pins to output for direct LED control
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(11, OUTPUT);
  
  analogWrite(OEPin, 0);
  Serial.begin(57600);
  Wire.begin();
  RTC.begin();

    // following line sets the RTC to the date & time this sketch was compiled
    RTC.adjust(DateTime(__DATE__, __TIME__));

  
}

void loop() {
  DateTime now = RTC.now();
  ShiftRegTime time = calcTime(now);
  printTime(now);
  shiftOutTime(time);
  lightPeripheralLEDs(now);
  delay(1000);
}

void lightPeripheralLEDs(DateTime time) {
  if (time.minute()%5 == 1) {
    digitalWrite(4, HIGH);
    digitalWrite(5, LOW);
    digitalWrite(6, LOW);
    digitalWrite(7, LOW);
  } else if (time.minute()%5 == 2) {
    digitalWrite(4, HIGH);
    digitalWrite(5, HIGH);
    digitalWrite(6, LOW);
    digitalWrite(7, LOW);
  } else if (time.minute()%5 == 3) {
    digitalWrite(4, HIGH);
    digitalWrite(5, HIGH);
    digitalWrite(6, HIGH);
    digitalWrite(7, LOW);
  } else if (time.minute()%5 == 4) {
    digitalWrite(4, HIGH);
    digitalWrite(5, HIGH);
    digitalWrite(6, HIGH);
    digitalWrite(7, HIGH);
  } else if (time.minute()%5 == 0) {
    digitalWrite(4, LOW);
    digitalWrite(5, LOW);
    digitalWrite(6, LOW);
    digitalWrite(7, LOW);
  }
  if (time.hour() >= 12) {
    digitalWrite(10, HIGH);
    digitalWrite(3, LOW);
  } else {
    digitalWrite(10, LOW);
    digitalWrite(3, HIGH);
  }
}

struct ShiftRegTime calcTime(DateTime time) {
  int second = time.second();
  int minute = time.minute();
  int hour = time.hour()%12;
  int day = time.day();
  int month = time.month();
  Serial.print(hour, DEC);
  ShiftRegTime SRtime = {0,0,0,0};
  SRtime.SR1 = SRtime.SR1 | 128;
  if (minute < 35) {
    if (minute >= 0 && minute < 5) {
      SRtime.SR4 = SRtime.SR4 | 16; //oclock
    } else {
      SRtime.SR2 = SRtime.SR2 | 64; //past
    }
    switch (hour) {
      case 0:
        SRtime.SR2 = SRtime.SR2 | 4;
        break;
      case 1:
        SRtime.SR1 = SRtime.SR1 | 2;
        break;
      case 2:
        SRtime.SR3 = SRtime.SR3 | 16;
        break;
      case 3:
        SRtime.SR2 = SRtime.SR2 | 16;
        break;
      case 4:
        SRtime.SR3 = SRtime.SR3 | 8;
        break;
      case 5:
        SRtime.SR2 = SRtime.SR2 | 8;
        break;
      case 6:
        SRtime.SR2 = SRtime.SR2 | 128;
        break;
      case 7:
        SRtime.SR2 = SRtime.SR2 | 2;
        break;
      case 8:
        SRtime.SR3 = SRtime.SR3 | 64;
        break;
      case 9:
        SRtime.SR3 = SRtime.SR3 | 4;
        break;
      case 10:
        SRtime.SR3 = SRtime.SR3 | 32;
        break;
      case 11:
        SRtime.SR3 = SRtime.SR3 | 2;
        break;
    }
    if (minute >= 5 && minute < 10) {
      SRtime.SR1 = SRtime.SR1 | 16;
    } else if (minute >= 10 && minute < 15) {
      SRtime.SR1 = SRtime.SR1 | 32;
    } else if (minute >= 15 && minute < 20) {
      SRtime.SR1 = SRtime.SR1 | 4;
    } else if (minute >= 20 && minute < 25) {
      SRtime.SR3 = SRtime.SR3 | 128;
    } else if (minute >= 25 && minute < 30) {
      SRtime.SR3 = SRtime.SR3 | 128;
      SRtime.SR1 = SRtime.SR1 | 16;
    } else if (minute >= 30 && minute < 35) {
      SRtime.SR1 = SRtime.SR1 | 64;
    }
  } else if (minute >= 35) {
    SRtime.SR2 = SRtime.SR2 | 32; //to
    switch (hour) {
      case 0:
        SRtime.SR1 = SRtime.SR1 | 2;
        break;
      case 1:
        SRtime.SR3 = SRtime.SR3 | 16;
        break;
      case 2:
        SRtime.SR2 = SRtime.SR2 | 16;
        break;
      case 3:
        SRtime.SR3 = SRtime.SR3 | 8;
        break;
      case 4:
        SRtime.SR2 = SRtime.SR2 | 8;
        break;
      case 5:
        SRtime.SR2 = SRtime.SR2 | 128;
        break;
      case 6:
        SRtime.SR2 = SRtime.SR2 | 2;
        break;
      case 7:
        SRtime.SR3 = SRtime.SR3 | 64;
        break;
      case 8:
        SRtime.SR3 = SRtime.SR3 | 4;
        break;
      case 9:
        SRtime.SR3 = SRtime.SR3 | 32;
        break;
      case 10:
        SRtime.SR3 = SRtime.SR3 | 2;
        break;
      case 11:
        SRtime.SR2 = SRtime.SR2 | 4;
        break;
    }
    if (minute >= 35 && minute < 40) {
      SRtime.SR3 = SRtime.SR3 | 128;
      SRtime.SR1 = SRtime.SR1 | 16;
    } else if (minute >= 40 && minute < 45) {
      SRtime.SR3 = SRtime.SR3 | 128;
    } else if (minute >= 45 && minute < 50) {
      SRtime.SR1 = SRtime.SR1 | 4;
    } else if (minute >= 50 && minute < 55) {
      SRtime.SR1 = SRtime.SR1 | 32;
    } else if (minute >= 55 && minute < 60) {
      SRtime.SR1 = SRtime.SR1 | 16;
    }
  }
  if (month == 11 && day == 8) {
    SRtime.SR4 = SRtime.SR4 | 128 | 8;
  }
  if (month == 12 && day >= 24) {
    SRtime.SR4 = SRtime.SR4 | 128 | 64;
  }
  if (hour == 11 && minute == 11) {
    SRtime.SR4 = SRtime.SR4 | 4;
  }
  return SRtime;
}

void printTime(DateTime now)
{
  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(' ');
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();
}

void shiftOutTime(ShiftRegTime time)
{
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, LSBFIRST, time.SR4);
  shiftOut(dataPin, clockPin, LSBFIRST, time.SR3);
  shiftOut(dataPin, clockPin, LSBFIRST, time.SR2);
  shiftOut(dataPin, clockPin, LSBFIRST, time.SR1);
  digitalWrite(latchPin, HIGH);
}
