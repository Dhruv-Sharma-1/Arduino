// Calculator using LCD and Keypad
// Dhruv Sharma
// ds1
// 99-355 Final Project
// 02/07/2020

/* Legend
 * 
 * Wires:
 * Red / Orange = 5V
 * Black / Brown = GND
 * White = Cols
 * Grey = Rows
 * Special Keys
 * Button = State Key
 * 
 * State  | Key | Operator 
 *  0       A       +
 *  0       B       -
 *  0       D       /
 *  0       *       *
 *  1       A       sqrt
 *  1       B       pow
 *  1       D       log
 *  1       *       exp
 *  
 * Arduino Pin | Input
 * SDA          LCD SDA
 * SCL          LCD SCL
 * 2 - 5        Keypad - Column
 * 6 - 9        Keypad - Row
 * A2           Switch
 * 
 * Arduino Pin | Output
 * 13           LED   
 * 
 * 
 * 
 * 
 * References
 * https://courses.ideate.cmu.edu/60-223/f2019/work/programmable-workout-timer/
 * https://playground.arduino.cc/Code/Keypad/#Creation
 * 
 */

#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

// Setup Keypad
const byte ROWS = 4;
const byte COLS = 4;

byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {5, 4, 3, 2};

char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '=', 'D'}
};

Keypad numPad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// Setup LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Button Pin
const int switchPin = 12;
const int LED = 13;


// Initalize Calculator

String char1;
String char2;
float num1;
float num2;
char op;
float res;

int state = 1;
bool firstNum = true;



void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  lcd.backlight();
  lcd.init();
  lcd.cursor();
  //lcd.setCursor(0, 0);
  lcd.home();

  pinMode(switchPin, INPUT); 
  pinMode(LED, OUTPUT);  
    
  
}

void loop() {
  // put your main code here, to run repeatedly:
  lcd.home();
  delay(500);

  // get a new key entered
  char currKey = numPad.getKey();

  //determine state of keypad
  bool tick = digitalRead(switchPin);
  if (tick == HIGH) {
    state += 1;
    state %= 2;
    Serial.print("State = ");
    Serial.println(state);
  }
  

  // State 0 is basic arithmetic
  if (state == 0) {
    digitalWrite(13, LOW);
    if (currKey != NO_KEY) {
      if (currKey == '1' || currKey == '2' || currKey == '3' || currKey == '4' || currKey == '5' || currKey == '6' || currKey == '7'
                  || currKey == '8' || currKey == '9' || currKey == '0') {
        if (firstNum) {
          char1 = char1 + currKey;
          lcd.home();
          lcd.print(char1);
          //Serial.println(char1);
        }
        else {
          char2 = char2 + currKey;
          lcd.setCursor(11, 0);
          lcd.print(char2);
          //Serial.println(char2);
        }
      }
                
      if (currKey == 'A' || currKey == 'B' || currKey == 'D' || currKey == '*') {
        if (firstNum) firstNum = false;
        op = currKey;
        if (res != 0) {
          lcd.clear();
          lcd.home();
          lcd.print(res);
        }
        
        if (op == 'A') op = '+';
        if (op == 'B') op = '-';
        if (op == 'D') op = '/';
        if (op == '*') op = 'x';
        lcd.setCursor(8, 0);
        lcd.print(op);
        //Serial.println(op);

      }
    
      if (currKey == '=') {
        num1 = char1.toFloat();
        if (res != 0) num1 = res;
        
        num2 = char2.toFloat();
      
        if (op == '+') 
          res = num1 + num2;
        if (op == '-')
          res = num1 - num2;
        if (op == 'x')
          res = num1 * num2;
        if (op == '/')
          res = num1 / num2;

        lcd.setCursor(0, 1);
        lcd.print('=');
        lcd.setCursor(5, 1);
        lcd.print(res);
       
        //Serial.print(num1);
        //Serial.print(' ');
        //Serial.print(op);
        //Serial.print(' ');
        //Serial.print(num2);
        //Serial.print(' ');
        //Serial.print("= ");
        //Serial.println(res);

        char2 = "";
        op = ' ';
        delay(2000);
        
      }

      if (currKey == 'C') {
        //Serial.println("Cleared input");
        clr();
 
      }
    }
  }

  // State 1 is some advanced functions
  if (state == 1) {
    digitalWrite(13, HIGH);
    if (currKey != NO_KEY) {
      if (currKey == '1' || currKey == '2' || currKey == '3' || currKey == '4' || currKey == '5' || currKey == '6' || currKey == '7'
                  || currKey == '8' || currKey == '9' || currKey == '0') {
        if (firstNum) {
          char1 = char1 + currKey;
          lcd.home();
          lcd.print(char1);
          //Serial.println(char1);
        }
        else {
          char2 = char2 + currKey;
          lcd.setCursor(11, 0);
          lcd.print(char2);
          //Serial.println(char2);
        }
      }
                
      if (currKey == 'A' || currKey == 'B' || currKey == 'D' || currKey == '*') {
        if (firstNum) firstNum = false;
        op = currKey;
        if (res != 0) {
          lcd.clear();
          lcd.home();
          lcd.print(res);
        }
        
        if (op == 'A') op = 'r';
        if (op == 'B') op = '^';
        if (op == 'D') op = 'l';
        if (op == '*') op = 'e';
        lcd.setCursor(8, 0);
        lcd.print(op);
        //Serial.println(op);

      }
    
      if (currKey == '=') {
        num1 = (double)char1.toFloat();
        if (res != 0) num1 = (double)res;
        
        num2 = (double)char2.toFloat();
      
        if (op == 'r') 
          res = sqrt(num1);
        if (op == '^')
          res = pow(num1, num2);
        if (op == 'l')
          res = log(num1);
        if (op == 'e')
          res = exp(num1);

        lcd.setCursor(0, 1);
        lcd.print('=');
        lcd.setCursor(5, 1);
        lcd.print(res);
       
        //Serial.print(num1);
        //Serial.print(' ');
        //Serial.print(op);
        //Serial.print(' ');
        //Serial.print(num2);
        //Serial.print(' ');
        //Serial.print("= ");
        //Serial.println(res);

        char2 = "";
        op = ' ';
        delay(2000);
        
      }

      if (currKey == 'C') {
        //Serial.println("Cleared input");
        clr();
 
      }
      

 
    }
  }
}



void clr() {
  // helper function to clear LCD screen
  
  lcd.clear();
  lcd.home();
  lcd.print("Cleared Output");
  lcd.setCursor(0, 1);
  lcd.print("Wait 2 seconds");
  delay(2000);
  lcd.clear();
  char1 = "";
  char2 = "";
  num1 = 0;
  num2 = 0;
  firstNum = true;
  op = ' ';
  res = 0;
  state = 0;
}
