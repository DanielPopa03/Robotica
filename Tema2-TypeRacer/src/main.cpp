#include <Arduino.h>
//#include <Usart.h>

#define BUFFER_SIZE 50 // Define maximum buffer size
#define redRgb 6
#define greenRgb 5
#define blueRgb 4
#define buttonSwitchDifficulty 3
#define buttonStartStop 2
#define MATRIX_SIZE 10
#define maximumNumberOfWordPerGame  10 //is 10 because at the hard difficulty the time is 3000 and there are 30s

const char* wordMatrix[MATRIX_SIZE][MATRIX_SIZE] = {
    {"apple", "banana", "cherry", "date", "elderberry", "fig", "grape", "honeydew", "kiwi", "lemon"},
    {"mango", "nectarine", "orange", "papaya", "quince", "raspberry", "strawberry", "tangerine", "ugli", "vanilla"},
    {"watermelon", "xigua", "yuzu", "zucchini", "avocado", "blueberry", "cantaloupe", "durian", "eggplant", "fig"},
    {"guava", "honeyberry", "indian fig", "jackfruit", "kumquat", "lime", "mulberry", "nectarine", "olive", "pear"},
    {"peach", "plum", "pomegranate", "quince", "raspberry", "strawberry", "tamarind", "tomato", "ugli", "vanilla"},
    {"watermelon", "xigua", "yuzu", "zucchini", "apricot", "blackberry", "cantaloupe", "dragonfruit", "elderberry", "fig"},
    {"grapefruit", "honeydew", "ice plant", "jabuticaba", "kiwano", "lemon", "mango", "nectarine", "orange", "plum"},
    {"quince", "raspberry", "salak", "tamarillo", "ugli", "voavanga", "watermelon", "ximenia", "yellow passionfruit", "zucchini"},
    {"apple", "banana", "cherry", "date", "elderberry", "fig", "grape", "honeydew", "kiwi", "lemon"},
    {"mango", "nectarine", "orange", "papaya", "quince", "raspberry", "strawberry", "tangerine", "ugli", "vanilla"}
};

int myFunction(int, int);
void switchDifficulty();
void startStop();
void typeRacer();
bool hasUserSendInput();
void chooseTheWordForTheGame();

void setup() {
  // Initialize the random number generator with a random seed
  randomSeed(analogRead(0)); // Use analog input noise to randomize seed
  //USART0_init(MYUBRR);
  Serial.begin(9600);
  pinMode(redRgb, OUTPUT);
  pinMode(greenRgb, OUTPUT);
  pinMode(blueRgb, OUTPUT);
  digitalWrite(redRgb, LOW);
  digitalWrite(greenRgb, LOW);
  digitalWrite(blueRgb, HIGH);
  attachInterrupt(digitalPinToInterrupt(buttonSwitchDifficulty), switchDifficulty, CHANGE);
  attachInterrupt(digitalPinToInterrupt(buttonStartStop), startStop, CHANGE);
}

int difficulty = -1;
bool debouncingSelectDifficulty = false;
long timer;

int i = 0;
long timeAllocatedForTheGame = 30000;
long aux = 0;
int counterWords = 0;
int numberOfCorrectWords = 0;

char buffer[BUFFER_SIZE];
int bufferIndex = 0;  

int numberOfWords = 0;
const char * theWordsUsedInTheTypeRacer[maximumNumberOfWordPerGame];
bool startTheGame = false;

void switchDifficulty() {
  
  digitalWrite(greenRgb, LOW);
  if(debouncingSelectDifficulty == false) {
    timer = millis();
    debouncingSelectDifficulty = true;
  } else if(!startTheGame) {
    long elapsed = millis() - timer;
    if(elapsed >= 1000) {
      difficulty += 1;
      difficulty %= 3;
      switch (difficulty) {
      case 0:
        Serial.println("Easy mode selected!");
        break;
      case 1:
        Serial.println("Medium mode selected!");
        break;
      case 2:
        Serial.println("Hard mode selected!");
        break;
      default:
        Serial.println("Error occured !!");
        break;
      }
      debouncingSelectDifficulty = false;
    }
  }

}


bool debouncingStartStop = false;
bool blickAnimation = false;
long timerOfTheGame;
long timeToWait;
void startStop() {
  if(debouncingStartStop == false) {
    timer = millis();
    debouncingStartStop = true;
  } else {
    long elapsed = millis() - timer;
    if(elapsed >= 1000) {
      switch(difficulty) {
        case -1:
          Serial.println("Please select a difficulty!");
          debouncingStartStop = false;
          return;
          break;
        case 0:
          timeToWait = 6000;
          break;
        case 1:
          timeToWait = 5000;
          break;
        case 2:
          timeToWait = 3000;
          break;
        default:
          break;
      }
      debouncingStartStop = false;
      startTheGame ^= 1;
      if(startTheGame) {
        // Serial.println("A inceput\n");
        i = 0;
        blickAnimation = true;
        timerOfTheGame = millis();
      } else {
        startTheGame = false;
        counterWords = 0; numberOfCorrectWords = 0; i = 0; bufferIndex = 0;
        buffer[bufferIndex] = '\0';
        timerOfTheGame = millis();
        digitalWrite(redRgb, LOW);
        digitalWrite(greenRgb, LOW);
        digitalWrite(blueRgb, HIGH);
      }
    }
    
  }
}

