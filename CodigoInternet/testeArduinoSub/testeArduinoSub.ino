 /************************ Add bibliotecas **********************/
#include <A2a.h>
#include "config.h"
#include <Servo.h>

/************************ Config objetos *******************************/
#define endereco 0x08
#define ABERTO 1
#define FECHADO 0
Servo myservo;
#define pinPortao 3
AdafruitIO_Feed *abreportao = io.feed("abreportao");
A2a arduinoSlave;

//declarar funções
void configuraSlave();
void configuraMQTT();
void abrirPortao();
void fecharPortao();

//declaração de variaveis
bool comandoRecebido = false;
bool estadoPortao = FECHADO;

void setup() {
  Serial.begin(9600);
  myservo.attach(3);
  while(! Serial);

  configuraMQTT();

  arduinoSlave.begin(0, 2);
  configuraSlave();
  
  Serial.print("Conferindo posição do portão");
  
  abreportao->get();
  io.run();

  if(estadoPortao == FECHADO) {
    if(arduinoSlave.digitalWireRead(endereco, pinPortao)) {
      fecharPortao();
    }
  } else {
    if(arduinoSlave.digitalWireRead(endereco, pinPortao)) {
      abrirPortao();
    }
  }
  
  Serial.println("Portão posicionado");
  Serial.println("Fim Setup");

}

void loop() {
  io.run();
  
  if(comandoRecebido){
    if (estadoPortao == FECHADO) {
      estadoPortao = ABERTO;
      abreportao->save(estadoPortao);
      io.run();

      abrirPortao();
      comandoRecebido = false;
    } else {
      estadoPortao = FECHADO;
      abreportao->save(estadoPortao);
      io.run();

      fecharPortao();
      comandoRecebido = false;
    }
  }
}


void configuraMQTT() {
  Serial.print("Conectando Adafruit IO");
  io.connect();

  abreportao->onMessage(portaoMQTT);

  while (io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println();
  Serial.println(io.statusText());
}

void configuraSlave() {
  Serial.println("CONFIGURANDO PINMODE DO ARDUINO!!");
  arduinoSlave.pinWireMode(endereco, pinPortao, OUTPUT);
  arduinoSlave.digitalWireWrite(endereco, pinPortao, LOW);
  Serial.println("SLAVE CONFIGURADO!!");
}

void portaoMQTT(AdafruitIO_Data *estado) {
  Serial.println("Portao Recebido <- ");
  estadoPortao = estado->toBool();
  Serial.println(estado->value());
}

void abrirPortao(){
  myservo.write(90);
}

void fecharPortao(){
  myservo.write(0);
}
