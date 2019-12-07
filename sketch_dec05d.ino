#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
int red = 4;
int green = A3;
int tm = 180;
int fs = 5;
int btn_timer = 7;
int buzzer = 8;
boolean on = false;
int tones[] = {523, 523, 392, 330, 261};

int aRed = 11;
int aGreen = 10;
int aBlue = 9;

int btn_pin = 2;

int trig_pin = 13;
int echo_pin = 12;

int mode = 0; //1: color?bright? 2: color? 3: bright?
int color = 0; //1: red 2: green 3: blue

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  lcd.init();
  lcd.backlight();
  pinMode(btn_pin, INPUT_PULLUP);
  pinMode(btn_timer, INPUT_PULLUP);
  pinMode(red, OUTPUT);
  pinMode(buzzer, OUTPUT);

  pinMode(trig_pin, OUTPUT);
  pinMode(echo_pin, INPUT);

}

void changeColor(){
  char c = '0';
  if(mode = 2){
    Serial.println("select color [r, g, b] : ");
    
    while(Serial.available() > 0){
      c = Serial.read();
    }
  }

  if(c == 'r'){
    Serial.println("red");
    analogWrite(aRed, 255);
    analogWrite(aGreen, 0);
    analogWrite(aBlue, 0);
    
    mode = 1;
    color = 1;
  }
  else if(c == 'g'){
    Serial.println("green");
    analogWrite(aRed, 0);
    analogWrite(aGreen, 255);
    analogWrite(aBlue, 0);

    mode = 1;
    color = 2;
  }
  else if(c == 'b'){
    Serial.println("blue");
    analogWrite(aRed, 0);
    analogWrite(aGreen, 0);
    analogWrite(aBlue, 255);

    mode = 1;
    color = 3;
  }
}

void changeBrightness(){
  char b = '0';
  if(mode == 3){
    Serial.println("Select brightness [f, s, t] : ");

    while(Serial.available() > 0){
      b = Serial.read();
    }
  }

  if(b == 'f'){
    Serial.println("less");
    if(color == 1){
      analogWrite(aRed, 40);
    }
    else if(color == 2){
      analogWrite(aGreen, 40);
    }
    else if(color == 3){
      analogWrite(aBlue, 40);
    }

    mode = 1;
  }
  else if(b == 's'){
    Serial.println("soso");
    if(color == 1){
      analogWrite(aRed, 130);
    }
    else if(color == 2){
      analogWrite(aGreen, 130);
    }
    else if(color == 3){
      analogWrite(aBlue, 130);
    }

    mode = 1;
  }
  else if(b == 't'){
    Serial.println("lot");
    if(color == 1){
      analogWrite(aRed, 255);
    }
    else if(color == 2){
      analogWrite(aGreen, 255);
    }
    else if(color == 3){
      analogWrite(aBlue, 255);
    }

    mode = 1;
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  // Serial.println("[Start]"); 
///timer 
  lcd.clear();
  lcd.setCursor(1,1);
  lcd.print("ENEMY of DIET!");
  
  if(on && digitalRead(btn_timer) == 0){ //restart
      on = false;
      lcd.setCursor(5,0);
      lcd.print("Hi ^_^");
      tm = 180;
   }
  if (fs == 0){    //buzzer, led end 도미솔도~
   tm = 180;
   on = false;
   digitalWrite(red, LOW);
   analogWrite(green, 0);
   noTone(buzzer);
   fs = 5;
   }
  if(tm == 0 && on){   //5초동안 buzzer, led 도미솔도~ 
    fs -= 1;
    tone(buzzer, tones[fs]);
    if(fs % 2 ==0){
      digitalWrite(red, HIGH);
      analogWrite(green, 0);
      }
     else{
      digitalWrite(red, LOW);
      analogWrite(green, 255);
     
      }
     ////twinkle
   
  }
  if(!on && (mode == 0 || mode == 4)){ //타이머 시작?
   Serial.println("Do U wanna start the timer?");
   lcd.setCursor(5,0);
    lcd.print("Hi ^_^");
   if(digitalRead(btn_timer) == 0){
    on = true;
    }

   }
  if(on && fs == 5){ //타이머 가는중~
    lcd.clear();
    lcd.setCursor(0,1);
    lcd.print("Tik Tok Tik Tok!");
    lcd.setCursor(5,0);
    tm -= 1;
    if(tm >=0 && tm <60){
      lcd.print("00:");
      if(tm < 10){
        lcd.print("0");
        }
      lcd.print(tm);
      }
    else if(tm >= 60 && tm < 120){
      lcd.print("01:");
      if(tm - 60 < 10){
         lcd.print("0");
        }
      lcd.print(tm - 60);
    }
    else{
      lcd.print("02:");
        if(tm - 120 < 10){
         lcd.print("0");
        }
      lcd.print(tm - 120);
      }
    if(tm == 0){
      lcd.clear();
      lcd.setCursor(0,1);
      lcd.print("Eat & Be fat");
    }
  }
///// 초음파 가까이 가면 불끔
  long duration, distance;   
  digitalWrite(trig_pin, LOW);
  delayMicroseconds(2);
  digitalWrite(trig_pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig_pin, LOW);
  duration = pulseIn(echo_pin, HIGH);

  distance = duration * 17 / 1000;

  if(distance < 10){
    Serial.println("ON!!");
    analogWrite(aRed, 255);
    analogWrite(aGreen, 255);
    analogWrite(aBlue, 255);

    mode = 1;
  }
  
  int btn = digitalRead(btn_pin);
  //Serial.println(btn);

  if(btn == 0){
    Serial.println("OFF...");
    analogWrite(aRed, 0);
    analogWrite(aGreen, 0);
    analogWrite(aBlue, 0);

    mode = 4;
  }
  
  Serial.flush();
  
  char m = '0';
  if(mode == 1){
    Serial.println("Select mode - 1: color, 2: brightness");
    
    while(Serial.available() > 0){
      m = Serial.read();
    }

  }
  
  if(m == '1'){
    Serial.println("change color!");
    mode = 2;
  }
  else if(m == '2'){
    Serial.println("change brightness!");
    mode = 3;
  }

  if(mode == 2){
    changeColor();
  }
  if(mode == 3){
    changeBrightness();
  }

  delay(1000);
}
