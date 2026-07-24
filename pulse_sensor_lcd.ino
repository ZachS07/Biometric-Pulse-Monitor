#include <LiquidCrystal.h>

const int rs = 12;
const int en = 11;
const int d4 = 5;
const int d5 = 4;
const int d6 = 3;
const int d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


const int pulsePin = A0;


const int ledPin = 6;

// variables
int BPM = 0;                    // heart rate
int Signal;                     // analog input from sensor
int Threshold = 550;            // detecting a beat
boolean heartBeat = false;      // detecting a new beat

// timing
unsigned long lastBeatTime = 0;
unsigned long currentTime = 0;
int beatCount = 0;
unsigned long refreshTime = 0;

void setup() {
  Serial.begin(9600);           // debug
  
  
  lcd.begin(16, 2);
  
  
  pinMode(pulsePin, INPUT);
  pinMode(ledPin, OUTPUT);
  
  // startup msg
  lcd.print("Pulse Sensor");
  lcd.setCursor(0, 1);
  lcd.print("Ready...");
  delay(2000);
  lcd.clear();
  
  
  lcd.print("Heart Rate:");
  lcd.setCursor(0, 1);
  lcd.print("BPM: ");
}

void loop() {
  
  Signal = analogRead(pulsePin);
  currentTime = millis();
  
  // see if signal meets threshold
  if (Signal > Threshold && !heartBeat) {
    heartBeat = true;
    
    // bpm
    unsigned long beatInterval = currentTime - lastBeatTime;
    
    if (beatInterval > 300) {  
      lastBeatTime = currentTime;
      
      // bpm
      if (beatCount > 0) {
        BPM = (int)(60000 / beatInterval);
      }
      beatCount++;
      
      // Flash the LED
      digitalWrite(ledPin, HIGH);
      Serial.println(BPM);  // debug
    }
  }
  
  // turns led off
  if (heartBeat && (currentTime - lastBeatTime) > 150) {
    digitalWrite(ledPin, LOW);
    heartBeat = false;
  }
  
  // Update LCD display every 500ms
  if (currentTime - refreshTime > 500) {
    refreshTime = currentTime;
    
    // Display BPM on LCD
    lcd.setCursor(5, 1);
    
    if (BPM < 100) {
      lcd.print(" ");  // so it properly fits
    }
    lcd.print(BPM);
    lcd.print(" ");    
  }
  
  delay(10); 
}
