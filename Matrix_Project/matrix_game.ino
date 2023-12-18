#include "LedControl.h"
#include <LiquidCrystal.h>
#include <EEPROM.h>

// definirea pinilor pentru LCD
const byte rs = 9;
const byte en = 8;
const byte d4 = 7;
const byte d5 = 6;
const byte d6 = 5;
const byte d7 = 4;

// initializarea obiectului pentru controlul LCD
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// definirea pinilor pentru matricea LED si a dimensiunii acesteia
const byte dinPin = 12;
const byte clockPin = 11;
const byte loadPin = 10;
const byte matrixSize = 8;

// initializarea obiectului pentru controlul matricei LED
LedControl lc = LedControl(dinPin, clockPin, loadPin, 1); 

// definirea pinilor pentru joystick
const byte pinSW = 13;
const byte pinX = A0;
const byte pinY = A1;
const byte buzzerPin = 2;

byte matrixBrightness = 1;
byte aboutScrollText = 0;
byte HTPscrollText = 0;

byte upDownArrows[8] = {
  B00100,
  B01010,
  B10001,
  B00000,
  B00000,
  B10001,
  B01010,
  B00100
};

byte block[8] = {
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
};

const byte matrixHello[matrixSize] = {
  0b01010100,
  0b01110100,
  0b01010100,
  0b00000000,
  0b01101100,
  0b01111100,
  0b00111000,
  0b00010000
};

const byte matrixMenu[5][8] = {
    { 0b00111100,
    0b01010010,
    0b10011001,
    0b10011101,
    0b10011101,
    0b10011001,
    0b01010010,
    0b00111100 },
    { 0b00100000,
    0b00010000,
    0b10010000,
    0b01111000,
    0b00011100,
    0b00001110,
    0b00000111,
    0b00000011 },
    { 0b00111000,
    0b01000100,
    0b01000100,
    0b00001100,
    0b00010000,
    0b00010000,
    0b00000000,
    0b00010000 },
  { 0b00000000,
    0b00001000,
    0b00000000,
    0b00011000,
    0b00001000,
    0b00001000,
    0b00001000,
    0b00011100 }
};

const byte matrixHappy[8] = {
  0b00000000,
  0b01100110,
  0b01100110,
  0b00000000,
  0b10000001,
  0b01000010,
  0b00111100,
  0b00000000
};

byte matrix[matrixSize][matrixSize] = {
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 }
};

const String menuOptions[4] = {
  "Start game",
  "Settings",
  "How to play",
  "About"
};

const String settingsOptions[3] = {
  "LCD bright",
  "Matrix bright",
  "Sounds",
};

byte menuCurrentItem = 0; // stocheaza indicele optiunii curente din meniu
bool swState = LOW;       // stocheaza starea curenta a butonului joystick-ului
bool lastSwState = LOW;   // stocheaza starea anterioara a butonului
byte state = 0;           // stocheaza starea actuala a programului
byte switchState = HIGH;  // Starea curenta a comutatorului
int xValue = 0;           // stocheaza valoarea citita de pe axa X a joystick-ului
int yValue = 0;           // stocheaza valoarea citita de pe axa Y a joystick-ului
byte subMenuOption = 0;   // stocheaza optiunea curenta din submeniul 'settings'
bool joyBackToMiddleX = LOW;   // indica daca joystick-ul s-a intors la pozitia sa centrala
bool joyBackToMiddleY = LOW;
int minThreshold = 312;
int maxThreshold = 712;
unsigned long lastDebounceTime = 0;
unsigned long prevScrollTime = 0;  // stocheaza momentul ultimului scroll al textului pe LCD 
unsigned int debounceDelay = 50;   
bool matrixChanged = true;        // indica dacă matricea care controleaza afisarea a fost modificata si necesita o actualizare
unsigned int score = 0;
byte xPos = 0;    // stocheaza pozitia curenta a sarpelui in matrice
byte yPos = 0;
byte xLastPos = 0;  // stocheaza pozitia anterioara a sarpelui
byte yLastPos = 0;
byte lastFoodRow = 0;  // stocheaza pozitia anterioara a 'mancarii' pentru a  o sterge
byte lastFoodColumn = 0;
int scrollTextPosition = 0;
byte currentFoodRow = 0;     // stocheaza pozitia curenta a alimentului
byte settings = 0;           // stocheaza starea curenta a meniului de setari
byte settingsPos = 0;        // stocheaza pozitia curenta din meniul de setari
byte currentFoodColumn = 0;
byte startGame = 0;
String messageAbout = "Made by Cirstea Ruxandra-Gabriela. Github user: RuxiC ";
String messageHTP = "Eat the blinking food to increase the score. When reaching 10, the game is over.";
const byte moveInterval = 120;
int LCDbrightness;
byte sounds;
unsigned long long lastBlink = 0;
byte blinkLetter = LOW;
const int blinkInterval = 250; 
unsigned long long lastMoved = 0;    // stocheaza momentul ultimei miscari a sarpelui.
const int gameEndSound = 1500;       // frecventa sunetului
const int gameEndSoundDuration = 500;

