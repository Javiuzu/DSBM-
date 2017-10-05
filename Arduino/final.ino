#include <SoftwareSerial.h>

SoftwareSerial BTserial (2,3); // RX | TX

const int EchoPin = 5;
const int TriggerPin = 6;
const int SensorLLumPin = A0;

char c = ' ';
char buff[10];
int V;
int ilum;

void setup()
{
  Serial.begin(9600);
  pinMode(TriggerPin, OUTPUT);
  pinMode(EchoPin, INPUT);
  
  BTserial.begin(9600);
}

void loop() {
  int cm = ping(TriggerPin, EchoPin);
  BTserial.print("Distancia: ");
  BTserial.println(cm);
  V = analogRead(SensorLLumPin);
  Serial.print("Distancia: ");
  Serial.println(cm);
  ilum = ((long)V*1000*10)/((long)15*10*(1024-V));
  BTserial.print("Llum: ");
  BTserial.println(ilum);
  Serial.print("Llum: ");
  Serial.println(ilum);
  delay(500);
  
  if (Serial.available())
  {
    BTserial.print("Acc: ");
    c = Serial.read();
    BTserial.write(c);
    while (c != '\n') {
      c = Serial.read();
      BTserial.write(c);
    }
  }
  
  if (Serial.available())
  {
    BTserial.print("Pantalla: ");
    c = Serial.read();
    BTserial.write(c);
    while (c != '\n') {
      c = Serial.read();
      BTserial.write(c);
    }
  }

}
 
int ping(int TriggerPin, int EchoPin) {
  long duration, distanceCm;
   
  digitalWrite(TriggerPin, LOW);  //para generar un pulso limpio ponemos a LOW 4us
  delayMicroseconds(4);
  digitalWrite(TriggerPin, HIGH);  //generamos Trigger (disparo) de 10us
  delayMicroseconds(10);
  digitalWrite(TriggerPin, LOW);
   
  duration = pulseIn(EchoPin, HIGH);  //medimos el tiempo entre pulsos, en microsegundos
   
  distanceCm = duration * 10 / 292/ 2;   //convertimos a distancia, en cm
  return distanceCm;
}
