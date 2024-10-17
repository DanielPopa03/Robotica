#include <Arduino.h>

uint8_t state = 0;
uint8_t flickeringMode = 0;

bool buttonStartPressed = false;
bool charging = false;
bool buttonStopPressed = false;

long startTimer = 0;
long flickeringModeTimer = 0;
long stopTimer = 0;
long elapsed = 0;

void setup() {
  pinMode(10, OUTPUT);//Led 25%
  pinMode(9, OUTPUT);//Led 50%
  pinMode(8, OUTPUT);//Led 75%
  pinMode(7, OUTPUT);//Led 100%
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
}

void loop() {
  //Incepe incarcarea
  if(digitalRead(3) == LOW && charging == false && buttonStartPressed == false) {
    buttonStartPressed = true;
    startTimer = millis();
  } 

  //Verifica ca este o apasare scurta
  if(buttonStartPressed == true && digitalRead(3) == HIGH) {
    elapsed = millis() - startTimer;
    if(elapsed >= 500) {
      digitalWrite(6, HIGH);
      digitalWrite(5, LOW);
      charging = true;
      buttonStartPressed = false;
      startTimer = flickeringModeTimer = millis();
    }
  }

  //In timpul incarcarii
  if(charging == true) {
    //Aici face ca ledurile sa clipeasca inainte sa fie aprinse
    elapsed = millis() - flickeringModeTimer;
    if(elapsed >= 300 && state < 4) {
      flickeringModeTimer = millis();
      if(flickeringMode == 0) {
        digitalWrite(10 - state, HIGH);
        flickeringMode = 1;
      } else {
        digitalWrite(10 - state, LOW);
        flickeringMode = 0;
      }
    }

    //Trei sclipeiri pt toate ledurile atunci cand incarcarea este completa
    elapsed = millis() - startTimer;
    if(state > 3 && elapsed >= 1000) {
      if(state == 10) { //S-a terminat incarcarea, stingem becurile si facem ledul verde sa se aprinda
        charging = false;
        digitalWrite(6, LOW);
        digitalWrite(5, HIGH);
        digitalWrite(10, LOW);
        digitalWrite(9, LOW);
        digitalWrite(8, LOW);
        digitalWrite(7, LOW);
        state = 0;
      } else { //Sclipeste de 3 ori, cand e par se aprind, cand e impar se sting
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
    if(elapsed >= 3000) { //O data la 3 secunde se incarca 25% din baterie
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

  if(charging == true && digitalRead(2) == LOW && buttonStopPressed == false) {//Activeaza butonul de stop
    buttonStopPressed = true;
    stopTimer = millis();
  }

  if(digitalRead(2) == LOW) {
    if(buttonStopPressed == true && millis() - stopTimer >= 2000) { //La apasarea lunga a butonului se opreste incarcarea
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
    if(buttonStopPressed == true) { //Daca  nu este apasat lung se opreste butonul de stop
      buttonStopPressed = false;
    }
  }
  
  delay(50);
}
