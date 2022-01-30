/************************ Configurações do Adafruit IO *******************************/
#define IO_USERNAME ""/*usuario adafruit*/
#define IO_KEY ""/*chave do projeto*/
 
/********************* Configuração do WIFI **************************************/
#define WIFI_SSID ""/*wifi*/
#define WIFI_PASS ""/*senha wifi*/
 
/***************Adicionando Bibliotecas************/
#include "AdafruitIO_WiFi.h"
#include <Servo.h>
#include <Ultrasonic.h>
/***************Criando objetos************/
Servo myservo;
/***************Configurações do comunicaçaão************/
AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS);

/**************************Adicionando feeds**************************/
AdafruitIO_Feed *abreportao = io.feed("abreportao");
AdafruitIO_Feed *detectavaga = io.feed("detectavaga");
AdafruitIO_Feed *sensorgas = io.feed("sensorgas");

/*** SENSOR ULTRASONICO ***/
Ultrasonic sensor(D7, D8); //Trig, echo
int distancia;  
bool detectandoVaga();

/*sendor de gas*/
#define pinMQ2 A0 // DECLARACAO SENSOR GAS/FUMACA
int NivelMQ2 = 435; // NIVEL LIMITE GAS AMBIENTE
bool detectaGasFumaca();

void setup() {
    /*pinMode(pinMQ2, INPUT);*/
    myservo.attach(D4);
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
   
    /*if (millis() > 2000) {*/
        x = detectandoVaga();
        detectavaga->save(x);
        Serial.print(" valor retornado vaga ");
        Serial.println(x);
    /*}*/
    
    /*if (millis() > 1000) {*/
        c = detectaGasFumaca();
        sensorgas->save(c);
        Serial.print(" valor retornado do gas ");
        Serial.println(c);
     /*}*/

    delay(3500);
}

void AbrindoPortao(AdafruitIO_Data *data) {
  Serial.print("Recebido  <- ");
  Serial.print(data->feedName());
  Serial.print(" : ");
  Serial.println(data->value());
  
  if(data->isTrue()) { //Aciona saída conforme dado recebido
    myservo.write(150);
  } else {
    myservo.write(2);
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
    //myservo.write(90);
    return 1;
  } else { 
    Serial.println(" Gas não detectado"); 
    //myservo.write(0);
    return 0;
  }
}

void conectaBroker(){
  //mensagem inicial
  Serial.print("Conectando ao Adafruit IO");
 
  // chama função de conexão io.adafruit.com
  io.connect();
 
   // instancia um novo handler para recepção da mensagem do feed Rele
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
