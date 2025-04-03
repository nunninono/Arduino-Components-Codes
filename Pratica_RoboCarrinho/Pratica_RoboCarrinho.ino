#include <Servo.h>

// Definição dos pinos dos motores
#define IN1 7
#define IN2 6
#define IN3 5
#define IN4 4

// Pinos do sensor ultrassônico
#define TRIG 3
#define ECHO 12

// Pinos dos LEDs
#define LED1 13
#define LED2 8

// Pinos para controle de velocidade dos motores
#define ENA 10
#define ENB 11

Servo myservo;
float distance;
long duration;

unsigned long previousMillis = 0;
unsigned long ultimoMovServo = 0;
const long intervaloUltrassonico = 100;  // Intervalo para leitura do sensor
const long intervaloServo = 50;  // Intervalo para movimento do servo
int pos = 90;  // Posição inicial do servo no centro
bool increasing = true;  // Controle do movimento do servo
char cha = 0;  // Variável para comandos seriais
bool roboAtivo = false;  // Estado do robô (parado ou em movimento)

void setup() {
  Serial.begin(9600);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);

  myservo.attach(9);  // Pino do servo

  // Configuração da velocidade inicial dos motores
  analogWrite(ENA, 150);
  analogWrite(ENB, 150);

  Serial.println("Aguardando comando 'S' para iniciar...");
}

float lerDistancia() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  duration = pulseIn(ECHO, HIGH, 30000);  // Timeout de 30ms
  distance = duration * 0.034 / 2;

  return distance;
}

void moverRobo(float dist) {
  if (dist < 25) {
    // Obstáculo detectado -> girar para a direita
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
  } else {
    // Nenhum obstáculo -> seguir em frente
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
  }
}

void pararRobo() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
}

void loop() {
  // Leitura do comando serial
  if (Serial.available() > 0) {
    cha = Serial.read();
    
    if (cha == 'S') {
      roboAtivo = true;
      Serial.println("Robô iniciado!");
    } else if (cha == 'P') {
      roboAtivo = false;
      pararRobo();
      Serial.println("Robô parado!");
    } else if (cha == '1') {
      digitalWrite(LED1, HIGH);
      digitalWrite(LED2, HIGH);
    } else if (cha == '2') {
      digitalWrite(LED1, LOW);
      digitalWrite(LED2, LOW);
    }
  }

  // Se o robô estiver ativo, executar as funções
  if (roboAtivo) {
    // Leitura do sensor ultrassônico
    if (millis() - previousMillis >= intervaloUltrassonico) {
      previousMillis = millis();
      distance = lerDistancia();
      moverRobo(distance);
    }

    // Movimento do servo motor
    if (millis() - ultimoMovServo >= intervaloServo) {
      ultimoMovServo = millis();

      if (increasing) {
        pos += 5;
        if (pos >= 180) {
          increasing = false;
        }
      } else {
        pos -= 5;
        if (pos <= 0) {
          increasing = true;
        }
      }
      myservo.write(pos);
    }

    // Pisca os LEDs para indicar que está em movimento
    digitalWrite(LED1, !digitalRead(LED1));
    digitalWrite(LED2, !digitalRead(LED2));
  }
}
