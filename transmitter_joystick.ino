#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define led 2

RF24 radio(7, 8); // CE, CSN
const byte addresses[][6] = {"00001", "00002"};

void setup() {
  pinMode(A0, INPUT);
  pinMode(led, OUTPUT);
  radio.begin();
  Serial.begin(9600);
  radio.openWritingPipe(addresses[1]); // 00002
  radio.openReadingPipe(1, addresses[0]);
  radio.setPALevel(RF24_PA_MIN);
}

void loop() {
  delay(1);
  radio.stopListening();
  digitalWrite(led, LOW);
  int sensorVal = analogRead(A0);
  sensorVal = map(sensorVal, 0, 1022, 0, 300);
  radio.write(&sensorVal, sizeof(sensorVal));

  delay(1);
  radio.startListening();
    if (radio.available()) {
    char text[32] = "";
    radio.read(&text, sizeof(text));
    Serial.println(text);
    digitalWrite(led, HIGH);
    delay(500);
  }
}