void setup() {
  pinMode(buzzerPin, OUTPUT);
  playStartupSound();      // sunetul la începutul jocului
  LCDbrightness = EEPROM.read(1);
  matrixBrightness = EEPROM.read(2);
  sounds = EEPROM.read(4);
  analogWrite(1, LCDbrightness * 51);
  pinMode(pinSW, INPUT_PULLUP);
  Serial.begin(9600);
  lc.shutdown(0, false);
  lc.setIntensity(0, 3 * matrixBrightness);
  lc.clearDisplay(0);
  for (int row = 0; row < matrixSize; row++) {
    lc.setRow(0, row, matrixHello[row]);
  }
  lcd.begin(16, 2);
  lcd.print("     SNAKE!");
  lcd.setCursor(0, 1);
  lcd.print(" Press to begin");
  lcd.createChar(0, upDownArrows);
  lcd.createChar(1, block);
}

void loop() {
  if (startGame == 1) {
    game();
    if (score == 10) {
      exitGame();
    }
  } 
  else {
    if (aboutScrollText == 1 && scrollTextPosition < messageAbout.length() - 14) {
      lcd.setCursor(0, 1);
      lcd.print(messageAbout.substring(scrollTextPosition - 1, 16 + scrollTextPosition));
      if (millis() - prevScrollTime >= 500) {
        prevScrollTime = millis();
        lcd.setCursor(0, 1);
        lcd.print(messageAbout.substring(scrollTextPosition, 16 + scrollTextPosition));
        scrollTextPosition++;
      }
    }
    if (HTPscrollText == 1 && scrollTextPosition < messageAbout.length() + 11) {
      lcd.setCursor(0, 0);
      lcd.print("<HOW TO PLAY>  ");
      lcd.setCursor(0, 1);
      lcd.print(messageHTP.substring(scrollTextPosition-1, 16 + scrollTextPosition));
      if (millis() - prevScrollTime >= 500) {
        prevScrollTime = millis();
        lcd.setCursor(0, 1);
        lcd.print(messageHTP.substring(scrollTextPosition, 16 + scrollTextPosition));
        scrollTextPosition++;
      }
    }
    swState = digitalRead(pinSW);
    xValue = analogRead(pinX);
    yValue = analogRead(pinY);
    buttonLogic();
    yAxisLogic();
    xAxisLogic();
  }
}

// functie pentru a reda sunetul la începutul jocului
void playStartupSound() {
  tone(buzzerPin, 1000, 200);  
  delay(250);  
  tone(buzzerPin, 1500, 200);  
  delay(250);  
}

// functie pentru a reda sunetul la finalul jocului
void playGameEndSound() {
  tone(buzzerPin, gameEndSound, gameEndSoundDuration);
  delay(100);  
}

