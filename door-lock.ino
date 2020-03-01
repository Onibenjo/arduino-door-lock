#include <Keypad.h>
#include<LiquidCrystal.h>
#include<EEPROM.h>
#include <Servo.h> 
//Declaring the lcd
LiquidCrystal lcd(9,8,7,6,5,4);
// decalring an empty array of the password
char password[4];
char pass[4],pass1[4];
int i=0;
char customKey=0;
// Keypad declaration
const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
char hexaKeys[ROWS][COLS] = {
{'1','2','3','A'},
{'4','5','6','B'},
{'7','8','9','C'},
{'*','0','#','D'}
};
byte rowPins[ROWS] = {A0,A1,A2,A3}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {A4,A5,3,2};   //connect to the column pinouts of the keypad
//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);
int led = 12;
int leds = 13;
int buzzer = 10;
// Servo motor pin
int servoPin = 11;
Servo Servo1;
// Setup
void setup()
{
  //Attaching the servo pin
  Servo1.attach(servoPin);
  Serial.begin(9600);
  Servo1.write(180); 
  lcd.begin(16,2);
  pinMode(led, OUTPUT);
  pinMode(leds, OUTPUT);
  pinMode(buzzer, OUTPUT);
  lcd.print("Arduino");
  Serial.print("Arduino");
  lcd.setCursor(0,1);
  lcd.print("Secure Doorlock");
  Serial.print("doorlock");
  delay(2000);
  lcd.clear();
  lcd.print("Enter Password:");
  Serial.println("Enter Password:");
  lcd.setCursor(0,1);
  for(int j=0;j<4;j++)
    EEPROM.write(j, j+49);
  for(int j=0;j<4;j++)
    pass[j]=EEPROM.read(j);
//    pass[j]=EEPROM.read(j);
}

void loop()
{
//  digitalWrite(11, LOW);
  customKey = customKeypad.getKey();
  if(customKey=='#')
    change();
   if(customKey=='*'){
    i = 0;
    lcd.clear();
    lcd.print("Enter Password:");
    Serial.println("Enter Password:");
    lcd.setCursor(0,1);
    customKey=0;
   }
  if (customKey)
  {
    password[i++]=customKey;
    lcd.print(customKey);
    Serial.print(customKey);
    beep();
  }
  if(i==4)
  {
    delay(200);
    for(int j=0;j<4;j++)
      pass[j]=EEPROM.read(j);
    if(!(strncmp(password, pass,4)))
    {
      digitalWrite(led, HIGH);
      beep();
      lcd.clear();
      lcd.print("Accepted");
      Serial.println("Accepted");
//      digitalWrite(11, HIGH);

      // Make servo go to 90 degrees 
       Servo1.write(90); 
      
      delay(1000);
      lcd.setCursor(0,1);
      lcd.print("#Change Password");
      Serial.println("#Change Password");
      delay(1000);
      Servo1.write(180); 
      lcd.clear();
      lcd.print("Enter Password");
      Serial.println("Enter Password");
      lcd.setCursor(0,1);
      i=0;
      digitalWrite(led, LOW);
      digitalWrite(leds, LOW);
     
    }
    else
    {
      digitalWrite(buzzer, HIGH);
      digitalWrite(led, LOW);
      digitalWrite(leds, HIGH);
      lcd.clear();
      lcd.print("WRONG PASSWORD");
      Serial.println("WRONG PASSWORD");
      lcd.setCursor(0,1);
      lcd.print("#Change Password");
      Serial.println("#Change Password");
      delay(1000);
      lcd.clear();
      lcd.print("Enter Password:");
      Serial.println("Enter Password:");
      lcd.setCursor(0,1);
      i=0;
      digitalWrite(buzzer, LOW);
      digitalWrite(led, LOW);
      digitalWrite(leds, LOW);
    }
  }
}
void change()
{
  int j=0;
  lcd.clear();
  lcd.print("Enter Curr pass");
  Serial.println("Enter Curr Pass ");
  lcd.setCursor(0,1);
  while(j<4)
  {
    char key=customKeypad.getKey();
    if(key)
    {
      pass1[j++]=key;
      lcd.print(key);
      Serial.print(key);
      beep();
    }
    key=0;
  }
  delay(500);

  if((strncmp(pass1, pass, 4)))
  {
    lcd.clear();
    lcd.print("WRONG PASSWORD...");
    Serial.println("WRONG PASSWORD...");
    lcd.setCursor(0,1);
    lcd.print("Try Again");
    Serial.println("Try Again");
    delay(1000);
  }
  else
  {
    j=0;
    lcd.clear();
    lcd.print("Enter New Pass:");
    Serial.println("Enter New Pass:");
    lcd.setCursor(0,1);
    while(j<4)
    {
      char key=customKeypad.getKey();
      if(key)
      {
        pass[j]=key;
        lcd.print(key);
        Serial.print(key);
        EEPROM.write(j,key);
        j++;
        beep();
      }
    }
    lcd.print(" Success..");
    Serial.println(" Success..");
    delay(1000);
  }
  lcd.clear();
  lcd.print("Enter Password:");
  Serial.println("Enter Password:");
  lcd.setCursor(0,1);
  customKey=0;
}
void beep()
{
  digitalWrite(buzzer, HIGH);
  delay(20);
  digitalWrite(buzzer, LOW);
}
