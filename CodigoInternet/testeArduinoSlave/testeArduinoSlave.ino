#include <Servo.h>
Servo myservo;
#include <A2a.h>
#define endereco 0x08

A2a arduinoMaster;

void setup() {
  myservo.attach(3);
  arduinoMaster.begin(endereco);
  arduinoMaster.onReceive(receberDdos);
  arduinoMaster.onRequest(enviarDdos);
}

void loop() {

}

void receberDados() {
  arduinoMaster.receiveData();
}

void enviarDados() {
  arduinoMaster.sendData();
}