// functie folosita pentru diferite functii ale butonului joystickului
void buttonLogic() {
  if (swState != lastSwState) {
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (swState != switchState) {
      switchState = swState;
      if (switchState == LOW) {
        if (state == 0 && startGame == 0 || state == 4) {  // trece de mesajul cu wellcome
          state = 1;
          lcd.clear();
          lcd.print("<MAIN MENU>");
          lcd.setCursor(15, 0);
          lcd.write(byte(0));
          lcd.setCursor(0, 1);
          lcd.print(">");
          lcd.print(menuOptions[menuCurrentItem]);
          lc.clearDisplay(0);
          for (int row = 0; row < matrixSize; row++) {
            lc.setRow(0, row, matrixMenu[menuCurrentItem][row]);
          }
        } 
        else if (subMenuOption == 1) {  // cand incepe jocul
          startGame = 1;
          lcd.clear();
          lcd.setCursor(0, 1);
          lcd.print("SCORE: ");
          lcd.print(score);
          lc.clearDisplay(0); 
          matrix[xPos][yPos] = 1;
          generateFood();
        } 
      }
    }
  }
  lastSwState = swState;
}

// functie pentru diferite functii ale axei y a joystickului
void yAxisLogic() {
  if (yValue > maxThreshold && joyBackToMiddleY == LOW && state == 1 && startGame == 0) {  // selecteaza una dintre optiunile din meniu
    state = 2;
    if (menuCurrentItem == 0) {
      subMenuOption = 1;
      lcd.clear();
      lcd.print("<START GAME>");
      lcd.setCursor(0, 1);
      lcd.print("Press to start");
    } 
    else if (menuCurrentItem == 1) {
      lcd.clear();
      lcd.print("<SETTINGS>");
      lcd.setCursor(15, 0);
      lcd.write(byte(0));
      settings = 1;
      lcd.setCursor(0, 1);
      lcd.print("                ");
      lcd.setCursor(0, 1);
      lcd.print(">");
      lcd.print(settingsOptions[settingsPos]);
    } 
    else if (menuCurrentItem == 2) {
      lcd.clear();
      lcd.print("<HOW TO PLAY>");
      lcd.setCursor(0, 1);
      HTPscrollText = 1;
    } 
    else if (menuCurrentItem == 3) {
      lcd.clear();
      lcd.print("<ABOUT>");
      lcd.setCursor(0, 1);
      aboutScrollText = 1;
    }
    joyBackToMiddleY = HIGH;
  } 
  else if (yValue < minThreshold && joyBackToMiddleY == LOW && state == 2 && startGame == 0) {  // miscare pentru a te intoarce inapoi in meniu
    state = 1;
    aboutScrollText = 0;
    HTPscrollText = 0;
    scrollTextPosition = 0;
    subMenuOption = 0;
    settingsPos = 0;
    settings = 0;
    lcd.clear();
    lcd.print("<MAIN MENU>");
    lcd.setCursor(15, 0);
    lcd.write(byte(0));
    lcd.setCursor(0, 1);
    lcd.print(">");
    lcd.print(menuOptions[menuCurrentItem]);
    lc.clearDisplay(0);
    for (int row = 0; row < matrixSize; row++) {
      lc.setRow(0, row, matrixMenu[menuCurrentItem][row]);
    }
    joyBackToMiddleY = HIGH;
  } 
  else if (yValue > maxThreshold && joyBackToMiddleY == LOW && state == 2 && startGame == 0 && settings == 1) {  // selecteaza o optiune din submeniul 'settings'
    state = 3;
    lcd.clear();
    if (settingsPos == 0) {
      lcd.print("<LCD BRIGHT>");
      lcd.setCursor(0, 1);
      lcd.write(byte(0));
      lcd.setCursor(4, 1);
      lcd.print("-");
      for (int i = 0; i < LCDbrightness; ++i)
        lcd.write(byte(1));
      lcd.setCursor(10, 1);
      lcd.print("+");
    } 
    else if (settingsPos == 1) {
      lcd.print("<MATRIX BRIGHT>");
      lcd.setCursor(0, 1);
      lcd.write(byte(0));
      lcd.setCursor(4, 1);
      lcd.print("-");
      for (int i = 0; i < LCDbrightness; ++i)
        lcd.write(byte(1));
      lcd.setCursor(10, 1);
      lcd.print("+");
    }  
    else if (settingsPos == 2) {
      lcd.print("<SOUNDS>");
      lcd.setCursor(0, 1);
      lcd.write(byte(0));
      lcd.print(" ");
      if (sounds == 1)
        lcd.print("ON");
      else
        lcd.print("OFF");
    }
    joyBackToMiddleY = HIGH;
  } 
  else if (yValue < minThreshold && joyBackToMiddleY == LOW && state == 3 && startGame == 0 && settings == 1 and settingsPos != -1) {  // miscare pentru a te intoarce inapoi in submeniul 'settings'
    state = 2;
    lcd.clear();
    lcd.print("<SETTINGS>");
    lcd.setCursor(15, 0);
    lcd.write(byte(0));
    lcd.setCursor(0, 1);
    lcd.print(">");
    lcd.print(settingsOptions[settingsPos]);
    lc.clearDisplay(0);
    for (int row = 0; row < matrixSize; row++) {
      lc.setRow(0, row, matrixMenu[menuCurrentItem][row]);
    }
    joyBackToMiddleY = HIGH;
  }  
  else if (joyBackToMiddleY == HIGH && yValue < maxThreshold && yValue > minThreshold && startGame == 0) {
    joyBackToMiddleY = LOW;
  }
}

