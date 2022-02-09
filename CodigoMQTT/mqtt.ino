/***************************** Configurações do Adafruit IO *******************************/
#define AIO_USERNAME          "" 
#define AIO_KEY               "" //chave do projeto
#define AIO_SERVER            "io.adafruit.com"      
#define AIO_SERVERPORT        1883

/***************************** Configuração do WIFI **************************************/
#define WLAN_SSID ""
#define WLAN_PASS ""   

/**************************** Adicionando Bibliotecas ***********************************/
#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"            
#include "Adafruit_MQTT_Client.h" 
#include <Servo.h>
#include <Ultrasonic.h>


/******************************** Criando objetos **************************************/
Servo myservo;

/*************************** Configurações do comunicação ******************************/
WiFiClient client;                                                                       
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);  

/********************************* Adicionando feeds ***********************************/
Adafruit_MQTT_Subscribe abreportao = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/abreportao"); 
Adafruit_MQTT_Publish detectavaga = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/detectavaga"); 
Adafruit_MQTT_Publish sensorgas = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/sensorgas"); 

/******************************* SENSOR ULTRASONICO ***********************************/
Ultrasonic sensor(D7, D8); //Trigger, echo
int distancia; 
int detectandoVaga();

/******************************* SENSOR GÁS/FUMAÇA ************************************/
#define pinMQ2 A0 // DECLARAÇÃO SENSOR GÁS/FUMAÇA
int NivelMQ2 = 200; // NÍVEL LIMITE GÁS AMBIENTE
int gatilho = 1;
int detectaGasFumaca(int gatilho);

void MQTT_connect();

void setup() {
  pinMode(pinMQ2, INPUT);
  myservo.attach(D1);
  Serial.begin(9600);

  WiFi.begin(WLAN_SSID, WLAN_PASS);  
  while (WiFi.status() != WL_CONNECTED) { 
    delay(500);                    
  }

  mqtt.subscribe(&abreportao); 
}

uint32_t x=0;  

void loop() {
  MQTT_connect(); 
  int x, c;

  /* Chama função na qual detecta vaga */
  x = detectandoVaga();
  if(! detectavaga.publish(x)) {
    Serial.println("falha na publicação vaga");
  } else {
    Serial.println("publicação de vaga publicada");
  }

  /* Chama função na qual detecta gás/fumaça */
  c = detectaGasFumaca(gatilho);
  if(! sensorgas.publish(c)) {
    Serial.println("falha na publicação gas");
  } else {
    Serial.println("publicação de gas publicada");
  }

  Adafruit_MQTT_Subscribe *subscription;

  /*Bloco de código que envia dados do Adafruit para abrir/fechar o portão*/
  while((subscription = mqtt.readSubscription(4000))) {
    if(subscription == &abreportao) {

      if(strcmp((char *)abreportao.lastread, "true") == 0) { //portão aberto pelo adafruit
        myservo.write(130);
        detectaGasFumaca(0);
      }

      if(strcmp((char *)abreportao.lastread, "false") == 0) { //portão fechado pelo adafruit
        myservo.write(0);
        detectaGasFumaca(1);
      }
      
    }
  }

  if(! mqtt.ping()) {
    mqtt.disconnect();
  }
  
}

/* Função para detecção de vaga */
int detectandoVaga(){
  distancia = sensor.distanceRead();

  if(distancia <= 5) {
    Serial.println(" Não tem vaga");
    return 1;
  } else {
      Serial.println(" Tem vaga");
      return 0;
  }
  
}

/* Função para detectar gás/fumaça no ambiente
 * Variável gatilho recebe um valor onde informa se o usuário abriu ou não o portão
 */
int detectaGasFumaca(int gatilho) {
  int lendoMQ2 = analogRead(pinMQ2);
  
  if(gatilho == 0) { //portão aberto pelo adafruit
    
    if (lendoMQ2 < NivelMQ2) {
      Serial.println(" Sem Gás");
      return 0;
    } else {
        Serial.println(" Gas detectado");
        return 1;
    }
    
  } else if (gatilho == 1) { //portão fechado pelo adafruit
      if(lendoMQ2 > NivelMQ2) { 
        Serial.println(" Gas detectado");
        myservo.write(130);
        return 1;
      } else {
          Serial.println(" Sem Gás");
          myservo.write(0);
          return 0;
      }
    
  }
  
  return 0;
}


/* Função que conecta com o broker */
void MQTT_connect() {        
  
  int8_t ret;             

  if (mqtt.connected()) {   
    return;                 
  }

  Serial.print("Conectando... ");   

  uint8_t retries = 3;      
  while ((ret = mqtt.connect()) != 0) { 
    Serial.println(mqtt.connectErrorString(ret)); 
    Serial.println("Reconectando em 5s..."); 
    mqtt.disconnect();   
    delay(5000);  
    retries--;     
    if (retries == 0) {
      while (1);
    }
  }

  Serial.println("Conectado!");   
}
