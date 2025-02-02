#include <Servo.h>
#include <LiquidCrystal.h>
#include <Melody.h>
#include <Timer.h>
#include "pitches.h"

using namespace std;
int songNo = 0;
struct Tone {
  int note;
  unsigned long duration;
};
const int SERVO = 3;
const int NEXT_BUTTON = 7;
const int CONF_BUTTON = 8;
const int SPEAKER = 2;
const int POT = A5;
unsigned long currentMillis = 0;
int toneIndex = 0;
unsigned long previousMillis = 0;


bool isPlaying = false;
bool refill = false;
bool timerOn = false;
unsigned long interval = 0;
int potVal = 0;
int servoState = 0; 
LiquidCrystal lcd(5, 6, 10, 11, 12, 13);
Servo myServo;
Timer timer;
void setup() {
  // put your setup code here, to run once:
  lcd.begin(16, 2);
  pinMode(NEXT_BUTTON, INPUT_PULLUP);
  pinMode(CONF_BUTTON, INPUT_PULLUP);
  pinMode(SPEAKER, OUTPUT);
  myServo.attach(SERVO);
  myServo.write(60);
  Serial.begin(9600);
}

void printTime(unsigned long milliseconds){
  unsigned long total_seconds = milliseconds / 1000;

  int hours = total_seconds / 3600;
  int minutes = (total_seconds % 3600) / 60;
  int seconds = total_seconds % 60;
  String output = (hours < 10 ? "0" + String(hours) : String(hours)) + ":" 
              + (minutes < 10 ? "0" + String(minutes) : String(minutes)) + ":" 
              + (seconds < 10 ? "0" + String(seconds) : String(seconds)) + "     ";
  if(!refill){
    lcd.setCursor(0, 0);
    lcd.print("Time Remaining   ");
    lcd.setCursor(0,1);
    lcd.print(output);
  }
  else{
    songNo=2;
    lcd.setCursor(0, 0);
    lcd.print("REQUIRES       ");
    lcd.setCursor(0,1);
    lcd.print("REFILL         ");
  }
}

int turnServo(){
  if(!refill){
    if(servoState == 0){
      myServo.write(120);
      songNo=1;
        servoState++;
    }
    else if(servoState == 1){
      myServo.write(0);
      songNo=1;
      refill = true;
      servoState++;
      delay(1000);
    }
  }
  else{
      servoState = 0;
      myServo.write(60);
  }

    if(buttonPress(CONF_BUTTON)){
      refill = false;
    }
  
    return servoState;
}

bool buttonPress(int BUTTON){
  if(digitalRead(BUTTON) == 0){
  songNo=3;
  }
  return digitalRead(BUTTON) == 0;
}

