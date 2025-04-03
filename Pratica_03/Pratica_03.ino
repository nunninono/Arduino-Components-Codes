// SEMPRE RETIRAR O TX E RX ANTES DE MADNAR O CODIGO PRO ARDUINO VIA USB
#define TXpin 1
#define RXpin 0
 
int led1 = 8;
int led2 = 12;
 
 
void setup() {
  Serial.begin(9600);
  pinMode(8, OUTPUT);
  pinMode(12, OUTPUT);
 
  Serial.println("Esperando comandos!!...");
}
 
void loop() {
  if (Serial.available()) {  // se houver info. via bluetooth
    char recebido = Serial.read(); // Le o a info. enviada via bluetooth
    Serial.print("Recebido: ");
    Serial.println(recebido);
 
    if (recebido == '1') {
      digitalWrite(8, HIGH);
      Serial.println("LED 1 HIGH");
    }
    else if (recebido == '2') {
      digitalWrite(12, HIGH);
      Serial.println("LED 2 HIGH!");
    }
    else if (recebido == '0') {
      digitalWrite(8, LOW);
      digitalWrite(12, LOW);
      Serial.println("LEDs 1 e 2 LOW!");
    }
  }
}