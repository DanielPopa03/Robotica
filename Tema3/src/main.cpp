#include <Arduino.h>
#include <LiquidCrystal.h>
#include <Servo.h>
#include <SPI.h>

// // initialize the library by associating any needed LCD interface pin
// // with the arduino pin number it is connected to
#define StartButton 3
#define ServoMotor 2
#define VRxPin A0

const int rs = 4, en = 5, d4 = 6, d5 = 7, d6 = 8, d7 = 9;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
Servo timerServo;

int player1Score = 0, player2Score = 0;
char receiveResult() {
  char response;
  digitalWrite(SS, LOW);
  response = SPI.transfer('-');
  digitalWrite(SS, HIGH);
  return response;
}

char sendCommand(char * command) {
  char response;
  bool firstExchange = true;
  int i = 0;
   do {
    digitalWrite(SS, LOW);
    response = SPI.transfer(*command);
    digitalWrite(SS, HIGH);
    if (firstExchange) {
    Serial.print(i);
    Serial.print("Response: ");
    Serial.println(response);
      if (response == '1') {
        lcd.setCursor(0, 1);
        lcd.print("Player 1!");
        player1Score++;
      } 
      if (response == '2') {
        lcd.setCursor(0, 1);
        lcd.print("Player 2!");
        player2Score++;
      }
      firstExchange = false;
    }
    command++;
    delay(50);
  } while (*command);
  digitalWrite(SS, LOW);
  response = SPI.transfer('\0');
  digitalWrite(SS, HIGH);
  delay(50);
  return response;
}

char commandSend[10];
void sendColorCommand(bool lastTurn = false) {
  int color1 = random(0, 3), color2 = random(0,3);
  color1 == 0 ? commandSend[0] = 'r' : color1 == 1 ? commandSend[0] = 'b' : commandSend[0] = 'g';
  color2 == 0 ? commandSend[1] = 'r' : color2 == 1 ? commandSend[1] = 'b' : commandSend[1] = 'g';
  commandSend[2] = '\0';
  if (lastTurn) {
    commandSend[2] = 'l';
    commandSend[3] = '\0';
  }
  sendCommand(commandSend);
}

bool gameStarted = false;
long timerStartGame = 0;
long gameStartTime = 0;
int counter = 0;
long timeBetweenTurns = 0;
long gameDuration = 31000;
long timeBetweenTurnsDuration = 10000;
bool firstTurn = false;
bool showTurn = false;
bool sendTurnCommand = false;
bool waitForDisplay = false;

void startGame() {
    if (timerStartGame == 0 && !gameStarted) {
      timerStartGame = millis();
    }
    if (millis() - timerStartGame >= 3000) {
      player1Score = player2Score =  counter = 0;
      firstTurn = true;
      showTurn = sendTurnCommand = waitForDisplay = false;
      timerStartGame = millis();
      Serial.print("Game started at:");
      Serial.println(millis() / 1000);
      lcd.setCursor(0, 0);
      gameStartTime = millis();
      lcd.clear();
      lcd.print("The Game started!");
      gameStarted = true;
    }
}



void setup() {
  // set up the LCD's number of columns and rows:
  randomSeed(analogRead(A0));
  pinMode(StartButton, INPUT_PULLUP);
  Serial.begin(9600);

  //spi
  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV8);
  pinMode(SS, OUTPUT);
  digitalWrite(SS, HIGH);

  timerServo.attach(ServoMotor);
  timerServo.write(0);
  lcd.clear();
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("Press to Start!");

}

int turnNumber = 0;
long timerAnimation = 0;