bool hasUserSendInput() { //return true if user has send his submission, otherwise it return false
  if (Serial.available() > 0) { // Check if data is available to read
    char incomingByte = Serial.read(); // Read the next byte (character)

    // Handle Enter (ASCII 13 or '\n')
    if (incomingByte == '\n' || incomingByte == '\r') {
      if(buffer[0] == '\0') return false;
      return  true;
    }
    // Handle Backspace (ASCII 8)
    else if (incomingByte == 8) {
      if (bufferIndex > 0) {
        bufferIndex--;             // Move back in the buffer
        buffer[bufferIndex] = '\0'; // Remove the last character
        Serial.print("\b \b");      // Optional: update serial monitor output
      }
    }
    // Add regular characters to the buffer
    else if (bufferIndex < BUFFER_SIZE - 1) {
      buffer[bufferIndex] = incomingByte;
      bufferIndex++;
      buffer[bufferIndex] = '\0';   // Null-terminate the string
      Serial.print(incomingByte);   // Echo the character
    }
  }
  return false;
}


void chooseTheWordForTheGame() {
  randomSeed(millis());
  numberOfWords = 30000 / timeToWait;
  if(numberOfWords > maximumNumberOfWordPerGame) {
    Serial.println("errror to many word!!");
  }
  for(int i = 0; i < numberOfWords; i++) {
    theWordsUsedInTheTypeRacer[i] = wordMatrix[i][random(numberOfWords)];
  }
  for(int i = 0; i < numberOfWords; i++) {
    Serial.println(theWordsUsedInTheTypeRacer[i]);
  }
  Serial.println("-------------------------------------");
} 

int areStringsIdentical(const char *str1, const char *str2) {
    int i = 0;

    // Check each character of both strings
    while (str1[i] != '\0' && str2[i] != '\0') {
        if (str1[i] != str2[i]) {
            return 0; // Strings are not identical
        }
        i++;
    }

    // Check if both strings ended at the same length
    if (str1[i] == '\0' && str2[i] == '\0') {
        return 1; // Strings are identical
    } else {
        return 0; // Strings are not identical
    }
}

void typeRacer() {
  if(startTheGame) {
    if(blickAnimation) {
      if(i == 0) {
        digitalWrite(redRgb, LOW);
        digitalWrite(greenRgb, LOW);
        digitalWrite(blueRgb, LOW);
        i ++;
        Serial.println(3);
      }
      if(millis() - timerOfTheGame >= 500) {
        aux +=  millis() - timerOfTheGame ;
        digitalWrite(redRgb, LOW);
        digitalWrite(greenRgb, LOW);
        i % 2 == 0 ? digitalWrite(blueRgb, LOW) : digitalWrite(blueRgb, HIGH);
        if(i == 3) Serial.println(2);
        if(i == 5) Serial.println(1);
        timerOfTheGame = millis();
        i++;
      }
      //Asteapta pt 3000 de secunde
      if(i >= 7) {
        i = 0;
        blickAnimation = false; 
        Serial.println(aux);
         chooseTheWordForTheGame();
         timerOfTheGame = millis();
        digitalWrite(redRgb, LOW);
        digitalWrite(greenRgb, HIGH);
        digitalWrite(blueRgb, LOW);
      }
      return;
    }
    bool readyToCompareWord = hasUserSendInput();
    //readyToCompareWord ? Serial.println("true") : i = 0;
    if(counterWords <= numberOfWords) {
      if(counterWords == 0) {
        Serial.println(theWordsUsedInTheTypeRacer[0]);
        counterWords++;
      }
      if(readyToCompareWord) {
        if(areStringsIdentical(buffer, theWordsUsedInTheTypeRacer[counterWords-1])) {
          digitalWrite(redRgb, LOW);
          digitalWrite(greenRgb, HIGH);
          digitalWrite(blueRgb, LOW);
          numberOfCorrectWords += 1;
          counterWords ++;
          if(counterWords <= numberOfWords){
            Serial.println("\n-------");Serial.println(theWordsUsedInTheTypeRacer[counterWords-1]);
            digitalWrite(redRgb, LOW);
            digitalWrite(greenRgb, HIGH);
            digitalWrite(blueRgb, LOW);
          } 
          timerOfTheGame = millis();
        } else {
          digitalWrite(redRgb, HIGH);
          digitalWrite(greenRgb, LOW);
          digitalWrite(blueRgb, LOW);
          Serial.println("!--");
        } 
        bufferIndex = 0;
        buffer[bufferIndex] = '\0'; // Reset the first element to end the string
      }
      if(millis() - timerOfTheGame >= timeToWait) {
        counterWords ++;
        if(counterWords <= numberOfWords) {
          Serial.println("\n-------!");Serial.println(theWordsUsedInTheTypeRacer[counterWords - 1]);
          digitalWrite(redRgb, LOW);
          digitalWrite(greenRgb, HIGH);
          digitalWrite(blueRgb, LOW);
          }
        bufferIndex = 0;
        buffer[bufferIndex] = '\0'; // Reset the first element to end the string
        timerOfTheGame = millis();
      }
      return;
    }
    Serial.print("\nGame over: number of words-");
    Serial.print(numberOfWords);
    Serial.print(" of which correct-");
    Serial.println(numberOfCorrectWords);
    startTheGame = false;
    counterWords = 0; numberOfCorrectWords = 0; i = 0; bufferIndex = 0;
    buffer[bufferIndex] = '\0';
    digitalWrite(redRgb, LOW);
    digitalWrite(greenRgb, LOW);
    digitalWrite(blueRgb, HIGH);
  }
}


void loop() {

  typeRacer();

}