// functie pentru diferite functii ale axei x a joystickului
void xAxisLogic() {
  if (xValue < minThreshold && joyBackToMiddleX == LOW && state == 1 && startGame == 0) {  // miscarea in jos prin meniu
    if (menuCurrentItem < 3) {
      menuCurrentItem++;
      lcd.setCursor(0, 1);
      lcd.print("                ");
      lcd.setCursor(0, 1);
      lcd.print(">");
      lcd.print(menuOptions[menuCurrentItem]);
      lc.clearDisplay(0);
      for (int row = 0; row < matrixSize; row++) {
        lc.setRow(0, row, matrixMenu[menuCurrentItem][row]);
      }
    } 
    else if (menuCurrentItem == 3) {
      menuCurrentItem = 0;
      lcd.setCursor(0, 1);
      lcd.print("                ");
      lcd.setCursor(0, 1);
      lcd.print(">");
      lcd.print(menuOptions[menuCurrentItem]);
      lc.clearDisplay(0);
      for (int row = 0; row < matrixSize; row++) {
        lc.setRow(0, row, matrixMenu[menuCurrentItem][row]);
      }
    }
    joyBackToMiddleX = HIGH;
  } 
  else if (xValue > maxThreshold && joyBackToMiddleX == LOW && state == 1 && startGame == 0) {  // miscare in sus prin meniu
    if (menuCurrentItem > 0) {
      menuCurrentItem--;
      lcd.setCursor(0, 1);
      lcd.print("                ");
      lcd.setCursor(0, 1);
      lcd.print(">");
      lcd.print(menuOptions[menuCurrentItem]);
      lc.clearDisplay(0);
      for (int row = 0; row < matrixSize; row++) {
        lc.setRow(0, row, matrixMenu[menuCurrentItem][row]);
      }
    } 
    else if (menuCurrentItem == 0) {
      menuCurrentItem = 3;
      lcd.setCursor(0, 1);
      lcd.print("                ");
      lcd.setCursor(0, 1);
      lcd.print(">");
      lcd.print(menuOptions[menuCurrentItem]);
      lc.clearDisplay(0);
      for (int row = 0; row < matrixSize; row++) {
        lc.setRow(0, row, matrixMenu[menuCurrentItem][row]);
      }
    }
    joyBackToMiddleX = HIGH;
  }  
  else if (xValue < minThreshold && joyBackToMiddleX == LOW && state == 2 && startGame == 0 && settings == 1) {  // miscarea in jos prin submeniul 'settings'
    if (settingsPos < 2) {
      settingsPos++;
      lcd.setCursor(0, 1);
      lcd.print("                ");
      lcd.setCursor(0, 1);
      lcd.print(">");
      lcd.print(settingsOptions[settingsPos]);
    }
    else if (settingsPos == 2) {
      settingsPos = 0;
      lcd.setCursor(0, 1);
      lcd.print("                ");
      lcd.setCursor(0, 1);
      lcd.print(">");
      lcd.print(settingsOptions[settingsPos]);
    }
    joyBackToMiddleX = HIGH;
  } 
  else if (xValue > maxThreshold && joyBackToMiddleX == LOW && state == 2 && startGame == 0 && settings == 1) {  // miscarea in sus prin submeniul 'settings'
    if (settingsPos > 0) {
      settingsPos--;
      lcd.setCursor(0, 1);
      lcd.print("                ");
      lcd.setCursor(0, 1);
      lcd.print(">");
      lcd.print(settingsOptions[settingsPos]);
    }
    else if (settingsPos == 0) {
      settingsPos = 2;
      lcd.setCursor(0, 1);
      lcd.print("                ");
      lcd.setCursor(0, 1);
      lcd.print(">");
      lcd.print(settingsOptions[settingsPos]);
    }
    joyBackToMiddleX = HIGH;
  } 
  else if (xValue < minThreshold && joyBackToMiddleX == LOW && state == 3 && startGame == 0 && settings == 1) {  // diferite valori in submeniul 'settings'
    if (settingsPos == 0) {  // schimbam luminozitatea pt LCD
      if (LCDbrightness > 1) {
        LCDbrightness--;
        lcd.setCursor(0, 1);
        lcd.print("                ");
        lcd.setCursor(0, 1);
        lcd.write(byte(0));
        lcd.setCursor(4, 1);
        lcd.print("-");
        for (int i = 0; i < LCDbrightness; ++i)
          lcd.write(byte(1));
        lcd.setCursor(10, 1);
        lcd.print("+");
        analogWrite(1, LCDbrightness * 51);
        EEPROM.update(1, LCDbrightness);
      }
    } 
    else if (settingsPos == 1) {  // schimbam luminozitatea pt matrice
      if (matrixBrightness > 1) {
        matrixBrightness--;
        lcd.setCursor(0, 1);
        lcd.print("                ");
        lcd.setCursor(0, 1);
        lcd.write(byte(0));
        lcd.setCursor(4, 1);
        lcd.print("-");
        for (int i = 0; i < matrixBrightness; ++i)
          lcd.write(byte(1));
        lcd.setCursor(10, 1);
        lcd.print("+");
        lc.setIntensity(0, 3 * matrixBrightness);
        EEPROM.update(2, matrixBrightness);
      }
    } 
    else if (settingsPos == 2) {  // schimbam sunetul
      if (sounds == 1) {
        sounds = 0;
        lcd.setCursor(0, 1);
        lcd.print("                ");
        lcd.setCursor(0, 1);
        lcd.write(byte(0));
        lcd.print(" ");
        lcd.print("OFF");
        EEPROM.update(4, sounds);
      }
    }
    joyBackToMiddleX = HIGH;
  } 
  else if (xValue > maxThreshold && joyBackToMiddleX == LOW && state == 3 && startGame == 0 && settings == 1) {  // different values when in the settings submenu
    if (settingsPos == 0) {
      if (LCDbrightness < 5) {
        LCDbrightness++;
        lcd.setCursor(0, 1);
        lcd.print("                ");
        lcd.setCursor(0, 1);
        lcd.write(byte(0));
        lcd.setCursor(4, 1);
        lcd.print("-");
        for (int i = 0; i < LCDbrightness; ++i)
          lcd.write(byte(1));
        lcd.setCursor(10, 1);
        lcd.print("+");
        analogWrite(1, LCDbrightness * 51);
        EEPROM.update(1, LCDbrightness);
      }
    } 
    else if (settingsPos == 1) {
      if (matrixBrightness < 5) {
        matrixBrightness++;
        lcd.setCursor(0, 1);
        lcd.print("                ");
        lcd.setCursor(0, 1);
        lcd.write(byte(0));
        lcd.setCursor(4, 1);
        lcd.print("-");
        for (int i = 0; i < matrixBrightness; ++i)
          lcd.write(byte(1));
        lcd.setCursor(10, 1);
        lcd.print("+");
        lc.setIntensity(0, 3 * matrixBrightness);
        EEPROM.update(2, matrixBrightness);
      }
    } 
    else if (settingsPos == 2) {
      if (sounds == 0) {
        sounds = 1;
        lcd.setCursor(0, 1);
        lcd.print("                ");
        lcd.setCursor(0, 1);
        lcd.write(byte(0));
        lcd.print(" ");
        lcd.print("ON");
        EEPROM.update(4, sounds);
      }
    }
    joyBackToMiddleX = HIGH;
  } 
  else if (joyBackToMiddleX == HIGH && xValue < maxThreshold && xValue > minThreshold && startGame == 0) {
    joyBackToMiddleX = LOW;
  }
}

