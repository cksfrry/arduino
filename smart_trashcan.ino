#include<Servo.h>


Servo servo;
const int seq[] = {2,3,4,5,6,7,8}; //7 segment display, 7段顯示器
const int servoOut = 9;  //servo motor output
const int trig = 10, echo = 11; //Ultrasonic sensor pin 
const int speaker = 12;  //speaker
const int led[]= {A0, A1, A2, A3, A4}; // Led display
int i, br, c;
const int Clear=10;
const byte num[11]={0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0xFD, 0x07, 0x7F, 0x6F, 0x00};
const int inter_time = 1; //Ultrasound detected distance
int mytime = 0;  //ultrasound detected period


//============Mario tone=================================
//High score:(8 ~ 9)
int f[6]={660, 660, 660, 510, 660, 770};
int d[6]={100, 100, 100, 100, 100 ,100};
int y[6]={150, 300, 300, 100, 300, 550};
//-------------------------------------------------------
//medium score:(5 ~ 7)
int f2[9]={500, 500, 500, 500, 580, 660, 500, 430, 380};
int d2[9]={60,  80,  60,  80,  80,  80,  80,  80,  80};
int y2[9]={150, 300, 350, 150, 350, 150, 300, 150, 600};
//-------------------------------------------------------
//Low score: (0 ~ 4)
int f3[7]={760, 720, 680, 620, 650, 380, 430};
int d3[7]={100, 100, 100, 150, 150, 100, 100};
int y3[7]={100, 150, 150, 300, 300, 150, 150};

void setup() {
  Serial.begin(9600);
  showNum(0);
  // number display
  for(int i=0;i<7;i++) {
    pinMode(seq[i], OUTPUT);
  }
  
  //LED
  for(int i=0;i<5;i++){
    pinMode(led[i], OUTPUT);
    digitalWrite(led[i], LOW);
  }

  //UltraSound
  pinMode (trig, OUTPUT);
  pinMode (echo, INPUT);
  servo.attach(servoOut);
}
  
void loop() {
  float d;
  c = c < 20 ? c+1: 0;
  d = Detect(c);
  if( d <= 20){
    servo.write(180);
    flashNum(int(d));
    servo.write(90);
  }
}

void playMario(int freq[], int dura[], int dly[], int size){
  for(int i=0; i < size; i++){
    tone(speaker, freq[i], dura[i]);
    delay(dly[i]);
  }
}

void flashNum(int n) 
{
  n = 10 - (abs(n - 10));
  n = n == 10 ? 9:n;
  for(int i=0;i<3;i++){
    showNum(Clear);
    for(int i=0;i<5;i++){ digitalWrite(led[i], LOW);}
    delay(100);
    showNum(n);
    for(int i=0;i<5;i++){ digitalWrite(led[i], HIGH);}
    delay(100);
  }
  if(n> 7){
    playMario(f, d, y, 6);
  } else if (n > 4){
    playMario(f2, d2, y2, 9);
  } else {
    playMario(f3, d3, y3, 7);
  }
}

void showNum(int n) 
{
  for(int i=0;i<7;i++){
    br = bitRead(num[n],i);
    digitalWrite(seq[i], br);
  }
}

float Detect(int c)
{
  if(c >= 0 && c < 5) {
      for(int i=0;i<5;i++) { digitalWrite(led[i], LOW);}
      digitalWrite(led[c], HIGH);
  }
  if(c >= 5 && c<=10) {
      for(int i=0;i<5;i++) { digitalWrite(led[i], LOW);}
      digitalWrite(led[9 - c], HIGH);
  }  
  float duration, distance;  
  digitalWrite(trig, HIGH);
  delayMicroseconds(100);
  digitalWrite(trig, LOW);
  duration = pulseIn (echo, HIGH);
  distance = (duration/2)/29;
  Serial.print("Data:");
  Serial.print (mytime/1000);
  Serial.print(", d = ");
  Serial.print(distance);
  Serial.println(" cm");
  mytime = mytime + inter_time;
  delay(inter_time);
  return distance;
}


