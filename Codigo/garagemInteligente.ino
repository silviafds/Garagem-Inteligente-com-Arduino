/************************ Configurações do Adafruit IO *******************************/
#define IO_USERNAME "" 
#define IO_KEY "" //chave do projeto

/************************* Configuração do WIFI **************************************/
#define WIFI_SSID ""
#define WIFI_PASS ""

/************************ Adicionando Bibliotecas ***********************************/
#include "AdafruitIO_WiFi.h"
#include <Servo.h>
#include <Ultrasonic.h>

/*************************** Criando objetos ***************************************/
Servo myservo;

/*********************** Configurações do comunicaçaão *****************************/
AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS);

/***************************** Adicionando feeds ***********************************/
AdafruitIO_Feed *abreportao = io.feed("abreportao");
AdafruitIO_Feed *detectavaga = io.feed("detectavaga");
AdafruitIO_Feed *sensorgas = io.feed("sensorgas");

/**************************** SENSOR ULTRASONICO ***********************************/
Ultrasonic sensor(D7, D8); //Trigger, echo
int distancia; 
bool detectandoVaga();

/**************************** SENSOR GÁS/FUMAÇA ************************************/
#define pinMQ2 A0 // DECLARAÇÃO SENSOR GÁS/FUMAÇA
int NivelMQ2 = 300; // NÍVEL LIMITE GÁS AMBIENTE
bool detectaGasFumaca();

void setup() {
    myservo.attach(D1);
    Serial.begin(9600);
    while(! Serial);
    conectaBroker();
}

void loop() {
    byte state = io.run();
    bool x;
    bool c;
    if(state == AIO_NET_DISCONNECTED || state == AIO_DISCONNECTED) {
        conectaBroker();
    }
   
        x = detectandoVaga();
        detectavaga->save(x);
        Serial.print(" valor retornado vaga ");
        Serial.println(x);
    
        c = detectaGasFumaca();
        sensorgas->save(c);
        Serial.print(" valor retornado do gas ");
        Serial.println(c);

    delay(4000);
}

void AbrindoPortao(AdafruitIO_Data *data) {
  Serial.print("Recebido  <- ");
  Serial.print(data->feedName());
  Serial.print(" : ");
  Serial.println(data->value());
  
  if(data->isTrue()) { //Aciona saída conforme dado recebido
    myservo.write(180);
  } else {
    myservo.write(0);
  }
}

bool detectandoVaga(){
  distancia = sensor.distanceRead();

  Serial.print(" distancia: ");
  Serial.print(distancia);
  Serial.print(" cm");

  if(distancia <= 5) {
    Serial.println(" Não tem vaga");
    return 1;
    
  } else {
        Serial.println(" Tem vaga");
        return 0;
  }

}

bool detectaGasFumaca() {
  int lendoMQ2 = analogRead(pinMQ2);

  Serial.print(" leitura: " );
  Serial.print(lendoMQ2 );
  
  if(lendoMQ2 > NivelMQ2) { 
    Serial.println(" Gas detectado");
    myservo.write(120);
    return 1;

  } else { 
        Serial.println(" Gas não detectado");
        myservo.write(0);
        return 0;
  }
}

void conectaBroker(){
  //mensagem de início
  Serial.print("Conectando ao Adafruit IO");
 
  // chama função para conexão com o io.adafruit.com
  io.connect();
 
   // instancia para o recebimento de dados que vem do broker
  abreportao->onMessage(AbrindoPortao);
 
  // Aguarda conexação ser estabelecida
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
 
  // Conectado
  Serial.println();
  Serial.println(io.statusText());
 
  // certifique-se de que todos os feeds obtenham seus valores atuais imediatamente
  abreportao->get();
}