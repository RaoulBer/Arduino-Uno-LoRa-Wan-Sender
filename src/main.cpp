#include <SPI.h>
#include <LoRa.h>

//Ultraschallsensor
#define PIN_TRIGGER 7
#define PIN_ECHO    6

const int SENSOR_MAX_RANGE = 300; // in cm
unsigned long waveSpeed = 58;
unsigned long duration;
unsigned long distance;


int counter = 0;

int measureDistance(){
  int returnValue;
  digitalWrite(PIN_TRIGGER, LOW);
  delayMicroseconds(2);

  digitalWrite(PIN_TRIGGER, HIGH);
  delayMicroseconds(10);

  duration = pulseIn(PIN_ECHO, HIGH);
  distance = duration/waveSpeed;
  Serial.println(String(distance));

  if (distance > SENSOR_MAX_RANGE || distance <= 0){
    returnValue = -1;
  } else {
    returnValue = distance;
  }
  return returnValue;
}

void setup() {
  Serial.begin(9600);
  pinMode(PIN_TRIGGER, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
  while (!Serial);
  Serial.println("LoRa Sender");

  if (!LoRa.begin(915E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
}

void loop() {
  Serial.print("Sending packet: ");
  Serial.print("Distance is: ");
  int distance = measureDistance();
  Serial.println(distance);

  // send packet
  LoRa.beginPacket();
  if (distance == -1){
    LoRa.print("Out of Range");
  }
  else{
    LoRa.print("Distance is: ");
    LoRa.print(String(distance));
  }
  LoRa.endPacket();
  delay(5000);
}