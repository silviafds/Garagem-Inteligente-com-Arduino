
# Garagem Inteligente com Arduino e Esp-01s

## Objetivo

Trazer segurança para os habitantes de uma residência que possui garagem. 

Uma garagem que além de detectar a disponibilidade de vaga, ela abre o portão automaticamente assim que detectar a presença de Gás CO, GLD e fumaça.
Além disso, ela também mede a temperatura ambiente.




## Software & Sistema Operacional

- Software: Adafruit.
- SO: Ubuntu Server.




## Arquitetura:

- Protocolo de comunicação: MQTT;	
- Dispositivos: Arduino UNO e Esp-01s.

## Dispositivos Utilizados:

- Arduino Uno Atmega 328 Smd;
- 1 Sensor MQ2 (detecta gás de cozinha e fumaça);
- 1 MQ7 (detecta monóxido de carbono)
- 1 Thermistor NTC (de 3 pinos);
- 1 Protoboard de 400 pontos;
- 1 Motor servo de 9g (abertura do portão); 
- 1 Buzzer ativo (ativa quando detecta gás/fumaça);
- 1 Sensor ultrassônico (detecta a existência de algum veículo na garagem);
- Cabos jumper de 20 cm macho X fêmea;
- Cabos jumper de 20 cm macho X macho;
- Cabos jumper de 20 cm fêmea x fêmea;
- 1 módulo Wi-Fi Esp-01s;
- 1 Adaptador Esp-01 para módulo Wi-Fi;
- 1 módulo para fazer a gravação na Esp-01s.

## Observação

O diretório CodigoOFF possui um arquivo .ino, no qual está no modo offline ,ou seja, conectado ao arduino ele funciona normalmente, porém ainda não possui configuração para executar na internet.






    