void menu(){
  static unsigned long timeRemaining = 0;
  static int menuMode = 0; //0 = Interval Selection 1 = Timer Display 2 = refill
  if(menuMode == 0){
    potVal = analogRead(POT);
    lcd.setCursor(0, 0);
    lcd.print("Select Interval      ");
    lcd.setCursor(0,1);
    if(potVal <= 30){
      lcd.print("10 Seconds      ");
      interval = 10000;
      lcd.setCursor(0,0);
      lcd.print("Select Interval      ");
      lcd.setCursor(0,1);
    }
    else if(potVal <= 128){
      lcd.print("15 Minutes      ");
      interval = 900000;
      lcd.setCursor(0,0);
      lcd.print("Select Interval      ");
      lcd.setCursor(0,1);
    }
    else if(potVal <= 128*2){
      lcd.print("30 Minutes      ");
      interval = 1.8 * pow(10, 6);
      lcd.setCursor(0,0);
      lcd.print("Select Interval     ");
      lcd.setCursor(0,1);
    }
    else if(potVal <= 128*3){
      lcd.print("1 Hour       ");
      interval = 3.6 * pow(10, 6);
      lcd.setCursor(0,0);
      lcd.print("Select Interval     ");
      lcd.setCursor(0,1);
    }
    else if(potVal <= 128*4){
      lcd.print("2 Hours     ");
      interval = 7.2 * pow(10, 6);
      lcd.setCursor(0,0);
      lcd.print("Select Interval     ");
      lcd.setCursor(0,1);
    }
    else if(potVal <= 128*5){
      lcd.print("4 Hours   ");
      interval = 1.44 * pow(10, 7);
      lcd.setCursor(0,0);
      lcd.print("Select Interval");
      lcd.setCursor(0,1);
    }
    else if(potVal <= 128*6){
      lcd.print("8 Hours     ");
      interval = 2.88 * pow(10, 7);
      lcd.setCursor(0,0);
      lcd.print("Select Interval");
      lcd.setCursor(0,1);
    }
    else if(potVal <= 128*7){
      lcd.print("12 Hours    ");
      interval = 4.32 * pow(10, 7);
      lcd.setCursor(0,0);
      lcd.print("Select Interval");
      lcd.setCursor(0,1);
    }
    else if(potVal <= 128*8){
      lcd.print("1 Day    ");
      interval = 8.64 * pow(10, 7);
      lcd.setCursor(0,0);
      lcd.print("Select Interval");
      lcd.setCursor(0,1);
    }
    if(buttonPress(CONF_BUTTON)){
      lcd.print("Selected!");
      //makeNoise(X); //confirmation noise
      timerOn = true;
      timer.start();
      timeRemaining = interval;
      menuMode++;
      delay(200);
    }
  }
  else if(menuMode == 1){
    
    if(timerOn){
      printTime((timeRemaining - timer.read()));
    }
    if(timeRemaining-timer.read() <= 50){
      timer.stop();
      timer.stop();
      timer.start();
      turnServo();
    }
    if(refill){
      turnServo();
    }
  }
  
}


Tone womp[] = {
   {NOTE_GS3, 500}, {NOTE_G3, 500}, {NOTE_F3, 750}, {NOTE_DS3, 250},
  {NOTE_GS3, 500}, {NOTE_G3, 500}, {NOTE_F3, 750}, {NOTE_DS3, 250},
  {NOTE_C3, 250}, {NOTE_D3, 250}, {NOTE_DS3, 250}, {NOTE_F3, 250},
  {NOTE_G3, 750}, {NOTE_F3, 250}, {NOTE_C3, 250}, {NOTE_D3, 250},
  {NOTE_DS3, 250}, {NOTE_F3, 250}, {NOTE_G3, 750}, {NOTE_F3, 250}
};
Tone ding[]= {
  {NOTE_GS4, 500}
};
Tone refilWarn[]={
  {NOTE_G3,500}, {NOTE_GS3, 500},
  {NOTE_G3,500}, {NOTE_GS3, 500},
  {NOTE_G3,500}, {NOTE_GS3, 500},
  {NOTE_G3,500}, {NOTE_GS3, 500},
  {NOTE_G3,500}, {NOTE_GS3, 500}
};
void play(Tone* tones, int size) {
  static int toneIndex = 0;  // Keep track of the current note
  static unsigned long previousMillis = 0;

  unsigned long currentMillis = millis();  // Update time

  if (!isPlaying) {  // Start playing sequence
    toneIndex = 0;  // Reset sequence
    isPlaying = true;
    previousMillis = currentMillis;
    tone(SPEAKER, tones[toneIndex].note, tones[toneIndex].duration);
  }

  if (isPlaying && (currentMillis - previousMillis >= tones[toneIndex].duration)) {
    noTone(SPEAKER);
    toneIndex++;

    if (toneIndex < size) {  // Check if there are more notes
      previousMillis = currentMillis;
      tone(SPEAKER, tones[toneIndex].note, tones[toneIndex].duration);
    } else {
      isPlaying = false;  // End of melody
      songNo = 0;
    }
  }
}
void makeNoise(int noise){
  Serial.println(songNo);
  if(noise == 1){
    play(womp,20);
  }
  if (noise ==2){ //alert
    unsigned long milli = millis();
    static unsigned long lastUpdate = milli;
    if (milli - lastUpdate >= 10000){
      play(refilWarn, 10);
    }
  }
  if (noise ==3){
    play(ding, 1);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  makeNoise(songNo);
  menu();
}