void loop() {
  if (digitalRead(StartButton) == LOW) {
    startGame();
  }
  long currentTime = 0;
  if (gameStarted) {
    currentTime = millis();
    if (firstTurn) {
      turnNumber = gameDuration / timeBetweenTurnsDuration;
      sendTurnCommand = true;
      firstTurn = false;
      timeBetweenTurns = millis();
    }
    ///////////
    if (waitForDisplay) {
      if (currentTime - timerAnimation >= 1000) {
        if (player1Score > player2Score) {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Player 1 wins!");
        } else if (player2Score > player1Score) {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Player 2 wins!");
        } else {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Draw!");
        }
      }
      
      if (currentTime - timerAnimation >= 6000) {
        gameStarted = false;
        timerServo.write(0);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Press to Start!");
      }
      return;
    }
    if (currentTime - timeBetweenTurns >= 1000 && showTurn) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Turn ");
      lcd.print(counter);
      lcd.print(":");
      showTurn = false;
    }
    ///////
    if (currentTime - timeBetweenTurns >= timeBetweenTurnsDuration) {
      sendTurnCommand = true;
      timeBetweenTurns = millis();
    }
    if (sendTurnCommand) {
      showTurn = true;
      if (counter == turnNumber) {
        Serial.println("Last turn");
        sendColorCommand(true);
        counter = 0;
      } else  {
        sendColorCommand();
        counter++;
      }
      sendTurnCommand = false;
    }
    if (currentTime - gameStartTime >= gameDuration) {
      waitForDisplay = true;
      timerAnimation = millis();
    }
    int angle = map(millis() - gameStartTime, 0, gameDuration, 45, 135);
    timerServo.write(angle);

  }
}

// //Slave

#define Player1ButtonRed 4
#define Player1ButtonGreen 5 
#define Player1ButtonBlue 6
#define Player2ButtonRed A2
#define Player2ButtonGreen A1
#define Player2ButtonBlue A0

#define RedLightPlayer1 9
#define GreenLightPlayer1 8
#define BlueLightPlayer1 7
#define RedLightPlayer2 A3
#define GreenLightPlayer2 A5
#define BlueLightPlayer2 A4

bool expectsResponse = false;
char p1Color = '-', p2Color = '-';
char  result = '-';



volatile char command[10];
volatile int sizeBuffer = 0;
bool sendCommand = false;

void setup() {
  Serial.begin(9600);          // For debugging
  SPCR |= bit(SPE);               // Enable SPI in slave mode
  pinMode(MISO, OUTPUT);          // Set MISO as output for SPI
  SPI.attachInterrupt();   

  // Initialize button pins as input with pull-up resistors
  pinMode(Player1ButtonRed, INPUT_PULLUP);
  pinMode(Player1ButtonGreen, INPUT_PULLUP);
  pinMode(Player1ButtonBlue, INPUT_PULLUP);
  pinMode(Player2ButtonRed, INPUT_PULLUP);
  pinMode(Player2ButtonGreen, INPUT_PULLUP);
  pinMode(Player2ButtonBlue, INPUT_PULLUP);


  //Attach interrupt to the SPI interrupt
  // 
  // Enable Pin Change Interrupt for Port D (pins 4 to 7)
PCICR |= (1 << PCIE2); // Enable Pin Change Interrupts for PCINT16-23 (Port D)
PCMSK2 |= (1 << PCINT20) | (1 << PCINT21) | (1 << PCINT22); // Enable PCINT20-22 for pins 4, 5, 6

  // Enable Pin Change Interrupt for Port C (A0 to A5)
  PCICR |= (1 << PCIE1); // Enable PCINT1 interrupt group
  PCMSK1 |= (1 << PCINT8) | (1 << PCINT9) | (1 << PCINT10); 

  // Initialize LED pins as outputs
  pinMode(RedLightPlayer1, OUTPUT);
  pinMode(GreenLightPlayer1, OUTPUT);
  pinMode(BlueLightPlayer1, OUTPUT);
  pinMode(RedLightPlayer2, OUTPUT);
  pinMode(GreenLightPlayer2, OUTPUT);
  pinMode(BlueLightPlayer2, OUTPUT);
  SPDR = '-';
  Serial.println("Received: ");
}
long timePlayer1 = 0;
ISR(PCINT2_vect) {
  char color = '-';
  if (digitalRead(Player1ButtonRed) == LOW) {
    color = 'r';
  } else if (digitalRead(Player1ButtonGreen) == LOW) {
    color = 'g';
  } else if (digitalRead(Player1ButtonBlue) == LOW) {
    color = 'b';
  } 

  Serial.print(result);
  Serial.println(" Player 1 pressed button ");
  timePlayer1 = millis();
  
  bool canCheck = false;
  if (expectsResponse) {
    if (result == '-') {
      canCheck = true;
    }
    
    if (canCheck) {
      
      if (color == p1Color) {
        expectsResponse = false;
        Serial.println(color);
        result = '1';
      }
    }
    SPDR = result;
  }

}


