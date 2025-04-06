#include <Servo.h>
#define TXpin 1
#define RXpin 0
 
//sensor ultrassônico
int led1 = 13;
int led2 = 8;
int echo = 12;
int trig = 3;
 
//servo motor
Servo myservo;
float dist;
long duration;
char cha;
// pinos de saída para os motores
#define IN1 7  //controle motor 1
#define IN2 6
#define IN3 5 // controle motor 2 
#define IN4 4
 
// pinos para velocidade dos motores
#define ENA 10
#define ENB 11

void setup() {
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
 
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  myservo.attach(9);
 
  // velocidade dos motores, entre 0 e 255
  analogWrite(ENA, 200);
  analogWrite(ENB, 200);
 
  myservo.write(90);
  Serial.println("Esperando comandos...");
}

float lerDist() {
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
 
  float duration = pulseIn(echo, HIGH);
  float distance = duration * 0.034 / 2;
 
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  delay(100);
 
  return distance;
}

void front() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void right() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void left() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void back() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void stop() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);

}

void loop() {
  if(Serial.available()){
    cha = Serial.read();
  }

  if (cha == '1') {
    dist = lerDist();
    if (dist > 25) {
      digitalWrite(led1, LOW);
      digitalWrite(led2, LOW);
      front();
    }
    else {  
      stop();
      delay(1000);
      digitalWrite(led1, HIGH);
      digitalWrite(led2, HIGH);
      for (int pos = 90; pos <= 180; pos += 10) {
        myservo.write(pos);
      }
      dist = lerDist();
      if (dist > 55) {
        for (int pos = 180; pos >= 90; pos -= 10) {
          myservo.write(pos);
        }
        left();
        delay(600);
        stop();
      }
      else {
        for (int pos = 180; pos >= 0; pos -= 10) {
          myservo.write(pos);
        }
        dist = lerDist();
        if (dist > 55) {
          for (int pos = 0; pos <= 90; pos += 10) {
            myservo.write(pos);
          }
          right();
          delay(600);
          stop();
        }
        else {
          right();
          delay(1200);
          stop();
        }
      }
    }
  }
  else if (cha == '0') {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
  }
}
