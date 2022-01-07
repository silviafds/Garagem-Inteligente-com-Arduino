/************************ Adafruit IO Config *******************************/
#define IO_USERNAME   "Silvia1"
#define IO_KEY        "aio_vfVp77TsEBUciBxSJ4CFlJgrLfSz"

/******************************* WIFI **************************************/
#define WIFI_SSID   "wmultix"
#define WIFI_PASS   "capital012345678900"
#include "AdafruitIO_WiFi.h"
AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS);
//arquivo .h
/****************************************************************************/
// INCLUSÃO DE BIBLIOTECAS
//#include <A2a.h>
#include <Servo.h>

#include "WiFiEsp.h" //INCLUSÃO DA BIBLIOTECA
#include "SoftwareSerial.h"//INCLUSÃO DA BIBLIOTECA

// DEFINIÇÕES
SoftwareSerial Serial1(11, 13); //PINOS QUE EMULAM A SERIAL, ONDE O PINO 6 É O RX E O PINO 7 É O TX
#define tempoAtualizacao 8000
char ssid[] = "NOME_DA_SUA_REDE_WIFI"; //VARIÁVEL QUE ARMAZENA O NOME DA REDE SEM FIO
char pass[] = "SENHA_DA_SUA_REDE_WIFI";//VARIÁVEL QUE ARMAZENA A SENHA DA REDE SEM FIO
 
int status = WL_IDLE_STATUS; //STATUS TEMPORÁRIO ATRIBUÍDO QUANDO O WIFI É INICIALIZADO E PERMANECE ATIVO
//ATÉ QUE O NÚMERO DE TENTATIVAS EXPIRE (RESULTANDO EM WL_NO_SHIELD) OU QUE UMA CONEXÃO SEJA ESTABELECIDA
//(RESULTANDO EM WL_CONNECTED)
 
WiFiEspServer server(80); //CONEXÃO REALIZADA NA PORTA 80
 
RingBuffer buf(8); //BUFFER PARA AUMENTAR A VELOCIDADE E REDUZIR A ALOCAÇÃO DE MEMÓRIA

// INSTANCIANDO OBJETOS
AdafruitIO_Feed *sensorgas = io.feed("sensorgas");
AdafruitIO_Feed *statusgas = io.feed("statusgas");

//A2a arduinoSlave;
Servo myservo;
// DECLARAÇÃO DE FUNÇÕES
void configuraMQTT();
bool monitoraSensor();

// DECLARAÇÃO DE VARIÁVEIS
unsigned long controleTempo = 0;
bool comandoRecebido = false;
unsigned int valorSensor;
unsigned int valorSensorStts;

// declaração particular
int pinBuzzer = 13;
int PinA3 = A3; //DECLARACAO CODIGO MQ2
int NivelMQ2 = 220; //DEFINE VALOR LIMITE (NÍVEL GÁS)

void setup() {
    Serial.begin(9600);
    while (! Serial);

    configuraMQTT();

    Serial1.begin(9600); //INICIALIZA A SERIAL PARA O ESP8266
    WiFi.init(&Serial1); //INICIALIZA A COMUNICAÇÃO SERIAL COM O ESP8266
    WiFi.config(IPAddress(192,168,2,110)); //COLOQUE UMA FAIXA DE IP DISPONÍVEL DO SEU ROTEADOR
    if(WiFi.status() == WL_NO_SHIELD){
        while (true);
    }
    while(status != WL_CONNECTED){
        status = WiFi.begin(ssid, pass);
    }
    server.begin();

    pinMode(PinA3, INPUT);
    pinMode(pinBuzzer, OUTPUT);
    myservo.attach(3); //servo liga no pino digital 3
    io.run();
    Serial.println("Fim Setup");
}

void loop() {
    io.run();

    if (millis() > controleTempo + tempoAtualizacao) {
        if (monitoraSensor()) {
            controleTempo = millis();
            sensorgas->save(valorSensor);
        }
        if (monitoraGasFumaca()) {
            controleTempo = millis;
            statusgas->save(valorSensorStts);
        }
    }
}

void configuraMQTT() {
    Serial.print("Conectando ao Adafruit IO");
    io.connect();

    while (io.status() < AIO_CONNECTED) {
        Serial.print(".");
        delay(500);
    }

    Serial.println();
    Serial.println(io.statusText());
}

bool detectaGasFumaca() {
    int LendoMQ2 = analogRead(PinA3);
  
    if(LendoMQ2 > NivelMQ2) {
        digitalWrite(pinBuzzer, HIGH);
        myservo.write(90); //comando servo abrir em 90 graus
        /*Serial.print("COM gas, valor lido: ");
        Serial.println(LendoMQ2);*/
        return true;
    } else {
        digitalWrite(pinBuzzer, LOW);  
        myservo.write(0); //comando servo abrir em 0 graus
        /*Serial.print("sem gas, valor lido: ");
        Serial.println(LendoMQ2);*/
        return false;
    }
}

int monitoraGasFumaca() {
    int LerGas = analogRead(PinA3);
    return LerGas;
}