// functia folosita pentru logica jocului
void game() {
  if (millis() - lastBlink > blinkInterval) {
    matrix[currentFoodRow][currentFoodColumn] = !matrix[currentFoodRow][currentFoodColumn];
    updateMatrix();
    lastBlink = millis();
  }
  if (millis() - lastMoved > moveInterval) {
    updatePositions();
    lastMoved = millis();
  }
  if (matrixChanged == true) {
    updateMatrix();
    matrixChanged = false;
  }
}

// functie care genereaza mancarea (punctul care clipeste)
void generateFood() {
  lastFoodRow = currentFoodRow;
  lastFoodColumn = currentFoodColumn;
  currentFoodRow = random(0, 8);
  currentFoodColumn = random(0, 8);
  matrix[lastFoodRow][lastFoodColumn] = 0;         // sterge vechea pozitie a bucatii de mancare
  matrix[currentFoodRow][currentFoodColumn] = 1;   // marcheaza noua pozitie a bucatii de mancare
  matrixChanged = true;
}


void updateMatrix() {
  for (int row = 0; row < matrixSize; row++) {
    for (int col = 0; col < matrixSize; col++) {
      lc.setLed(0, row, col, matrix[row][col]);
    }
  }
}

void updatePositions() {
  int xValue = analogRead(pinX);
  int yValue = analogRead(pinY);

  xLastPos = xPos;
  yLastPos = yPos;
  if (xValue < minThreshold) {
    if (xPos < matrixSize - 1) {
      xPos++;
    } 
    else {
      xPos = 0;
    }
  }
  if (xValue > maxThreshold) {
    if (xPos > 0) {
      xPos--;
    } 
    else {
      xPos = matrixSize - 1;
    }
  }
  if (yValue > maxThreshold) {
    if (yPos < matrixSize - 1) {
      yPos++;
    } 
    else {
      yPos = 0;
    }
  }
  if (yValue < minThreshold) {
    if (yPos > 0) {
      yPos--;
    } 
    else {
      yPos = matrixSize - 1;
    }
  }
  if (xPos != xLastPos || yPos != yLastPos) {
    if (xPos == currentFoodRow && yPos == currentFoodColumn) {
      score++;
      lcd.setCursor(7, 1);
      lcd.print(score);
      generateFood();
      Serial.print("Score is: ");
      Serial.println(score);
    }
    matrixChanged = true;
    matrix[xLastPos][yLastPos] = 0;
    matrix[xPos][yPos] = 1;
  }
}

// functie folosita dupa ce se termina jocul
void exitGame() {
  for (int row = 0; row < matrixSize; row++) {
    lc.setRow(0, row, matrixHappy[row]);
  }
  playGameEndSound();
  lcd.clear();
  menuCurrentItem = 0;
  subMenuOption = 0;
  startGame = 0;
  lcd.print("Congrats !");
  lcd.setCursor(0, 1);
  lcd.print("Your score: ");
  lcd.print(score);
  delay(1000);
  score = 0;
  delay(5000);
  lcd.clear();
  lcd.print("<MAIN MENU>");
  lcd.setCursor(0, 1);
  lcd.print(">");
  lcd.print(menuOptions[menuCurrentItem]);
  lc.clearDisplay(0);
  for (int row = 0; row < matrixSize; row++) {
    lc.setRow(0, row, matrixMenu[menuCurrentItem][row]);
  }
}
