# include "LiquidCrystal.h"
#include<Keypad.h>

//sonar
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);
const int trigPin = 12; // Trigger Pin of Ultrasonic Sensor
const int echoPin = 11; // Echo Pin of Ultrasonic Sensor
long duration;
int distanceCm;
float liquid;

//temperature
const int rs = 22, en = 23, d4 = 24, d5 = 25, d6 = 26, d7 = 27;
LiquidCrystal lcd2(rs, en, d4, d5, d6, d7);
float temp;

//unlocking
LiquidCrystal lcd3(42, 43, 44, 45, 46, 47);
char keys[4][3]={
 {'1','2','3'},
 {'4','5','6'},
 {'7','8','9'},
 {'*','0','#'}};
 
byte rowPin[4]={41,40,39,38};
byte colPin[3]={36,35,34};
String password = "123";
int redPin = 32;
int greenPin = 31;
int buzzer = 30; 
int wrong = 0;
Keypad keypad=Keypad(makeKeymap(keys),rowPin,colPin,4,3);
int total = 0, position = 0;


void setup() {
  Serial.begin(9600);
  //door
  pinMode(redPin,OUTPUT);
  pinMode(greenPin,OUTPUT);
  pinMode(buzzer, OUTPUT);
  lcd3.begin(16, 2);
  lcd3.print("CODE");
  delay(500);
  lcd3.clear();
  
  //sonar sensor
  lcd.begin(16, 2);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  lcd.setCursor(0, 0);
  lcd.print("Distance");
  lcd.setCursor(0, 1);
  lcd.print("Measurement");
  delay(500);
  lcd.clear();

  //motor
  pinMode(18, OUTPUT);
  pinMode(19, OUTPUT);
  pinMode(20, OUTPUT);
  pinMode(21, OUTPUT);
  pinMode(17, OUTPUT);
  digitalWrite(17, LOW);

  //window
  pinMode(48, OUTPUT);
  pinMode(49, OUTPUT);

  //temperature
  pinMode(A1, INPUT);
  pinMode(50, OUTPUT);
  pinMode(51, OUTPUT);
  pinMode(52, OUTPUT);
  digitalWrite(50, LOW);
  digitalWrite(51, LOW);
  digitalWrite(52, LOW);
  lcd2.begin(16, 2);
  lcd2.setCursor(0, 0);
  lcd2.print("Temperature");
  delay(100);
  lcd2.clear();
}

void loop() {
  //door
  lcd3.clear();
  lcd3.print("DCode: ");
  delay(100);
  char pressed=keypad.getKey();
  String key[3];
  
  if(pressed){  
  lcd3.clear();
  lcd3.setCursor(position,2);
  lcd3.print(pressed);
  delay(500);
  if(pressed == '*' || pressed == '#'){
          position = 0;
          setLocked(true);
          lcd3.clear();
      }
    else if(pressed == password[position]){
          key[position]=pressed;
          position++;
      }
    else if (pressed != password[position] ){
          wrong++;
          position ++;
      }
    if(position == 3){
          if( wrong >0){
                total++;
                wrong = 0;
                position = 0;
                lcd3.clear();
                lcd3.setCursor(0,2);
                lcd3.print("Wrong Code!");
                //delay(1000);
                delay(500);
                setLocked(true);
            }
          else if(position == 3 && wrong == 0){
                position = 0;
                wrong = 0;
                lcd3.clear();
                lcd3.setCursor(0,1);
                lcd3.print("  Welcome!");
                lcd3.setCursor(5,2);
                lcd3.print(" Door Open");
                //delay(2000);
                delay(500);
                setLocked(false);
            }
             if(total ==3){
                total=0;
                buzzer_beep();
                delay(500);
            }
        }
   }
  
  //temperature
  temp = analogRead(A1);
  temp = temp * 1100 / 10240;
  if (temp < 17) {
    digitalWrite(50, HIGH);
    digitalWrite(51, LOW);
    digitalWrite(52, LOW);
    digitalWrite(48, 0);
    digitalWrite(49, 1);
  }
  else if (temp >= 17 && temp < 24) {
    digitalWrite(50, LOW);
    digitalWrite(51, HIGH);
    digitalWrite(52, LOW);
    digitalWrite(48, 0);
    digitalWrite(49, 0);
  }
  else {
    digitalWrite(50, LOW);
    digitalWrite(51, LOW);
    digitalWrite(52, HIGH);
    digitalWrite(48, 1);
    digitalWrite(49, 0);
    delay(1000);
    digitalWrite(48, 0);
    digitalWrite(49, 0);
  }
  lcd2.print("TEMP : ");
  lcd2.print(temp);
  delay(500);
  lcd2.clear();

  //sonar sensor
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distanceCm = duration * 0.034 / 2;
  lcd.setCursor(0, 0);
  lcd.print("Distance");
  delay(10);
  lcd.setCursor(0, 1);
  lcd.print("Distance");
  lcd.print(distanceCm);
  lcd.print("cm");
  delay(10);
  if (distanceCm <= 552) {
    slow();
  }
  else {
    fast();
  }
}

void slow() {
  analogWrite(18, 100);
  analogWrite(19, 0);
  analogWrite(20, 100);
  analogWrite(21, 0);
  digitalWrite(17, HIGH);
}
void fast() {
  analogWrite(18, 250);
  analogWrite(19, 0);
  analogWrite(20, 250);
  analogWrite(21, 0);
  digitalWrite(17, LOW);
}
void setLocked(int locked)
  {
    if (locked)
      {
          digitalWrite(redPin, HIGH);
          digitalWrite(greenPin, LOW);
          //delay(1000);
          delay(200);
      }
    else
      {
          digitalWrite(redPin, LOW);
          digitalWrite(greenPin, HIGH);
          delay(2000);
          digitalWrite(redPin, HIGH);
          digitalWrite(greenPin, LOW);
      }
  }
void buzzer_beep()
{
   lcd3.clear();
   lcd3.setCursor(0,1);
   lcd3.print("    WARNING  !!!!   ");
   lcd3.setCursor(0,2);
   lcd3.print("   Access Denied");
   for (int i=0;i<10;i++){
   digitalWrite(buzzer,HIGH);
   delay(1000);
   digitalWrite(buzzer,LOW);
   delay(1000);
   }
}
