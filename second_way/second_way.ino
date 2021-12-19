#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define PIN_BUZZER 2
#define PIN_ECHO 12 
#define PIN_TRIG 11
#define PIN_BTN 10

const int rows = 20;
const int columns = 4;

LiquidCrystal_I2C lcd(0x27, 20, 4);

void beep(int d);
void typewriter(String str, int column, int del);
void print_center(String str, int column);
int get_distance();

int setup_distance;
bool x = false;

void setup() {

  Serial.begin(9600);
  // initialize the LCD
  lcd.init();
  lcd.backlight();
  
  pinMode(PIN_BTN, INPUT_PULLUP);
  pinMode(PIN_BUZZER, OUTPUT);
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
  
  typewriter("Is Adamlari MTAL", 1, 1000);
  print_center("Kalibrasyon", 1);
  print_center("bekleniyor...", 2);
  
  /*for(int i = 0; i < 3; i++){
   
    beep(100);
    delay(100);
    
  }*/

}

void loop() {
  
  int diff;
  int distance;
  int btn;
  
  while(true){
    
    delay(50);
    
    distance = get_distance();
    diff = setup_distance - distance;
    btn = digitalRead(PIN_BTN);
    
    if(diff < 200 && diff > 10) break;
    if(btn == LOW) break;
  
  }
  
  if(btn == LOW){

    lcd.clear();
    setup_distance = distance;
    print_center("Kalibrasyon:", 1);
    print_center(String(setup_distance), 2);
    delay(1000);
    
  }else{

    lcd.clear();
    print_center("Boy:", 1);
    print_center(String(diff), 2);
    beep(200);
    
    while(digitalRead(PIN_BTN) == HIGH)
        delay(50);
    
  }
  
  lcd.clear();
  print_center("Olcum", 1);
  print_center("bekleniyor...", 2);
  delay(500);
  
}

int get_distance(){
  
  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);
  long duration = pulseIn(PIN_ECHO, HIGH);
  int distance = duration * 0.034 / 2;
  return distance;
  
}

void print_center(String str, int column){
  
  int len = str.length();
  int start = (rows - len) / 2;
  lcd.setCursor(start, column);
  lcd.print(str);
  
}

void typewriter(String str, int column, int del){
  
  lcd.blink();
  
  int len = str.length();
  int start = (rows - len) / 2;
  char arr[len+1];
  
  str.toCharArray(arr, len+1);
  
  for(int i = 0; i < len; i++){
    lcd.setCursor(start+i, column);
    lcd.write(arr[i]);
    delay(100);
  }
  
  delay(del);

  for(int i = len; i >= 0; i--){
    lcd.setCursor(start+i, column);
    lcd.write((byte) 160);
    delay(100);
  }
  
  lcd.noBlink();
  
}

void beep(int d){
  
  tone(PIN_BUZZER, 1000);
  delay(d);
  noTone(PIN_BUZZER);  
  
}
