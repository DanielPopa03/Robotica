#include <Arduino.h>

// put function declarations here:
int myFunction(int, int);

uint8_t state = 0;

bool buttonStartPressed = false;
bool charging = false;
bool buttonStopPressed = false;

long startTimer = 0;
long stopTimer = 0;
long elapsed = 0;

void setup() {
  pinMode(10, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(7, OUTPUT);
  // 6 - ROSU  5 - VERDE
  pinMode(6, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(4, OUTPUT);
  digitalWrite(10, LOW);
  digitalWrite(9, LOW);
  digitalWrite(8, LOW);
  digitalWrite(7, LOW);
  digitalWrite(6, LOW);
  digitalWrite(5, HIGH);
  digitalWrite(4, LOW);
  
  pinMode(3, INPUT);
  pinMode(2, INPUT);
  int result = myFunction(2, 3);
}

void loop() {
  
  if(digitalRead(3) == LOW && charging == false && buttonStartPressed == false) {
    buttonStartPressed = true;
    startTimer = millis();
  } 

  if(buttonStartPressed == true && digitalRead(3) == HIGH) {
    elapsed = millis() - startTimer;
    if(elapsed >= 500) {
      digitalWrite(6, HIGH);
      digitalWrite(5, LOW);
      charging = true;
      buttonStartPressed = false;
      startTimer = millis();
    }
  }

  if(charging == true) {
    elapsed = millis() - startTimer;
    if(state > 3 && elapsed >= 1000) {
      if(state == 10) {
        charging = false;
        digitalWrite(6, LOW);
        digitalWrite(5, HIGH);
        digitalWrite(10, LOW);
        digitalWrite(9, LOW);
        digitalWrite(8, LOW);
        digitalWrite(7, LOW);
        state = 0;
      } else {
        if(state % 2 == 0) {
          digitalWrite(10, LOW);
          digitalWrite(9, LOW);
          digitalWrite(8, LOW);
          digitalWrite(7, LOW);
        } else {
          digitalWrite(10, HIGH);
          digitalWrite(9, HIGH);
          digitalWrite(8, HIGH);
          digitalWrite(7, HIGH);
        }
        startTimer = millis();
        state ++;
      }
    }
    if(elapsed >= 3000) {
      switch (state)
      {
      case 0:
        digitalWrite(10, HIGH);
        break;
      case 1:
        digitalWrite(9, HIGH);
        break;
      case 2:
        digitalWrite(8, HIGH);
        break;
      case 3:
        digitalWrite(7, HIGH);
        break;
      default:
        break;
      }
      startTimer = millis();
      state++;
    }
  }

  if(charging == true && digitalRead(2) == LOW && buttonStopPressed == false) {
    buttonStopPressed = true;
    stopTimer = millis();
  }

  if(digitalRead(2) == LOW) {
    if(buttonStopPressed == true && millis() - stopTimer >= 2000) {
      digitalWrite(10, LOW);
      digitalWrite(9, LOW);
      digitalWrite(8, LOW);
      digitalWrite(7, LOW);
      digitalWrite(5, LOW);
      charging = false;
      digitalWrite(6, LOW);
      digitalWrite(5, HIGH);
      buttonStopPressed = false;
      buttonStartPressed = false;
      state = 0;
    }
  } else {
    if(buttonStopPressed == true) {
      buttonStopPressed = false;
    }
  }
  
  delay(50);
}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}