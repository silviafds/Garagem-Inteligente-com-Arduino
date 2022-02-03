/******************** INCLUSAO DE BIBLIOTECAS ********************/
#include <Ultrasonic.h>
#include <Servo.h>
#include <thermistor.h>

/********************** CRIANDO OBJETOS *************************/
Servo myservo;

/********************* DECLARANDO VARIAVEIS **********************/
int pinBuzzer = 13;
int PinA3 = A3; //DECLARACAO CODIGO MQ2
int NivelMQ2 = 220; //DEFINE VALOR LIMITE (NÍVEL GÁS)
int PinA4 = A4; //DECLARACAO CODIGO MQ7
int NivelMQ7 = 210; //DEFINE VALOR LIMITE (NÍVEL GÁS)

/*** SENSOR ULTRASONICO ***/
Ultrasonic sensor(8, 7); //Configura os pinos do sensor ultrassonico | TRIGGER = 8 e Echo = 7
long distancia;

/*** SENSOR DE TEMPERATURA ***/
#define NTC_PIN 5
THERMISTOR thermistor(NTC_PIN,        // Analog pin
                      18750,          // Nominal resistance at 25 ºC
                      3950,           // thermistor's beta coefficient
                      10000);         // Value of the series resistor
uint16_t temp;
                  

void setup() {
  pinMode(PinA3, INPUT);
  pinMode(pinBuzzer, OUTPUT);
  myservo.attach(3); //servo liga no pino digital 3
  Serial.begin(9600);
}

void loop() {
  detectaGasFumaca();
  detectaCO();
  detectaVaga();
  detectaTemperatura();
  delay(1000);
}

void detectaGasFumaca() {
  int LendoMQ2 = analogRead(PinA3);
  
  if(LendoMQ2 > NivelMQ2) {
    digitalWrite(pinBuzzer, HIGH);
    myservo.write(90); //comando servo abrir em 90 graus
    /*Serial.print("COM gas, valor lido: ");
    Serial.println(LendoMQ2);*/
  } else {
      digitalWrite(pinBuzzer, LOW);  
      myservo.write(0); //comando servo abrir em 0 graus
      /*Serial.print("sem gas, valor lido: ");
      Serial.println(LendoMQ2);*/
  }

}

void detectaCO() {
  int LendoMQ7 = analogRead(PinA4);
  
  if(LendoMQ7 > NivelMQ7) {
    digitalWrite(pinBuzzer, HIGH);
    myservo.write(90); //comando servo abrir em 90 graus
    /*Serial.print("COM gas, valor lido: ");
    Serial.println(LendoMQ7);*/
  } else {
      digitalWrite(pinBuzzer, LOW);  
      myservo.write(0); //comando servo abrir em 0 graus
      /*Serial.print("sem gas, valor lido: ");
      Serial.println(LendoMQ7);*/
  }

}

void detectaVaga() {
  distancia = sensor.Ranging(CM);
  /*Serial.print(distancia);
  Serial.println(" cm");*/
  
  if(distancia <= 5){
    /*Serial.println("Não Tem vaga");  */
  } else if(distancia >= 6){
      /*Serial.println("Tem vaga");  */
  }

}

void detectaTemperatura() {
  temp = thermistor.read();
  Serial.print("LEITURA THERMISTOR.READ: ");
  Serial.print(temp);

}