ISR(PCINT1_vect) {
  char color = '-';
  // Check the state of each button
  if (digitalRead(Player2ButtonRed) == LOW) {
    color = 'r';
  } else if (digitalRead(Player2ButtonGreen) == LOW) {
    color = 'g';
  } else if (digitalRead(Player2ButtonBlue) == LOW) {
    color = 'b';
  }
  
  bool canCheck = false;
  if (expectsResponse) {
    Serial.print(color);
    Serial.println(" Player 2 pressed button ");
    if (result == '-') {
      canCheck = true;
    } 
    Serial.print(p2Color);
    if (canCheck) {
      if (color == p2Color) {
        expectsResponse = false;
        Serial.print(" it got there:");
        Serial.println(color);
        result = '2';
      }
    }
    SPDR = result;
  }
  
  
}

ISR(SPI_STC_vect) {
  char receivedChar = SPDR;
  Serial.println("Received: ");
  Serial.println(receivedChar);
  if (receivedChar != '\0' && sizeBuffer < 9) {
    command[sizeBuffer] = receivedChar;
    sizeBuffer++;
  } else {
    command[sizeBuffer] = '\0';
    sizeBuffer = 0;
    sendCommand = true;
    result = '-';
  }
}

void resetLeds() {
  digitalWrite(RedLightPlayer1, LOW);
  digitalWrite(GreenLightPlayer1, LOW);
  digitalWrite(BlueLightPlayer1, LOW);
  digitalWrite(RedLightPlayer2, LOW);
  digitalWrite(GreenLightPlayer2, LOW);
  digitalWrite(BlueLightPlayer2, LOW);
}

void activateLeds(char colorPlayer1, char colorPlayer2) {
  resetLeds();
  switch (colorPlayer1) {
  case 'r':
    digitalWrite(RedLightPlayer1, HIGH);
    break;
  case 'g':
    digitalWrite(GreenLightPlayer1, HIGH);
    break;
  case 'b':
    digitalWrite(BlueLightPlayer1, HIGH);
    break;
  default:
    Serial.print("INVALID COLOR WAS RECEIVED!!!!!!!");
    break;
  }
  switch (colorPlayer2) {
  case 'r':
    digitalWrite(RedLightPlayer2, HIGH);
    break;
  case 'g':
    digitalWrite(GreenLightPlayer2, HIGH);
    break;
  case 'b':
   digitalWrite(BlueLightPlayer2, HIGH);
    break;
  default:
    Serial.print("INVALID COLOR WAS RECEIVED!!!!!!!");
    break;
  }

}



char commandReceived[10];
long timer = 0; bool checkTimer = false;
void loop() {
  if (checkTimer) {
    if (millis() - timer >= 10000) {
      resetLeds();
      checkTimer = false;
    }
  }
 
  if (sendCommand) {
    SPDR = '-';
    result = '-';
    p1Color = command[0];
    p2Color = command[1];
    activateLeds(p1Color, p2Color);
    Serial.print(p1Color);
    Serial.print(p2Color);
    Serial.print(command[2]);
     Serial.print(' ');
     Serial.println(result);
    sendCommand = false;
    if (command[2] != 'l') {
    expectsResponse = true;
    } else {
      SPDR = '-';
      expectsResponse = false;
      timer = millis();
      checkTimer = true;
  }

 }
}